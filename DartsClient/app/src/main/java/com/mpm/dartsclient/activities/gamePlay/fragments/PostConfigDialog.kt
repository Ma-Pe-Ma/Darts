package com.mpm.dartsclient.activities.gamePlay.fragments

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.DialogFragment
import com.mpm.dartsclient.MessageHandler
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.gamePlay.GamePlay

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
            MessageHandler.requestGameStart(0)
        }

        reversed?.setOnClickListener {
            MessageHandler.requestGameStart(1)
        }

        same?.setOnClickListener {
            MessageHandler.requestGameStart(2)
        }

        back?.setOnClickListener {
            requireActivity().finish()
            //MessageHandler.sendGameConfig()

            //val intent = Intent(requireActivity(), Config::class.java)
            //startActivity(intent)
        }

        return view
    }

}