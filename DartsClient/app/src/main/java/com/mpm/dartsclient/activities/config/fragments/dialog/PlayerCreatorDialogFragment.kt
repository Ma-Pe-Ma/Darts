package com.mpm.dartsclient.activities.config.fragments.dialog

import android.app.AlertDialog
import android.app.Dialog
import android.content.Context
import android.content.DialogInterface
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.fragment.app.DialogFragment
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.sqlhelper.SQLTables
import com.skydoves.colorpickerview.AlphaTileView
import com.skydoves.colorpickerview.ColorEnvelope
import com.skydoves.colorpickerview.ColorPickerDialog
import com.skydoves.colorpickerview.ColorPickerView
import com.skydoves.colorpickerview.flag.FlagView
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener

class PlayerCreatorDialogFragment() : DialogFragment() {
    companion object {
        var position : Int? = null
        var tempPlayer : PlayerProfile? = null
    }

    constructor(positionT : Int?) : this() {
        position = positionT
    }

    //GUI elements
    var backColorButton : Button? = null
    var textColorButton : Button? = null
    var nameField : EditText? = null
    var nicknameField : EditText? = null

    val defaultBackgroundColor = 0
    val defaulTextColor = -16777216

    override fun onResume() {
        super.onResume()
        dialog!!.window!!.setLayout(700, 700)
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return object : Dialog(requireActivity(), theme) {
            override fun cancel() {
                if (activity != null && view != null) {
                    val imm = activity!!.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
                    imm.hideSoftInputFromWindow(view!!.windowToken, 0)
                }
                super.cancel()
            }
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val dialogView: View = inflater.inflate(R.layout.fragment_player_creator, null)

        //initialize GUI elements
        createGUIElements(dialogView)

        //create temporary playerprofile which will be modified
        if (tempPlayer == null && position != null) {
            var player = PlayerProfile.playerProfiles[position!!]

            //copying editable player profile
            tempPlayer = PlayerProfile(player.name, player.nickname, player.backgroundColor, player.textColor)
        }
        else if (tempPlayer == null && position == null) {
            tempPlayer = PlayerProfile("", "", defaultBackgroundColor, defaulTextColor)
        }

        //set the proper values for the gui elements
        nameField?.setText(tempPlayer?.name)
        nicknameField?.setText(tempPlayer?.nickname)
        backColorButton?.setBackgroundColor(if (tempPlayer?.backgroundColor != null) tempPlayer?.backgroundColor!! else defaultBackgroundColor)
        textColorButton?.setBackgroundColor(if (tempPlayer?.textColor != null) tempPlayer?.textColor!! else defaulTextColor)

        //Show keyboard and request focus for name text
        val imm: InputMethodManager? = activity?.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager?
        imm?.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0)

        nameField?.requestFocus()
        nameField?.setSelection(nameField?.text!!.length)

        var positiveButton = dialogView.findViewById<Button>(R.id.confirmPlayer)
        positiveButton.setOnClickListener {
            if (tempPlayer!!.name == "" || tempPlayer!!.nickname == "") {
                var builder = AlertDialog.Builder(activity)
                builder.setTitle("Üres mező!")
                builder.setMessage("Egyik mező sem lehet üres")
                builder.setPositiveButton(
                    "Jah",
                    DialogInterface.OnClickListener { _: DialogInterface, _: Int ->

                    })

                builder.create().show()
            }
            else if (tempPlayer!!.backgroundColor == null) {
                var builder = AlertDialog.Builder(activity)
                builder.setTitle("Nincs szín!")
                builder.setMessage("kell szín!")
                builder.setPositiveButton(
                    "Jah",
                    DialogInterface.OnClickListener { _: DialogInterface, _: Int ->

                    })

                builder.create().show()
            }
            else if (checkTakenName()) {
                var builder = AlertDialog.Builder(activity)
                builder.setTitle("Foglalt név!")
                builder.setMessage("új név kell")
                builder.setPositiveButton(
                    "Jah",
                    DialogInterface.OnClickListener { _: DialogInterface, _: Int ->

                    })

                builder.create().show()
            }
            else if (checkTakenNickname()) {
                var builder = AlertDialog.Builder(activity)
                builder.setTitle("Foglalt becenév!")
                builder.setMessage("új becenév kell!")
                builder.setPositiveButton(
                    "Jah",
                    DialogInterface.OnClickListener { _: DialogInterface, _: Int ->

                    })

                builder.create().show()
            }
            else {
                val imm: InputMethodManager? = activity?.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager?
                imm?.hideSoftInputFromWindow(view?.windowToken, 0)

                if (position == null) {
                    SQLTables.PlayersTable.addNewPlayer(tempPlayer!!)
                    PlayerProfile.playerProfiles.add(tempPlayer!!)
                    position = PlayerProfile.playerProfiles.size - 1
                }
                else {
                    var player = PlayerProfile.playerProfiles[position!!]
                    val oldKey = player.nickname

                    player.name = tempPlayer!!.name
                    player.nickname = tempPlayer!!.nickname
                    player.backgroundColor = tempPlayer!!.backgroundColor
                    player.textColor = tempPlayer!!.textColor

                    /*for (i in 0 until PlayerProfile.chosenPlayerProfiles.size) {
                        if (PlayerProfile.chosenPlayerProfiles[i].nickname == oldkey) {
                            PlayerProfile.chosenPlayerProfiles[i] = tempPlayer!!
                        }
                    }*/

                    SQLTables.PlayersTable.updatePlayer(tempPlayer!!, oldKey)
                }

                (activity as Config).notifyAboutModifiedPlayerEntry(position!!)
                position = null
                tempPlayer = null
                dialog?.dismiss()
            }
        }

        return dialogView
    }

