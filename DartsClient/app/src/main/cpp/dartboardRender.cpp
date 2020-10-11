/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code
#include <dlfcn.h>
#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/sensor.h>

#include <cstdint>
#include <cassert>
#include <string>

#include "shader_m.h"

#define  LOG_TAG    "DARTS"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

const int LOOPER_ID_USER = 3;
const int SENSOR_HISTORY_LENGTH = 100;
const int SENSOR_REFRESH_RATE_HZ = 100;
constexpr int32_t SENSOR_REFRESH_PERIOD_US = int32_t(1000000 / SENSOR_REFRESH_RATE_HZ);
const float SENSOR_FILTER_ALPHA = 0.1f;

/*
 * AcquireASensorManagerInstance(void)
 *    Workaround AsensorManager_getInstance() deprecation false alarm
 *    for Android-N and before, when compiling with NDK-r15
 */
#include <dlfcn.h>
#include <jni.h>

#include <map>
std::map<int, int> sectorMap;
std::map<int, int> indiceSizeMap;

const int arcResolutionFactor = 1;
float** arcVertices;
//std::vector<std::vector<float>> arcVertices;

float** sectorVertices;
GLuint** indices;
float  fillerVertices[7][10];
const GLuint fillerEBOArray[6] {
    0,1,2,
    0,3,4
};

float orientationMap[20][16] = {};

float colorMap[2][2][4];

float radius[8] {
        0,
        10.0f, 20.0f,
        74.0f, 84.0f,
        134.0f, 144.0f,
        180.0f
};

float ratio[8] {
        radius[0] / radius[1],
        radius[1] / radius[1], radius[2] / radius[1],
        radius[3] / radius[1], radius[4] / radius[1],
        radius[5] / radius[1], radius[6] / radius[1],
        radius[7] / radius[1]
};

float middle[7] {
        0,
        (radius[1] + radius[2]) / 2, (radius[2] + radius[3]) / 2,
        (radius[3] + radius[4]) / 2, (radius[4] + radius[5]) / 2,
        (radius[5] + radius[6]) / 2, (radius[6] + radius[7]) / 2
};

void initMap() {
    sectorMap.insert({20, 0});
    sectorMap.insert({1, 1});
    sectorMap.insert({18, 2});
    sectorMap.insert({4, 3});
    sectorMap.insert({14, 4});

    sectorMap.insert({6, 5});
    sectorMap.insert({10, 6});
    sectorMap.insert({15, 7});
    sectorMap.insert({2, 8});
    sectorMap.insert({17, 9});

    sectorMap.insert({3, 10});
    sectorMap.insert({19, 11});
    sectorMap.insert({7, 12});
    sectorMap.insert({16, 13});
    sectorMap.insert({8, 14});

    sectorMap.insert({11, 15});
    sectorMap.insert({14, 16});
    sectorMap.insert({9, 17});
    sectorMap.insert({12, 18});
    sectorMap.insert({5, 19});

    sectorMap.insert({25, -1});

    //Black
    colorMap[0][0][0] = 0.0f;
    colorMap[0][0][1] = 0.0f;
    colorMap[0][0][2] = 0.0f;
    colorMap[0][0][3] = 1.0f;

    //Red
    colorMap[0][1][0] = 1.0f;
    colorMap[0][1][1] = 0.0f;
    colorMap[0][1][2] = 0.0f;
    colorMap[0][1][3] = 1.0f;

    //Beige
    colorMap[1][0][0] = 245.0f/256;
    colorMap[1][0][1] = 245.0f/256;
    colorMap[1][0][2] = 220.0f/256;
    colorMap[1][0][3] = 1.0f;

    //Green
    colorMap[1][1][0] = 0.0f;
    colorMap[1][1][1] = 0.5f;
    colorMap[1][1][2] = 0.0f;
    colorMap[1][1][3] = 1.0f;

    for (int i = 0; i < 20; i++) {
        float cosValue = cos ( i * (M_PI / 10));
        float sinValue = sin ( i * (M_PI / 10));

        orientationMap[i][0] = cosValue;
        orientationMap[i][1] = -sinValue;
        orientationMap[i][2] = 0;
        orientationMap[i][3] = 0;

        orientationMap[i][4] = sinValue;
        orientationMap[i][5] = cosValue;
        orientationMap[i][6] = 0;
        orientationMap[i][7] = 0;

        orientationMap[i][8] = 0;
        orientationMap[i][9] = 0;
        orientationMap[i][10] = 1;
        orientationMap[i][11] = 0;

        orientationMap[i][12] = 0;
        orientationMap[i][13] = 0;
        orientationMap[i][14] = 0;
        orientationMap[i][15] = 1;
    }
}

