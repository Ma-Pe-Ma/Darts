package com.mpm.dartsclient.activities.config.adapters

import android.util.Log
import android.view.LayoutInflater
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.loadedSQLData.AverageScore
import kotlinx.android.synthetic.main.player_config_row.view.*

class StatisticsResultAdapter(var resultMap : HashMap<Int, AverageScore>) : RecyclerView.Adapter<StatisticsResultAdapter.Result>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) : Result {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)

        return Result(view)
    }

    override fun onBindViewHolder(holder : Result, position: Int) {
        var score = resultMap[position + 1]!!

        holder.sequenceNr.text = (position + 1).toString() +"."
        holder.positionRate.text = (score.averagePosition * 100).toInt().toString() + "%"

        if (score.averagePosition > 0.0f) {
            holder.averageRound.text = score.averageRound.toString()
            holder.averageScore.text = score.averageScore.toString()
        }
        else {
            holder.averageRound.text = "-"
            holder.averageScore.text = "-"
        }
    }

    override fun getItemCount(): Int {
        return resultMap.size
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.statistics_result
    }

    class Result(view: View) : RecyclerView.ViewHolder(view) {
        var holderView = view
        var sequenceNr = view.findViewById<TextView>(R.id.resultSequenceNr)
        var averageScore = view.findViewById<TextView>(R.id.scoreAverage)
        var averageRound = view.findViewById<TextView>(R.id.roundAverage)
        var positionRate = view.findViewById<TextView>(R.id.positionRate)
    }
}