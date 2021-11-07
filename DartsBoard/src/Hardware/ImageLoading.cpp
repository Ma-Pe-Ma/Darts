#include "ImageLoading.h"

namespace ImageLoading {

    char namebuf[32] = ""; 
    File root;
    int pathlen;

    uint16_t read16(File& f) {
        uint16_t result;         // read little-endian
        f.read(&result, sizeof(result));
        return result;
    }

    uint32_t read32(File& f) {
        uint32_t result;
        f.read(&result, sizeof(result));
        return result;
    }

    void initialize() {
        bool good = SD.begin(SD_CS);
        if (!good) {
            Serial.print(F("cannot start SD"));
            while (1);
        }
        
        root = SD.open(namebuf);
        pathlen = strlen(namebuf);
    }

    uint8_t showBMP(char *nm, int x, int y) {
        Serial.print("Showing BMP...");
        File bmpFile;
        int bmpWidth, bmpHeight;    // W+H in pixels
        uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
        uint32_t bmpImageoffset;    // Start of image data in file
        uint32_t rowSize;           // Not always = bmpWidth; may have padding
        uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
        uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
        uint8_t bitmask, bitshift;
        boolean flip = true;        // BMP is stored bottom-to-top
        int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
        uint32_t pos;               // seek position
        boolean is565 = false;      //

        uint16_t bmpID;
        uint16_t n;                 // blocks read
        uint8_t ret;

        if ((x >= DisplayContainer::displayContainer.getTFT()->width()) || (y >= DisplayContainer::displayContainer.getTFT()->height()))
            return 1;               // off screen

        bmpFile = SD.open(nm);      // Parse BMP header
        bmpID = read16(bmpFile);    // BMP signature
        (void) read32(bmpFile);     // Read & ignore file size
        (void) read32(bmpFile);     // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile);       // Start of image data
        (void) read32(bmpFile);     // Read & ignore DIB header size
        bmpWidth = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        n = read16(bmpFile);        // # planes -- must be '1'
        bmpDepth = read16(bmpFile); // bits per pixel
        pos = read32(bmpFile);      // format
        if (bmpID != 0x4D42) ret = 2; // bad ID
        else if (n != 1) ret = 3;   // too many planes
        else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
        else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette 
        else {
            bool first = true;
            is565 = (pos == 3);               // ?already in 16-bit format
            // BMP rows are padded (if needed) to 4-byte boundary
            rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
            if (bmpHeight < 0) {              // If negative, image is in top-down order.
                bmpHeight = -bmpHeight;
                flip = false;
            }

            w = bmpWidth;
            h = bmpHeight;
            if ((x + w) >= DisplayContainer::displayContainer.getTFT()->width())       // Crop area to be loaded
                w = DisplayContainer::displayContainer.getTFT()->width() - x;
            if ((y + h) >= DisplayContainer::displayContainer.getTFT()->height())      //
                h = DisplayContainer::displayContainer.getTFT()->height() - y;

            if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
                bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
                bitmask = 0xFF;
                if (bmpDepth < 8)
                    bitmask >>= bmpDepth;
                bitshift = 8 - bmpDepth;
                n = 1 << bmpDepth;
                lcdbufsiz -= n;
                palette = lcdbuffer + lcdbufsiz;
                for (col = 0; col < n; col++) {
                    pos = read32(bmpFile);    //map palette to 5-6-5
                    palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
                }
            }

            // Set DisplayContainer::displayContainer.getTFT() address window to clipped image bounds
            DisplayContainer::displayContainer.getTFT()->setAddrWindow(x, y, x + w - 1, y + h - 1);
            for (row = 0; row < h; row++) { // For each scanline...
                // Seek to start of scan line.  It might seem labor-
                // intensive to be doing this on every line, but this
                // method covers a lot of gritty details like cropping
                // and scanline padding.  Also, the seek only takes
                // place if the file position actually needs to change
                // (avoids a lot of cluster math in SD library).
                uint8_t r, g, b, *sdptr;
                int lcdidx, lcdleft;
                if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                    pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                else        // Bitmap is stored top-to-bottom
                    pos = bmpImageoffset + row * rowSize;
                if (bmpFile.position() != pos) { // Need seek?
                    bmpFile.seek(pos);
                    buffidx = sizeof(sdbuffer); // Force buffer reload
                }

                for (col = 0; col < w; ) {  //pixels in row
                    lcdleft = w - col;
                    if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                    for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                        uint16_t color;
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) { // Indeed
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; // Set index to beginning
                            r = 0;
                        }
                        switch (bmpDepth) {          // Convert pixel from BMP to DisplayContainer::displayContainer.getTFT() format
                            case 24:
                                b = sdbuffer[buffidx++];
                                g = sdbuffer[buffidx++];
                                r = sdbuffer[buffidx++];
                                color = DisplayContainer::displayContainer.getTFT()->color565(r, g, b);
                                break;
                            case 16:
                                b = sdbuffer[buffidx++];
                                r = sdbuffer[buffidx++];
                                if (is565)
                                    color = (r << 8) | (b);
                                else
                                    color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                                break;
                            case 1:
                            case 4:
                            case 8:
                                if (r == 0)
                                    b = sdbuffer[buffidx++], r = 8;
                                color = palette[(b >> bitshift) & bitmask];
                                r -= bmpDepth;
                                b <<= bmpDepth;
                                break;
                        }
                        lcdbuffer[lcdidx] = color;

                    }
                    DisplayContainer::displayContainer.getTFT()->pushColors(lcdbuffer, lcdidx, first);
                    first = false;
                    col += lcdidx;
                }           // end cols
            }               // end rows
            DisplayContainer::displayContainer.getTFT()->setAddrWindow(0, 0, DisplayContainer::displayContainer.getTFT()->width() - 1, DisplayContainer::displayContainer.getTFT()->height() - 1); //restore full screen
            ret = 0;        // good render
        }
        bmpFile.close();
        Serial.println("width: "+String(DisplayContainer::displayContainer.getTFT()->width()) +", "+String(DisplayContainer::displayContainer.getTFT()->height()));
        return (ret);
    }
}