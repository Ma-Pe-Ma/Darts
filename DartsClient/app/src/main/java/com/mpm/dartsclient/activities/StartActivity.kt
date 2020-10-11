package com.mpm.dartsclient.activities

import android.app.Activity
import android.app.AlertDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.*
import android.graphics.Color.GREEN
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.RadioButton
import androidx.fragment.app.DialogFragment
import androidx.localbroadcastmanager.content.LocalBroadcastManager
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.mpm.dartsclient.BluetoothCommunicator
import com.mpm.dartsclient.ButtonState
import com.mpm.dartsclient.DartsClientApplication
import com.mpm.dartsclient.R
import com.mpm.dartsclient.adapters.BTRecyclerViewAdapter
import com.mpm.dartsclient.fragments.BTDeviceSelectorDialogFragment
import com.mpm.dartsclient.scoring.Dart
import kotlinx.android.synthetic.main.content_start.*

class StartActivity : AppCompatActivity() {
    private var broadcastReceiver : BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            var state = intent?.getBooleanExtra("conState", false)
            processState(state!!)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_start)

        LocalBroadcastManager.getInstance(this).registerReceiver(
            broadcastReceiver,
            IntentFilter("BTCONNECTION")
        );

        setupButtons()

        DartsClientApplication.getBluetoothCommunicator()

        findViewById<FloatingActionButton>(R.id.fabStart).setOnClickListener { view ->

            when (buttonState) {
                ButtonState.offline -> {
                    offlineNotImplementedYet()
                }

                ButtonState.onlineConnected -> {
                    Log.i("DARTS", "BOARD CONNECTED")
                    val intent = Intent(this, Config::class.java)
                    startActivity(intent)
                }

                ButtonState.onlineDisconnected -> {
                    Log.i("DARTS", "BOARD DISCONNECTED")
                }
            }
        }
    }

    val REQUEST_ENABLE_BT : Int = 101

    var buttonState : ButtonState = ButtonState.offline

    var onlineButton : RadioButton? = null
    var offlineButton : RadioButton? = null
    var btChooserButton : Button? = null

    fun setupButtons() {
        offlineButton = findViewById(R.id.offline)
        offlineButton?.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                onlineButton?.apply {
                    onlineButton?.isChecked = false
                }

                buttonState = ButtonState.offline

                DartsClientApplication.getBluetoothCommunicator().connectThread?.cancel()
            }
        }
        offlineButton?.apply {
            isChecked = true
        }

        onlineButton = findViewById(R.id.online)
        onlineButton?.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                offlineButton?.apply {
                    offlineButton?.isChecked = false
                }
                initializeBluetooth()
            }
        }

        btChooserButton = findViewById(R.id.bluetoothChooser)
        btChooserButton?.setOnClickListener {
            onlineButton?.apply {
                isChecked = true
            }
        }
    }

    private fun initializeBluetooth() {
        val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if (bluetoothAdapter == null) {
            // Device doesn't support Bluetooth
        } else {
            if (!bluetoothAdapter?.isEnabled) {
                val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT)
            } else {
                DartsClientApplication.getBluetoothCommunicator().bluetoothAdapter = bluetoothAdapter
                setupBluetooth()
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        when(requestCode) {
            REQUEST_ENABLE_BT -> {
                when (resultCode) {
                    Activity.RESULT_OK -> {
                        DartsClientApplication.getBluetoothCommunicator().bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
                        setupBluetooth()
                    }

                    Activity.RESULT_CANCELED -> {
                        deniedBluetoothDialog()
                        offlineButton?.apply { isChecked = true }
                    }
                }
            }
            else -> {}
        }
    }

    fun setupBluetooth() {
        var deviceList : MutableList<String> = mutableListOf()

        val pairedDevices: Set<BluetoothDevice>? = DartsClientApplication.getBluetoothCommunicator().bluetoothAdapter!!.bondedDevices
        pairedDevices?.forEach { device ->
            val deviceName = device.name
            val deviceHardwareAddress = device.address // MAC address
            deviceList.add(deviceName)
            Log.i("DARTS", "DEV: "+ deviceName)
        }

        var btDeviceSelectorDialogFragment = BTDeviceSelectorDialogFragment(deviceList)

        btDeviceSelectorDialogFragment.show(supportFragmentManager, "BTLIST")

        supportFragmentManager.executePendingTransactions()

        btDeviceSelectorDialogFragment.dialog!!.setOnCancelListener {
            if (DartsClientApplication.getBluetoothCommunicator().bluetoothDevice == null) {
                offlineButton?.isChecked = true
            }
        }
    }

    private fun deniedBluetoothDialog() {
        var builder = AlertDialog.Builder(this)

        builder.setTitle("Kikapcsolt BT!")
        builder.setMessage("Bluetooth nélkül nem fog menni!")
        builder.setPositiveButton(
            "Rendben",
            DialogInterface.OnClickListener { _, _ -> }
        )
        builder.create().show()
    }

    private fun offlineNotImplementedYet() {
        var builder = AlertDialog.Builder(this)

        builder.setTitle("Nem létező mód")
        builder.setMessage("Sajnos ez a mód még nem készült el! :(")
        builder.setPositiveButton(
            "Rendben",
            DialogInterface.OnClickListener { _, _ -> }
        )
        builder.create().show()
    }

    fun closeBTDialog(board : String) {
        var btDialog = supportFragmentManager.findFragmentByTag("BTLIST")
        (btDialog as DialogFragment).dismiss()

        buttonState = ButtonState.onlineDisconnected
        btChooserButton?.text = board

        DartsClientApplication.getBluetoothCommunicator().boardID = board
        DartsClientApplication.getBluetoothCommunicator().joinBondedDevice()
    }

    fun processState(state: Boolean) {
        Log.i("DARTS", "state set to: "+state)

        if (state) {
            buttonState = ButtonState.onlineConnected
            btChooserButton?.setBackgroundColor(getColor(R.color.GREEN))
        }
        else {
            buttonState = ButtonState.onlineDisconnected
            btChooserButton?.setBackgroundColor(getColor(R.color.RED))
        }
    }
}