    private fun createGUIElements(dialogView: View) {
        nameField = dialogView.findViewById(R.id.playerNameTextValue)
        nicknameField = dialogView.findViewById(R.id.playerNicknameTextValue)
        backColorButton = dialogView.findViewById(R.id.backColorButton)
        textColorButton = dialogView.findViewById(R.id.textColorButton)

        nameField?.addTextChangedListener(object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                //TODO("Not yet implemented")
            }

            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
                //TODO("Not yet implemented")
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                tempPlayer?.name = s.toString()
            }
        })

        nicknameField?.addTextChangedListener(object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                //TODO("Not yet implemented")
            }

            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
                //TODO("Not yet implemented")
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                tempPlayer?.nickname = s.toString()
                //nicknameValue = s.toString()
                //(activity as Config).prevPlayerProfile?.nickname = nicknameValue
            }
        })

        backColorButton?.setOnClickListener {
            createColorPicker(backColorButton!!)
        }

        textColorButton?.setOnClickListener {
            createColorPicker(textColorButton!!)
        }
    }

    private fun checkTakenName() : Boolean {
        var modifiableName = tempPlayer!!.name.toLowerCase()

        for ((i, playerProfile) in PlayerProfile.playerProfiles.withIndex()) {
            var nameFromList = playerProfile.name.toLowerCase()
            if (nameFromList == modifiableName) {
                if (position != null && position == i) {
                    continue
                }

                return true
            }
        }
        return false
    }

    private fun checkTakenNickname() : Boolean {
        var modifiableNick = tempPlayer!!.nickname.toLowerCase()

        for ((i, playerProfile) in PlayerProfile.playerProfiles.withIndex()) {
            var nickFromList = playerProfile.nickname.toLowerCase()

            if (nickFromList == modifiableNick) {
                if (position != null && position == i) {
                    continue
                }

                return true;
            }
        }
        return false
    }

    private fun createColorPicker(colorPicker : Button) {
        var builder = ColorPickerDialog.Builder(activity)
        builder.setTitle("Szín")
        builder.attachBrightnessSlideBar(true)
        builder.attachAlphaSlideBar(false)
        builder.setBottomSpace(12)

        builder.setPositiveButton(
            activity?.getString(R.string.chooseColor),
            ColorEnvelopeListener { envelope, which ->
                if (colorPicker == backColorButton) {
                    tempPlayer?.backgroundColor = envelope.color
                    backColorButton?.setBackgroundColor(envelope.color)
                }

                if (colorPicker == textColorButton) {
                    tempPlayer?.textColor = envelope.color
                    textColorButton?.setBackgroundColor(envelope.color)
                }
            })
        builder.setNegativeButton(
            activity?.getString(R.string.cancel),
            DialogInterface.OnClickListener { dialog, which ->
                dialog.dismiss();
            })

        var colorPickerView : ColorPickerView = builder.colorPickerView
        colorPickerView.flagView = CustomFlag(activity, R.layout.color_flag_layout)

        if (colorPicker == backColorButton) {
            colorPickerView.pureColor = tempPlayer?.backgroundColor!!
        }

        if (colorPicker == textColorButton) {
            colorPickerView.pureColor = tempPlayer?.textColor!!
        }

        builder.show()
    }

    private inner class CustomFlag(context: Context?, layout: Int) : FlagView(context, layout) {
        private val textView: TextView = findViewById(R.id.flag_color_code)
        private val alphaTileView: AlphaTileView = findViewById(R.id.flag_color_layout)

        override fun onRefresh(colorEnvelope: ColorEnvelope) {
            textView.text = "#" + colorEnvelope.hexCode
            alphaTileView.setPaintColor(colorEnvelope.color)
        }
    }

    override fun onCancel(dialog: DialogInterface) {
        super.onCancel(dialog)

        var confirmPlayerCreatorCancel = ConfirmPlayerCreatorCancel()

        if (position == null) {
            if (tempPlayer!!.name != "" || tempPlayer!!.nickname != "" || tempPlayer!!.backgroundColor != defaultBackgroundColor || tempPlayer!!.textColor != defaulTextColor) {
                confirmPlayerCreatorCancel.show(parentFragmentManager, "CANCELCREATOR")
            }
            else {
                position = null
                tempPlayer = null
            }
        }
        else {
            var player = PlayerProfile.playerProfiles[position!!]

            if (tempPlayer!!.name != player.name || tempPlayer!!.nickname != player.nickname || tempPlayer!!.backgroundColor != player.backgroundColor || tempPlayer!!.textColor != player.textColor) {
                confirmPlayerCreatorCancel.show(parentFragmentManager, "CANCELCREATOR")
            }
            else {
                position = null
                tempPlayer = null
            }
        }
    }
}