void initSectorVertices() {
    initMap();
    arcVertices = new float*[8];

    // 0 == double bull, 1 == simple bull, 2==inner simple, 3 == triple, 4 == outer simple, 5 == double, 6 == board
    for (int i = 0; i < 8; i++) {
        int arcResolution = arcResolutionFactor * ((int) ratio[i]);

        arcVertices[i] = new float[2 * (arcResolution + 1)];
        for (int j = 0; j < arcResolution + 1; j++) {
            if (i == 0) {
                arcVertices[i][2 * j + 0] = 0;
                arcVertices[i][2 * j + 1] = 0;
            }
            else {
                arcVertices[i][2 * j + 0] = sin (- M_PI / 20 + M_PI / 10 * j / arcResolution) * radius[i] / radius[7];
                arcVertices[i][2 * j + 1] = cos (- M_PI / 20 + M_PI / 10 * j / arcResolution) * radius[i] / radius[7];
            }
        }
    }

    sectorVertices = new float*[7 * 2];

    for (int i = 0; i < 7; i++) {
        int downHalfSize = 2 + arcResolutionFactor * ((int) ratio[i])  ;
        int upHalfSize = 2 + arcResolutionFactor * ((int) ratio[i + 1]);

        sectorVertices[2 * i + 0] = new float[downHalfSize * 2];
        sectorVertices[2 * i + 1] = new float[upHalfSize * 2];

        for (int j = 0; j < downHalfSize; j++) {
            if (j == 0) {
                sectorVertices[2 * i + 0][2 * j + 0] = 0;
                sectorVertices[2 * i + 0][2 * j + 1] = middle[i] / radius[7];
            }
            else {
                sectorVertices[2 * i + 0][2 * j + 0] = arcVertices[i][2 * j - 2];
                sectorVertices[2 * i + 0][2 * j + 1] = arcVertices[i][2 * j - 1];
            }
        }

        for (int j = 0; j < upHalfSize; j++) {
            if (j == 0) {
                sectorVertices[2 * i + 1][2 * j + 0] = 0;
                sectorVertices[2 * i + 1][2 * j + 1] = middle[i] / radius[7];;
            }
            else {
                sectorVertices[2 * i + 1][2 * j + 0] = arcVertices[i + 1][2 * j - 2];
                sectorVertices[2 * i + 1][2 * j + 1] = arcVertices[i + 1][2 * j - 1];
            }
        }
    }

    indices = new GLuint*[7 * 2];

    for (int i = 0; i < 7 * 2; i++) {
        int numberOfTriangles = arcResolutionFactor * ((int) ratio[(i + 1) / 2]);
        int numberOfIndices = 3 * numberOfTriangles;

        indices[i] = new GLuint[numberOfIndices];
        indiceSizeMap.insert({i, numberOfIndices});

        for (int j = 0; j < numberOfTriangles; j++) {
            indices[i][3 * j + 0] = 0;
            indices[i][3 * j + 1] = j + 2;
            indices[i][3 * j + 2] = j + 1;
        }
    }

    for (int i = 1; i < 7; i++) {
        fillerVertices[i][0] = 0;
        fillerVertices[i][1] = middle[i] / radius[7];

        fillerVertices[i][2] = sin (- M_PI / 20 ) * radius[i] / radius[7];
        fillerVertices[i][3] = cos (- M_PI / 20 ) * radius[i] / radius[7];

        fillerVertices[i][4] = sin (- M_PI / 20 ) * radius[i + 1] / radius[7];
        fillerVertices[i][5] = cos (- M_PI / 20 ) * radius[i + 1] / radius[7];

        fillerVertices[i][6] = sin ( M_PI / 20 ) * radius[i + 1] / radius[7];
        fillerVertices[i][7] = cos ( M_PI / 20 ) * radius[i + 1] / radius[7];

        fillerVertices[i][8] = sin ( M_PI / 20 ) * radius[i] / radius[7];
        fillerVertices[i][9] = cos ( M_PI / 20 ) * radius[i] / radius[7];
    }
}

