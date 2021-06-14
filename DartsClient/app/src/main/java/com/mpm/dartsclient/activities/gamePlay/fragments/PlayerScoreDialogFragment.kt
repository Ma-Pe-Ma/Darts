package com.mpm.dartsclient.activities.gamePlay.fragments

import android.app.Activity
import android.graphics.Color
import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.gamePlay.adapters.PlayerScoreRecyclerViewAdapter

class PlayerScoreDialogFragment : DialogFragment() {
    var recyclerView : RecyclerView? = null
    var title : TextView? = null

    override fun onResume() {
        super.onResume()
        var height = 100 + PlayerProfile.chosenPlayerProfiles.size * 200

        if (height > 1000) {
            height = 1000
        }

        dialog!!.window!!.setLayout(700, height)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        var view = inflater.inflate(R.layout.fragment_player_score, container, false)

        var playerList = view.findViewById<ConstraintLayout>(R.id.playerScoreList)
        var explainingRow = playerList.findViewById<ConstraintLayout>(R.id.explainingRow)
        setExplainingRow(explainingRow)

        recyclerView = playerList.findViewById(R.id.playerRecyclerList)
        title = view.findViewById(R.id.playerScoreListTitle)

        val itemDecoration =
            DividerItemDecoration(recyclerView!!.context, DividerItemDecoration.VERTICAL)
        val drawable = GradientDrawable(
            GradientDrawable.Orientation.BOTTOM_TOP,
            intArrayOf(Color.BLACK, Color.BLACK)
        )

        drawable.setSize(1, 2)
        itemDecoration.setDrawable(drawable)

        recyclerView!!.addItemDecoration(itemDecoration)

        title!!.text = getString(R.string.playerScores)

        var playerScoreRecyclerViewAdapter = PlayerScoreRecyclerViewAdapter(activity as Activity, PlayerProfile.currentCursor, false, PlayerProfile.chosenPlayerProfiles)

        playerScoreRecyclerViewAdapter.notifyDataSetChanged()

        recyclerView!!.apply {
            adapter =  playerScoreRecyclerViewAdapter
            layoutManager = LinearLayoutManager(activity)
        }

        return view
    }

    private fun setExplainingRow(explainingRow : View) {
        var sequenceNr = explainingRow!!.findViewById<TextView>(R.id.scoreSequenceNr)
        sequenceNr.text = requireActivity().getString(R.string.hashChar)

        var nick = explainingRow!!.findViewById<TextView>(R.id.scorePlayerName)
        nick.text = requireActivity().getString(R.string.nick)

        var score = explainingRow!!.findViewById<TextView>(R.id.scoreScore)
        score.text = requireActivity().getString(R.string.points)

        var round = explainingRow!!.findViewById<TextView>(R.id.scoreRoundCounter)
        round.text = requireActivity().getString(R.string.round)

        var position = explainingRow!!.findViewById<TextView>(R.id.scorePosition)
        position.text = requireActivity().getString(R.string.posAbbr)
    }
}