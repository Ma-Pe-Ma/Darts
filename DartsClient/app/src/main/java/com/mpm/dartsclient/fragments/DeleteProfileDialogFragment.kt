package com.mpm.dartsclient.fragments

import android.content.Context
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import androidx.fragment.app.DialogFragment
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.Config


class DeleteProfileDialogFragment(var position : Int) : DialogFragment() {

    override fun onResume() {
        super.onResume()
        dialog!!.window!!.setLayout(700, 1100)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        var view = inflater.inflate(R.layout.delete_profile, container, false)

        var okButton = view.findViewById<Button>(R.id.okButton)
        okButton.isEnabled = false
        okButton.setOnClickListener {
            PlayerProfile.playerProfiles.removeAt(position)
            (activity as Config).notifyPlayerListConfigurerDialog(position)
            dialog?.dismiss()
        }

        var confirmText = view.findViewById<EditText>(R.id.deleteConfirmText)

        confirmText?.addTextChangedListener( object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                //TODO("Not yet implemented")
            }

            override fun beforeTextChanged(
                s: CharSequence?,
                start: Int,
                count: Int,
                after: Int
            ) {
                //TODO("Not yet implemented")
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                okButton.isEnabled = s.toString() == activity?.getString(R.string.confirmString)
            }
        }

        )

        val imm: InputMethodManager = context?.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        imm.showSoftInput(confirmText, InputMethodManager.SHOW_IMPLICIT)

        return view
    }
}