package com.mpm.dartsclient.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.DialogFragment
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.GamePlay
import kotlinx.android.synthetic.main.post_config_dialog_fragment.*

class PostConfigDialog : DialogFragment() {

    var cyclical : Button? = null
    var reversed : Button? = null
    var same : Button? = null
    var back : Button? = null

    override fun onResume() {
        super.onResume()
        dialog!!.window!!.setLayout(700, 1100)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.post_config_dialog_fragment, container, false)

        cyclical = view.findViewById(R.id.cyclicalButtonID)
        reversed = view.findViewById(R.id.reversedButtonID)
        same = view.findViewById(R.id.sameButtonID)
        back = view.findViewById(R.id.backToMenu)

        cyclical?.setOnClickListener {
            (activity as GamePlay).setPostProcess(0, true)
        }

        reversed?.setOnClickListener {
            (activity as GamePlay).setPostProcess(1, true)
        }

        same?.setOnClickListener {
            (activity as GamePlay).setPostProcess(2, true)
        }

        back?.setOnClickListener {

        }

        return view
    }

}