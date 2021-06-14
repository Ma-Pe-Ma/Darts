package com.mpm.dartsclient.activities.config.adapters

import android.app.Activity
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.ExpandableCardView
import com.mpm.dartsclient.loadedSQLData.LoadedMatch


class StatisticsPlayerAdapter(var activity: Activity) :  RecyclerView.Adapter<StatisticsPlayerAdapter.Player>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): Player {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)

        return Player(view)
    }

    override fun onBindViewHolder(holder: Player, position: Int) {
        var player = PlayerProfile.chosenPlayerProfiles[position]
        holder.resultPos. apply {
            text = position.toString()
        }

        setupResultHeader(holder.resultHeader)

        var resultAdapter = StatisticsResultAdapter(LoadedMatch.playerStatistics[player]!!.second)

        holder.statisticsID.text = "${(position + 1)}."
        holder.statisticsPlayerNameText.text = player.nickname
        holder.statisticsPlayerNameText.setTextColor(player.textColor!!)
        holder.statisticsPlayerNameText.setBackgroundColor(player.backgroundColor!!)

        if (LoadedMatch.loadedMatches.size > 0) {
            holder.statisticsAveragePositionValue.text = LoadedMatch.playerStatistics[player]!!.first.averagePosition.toString()
            holder.statisticsAverageRoundValue.text = LoadedMatch.playerStatistics[player]!!.first.averageRound.toString()
            holder.statisticsAverageScoreValue.text = LoadedMatch.playerStatistics[player]!!.first.averageScore.toString()
        }
        else {
            holder.statisticsAveragePositionValue.text = "-"
            holder.statisticsAverageRoundValue.text = "-"
            holder.statisticsAverageScoreValue.text = "-"
        }

        holder.resultRecycler?.apply {
            adapter = resultAdapter
            layoutManager = LinearLayoutManager(activity)
        }

        holder.cardView.measure(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        holder.resultRecycler?.measure(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        holder.resultHeader?.measure(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        holder.statisticsPlayer?.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED)
        holder.cardView.resultRecyclerListWithHeader?.measure(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)

        //holder.cardView.expansionHeight = holder.resultRecycler!!.measuredHeight + holder.resultHeader!!.measuredHeight
        //holder.cardView.collapsedHeight = holder.cardView.measuredHeight - holder.cardView.expansionHeight

        holder.cardView.collapsedHeight = holder.statisticsPlayer!!.measuredHeight
        holder.cardView.expansionHeight = holder.cardView.measuredHeight - holder.cardView.collapsedHeight

        holder.cardView.layoutParams.height = holder.cardView.collapsedHeight
        holder.cardView.setBackgroundColor(activity.getColor(R.color.LIGHTGRAY))

        holder.resultHeader.setBackgroundColor(activity.getColor(R.color.DARKGREY))
        holder.resultRecycler.setBackgroundColor(activity.getColor(R.color.DARKGREY))

        holder.cardView.resultRecyclerListWithHeader?.visibility = View.INVISIBLE

        if (LoadedMatch.loadedMatches.size > 0) {
            holder.cardView.setOnClickListener {
                //holder.cardView.toggle()
                if (holder.cardView.resultRecyclerListWithHeader!!.visibility == View.INVISIBLE) {
                    holder.cardView.expand()
                }
                else {
                    holder.cardView.collapse()
                }
            }
        }
    }

    override fun getItemCount(): Int {
        return PlayerProfile.chosenPlayerProfiles.size
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.statistics_player
    }

    class Player(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var cardView = itemView as ExpandableCardView
        var statisticsID = cardView.findViewById<TextView>(R.id.statisticsPlayerID)
        var statisticsPlayerNameText = cardView.findViewById<TextView>(R.id.statisticsPlayerNameText)
        //var statisticsAveragePositionKey = cardView.findViewById<TextView>(R.id.statisticsAveragePositionKey)
        var statisticsAveragePositionValue = cardView.findViewById<TextView>(R.id.statisticsAveragePositionValue)
        //var statisticsAverageRoundKey = cardView.findViewById<TextView>(R.id.statisticsAverageRoundKey)
        var statisticsAverageRoundValue = cardView.findViewById<TextView>(R.id.statisticsAverageRoundValue)
        //var statisticsAverageScoreKey = cardView.findViewById<TextView>(R.id.statisticsAverageScoreKey)
        var statisticsAverageScoreValue = cardView.findViewById<TextView>(R.id.statisticsAverageScoreValue)

        var statisticsPlayer = cardView.findViewById<ConstraintLayout>(R.id.statisticsGeneralAverage)

        var resultHeader = cardView.findViewById<ConstraintLayout>(R.id.statisticsResultHeader)
        var resultRecycler = cardView.findViewById<RecyclerView>(R.id.statisticsResultRecycler)

        var resultPos = resultHeader.findViewById<TextView>(R.id.resultSequenceNr)

        init {
            cardView.resultRecyclerListWithHeader = cardView.findViewById(R.id.statisticResultListWithHeader)
        }
    }

    fun setupResultHeader(resultHeader : ConstraintLayout) {
        var sequenceNr = resultHeader.findViewById<TextView>(R.id.resultSequenceNr)
        sequenceNr.text = activity.getString(R.string.posAbbr)

        var positionRate = resultHeader.findViewById<TextView>(R.id.positionRate)
        positionRate.text = activity.getString(R.string.statisticsPositionRate)

        var scoreAverage = resultHeader.findViewById<TextView>(R.id.scoreAverage)
        scoreAverage.text = activity.getString(R.string.statisticsAverageScoreKey)

        var roundAverage = resultHeader.findViewById<TextView>(R.id.roundAverage)
        roundAverage.text = activity.getString(R.string.statisticsAverageRoundKey)
    }
}