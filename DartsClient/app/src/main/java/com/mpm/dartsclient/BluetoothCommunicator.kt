package com.mpm.dartsclient

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.util.Log
import org.json.JSONObject
import java.io.IOException
import java.io.InputStream
import java.io.OutputStream
import java.lang.NullPointerException
import java.util.*
import java.util.concurrent.ConcurrentLinkedQueue

class BluetoothCommunicator(var appContext: Context)  {
    val MY_UUID : UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    var serverBluetoothDevice : BluetoothDevice? = null
    var connectThread : ConnectThread? = null

    var boardID : String = ""
    var lineEnding : String = "\r\n"
    var partial : String = ""
    var inQueue : Queue<String> = LinkedList<String>()
    var outQueue : Queue<String> = ConcurrentLinkedQueue<String>()

    var bluetoothAdapter : BluetoothAdapter? = null
        set(value) {
            field = value

            /*val pairedDevices: Set<BluetoothDevice>? = field!!.bondedDevices
            pairedDevices?.forEach { device ->
                val deviceName = device.name
                val deviceHardwareAddress = device.address // MAC address

                //TODO(Create BT DEVICE CHOOSER)
                if (deviceName.equals("DARTS")) {
                    bluetoothDevice = device
                }
            }

            connectThread = ConnectThread(bluetoothDevice!!)
            connectThread?.start()*/
        }

    fun joinBondedDevice() {
        val pairedDevices: Set<BluetoothDevice>? = bluetoothAdapter!!.bondedDevices
        pairedDevices?.forEach devices@ { device ->
            val deviceName = device.name
            val deviceHardwareAddress = device.address // MAC address

            if (deviceName.equals(boardID)) {
                serverBluetoothDevice = device
                return@devices
            }
        }

        connectThread = ConnectThread(serverBluetoothDevice!!)
        connectThread?.start()
    }

    fun send(message : String) {
        outQueue.add(message)

        if (connectThread != null && connectThread!!.isAlive) {
            connectThread!!.write()
        }
        else {
            if (serverBluetoothDevice != null) {
                connectThread = ConnectThread(serverBluetoothDevice!!)
            }
        }
    }

    fun finish() {
        connectThread?.cancel()
    }

    inner class ConnectThread(var device: BluetoothDevice) : Thread() {
        private var bluetoothSocket : BluetoothSocket? = null
        private var mmInStream : InputStream? = null
        private var mmOutStream : OutputStream? = null
        private var mmBuffer : ByteArray = ByteArray(1024) // mmBuffer store for the stream

        init {
            // Cancel discovery because it otherwise slows down the connection.
            bluetoothAdapter?.cancelDiscovery()
            start()
        }

        //private val mmSocket: BluetoothSocket? by lazy(LazyThreadSafetyMode.NONE) {
        //    device.createRfcommSocketToServiceRecord(MY_UUID)
        //}

        override fun run() {
            mmBuffer = ByteArray(1024)
            var numBytes : Int = 0

            try {
                bluetoothSocket = device.createRfcommSocketToServiceRecord(MY_UUID)
                bluetoothSocket?.connect()
                mmInStream = bluetoothSocket!!.inputStream
                mmOutStream = bluetoothSocket!!.outputStream
            }
            catch (e : IOException) {

                return
            }

            while (true) {
                try {
                    numBytes = mmInStream!!.read(mmBuffer)
                    //Log.i("DARTS", "Listening?1")
                    if (numBytes > 0) {
                        var message : String = ""

                        for(i in 0 until numBytes) {
                            message += mmBuffer[i].toChar()
                            //Log.i("DARTS", ""+i+". :"+message[i])
                        }
                        //Log.i("DARTS", "Read out: \""+message +"\", size: "+message.length)
                        processMessage(message)
                    }
                }
                catch (ex : Exception) {
                    when (ex) {
                        is NullPointerException -> {

                        }

                        is IOException -> {

                        }

                        else -> {

                        }
                    }

                    break
                }
            }
        }

        // Closes the client socket and causes the thread to finish.
        fun cancel() {
            try {
                bluetoothSocket?.close()
                notifyNewState(false)
            } catch (e: IOException) {
                Log.e("DARTS", "Could not close the client socket", e)
            }
        }

        // Call this from the main activity to send data to the remote device.
        fun write() {
            try {
                var message: String? = null
                while (run {
                        message = outQueue.poll()
                        message
                    } != null) {
                     mmOutStream?.write(message!!.toByteArray())
                }

            } catch (e: IOException) {
                /* Log.e(TAG, "Error occurred when sending data", e)

                 // Send a failure message back to the activity.
                 val writeErrorMsg = handler.obtainMessage(MESSAGE_TOAST)
                 val bundle = Bundle().apply {
                     putString("toast", "Couldn't send data to the other device")
                 }
                 writeErrorMsg.data = bundle
                 handler.sendMessage(writeErrorMsg)
                 return*/
            }

            // Share the sent message with the UI activity.
            /*val writtenMsg = handler.obtainMessage(
                MESSAGE_WRITE, -1, -1, mmBuffer)
            writtenMsg.sendToTarget()*/
        }
    }

