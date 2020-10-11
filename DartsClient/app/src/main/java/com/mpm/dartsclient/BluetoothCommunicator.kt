package com.mpm.dartsclient

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.Intent
import android.util.Log
import androidx.localbroadcastmanager.content.LocalBroadcastManager
import org.json.JSONObject
import java.io.IOException
import java.io.InputStream
import java.io.OutputStream
import java.util.*

class BluetoothCommunicator(var appContext: Context)  {

    val MY_UUID : UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    var bluetoothDevice :BluetoothDevice? = null
    var bluetoothSocket :BluetoothSocket? = null
    var connectThread : ConnectThread? = null
    var boardID : String = ""

    var lineEnding : String = "\r\n"
    var partial : String = ""
    var queue : Queue<String> = LinkedList<String>()

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
                bluetoothDevice = device
                return@devices
            }
        }

        connectThread = ConnectThread(bluetoothDevice!!)
        connectThread?.start()
    }

    inner class ConnectThread(device: BluetoothDevice) : Thread() {

        private val mmSocket: BluetoothSocket? by lazy(LazyThreadSafetyMode.NONE) {
            device.createRfcommSocketToServiceRecord(MY_UUID)
        }

        private var mmInStream: InputStream? = null
        private var mmOutStream: OutputStream? = null
        private val mmBuffer: ByteArray = ByteArray(1024) // mmBuffer store for the stream

        override fun run() {
            Log.i("DARTS", "Trying to connect!")

            // Cancel discovery because it otherwise slows down the connection.
            bluetoothAdapter?.cancelDiscovery()

            mmSocket?.use { socket ->
                // Connect to the remote device through the socket. This call blocks
                // until it succeeds or throws an exception.

                var connected : Boolean = false

                try {
                    socket.connect()
                    Log.i("DARTS", "Successfully connected!")
                    bluetoothSocket = socket
                    mmInStream = socket.inputStream
                    mmOutStream  = socket.outputStream
                    connected = true
                }
                catch (e: IOException) {
                    Log.i("DARTS", "Could not connect!"+e.printStackTrace())
                    sendState(false)
                    connected = false
                }

                //Original code:
                // The connection attempt succeeded. Perform work associated with
                // the connection in a separate thread.
                //manageMyConnectedSocket(socket)

                //VERY IMPORTANT: this note contradicts the previous one
                //the receiving code should be on this thread, because if this thread finishes (ie. the socket creating thread) then the connection is lost

                if (connected) {
                    sendState(true)
                    read()
                }
                else {
                    sendState(false)
                    Thread {
                        connectThread = ConnectThread(bluetoothDevice!!)
                        connectThread?.start()
                    }.start()
                }
            }
        }

        // Closes the client socket and causes the thread to finish.
        fun cancel() {
            try {
                mmSocket?.close()
                sendState(false)
            } catch (e: IOException) {
                Log.e("DARTS", "Could not close the client socket", e)
            }
        }

        private fun read() {
            var numBytes: Int // bytes returned from read()
            var counter : Int = 0

            Log.i("DARTS","Started listening bt conn!")

            // Keep listening to the InputStream until an exception occurs.
            while (true) {

                //Log.i("DARTS","Listening...")
                var isConnected = bluetoothSocket!!.isConnected

                // Read from the InputStream.
                try {
                    //Log.i("DARTS", "Listening?0")
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
                } catch (e: IOException) {
                    Log.i("DARTS", "Input stream was disconnected", e)
                    isConnected = false
                    break
                }

                if (!isConnected) {
                    cancel()
                    break
                }

                /*// Send the obtained bytes to the UI activity.
                val readMsg = handler.obtainMessage(
                    MESSAGE_READ, numBytes, -1,
                    mmBuffer)
                readMsg.sendToTarget()*/
            }

            //TODO(Reimplement this reconnection in a nicer way!)
            var newConnection = Thread(
                Runnable {
                    connectThread = ConnectThread(bluetoothDevice!!)
                    connectThread?.start()
                })

            newConnection.start()
        }

        // Call this from the main activity to send data to the remote device.
        fun write(bytes: ByteArray) {
            try {
                mmOutStream?.write(bytes)
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

        var message: String? = null;
        while ({message = queue.poll(); message}() != null) {
            executeMessage(message)
        }
    }

    private fun executeMessage(input: String?) {
        var thread = Thread(Runnable {
            var intent = Intent("boardMessage")
            intent.putExtra("message", input)

            LocalBroadcastManager.getInstance(appContext).sendBroadcast(intent)
        })

        thread.start()
    }

    private fun processString(input : String) {
        var wasBroken = false
        val processable = partial + input

        for (i in 0 until (processable.length - lineEnding.length + 1)) {
            if (processable.substring(i, i +lineEnding.length) == lineEnding) {
                var newEntry =  processable.substring(0, i)
                Log.i("DARTS", "NEW ENTRY: "+newEntry)
                queue.add(newEntry)

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

    fun sendMessage(jsonObject: JSONObject) {
        //var localBroadcastManager : LocalBroadcastManager = LocalBroadcastManager.getInstance(appContext)
        connectThread?.write((jsonObject.toString() + lineEnding).toByteArray())
        Log.i("DARTS", "OUT: "+jsonObject.toString())
    }

    fun sendState(state : Boolean) {
        var thread = Thread(Runnable {
            var intent = Intent("BTCONNECTION")
            intent.putExtra("conState", state)

            LocalBroadcastManager.getInstance(appContext).sendBroadcast(intent)
        })

        thread.start()
    }
}