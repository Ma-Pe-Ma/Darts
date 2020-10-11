package com.mpm.dartsclient.fragments.gameconfig

import android.app.AlertDialog
import android.content.Context
import android.content.DialogInterface
import android.content.SharedPreferences
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.NumberPicker
import android.widget.RadioButton
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.R
import com.mpm.dartsclient.DartsClientApplication
import org.json.JSONObject


// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

/**
 * A simple [Fragment] subclass.
 * Use the [CricketConfig.newInstance] factory method to
 * create an instance of this fragment.
 */
class CricketConfig : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null

    private var cricketSharedPref = activity?.getSharedPreferences(
        getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE)

    private var cricketSharedPrefEditor : SharedPreferences.Editor? = null

    var customConfigLayout : ConstraintLayout? = null

    var interval : RadioButton? = null
    var random : RadioButton? = null
    var chaotic : RadioButton? = null
    var defined : RadioButton? = null

    var radioList : List<RadioButton?>? = null

    var numberOfNumbersValue : Int = 6
    var numberOfNumbers : Button? = null

    var intervalStartingNr : Button? = null
    var intervalStartingNrValue = 15

    var cricketType : String = ""
    var cricketNumberSet : String = ""
    var cricketCustomSet : String = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }

        cricketSharedPref = activity?.getSharedPreferences(
            getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE)

        cricketSharedPrefEditor = cricketSharedPref?.edit()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.fragment_cricket_config, container, false)

        setupTypeButtons(view)
        setupSetButtons(view)
        setupCustomButtons(view)

        return view
    }

    private fun setupTypeButtons(view : View) {
        val defaultType = resources.getString(R.string.cricketTypeM1)
        cricketType = cricketSharedPref!!.getString("CricketType", defaultType)!!

        var score = view.findViewById<RadioButton>(R.id.score)
        var noScore = view.findViewById<RadioButton>(R.id.noScore)
        var cutThroat = view.findViewById<RadioButton>(R.id.cutThroat)

        when (cricketType) {
            getString(R.string.cricketTypeM1) -> score.apply { isChecked = true }
            getString(R.string.cricketTypeM2) -> noScore.apply { isChecked = true
                Log.i("DARTS", "M2 TRUE!")}
            getString(R.string.cricketTypeM3) -> cutThroat.apply { isChecked = true }
            else ->  {
                score.apply { isChecked = true }
            }
        }

        score.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processTypeButton(getString(R.string.cricketTypeM1))
            }
        }

        noScore.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                Log.i("DARTS", "SZAR BENYOMVA!")
                processTypeButton(getString(R.string.cricketTypeM2))
            }
        }

        cutThroat.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processTypeButton(getString(R.string.cricketTypeM3))
            }
        }
    }

    private fun processTypeButton(type : String) {
        cricketType = type
        Log.i("DARTS", "PROCESSED TYPE: "+type +", this: "+this)
        cricketSharedPrefEditor!!.putString("CricketType", type)
        cricketSharedPrefEditor!!.commit()
        Log.i("DARTS", "PROCESSED TYPE2: "+type +", this: "+this)
        sendMessage()
    }

    private fun setupSetButtons(view: View) {
        customConfigLayout = view.findViewById(R.id.customSetLayout)

        var classic = view.findViewById<RadioButton>(R.id.classicSet)
        var full = view.findViewById<RadioButton>(R.id.fullSet)
        var custom = view.findViewById<RadioButton>(R.id.customSet)

        val defaultSet = resources.getString(R.string.cricketSetM1)
        cricketNumberSet = cricketSharedPref!!.getString("CricketNumberSet", defaultSet)!!

        when (cricketNumberSet!!) {
            getString(R.string.cricketSetM1) -> classic.apply { isChecked = true }
            getString(R.string.cricketSetM2) -> full.apply { isChecked = true }
            getString(R.string.cricketSetM3) -> custom.apply { isChecked = true }
            else ->  {
                classic.apply { isChecked = true }
            }
        }

        classic.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(getString(R.string.cricketSetM1))
            }
        }

        full.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(getString(R.string.cricketSetM2))
            }
        }

        custom.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(getString(R.string.cricketSetM3))
            }

            customConfigLayout!!.apply {
                visibility = if (isChecked) {
                    View.VISIBLE
                }else {
                    View.INVISIBLE
                }
            }
        }

        //Default: Hide custom layout

        if (cricketNumberSet != getString(R.string.cricketSetM3)) {
            customConfigLayout!!.apply {
                visibility = View.INVISIBLE
            }
        }
    }

    private fun processSetButton(set : String) {
        cricketNumberSet = set
        cricketSharedPrefEditor!!.putString("CricketNumberSet", set)
        cricketSharedPrefEditor!!.commit()

        sendMessage()
    }

    private fun setupCustomButtons(view: View) {
        interval = view.findViewById(R.id.intervallum)
        random = view.findViewById(R.id.random)
        chaotic = view.findViewById(R.id.chaotic)
        defined = view.findViewById(R.id.defined)

        val defaultCustom = resources.getString(R.string.customSetM1)
        cricketCustomSet = cricketSharedPref!!.getString("CricketCustomSet", defaultCustom)!!

        intervalStartingNr = view.findViewById(R.id.startingNr)
        numberOfNumbers = view.findViewById(R.id.nrOfNumbers)

        when (cricketCustomSet) {
            getString(R.string.customSetM1) -> interval!!.apply { isChecked = true }
            getString(R.string.customSetM2) -> random!!.apply { isChecked = true
                intervalStartingNr!!.isEnabled = false}
            getString(R.string.customSetM3) -> chaotic!!.apply { isChecked = true
                intervalStartingNr!!.isEnabled = false}
            getString(R.string.customSetM4) -> defined!!.apply { isChecked = true
                intervalStartingNr!!.isEnabled = false}
            else ->  {
                interval!!.apply { isChecked = true }
            }
        }

        radioList = listOf(interval, random, chaotic, defined)

        interval!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                intervalStartingNr!!.isEnabled = true
                uncheckOthers(interval!!, radioList)
                processCustomButton(getString(R.string.customSetM1))
            }
            else {
                intervalStartingNr!!.apply {
                    intervalStartingNr!!.isEnabled = false
                }
            }
        }

        random!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                uncheckOthers(random!!, radioList)
                processCustomButton(getString(R.string.customSetM2))
            }
        }

        chaotic!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                uncheckOthers(chaotic!!, radioList)
                processCustomButton(getString(R.string.customSetM3))
            }
        }

        defined!!.apply { isEnabled = false}
        defined!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                uncheckOthers(defined!!, radioList)
                processCustomButton(getString(R.string.customSetM4))
            }
        }

        numberOfNumbersValue = cricketSharedPref!!.getInt("CricketNr", 6)
        intervalStartingNrValue = cricketSharedPref!!.getInt("CricketStart", 15)

        numberOfNumbers!!.apply {
            text = numberOfNumbersValue.toString()
            setOnClickListener {
                createNrOfNrPicker()
            }
        }

        intervalStartingNr!!.apply {
            text = intervalStartingNrValue.toString()
            setOnClickListener {
                createIntervalStartingNr()
            }
        }


    }

    private fun uncheckOthers(chosen : RadioButton, buttonList : List<RadioButton?>?) {
        for (button in buttonList!!) {
            if (button == chosen) {
                continue
            }

            button!!.apply {
                isChecked = false
            }
        }
    }

    private fun processCustomButton( customSetType : String) {
        cricketCustomSet = customSetType

        cricketSharedPrefEditor!!.putString("CricketCustomSet", customSetType)
        cricketSharedPrefEditor!!.apply()

        sendMessage()
    }

    private fun createNrOfNrPicker() {
        val setButton: (Int) -> Unit = {
            chosen ->
            run {
                numberOfNumbersValue = chosen
                numberOfNumbers!!.apply {
                    text = chosen.toString()
                }

                cricketSharedPrefEditor!!.putInt("CricketNr", numberOfNumbersValue)

                if (intervalStartingNrValue > 21 - numberOfNumbersValue) {
                    intervalStartingNrValue = 21 - numberOfNumbersValue
                    intervalStartingNr!!.apply {
                        text = intervalStartingNrValue.toString()
                    }

                    cricketSharedPrefEditor!!.putInt("CricketStart", intervalStartingNrValue)
                }

                cricketSharedPrefEditor!!.apply()

                sendMessage()
            }
        }

        createNumberPicker(numberOfNumbersValue,6,20, setButton)
    }

    private fun createIntervalStartingNr() {
        val setButton: (Int) -> Unit = {
            chosen ->
            run {
                intervalStartingNr!!.apply {
                    text = chosen.toString()
                }
                intervalStartingNrValue = chosen
                cricketSharedPrefEditor!!.putInt("CricketStart", intervalStartingNrValue)
                cricketSharedPrefEditor!!.apply()

                sendMessage()
            }
        }

        createNumberPicker(intervalStartingNrValue,1,21 - numberOfNumbersValue, setButton)
    }

    private fun createNumberPicker(default :Int, min : Int, max : Int, setter: (Int) -> Unit) {
        var alertDialogBuilder : AlertDialog.Builder = AlertDialog.Builder(context)

        val inflater = this.layoutInflater
        val dialogView: View = inflater.inflate(R.layout.number_picker_dialog, null)
        alertDialogBuilder.setTitle("Title")
        alertDialogBuilder.setMessage("Message")
        alertDialogBuilder.setView(dialogView)

        val numberPicker = dialogView.findViewById<NumberPicker>(R.id.dialog_number_picker)

        numberPicker.apply {
            minValue = min
            maxValue = max
            value = default
        }

        alertDialogBuilder.setPositiveButton("OKSZI", DialogInterface.OnClickListener() { dialogInterface: DialogInterface, i: Int ->
               setter(numberPicker.value)
        })

        alertDialogBuilder.create().show()
    }

    fun sendMessage() {
        var message = JSONObject()
        message.put("MENU", "CONFIG")
        message.put("GAME", "CRICKET")

        var configObject = JSONObject()
        configObject.put("CricketType", cricketType)
        configObject.put("CricketNumberSet", cricketNumberSet)
        configObject.put("CricketCustomSet", cricketCustomSet)

        Log.i("DARTS", "customset: "+cricketCustomSet)

        configObject.put("CricketNr", numberOfNumbersValue)
        configObject.put("CricketStart", intervalStartingNrValue)

        message.put("CONFIG", configObject)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(message)
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment CricketConfig.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            CricketConfig().apply {
                arguments = Bundle().apply {
                    putString(ARG_PARAM1, param1)
                    putString(ARG_PARAM2, param2)
                }
            }
    }
}