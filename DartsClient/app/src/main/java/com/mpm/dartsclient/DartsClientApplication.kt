package com.mpm.dartsclient

import android.app.Application
import android.util.Log

//TODO: bluetooth kapcsolódás
//      stop bluetooth in background -> reconnect!
//      playeradatbázis
//      offline client + start activity where mode and bluetooth select!
//      soundplaying
//      winning - on client not implemented
//      saving last game -> complicated: needs work both on server and client
//      last message request - tesztelni
//      player text color
//      RTC, X01, stb.
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
    }

   companion object {
       var application : Application? = null

       private var bluetoothCommunicatorInstance : BluetoothCommunicator? = null

       fun getBluetoothCommunicator() : BluetoothCommunicator {
           if (bluetoothCommunicatorInstance == null) {

               bluetoothCommunicatorInstance = BluetoothCommunicator(
                   application!!.applicationContext)
           }

           return bluetoothCommunicatorInstance!!
       }
   }
}