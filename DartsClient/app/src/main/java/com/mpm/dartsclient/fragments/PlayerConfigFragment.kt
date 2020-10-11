package com.mpm.dartsclient.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.Config
import com.mpm.dartsclient.adapters.PlayerListSelectorRecyclerViewAdapter
import com.mpm.dartsclient.games.DartsGameContainer


class PlayerFragment : Fragment(), Config.FragmentCommunicator {

    var recyclerView : RecyclerView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

    }

    override fun onAttach(context: Context) {
        super.onAttach(context)

        (activity as Config).subscribeToCommunicator(this)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        var view = inflater.inflate(R.layout.fragment_player_config, container, false)

        recyclerView = view.findViewById(R.id.chosenPlayers)

        recyclerView?.apply {
            adapter = PlayerListSelectorRecyclerViewAdapter(activity as FragmentActivity)
            layoutManager = LinearLayoutManager(activity)
        }

        // Inflate the layout for this fragment
        return view
    }



    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment PlayerFragment.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            PlayerFragment().apply {
                arguments = Bundle().apply {
                 
                }
            }
    }

    //adding existing player
    override fun notifyPlayerConfigFragment(position: Int) {
        PlayerProfile.chosenPlayerProfiles.add(PlayerProfile.playerProfiles[position])
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()

        var dialogFragment = parentFragmentManager.findFragmentByTag("PlayerConfigDialog") as DialogFragment
        dialogFragment.dismiss()

        for (player in PlayerProfile.playerProfiles) {
            player.score = DartsGameContainer.currentGame!!.getScoreObject()
        }
    }

    //When new player created or existing modified
    override fun notifyPlayerListConfigurerDialog(position: Int) {
        (recyclerView?.adapter as PlayerListSelectorRecyclerViewAdapter).notifyDataSetChanged()
    }

    override fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        //TODO("Not yet implemented")
    }
}