void deInitSectorVertices() {
    for (int i = 0; i < 7; i++) {
        delete[] arcVertices[i];
    }

    delete[] arcVertices;
}

class DartsBoard {
    AAssetManager * assetManager;
    Shader* shader;
    GLuint vao;
    GLuint vbo[7*2];
    GLuint ebo[7*2];

    GLuint fillerVBO[7];
    GLuint fillerEBO;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

public:
    int currentMultplier = 0;
    int currentSector = 0;
    float currentColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    DartsBoard() {}

    std::string readFile(char* fileName) {
        AAsset *asset = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
        assert(asset != NULL);

        const void *vertexShaderBuf = AAsset_getBuffer(asset);
        assert(vertexShaderBuf != NULL);
        off_t vertexShaderLength = AAsset_getLength(asset);
        std::string assetData = std::string((const char*)vertexShaderBuf,
                                         (size_t)vertexShaderLength);
        AAsset_close(asset);

        return assetData;
    }

    void init(AAssetManager *assetManager) {
        this->assetManager = assetManager;
        vertexShaderSource = readFile("Shaders/Sector_VS.txt");
        fragmentShaderSource = readFile("Shaders/Sector_FS.txt");
    }

    void surfaceCreated() {
        initSectorVertices();

        shader = new Shader(vertexShaderSource, fragmentShaderSource);

        glGenVertexArrays(1, &vao);
        glGenBuffers(14, vbo);
        glGenBuffers(14, ebo);

        glBindVertexArray(vao);

        for (int i = 0; i < 14; i++) {
            int sectorSize = 2 + arcResolutionFactor * ((int) ratio[(i + 1) / 2]);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sectorSize * 2 , sectorVertices[i], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indiceSizeMap[i], indices[i], GL_STATIC_DRAW);
        }

        glGenBuffers(7, fillerVBO);
        glGenBuffers(1, &fillerEBO);

