package com.mpm.dartsclient.activities.config.fragments.gameconfig

import android.app.AlertDialog
import android.content.Context
import android.content.DialogInterface
import android.content.SharedPreferences
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.NumberPicker
import android.widget.RadioButton
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.R
import com.mpm.dartsclient.MessageHandler
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.games.cricket.CricketCustomSet
import com.mpm.dartsclient.games.cricket.CricketSet
import com.mpm.dartsclient.games.cricket.SubCricket
import com.mpm.dartsclient.sqlhelper.SQLTables

class CricketConfig : Fragment() {
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

    var cricketType : SubCricket = SubCricket.SCORE
    var cricketNumberSet : CricketSet = CricketSet.CLASSIC
    var cricketCustomSet : CricketCustomSet = CricketCustomSet.INTERVAL

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

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

        SQLTables.createStatistics()

        return view
    }

    private fun setupTypeButtons(view : View) {
        val defaultType = SubCricket.SCORE.toString()
        cricketType = SubCricket.valueOf(cricketSharedPref!!.getString("CricketType", defaultType)!!)
        DartsGameContainer.currentGame!!.subtype = cricketType.toString()

        var score = view.findViewById<RadioButton>(R.id.score)
        var noScore = view.findViewById<RadioButton>(R.id.noScore)
        var cutThroat = view.findViewById<RadioButton>(R.id.cutThroat)

        when (cricketType) {
            SubCricket.SCORE -> score.apply { isChecked = true }
            SubCricket.NOSCORE -> noScore.apply { isChecked = true }
            SubCricket.CUTTHROAT -> cutThroat.apply { isChecked = true }
            else ->  {
                score.apply { isChecked = true }
            }
        }

        score.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processTypeButton(SubCricket.SCORE)
            }
        }

        noScore.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processTypeButton(SubCricket.NOSCORE)
            }
        }

        cutThroat.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processTypeButton(SubCricket.CUTTHROAT)
            }
        }
    }

    private fun processTypeButton(type : SubCricket) {
        DartsGameContainer.currentGame?.subtype = type.toString()

        cricketType = type
        cricketSharedPrefEditor!!.putString("CricketType", type.toString())
        cricketSharedPrefEditor!!.commit()

        MessageHandler.sendGameConfig()
        SQLTables.createStatistics()
    }

    private fun setupSetButtons(view: View) {
        customConfigLayout = view.findViewById(R.id.customSetLayout)

        var classic = view.findViewById<RadioButton>(R.id.classicSet)
        var full = view.findViewById<RadioButton>(R.id.fullSet)
        var custom = view.findViewById<RadioButton>(R.id.customSet)

        val defaultSet = CricketSet.CLASSIC
        cricketNumberSet = CricketSet.valueOf(cricketSharedPref!!.getString("CricketNumberSet", defaultSet.toString())!!)

        when (cricketNumberSet) {
            CricketSet.CLASSIC -> classic.apply { isChecked = true }
            CricketSet.ALL -> full.apply { isChecked = true }
            CricketSet.CUSTOM -> custom.apply { isChecked = true }
            else ->  {
                classic.apply { isChecked = true }
            }
        }

        classic.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(CricketSet.CLASSIC)
            }
        }

        full.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(CricketSet.ALL)
            }
        }

        custom.setOnCheckedChangeListener {_, isChecked ->
            if (isChecked) {
                processSetButton(CricketSet.CUSTOM)
            }

            customConfigLayout!!.apply {
                visibility = if (isChecked) {
                    View.VISIBLE
                }else {
                    View.INVISIBLE
                }
            }
        }

        if (cricketNumberSet != CricketSet.CUSTOM) {
            customConfigLayout!!.apply {
                visibility = View.INVISIBLE
            }
        }
    }

    private fun processSetButton(cricketSet: CricketSet) {
        cricketNumberSet = cricketSet
        cricketSharedPrefEditor!!.putString("CricketNumberSet", cricketNumberSet.toString())
        cricketSharedPrefEditor!!.commit()

        MessageHandler.sendGameConfig()
    }

    private fun setupCustomButtons(view: View) {
        interval = view.findViewById(R.id.intervallum)
        random = view.findViewById(R.id.random)
        chaotic = view.findViewById(R.id.chaotic)
        defined = view.findViewById(R.id.defined)

        val defaultCustom = CricketCustomSet.INTERVAL.toString()
        cricketCustomSet = CricketCustomSet.valueOf(cricketSharedPref!!.getString("CricketCustomSet", defaultCustom)!!)

        intervalStartingNr = view.findViewById(R.id.startingNr)
        numberOfNumbers = view.findViewById(R.id.nrOfNumbers)

        when (cricketCustomSet) {
            CricketCustomSet.INTERVAL -> interval!!.apply { isChecked = true }
            CricketCustomSet.RANDOMINTERVAL -> random!!.apply { isChecked = true
                intervalStartingNr!!.isEnabled = false}
            CricketCustomSet.CHAOTIC -> chaotic!!.apply { isChecked = true
                intervalStartingNr!!.isEnabled = false}
            CricketCustomSet.DEFINED -> defined!!.apply { isChecked = true
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
                processCustomButton(CricketCustomSet.INTERVAL)
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
                processCustomButton(CricketCustomSet.RANDOMINTERVAL)
            }
        }

        chaotic!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                uncheckOthers(chaotic!!, radioList)
                processCustomButton(CricketCustomSet.CHAOTIC)
            }
        }

        defined!!.apply { isEnabled = false}
        defined!!.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                uncheckOthers(defined!!, radioList)
                processCustomButton(CricketCustomSet.DEFINED)
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

    private fun processCustomButton( customSetType : CricketCustomSet) {
        cricketCustomSet = customSetType

        cricketSharedPrefEditor!!.putString("CricketCustomSet", customSetType.toString())
        cricketSharedPrefEditor!!.apply()

        MessageHandler.sendGameConfig()
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

                MessageHandler.sendGameConfig()
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

                MessageHandler.sendGameConfig()
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
}