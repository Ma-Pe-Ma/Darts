package com.mpm.dartsclient.activities.config.fragments.dialog

import android.app.AlertDialog
import android.app.Dialog
import android.content.DialogInterface
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.DialogFragment

class ConfirmPlayerCreatorCancel() : DialogFragment() {

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        var builder = AlertDialog.Builder(activity)

        builder.setTitle("Biztos?")
        builder.setPositiveButton(
            "Jah",
            DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                PlayerCreatorDialogFragment.tempPlayer = null
                PlayerCreatorDialogFragment.position = null
            })

        builder.setNegativeButton(
            "Nope",
            DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                var playerCreatorDialogFragment = PlayerCreatorDialogFragment()
                playerCreatorDialogFragment.show(parentFragmentManager, "CREATOR")
            })

        return builder.create()
    }

    override fun onCancel(dialog: DialogInterface) {
        super.onCancel(dialog)
        PlayerCreatorDialogFragment.tempPlayer = null
        PlayerCreatorDialogFragment.position = null
    }
}