    private fun processMessage(input: String) {
        processString(input)

        var message: String? = null
        while (run {
                message = inQueue.poll()
                message
            } != null) {
            executeMessage(message)
        }
    }

    private fun executeMessage(input: String?) {
        try {
            var jsonObject = JSONObject(input)
            notifyNewMessagesReceived(jsonObject)
        }
        catch(e : Exception) {
            MessageHandler.requestLastMessage()
        }
    }

    fun sendMessage(jsonObject: JSONObject) {
        send(jsonObject.toString() + lineEnding)
    }

    private fun processString(input : String) {
        var wasBroken = false
        val processable = partial + input

        for (i in 0 until (processable.length - lineEnding.length + 1)) {
            if (processable.substring(i, i +lineEnding.length) == lineEnding) {
                var newEntry =  processable.substring(0, i)
                //Log.i("DARTS", "NEW ENTRY: "+newEntry)
                inQueue.add(newEntry)

                var newProcessable : String = processable.substring(i+lineEnding.length)

                if (i + lineEnding.length < processable.length) {
                    partial = ""
                    processString(newProcessable)
                }
                else {
                    partial = newProcessable
                }
                wasBroken = true;
                break
            }
        }

        if (!wasBroken) {
            partial = processable
        }
    }

    // ------ Sending
    var messageReceivers : MutableList<BTMessageReceiver> = mutableListOf()

    fun subscribeToMessages(messageReceiver: BTMessageReceiver) {
        if (!messageReceivers.contains(messageReceiver)) {
            messageReceivers.add(messageReceiver)
        }
    }

    fun unsubscribeToMessages(messageReceiver: BTMessageReceiver) {
        if (messageReceivers.contains(messageReceiver)) {
            messageReceivers.remove(messageReceiver)
        }
    }

    private fun notifyNewMessagesReceived(messageObject : JSONObject) {
        for (messageReceiver in messageReceivers) {
            messageReceiver.onBTReceive(messageObject)
        }
    }

    var stateReceivers : MutableList<BTStateReceiver> = mutableListOf()

    fun subscribeTpState(stateReceiver : BTStateReceiver) {
        if (!stateReceivers.contains(stateReceiver)) {
            stateReceivers.add(stateReceiver)
        }
    }

    fun unsubscribeToState(stateReceiver : BTStateReceiver) {
        if (stateReceivers.contains(stateReceiver)) {
            stateReceivers.remove(stateReceiver)
        }
    }

    private fun notifyNewState(state : Boolean) {
        for (stateReceiver in stateReceivers) {
            stateReceiver.notifyState(state)
        }
    }
}