        for (int i = 0; i < 7; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, fillerVBO[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10 , fillerVertices[i], GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fillerEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, fillerEBOArray, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void surfaceChanged(int w, int h) {
        glViewport(0, 0, w, h);
    }

    void drawBoard(int sector, int multiplier) {
        int location = sectorMap[sector];

        for (int i = 0; i < 20; i++) {
            if (location == -1) {
                drawSector(i, -1 * multiplier);
            }
            else if (i == location) {
                drawSector(i, multiplier);
            }
            else {
                drawSector(i, 0);
            }
        }
    }

    void drawSector(int position, int multiplier) {
        shader->use();
        glBindVertexArray(vao);
        shader->setMat4("sectorPos", orientationMap[position]);

        drawSimpleBull(multiplier == -1);
        drawDoubleBull(multiplier == -2);

        drawSimple(multiplier == 1, position);
        drawDouble(multiplier == 2, position);
        drawTriple(multiplier == 3, position);

        drawOuter();
    }

    void drawFiller(int i) {
        glBindBuffer(GL_ARRAY_BUFFER, fillerVBO[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fillerEBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);
    }

    void drawSectorBuffer(int i) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glDrawElements(GL_TRIANGLES, indiceSizeMap[i] , GL_UNSIGNED_INT, 0);
    }

    void setColor(bool highlight, int pos, int multiplier) {
        if (highlight) {
            shader->setVec4("color", 1.0f, 215.0f/255, 0.0f, 1.0f);
        }
        else {
            shader->setVec4("color", colorMap[pos%2][multiplier][0], colorMap[pos%2][multiplier][1], colorMap[pos%2][multiplier][2], colorMap[pos%2][multiplier][3]);
        }
    }

    void drawDoubleBull(bool highlight) {
        if (highlight) {
            shader->setVec4("color", 1.0f, 215.0f/255, 0.0f, 1.0f);
        }
        else {
            shader->setVec4("color", 1.0f, 0.0f, 0.0f, 1.0f);
        }

        drawSectorBuffer(1);
    }

    void drawSimpleBull(bool highlight) {
        if (highlight) {
            shader->setVec4("color", 1.0f, 215.0f/255, 0.0f, 1.0f);
        }
        else {
            shader->setVec4("color", 0, 0.5f, 0.0f, 1.0f);
        }

        drawSectorBuffer(2);
        drawSectorBuffer(3);
        drawFiller(1);
    }

    void drawSimple(bool highlight, int pos) {
        setColor(highlight, pos, 0);
        drawSectorBuffer(4);
        drawSectorBuffer(5);
        drawSectorBuffer(8);
        drawSectorBuffer(9);

        drawFiller(2);
        drawFiller(4);
    }

    void drawTriple(bool highlight, int pos) {
        setColor(highlight, pos, 1);
        drawSectorBuffer(6);
        drawSectorBuffer(7);
        drawFiller(3);
    }

    void drawDouble(bool highlight, int pos) {
        setColor(highlight, pos, 1);
        drawSectorBuffer(10);
        drawSectorBuffer(11);
        drawFiller(5);
    }

    void drawOuter() {
        shader->setVec4("color", 0.0f, 0.0f, 0.0f, 1.0f);
        drawSectorBuffer(12);
        drawSectorBuffer(13);
        drawFiller(6);
    }

    void update() {

    }

    void render() {
        glClearColor(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        drawBoard(currentSector, currentMultplier);
    }

    void pause() {

    }

    void resume() {

    }
};

DartsBoard dartsBoard;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_init(JNIEnv *env, jclass type, jobject assetManager) {
        (void)type;
        AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager);
        dartsBoard.init(nativeAssetManager);
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_surfaceCreated(JNIEnv *env, jclass type) {
        (void)env;
        (void)type;
        dartsBoard.surfaceCreated();
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_surfaceChanged(
            JNIEnv *env, jclass type, jint width, jint height) {
        (void)env;
        (void)type;
        //dartsBoard.surfaceChanged(width, height);
    }


    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_drawFrame(
            JNIEnv *env, jclass type) {
        (void)env;
        (void)type;

        //dartsBoard.update();
        dartsBoard.render();
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_pause(
            JNIEnv *env, jclass type) {
        (void)env;
        (void)type;
        dartsBoard.pause();
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_resume(
            JNIEnv *env, jclass type) {
        (void)env;
        (void)type;
        dartsBoard.resume();
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_highLightSector(
            JNIEnv *env, jclass type, jint multiplier, jint sector) {
        (void)env;
        (void)type;
        dartsBoard.currentMultplier = multiplier;
        dartsBoard.currentSector = sector;
    }

    JNIEXPORT void JNICALL
    Java_com_mpm_dartsclient_nativeElements_CustomRenderer_setCurrentColorNative(
            JNIEnv *env, jclass type, jfloat red, jfloat green, jfloat blue, jfloat alpha) {
        (void)env;
        (void)type;
        dartsBoard.currentColor[0] = red;
        dartsBoard.currentColor[1] = green;
        dartsBoard.currentColor[2] = blue;
        dartsBoard.currentColor[3] = alpha;

}

}
