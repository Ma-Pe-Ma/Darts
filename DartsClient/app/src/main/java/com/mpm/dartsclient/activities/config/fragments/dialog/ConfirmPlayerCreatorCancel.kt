package com.mpm.dartsclient.activities.config.fragments.dialog

import android.app.AlertDialog
import android.app.Dialog
import android.content.DialogInterface
import android.os.Bundle
import android.util.Log
import androidx.core.os.bundleOf
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.setFragmentResult

class ConfirmPlayerCreatorCancel() : DialogFragment() {

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        var builder = AlertDialog.Builder(activity)

        builder.setTitle("Biztos?")
        builder.setPositiveButton(
            "Igen",
            DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                setFragmentResult("CANCEL_REQUEST", bundleOf("result" to true))
            })

        builder.setNegativeButton(
            "Nem",
            DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                setFragmentResult("CANCEL_REQUEST", bundleOf("result" to false))
            })

        return builder.create()
    }

    override fun onCancel(dialog: DialogInterface) {
        super.onCancel(dialog)
        setFragmentResult("CANCEL_REQUEST", bundleOf("result" to true))
    }
}