package com.mpm.dartsclient.activities.config.fragments.dialog

import android.app.AlertDialog
import android.app.Dialog
import android.content.DialogInterface
import android.os.Bundle
import androidx.fragment.app.DialogFragment

class NeedPlayers : DialogFragment() {

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val builder = AlertDialog.Builder(activity)

        builder.setTitle("Nem adott meg játékosokat")
        builder.setMessage("Definiáljon játékosokat!")
        builder.setPositiveButton(
            "Jah",
            DialogInterface.OnClickListener { _, _ -> }
        )

        return builder.create()
    }

}