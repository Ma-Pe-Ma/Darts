package com.mpm.dartsclient.fragments

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
import com.mpm.dartsclient.activities.Config
import com.skydoves.colorpickerview.AlphaTileView
import com.skydoves.colorpickerview.ColorEnvelope
import com.skydoves.colorpickerview.ColorPickerDialog
import com.skydoves.colorpickerview.ColorPickerView
import com.skydoves.colorpickerview.flag.FlagView
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener


class PlayerCreatorDialogFragment() : DialogFragment() {

    var position = -1
    var color : Button? = null

    var name : EditText? = null
    var nickname : EditText? = null

    var nameValue : String = ""
    var nicknameValue : String = ""
    var colorValue : Int? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        arguments?.let {

        }
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        //return super.onCreateDialog(savedInstanceState)
        return object : Dialog(requireActivity(), theme) {
            override fun cancel() {
                if (activity != null && view != null) {
                    val imm =
                        activity!!.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
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

        position = requireArguments().getInt("position", -1)
        color = dialogView.findViewById<Button>(R.id.colorButton)

        createTexts(dialogView)

        var prevName = requireArguments().getString("name")
        var prevNickname = requireArguments().getString("nickname")
        var prevColor = requireArguments().getInt("color", -1)

        if (prevName != null || prevNickname != null || prevColor != -1) {
            name?.setText(prevName)
            nickname?.setText(prevNickname)
            color?.setBackgroundColor(prevColor)
            colorValue = prevColor
        }
        else if (position != -1) {
            name?.setText(PlayerProfile.playerProfiles[position].name)
            nickname?.setText(PlayerProfile.playerProfiles[position].nickname)
            color?.setBackgroundColor(PlayerProfile.playerProfiles[position].color)
            colorValue = PlayerProfile.playerProfiles[position].color

            (activity as Config).prevPlayerProfile = PlayerProfile(
                PlayerProfile.playerProfiles[position].name,
                PlayerProfile.playerProfiles[position].nickname,
                PlayerProfile.playerProfiles[position].color
            )
        }

        val imm: InputMethodManager? = activity?.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager?
        imm?.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0)

        name?.requestFocus()
        name?.setSelection(name?.text!!.length)

        var titleText = dialogView.findViewById<TextView>(R.id.creatorTitle)
        titleText.text = getString(R.string.creatorTitle)

        var positiveButton = dialogView.findViewById<Button>(R.id.confirmPlayer)
        positiveButton.setOnClickListener {
            nameValue = name?.text.toString()
            nicknameValue = nickname?.text.toString()

            if (nameValue == "" || nicknameValue == "") {
                var builder = AlertDialog.Builder(activity)
                builder.setTitle("Üres mező!")
                builder.setMessage("Egyik mező sem lehet üres")
                builder.setPositiveButton(
                    "Jah",
                    DialogInterface.OnClickListener { _: DialogInterface, _: Int ->

                    })

                builder.create().show()
            }
            else if (colorValue == null) {
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

                (activity as Config).prevPlayerProfile = null

                if (position == -1) {
                    PlayerProfile.playerProfiles.add(
                        PlayerProfile(
                            nameValue,
                            nicknameValue,
                            colorValue!!
                        )
                    )
                }
                else {
                    var player = PlayerProfile.playerProfiles[position]

                    Log.i("DARTS", "név: " + nameValue)
                    player.name = nameValue
                    player.nickname = nicknameValue
                    player.color = colorValue!!
                }
                (activity as Config).notifyPlayerListConfigurerDialog(position)
                dialog?.dismiss()
            }
        }

        color?.setOnClickListener {
            createColorPicker()
        }

        return dialogView
    }

    private fun createTexts(dialogView: View) {
        name = dialogView.findViewById(R.id.playerNameTextValue)
        nickname = dialogView.findViewById(R.id.playerNicknameTextValue)

        name?.addTextChangedListener(object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                //TODO("Not yet implemented")
            }

            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
                //TODO("Not yet implemented")
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                nameValue = s.toString()
                (activity as Config).prevPlayerProfile?.name = nameValue
            }
        })

        nickname?.addTextChangedListener(object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                //TODO("Not yet implemented")
            }

            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
                //TODO("Not yet implemented")
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                nicknameValue = s.toString()
                (activity as Config).prevPlayerProfile?.nickname = nicknameValue
            }
        })

    }

    private fun checkTakenName() : Boolean {
        var nameValueLowerCase = nameValue.toLowerCase()

        var i : Int = 0
        for (playerProfile in PlayerProfile.playerProfiles) {
            var lowerProfileName = playerProfile.name.toLowerCase()
            if (lowerProfileName == nameValueLowerCase && position != i) {
                return true
            }
            i++
        }
        return false
    }

    private fun checkTakenNickname() : Boolean {
        var nicknameValueLowerCase = nicknameValue.toLowerCase()

        var i : Int = 0
        for (playerProfile in PlayerProfile.playerProfiles) {
            var lowerProfileNickname = playerProfile.nickname.toLowerCase()
            if (lowerProfileNickname == nicknameValueLowerCase && position != i) {
                return true
            }
            i++
        }
        return false
    }

    private fun createColorPicker() {
        var builder = ColorPickerDialog.Builder(activity)
        builder.setTitle("Szín")
        builder.attachBrightnessSlideBar(true)
        builder.attachAlphaSlideBar(false)
        builder.setBottomSpace(12)

        builder.setPositiveButton(
            activity?.getString(R.string.chooseColor),
            ColorEnvelopeListener { envelope, which ->
                color?.setBackgroundColor(envelope.color)

                colorValue = envelope.color
                Log.i("DARTS", "color: " + colorValue)
                (activity as Config).prevPlayerProfile?.color = colorValue!!
            })
        builder.setNegativeButton(
            activity?.getString(R.string.cancel),
            DialogInterface.OnClickListener { dialog, which ->
                dialog.dismiss();
            })

        var colorPickerView : ColorPickerView = builder.colorPickerView
        colorPickerView.flagView = CustomFlag(activity, R.layout.color_flag_layout)

        if (colorValue != null) {
            colorPickerView.pureColor = colorValue!!
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

    companion object {
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            PlayerCreatorDialogFragment().apply {
                arguments = Bundle().apply {

                }
            }
    }
}