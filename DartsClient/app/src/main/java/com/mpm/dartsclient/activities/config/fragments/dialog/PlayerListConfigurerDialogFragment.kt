package com.mpm.dartsclient.activities.config.fragments.dialog

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.SearchView
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.adapters.PlayerListConfigurerRecyclerViewAdapter
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer

class PlayerListConfigurerDialogFragment(var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) : DialogFragment(), Config.FragmentCommunicator {
    private var recyclerView: RecyclerView? = null
    private var searchview: SearchView? = null

    private var addNewButton: Button? = null

    private var playerListConfigurerRecyclerViewAdapter : PlayerListConfigurerRecyclerViewAdapter? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        val dialogView: View = inflater.inflate(R.layout.fragment_player_configurer, null)

        addNewButton = dialogView.findViewById(R.id.createNewPlayer)
        addNewButton?.setOnClickListener {
           var playerCreatorDialogFragment = PlayerCreatorDialogFragment(null, profileContainer)
           playerCreatorDialogFragment.show(requireActivity().supportFragmentManager, "PLAYERCREATOR")
        }

        searchview = dialogView.findViewById(R.id.searchPlayer)
        searchview?.clearFocus()
        searchview?.setOnQueryTextListener(object: SearchView.OnQueryTextListener {
            override fun onQueryTextSubmit(query: String?): Boolean {
                return false
            }

            override fun onQueryTextChange(newText: String?): Boolean {
                playerListConfigurerRecyclerViewAdapter?.filter?.filter(newText)
                return true
            }
        })

        playerListConfigurerRecyclerViewAdapter = PlayerListConfigurerRecyclerViewAdapter(activity as Config, profileContainer, matchContainer, dartsGameContainer)

        recyclerView = dialogView.findViewById<RecyclerView>(R.id.availablePlayers)
        recyclerView?.apply {
            recyclerView?.adapter = playerListConfigurerRecyclerViewAdapter
            layoutManager = LinearLayoutManager(activity)
        }

        return dialogView
    }

    override fun onResume() {
        super.onResume()
        val width = (resources.displayMetrics.widthPixels * 0.90).toInt()
        dialog?.window?.setLayout(width, ViewGroup.LayoutParams.WRAP_CONTENT)
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        (activity as Config).subscribeToCommunicator(this)
    }

    override fun notifyPlayerConfigFragment(position: Int) {

    }

    override fun notifyAboutModifiedPlayerEntry() {
        recyclerView?.adapter?.notifyDataSetChanged()
    }
}