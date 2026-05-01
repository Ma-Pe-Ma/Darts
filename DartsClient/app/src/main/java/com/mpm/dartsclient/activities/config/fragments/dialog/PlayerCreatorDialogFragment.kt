package com.mpm.dartsclient.activities.config.fragments.dialog

import android.app.AlertDialog
import android.app.Dialog
import android.content.Context
import android.content.DialogInterface
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.fragment.app.DialogFragment
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.sqlhelper.SQLTables
import com.skydoves.colorpickerview.AlphaTileView
import com.skydoves.colorpickerview.ColorEnvelope
import com.skydoves.colorpickerview.ColorPickerDialog
import com.skydoves.colorpickerview.ColorPickerView
import com.skydoves.colorpickerview.flag.FlagView
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener

class PlayerCreatorDialogFragment : DialogFragment {
    var existingPlayer : PlayerProfile? = null

    //GUI elements
    var backColorButton : Button? = null
    var textColorButton : Button? = null
    var nameField : EditText? = null
    var nicknameField : EditText? = null

    val DEFAULT_BACKGROUND_COLOR = 0
    val DEFAULT_TEXT_COLOR = -16777216

    var profileContainer: ProfileContainer;

    constructor(existingPlayer: PlayerProfile?, profileContainer: ProfileContainer) : super() {
        this.profileContainer = profileContainer
        this.existingPlayer = existingPlayer
    }

    override fun onResume() {
        super.onResume()

        val width = (resources.displayMetrics.widthPixels * 0.90).toInt()
        dialog?.window?.setLayout(width, ViewGroup.LayoutParams.WRAP_CONTENT)
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

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        val dialogView: View = inflater.inflate(R.layout.fragment_player_creator, null)

        //initialize GUI elements
        createGUIElements(dialogView)

        fun showInvalidValueDialog(title: String, message: String) {
            var builder = AlertDialog.Builder(activity)
            builder.setTitle(title)
            builder.setMessage(message)
            builder.setPositiveButton(
                "Értem",
                DialogInterface.OnClickListener { _: DialogInterface, _: Int ->
                })
            builder.create().show()
        }

        var positiveButton = dialogView.findViewById<Button>(R.id.confirmPlayer)
        positiveButton.setOnClickListener {
            var newName: String? = nameField?.text.toString()
            var newNick: String? = nicknameField?.text.toString()

            if (newName.isNullOrEmpty()) {
                showInvalidValueDialog("Hiányzó név!", "Egyik név mező sem lehet üres")
            }
            else if (newNick.isNullOrEmpty()) {
                showInvalidValueDialog("Hiányzó szín!", "Adjon meg egy színt")
            }
            else if (profileContainer.checkTakenNickname(existingPlayer, newNick)) {
                showInvalidValueDialog("Foglalt becenév", "Adjon meg egy egyedi becenevet!")
            }
            else {
                val imm: InputMethodManager? = activity?.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager?
                imm?.hideSoftInputFromWindow(view?.windowToken, 0)

                if (existingPlayer == null) {
                    val newId = SQLTables.PlayersTable.addNewPlayer(newName, newNick, 0,0)?.toInt()
                    val newProfile = PlayerProfile(newId as Int, newName, newNick, (backColorButton!!.background as ColorDrawable).color, (textColorButton!!.background as ColorDrawable).color)
                    profileContainer.playerProfiles.add(newProfile)
                }
                else {
                    existingPlayer!!.name = newName
                    existingPlayer!!.nickname = newNick
                    existingPlayer!!.textColor = (textColorButton!!.background as ColorDrawable).color
                    existingPlayer!!.backgroundColor = (backColorButton!!.background as ColorDrawable).color

                    SQLTables.PlayersTable.updatePlayer(existingPlayer!!.id, existingPlayer!!.name, existingPlayer!!.nickname, existingPlayer!!.textColor!!, existingPlayer!!.backgroundColor!!)
                }

                (activity as Config).notifyAboutModifiedPlayerEntry()
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

        //set the proper values for the gui elements
        nameField?.setText(existingPlayer?.name)
        nicknameField?.setText(existingPlayer?.nickname)
        backColorButton?.setBackgroundColor(if (existingPlayer?.backgroundColor != null) existingPlayer?.backgroundColor!! else DEFAULT_BACKGROUND_COLOR)
        textColorButton?.setBackgroundColor(if (existingPlayer?.textColor != null) existingPlayer?.textColor!! else DEFAULT_TEXT_COLOR)

        backColorButton?.setOnClickListener {
            createColorPicker(backColorButton!!)
        }

        textColorButton?.setOnClickListener {
            createColorPicker(textColorButton!!)
        }
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
                colorPicker.setBackgroundColor(envelope.color)
            })
        builder.setNegativeButton(
            activity?.getString(R.string.cancel),
            DialogInterface.OnClickListener { dialog, which ->
                dialog.dismiss();
            })

        var colorPickerView : ColorPickerView = builder.colorPickerView
        colorPickerView.flagView = CustomFlag(activity, R.layout.color_flag_layout)

        if (colorPicker == backColorButton) {
            colorPickerView.pureColor = existingPlayer?.backgroundColor ?: DEFAULT_BACKGROUND_COLOR
        }
        else if (colorPicker == textColorButton) {
            colorPickerView.pureColor = existingPlayer?.textColor ?: DEFAULT_TEXT_COLOR
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

        override fun onFlipped(p0: Boolean?) {
            //TODO("Not yet implemented")
        }
    }

    override fun onCancel(dialog: DialogInterface) {
        super.onCancel(dialog)
    }
}