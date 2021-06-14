package com.mpm.dartsclient.activities.gamePlay.adapters

import android.app.Activity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R

//this adapter is used for two cases, when score is checked, and when the (partial) results are shown!
class PlayerScoreRecyclerViewAdapter(var activity : Activity, var cursor : Int, var result : Boolean, var playerProfiles : MutableList<PlayerProfile>) : RecyclerView.Adapter<PlayerScoreRecyclerViewAdapter.PlayerScoreHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerScoreHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)
        return PlayerScoreHolder(view)
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.player_score_row
    }

    override fun onBindViewHolder(holder: PlayerScoreHolder, position: Int) {
        holder.sequencNr.text = "" +(position + 1) +"."

        var player = playerProfiles[position]
        holder.playerName.text = player.nickname
        holder.playerName.setBackgroundColor(player.backgroundColor!!)
        holder.playerName.setTextColor(player.textColor!!)
        holder.score.text = "" + player.score!!.score
        holder.roundCounter.text = player.score!!.roundCount.toString()

        if (player.score!!.position == 0) {
            holder.roundCounter.text = holder.roundCounter.text.toString() + "*"
        }

        if (position == cursor) {
            holder.holderView.setBackgroundColor(activity.getColor(R.color.PINK))
        }

        if (result) {
            var idOfPlayer = -1;
            for ((i, chosenPlayer) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
                if (chosenPlayer == player) {
                    idOfPlayer = i
                    break
                }
            }

            holder.position.text = (idOfPlayer + 1).toString()
        }
        else {
            holder.position.text = if (player.score!!.position == 0) "-" else player.score!!.position.toString()
        }

        holder.holderView.setOnClickListener {
            if (result) {

            }
            else {

            }
        }
    }

    override fun getItemCount(): Int {
        return playerProfiles.size
    }

    class PlayerScoreHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
        var sequencNr = holderView.findViewById<TextView>(R.id.scoreSequenceNr)
        var playerName = holderView.findViewById<TextView>(R.id.scorePlayerName)
        var score = holderView.findViewById<TextView>(R.id.scoreScore)
        var roundCounter = holderView.findViewById<TextView>(R.id.scoreRoundCounter)
        var position = holderView.findViewById<TextView>(R.id.scorePosition)
    }
}