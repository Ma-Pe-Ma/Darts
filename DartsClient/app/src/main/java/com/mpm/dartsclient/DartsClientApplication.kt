package com.mpm.dartsclient

import android.app.Application
import android.content.Context

//TODO: bluetooth kapcsolódás
//      RTC, X01, stb.
//      at start proper score objects need to be attached!
//      stop bluetooth in background -> reconnect?
//      offline client + start activity where mode and bluetooth select!
//      backlog sound playing
//      backlog: saving last game -> complicated: needs much work both on server and client
//      backlog: OpenGL visualisation -> numbers

//TODO board:
// other playerscore
// physical buttons
// better bluetooth module / better touch screen
// speaker?

class DartsClientApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        application = this
        context = applicationContext
    }

   companion object {
       var context: Context? = null
       var application : Application? = null
       var bluetoothMode = true

       private var bluetoothCommunicatorInstance : BluetoothCommunicator? = null

       fun getBluetoothCommunicator() : BluetoothCommunicator {
           if (bluetoothCommunicatorInstance == null) {

               bluetoothCommunicatorInstance = BluetoothCommunicator(
                   application!!.applicationContext
               )
           }

           return bluetoothCommunicatorInstance!!
       }
   }
}