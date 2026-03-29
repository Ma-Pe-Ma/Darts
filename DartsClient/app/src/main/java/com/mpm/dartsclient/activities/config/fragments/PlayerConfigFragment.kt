package com.mpm.dartsclient.activities.config.fragments

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.adapters.PlayerListSelectorRecyclerViewAdapter
import com.mpm.dartsclient.activities.config.fragments.dialog.StatisticsDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer


class PlayerFragment(var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) : Fragment(), Config.FragmentCommunicator {

    var recyclerView : RecyclerView? = null
    var statisticsButton : Button? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        (activity as Config).subscribeToCommunicator(this)
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        val view = inflater.inflate(R.layout.fragment_player_config, container, false)

        recyclerView = view.findViewById(R.id.chosenPlayers)

        recyclerView?.apply {
            adapter = PlayerListSelectorRecyclerViewAdapter(activity as FragmentActivity, profileContainer, matchContainer, dartsGameContainer)
            layoutManager = LinearLayoutManager(activity)
        }

        statisticsButton = view.findViewById(R.id.statisticsButton)

        statisticsButton?.setOnClickListener {
            var statisticsDialog = StatisticsDialogFragment(profileContainer, matchContainer)
            statisticsDialog.show(parentFragmentManager, "STATISTICS")
        }

        // Inflate the layout for this fragment
        return view
    }

    //adding existing player
    override fun notifyPlayerConfigFragment(position: Int) {
        profileContainer.chosenPlayerProfiles.add(profileContainer.playerProfiles[position])
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()

        var dialogFragment = parentFragmentManager.findFragmentByTag("PlayerConfigDialog") as DialogFragment
        dialogFragment.dismiss()

        for (player in profileContainer.playerProfiles) {
            player.score = dartsGameContainer.currentGame!!.getScoreObject()
        }

        statisticsButton?.isEnabled = profileContainer.chosenPlayerProfiles.size != 0
    }

    //When new player created or existing modified
    override fun notifyAboutModifiedPlayerEntry(position: Int) {
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()
    }

    override fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        //TODO("Not yet implemented")
    }

    override fun notifyStatisticsButton() {
        if (profileContainer.chosenPlayerProfiles.size == 0) {
            statisticsButton?.isEnabled = false
        }
    }

    override fun onResume() {
        super.onResume()

        if (profileContainer.chosenPlayerProfiles.size == 0) {
            statisticsButton?.isEnabled = false
        }
    }
}