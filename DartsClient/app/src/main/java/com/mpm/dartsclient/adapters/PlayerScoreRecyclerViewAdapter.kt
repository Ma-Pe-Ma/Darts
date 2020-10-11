package com.mpm.dartsclient.adapters

import android.app.Activity
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R


class PlayerScoreRecyclerViewAdapter(var activity : Activity) : RecyclerView.Adapter<PlayerScoreRecyclerViewAdapter.PlayerScoreHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerScoreHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)
        return PlayerScoreHolder(view)
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.player_score_row
    }

    override fun onBindViewHolder(holder: PlayerScoreHolder, position: Int) {
        holder.sequencNr.text = "" +(position +1) +"."
        if (position == PlayerProfile.currentCursor) {
            holder.sequencNr.setBackgroundColor(activity.getColor(R.color.PINK))
        }

        var player = PlayerProfile.chosenPlayerProfiles[position]
        holder.playerName.text = player.nickname
        holder.playerName.setBackgroundColor(player.color)

        holder.score.text = "" + player.score!!.score

        holder.holderView.setOnClickListener {
            activity
        }
    }

    override fun getItemCount(): Int {
        return PlayerProfile.chosenPlayerProfiles.size
    }

    class PlayerScoreHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
        var sequencNr = holderView.findViewById<TextView>(R.id.scoreSequenceNr)
        var playerName = holderView.findViewById<TextView>(R.id.scorePlayerName)
        var score = holderView.findViewById<TextView>(R.id.scoreScore)
    }
}