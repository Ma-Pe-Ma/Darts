package com.mpm.dartsclient.activities.config.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.adapters.PlayerListSelectorRecyclerViewAdapter
import com.mpm.dartsclient.activities.config.fragments.dialog.PlayerListConfigurerDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer

class PlayerConfigFragment(var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) : Fragment(), Config.FragmentCommunicator {

    var recyclerView : RecyclerView? = null
    var addPlayerButton: Button? = null

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

        addPlayerButton = view.findViewById(R.id.newPlayerButton)
        addPlayerButton?.setOnClickListener {
            var playerConfigDialog = PlayerListConfigurerDialogFragment(profileContainer, matchContainer, dartsGameContainer)
            playerConfigDialog.show(parentFragmentManager, "PlayerConfig")
        }

        // Inflate the layout for this fragment
        return view
    }

    //adding existing player
    override fun notifyPlayerConfigFragment(position: Int) {
        profileContainer.chosenPlayerProfiles.add(profileContainer.playerProfiles[position])
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()

        var dialogFragment = parentFragmentManager.findFragmentByTag("PlayerConfig") as DialogFragment
        dialogFragment.dismiss()

        for (player in profileContainer.playerProfiles) {
            player.score = dartsGameContainer.currentGame!!.getScoreObject()
        }
    }

    //When new player created or existing modified
    override fun notifyAboutModifiedPlayerEntry() {
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()
    }
}