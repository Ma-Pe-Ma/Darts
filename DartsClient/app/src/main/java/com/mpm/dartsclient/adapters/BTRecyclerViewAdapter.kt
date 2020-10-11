package com.mpm.dartsclient.adapters

import android.app.Activity
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.core.view.isVisible
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.StartActivity

class BTRecyclerViewAdapter(var activity : Activity) : RecyclerView.Adapter<BTRecyclerViewAdapter.BTHolder>() {
    var deviceList: MutableList<String>? = null


    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): BTHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)

        Log.i("DARTS", "DEVICE SIZE: " + deviceList!!.size)

        return BTHolder(view)
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.player_row
    }

    override fun onBindViewHolder(holder: BTHolder, position: Int) {

        holder.holderView.apply {
            findViewById<TextView>(R.id.sequenceNr).text = "" + (position + 1)
            findViewById<TextView>(R.id.playerName).text = deviceList!![position]
            var removeButton = findViewById<ImageButton>(R.id.removeButton)
            removeButton.isEnabled = false
            removeButton.isVisible = false

            var editPlayer = findViewById<ImageButton>(R.id.editPlayer)
            editPlayer.isEnabled = false
            editPlayer.isVisible = false
        }

        holder.holderView.setOnClickListener {
            (activity as StartActivity).closeBTDialog(deviceList!![position])
        }
    }

    override fun getItemCount(): Int {
        return deviceList!!.size
    }

    class BTHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
    }


}