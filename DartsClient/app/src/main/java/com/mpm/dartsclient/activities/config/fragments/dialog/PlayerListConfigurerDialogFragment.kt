package com.mpm.dartsclient.activities.config.fragments.dialog

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SearchView
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.adapters.PlayerListConfigurerRecyclerViewAdapter

class PlayerListConfigurerDialogFragment : DialogFragment(), Config.FragmentCommunicator {
    companion object {
        var chosenPlayers : MutableList<PlayerProfile>? = null
    }

    private var recyclerView : RecyclerView? = null
    private var searchview : SearchView? = null
    private var playerListConfigurerRecyclerViewAdapter : PlayerListConfigurerRecyclerViewAdapter? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val dialogView: View = inflater.inflate(R.layout.fragment_player_configurer, null)
        configView(dialogView)

        return dialogView
    }

    private fun configView(view : View) {
        searchview = view.findViewById(R.id.searchPlayer)
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

        playerListConfigurerRecyclerViewAdapter = PlayerListConfigurerRecyclerViewAdapter(activity as Config, chosenPlayers!!)

        recyclerView = view.findViewById<RecyclerView>(R.id.availablePlayers)
        recyclerView?.apply {
            recyclerView?.adapter = playerListConfigurerRecyclerViewAdapter
            layoutManager = LinearLayoutManager(activity)
        }
    }

    override fun onResume() {
        super.onResume()
        dialog!!.window!!.setLayout(700, 700)
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        (activity as Config).subscribeToCommunicator(this)
    }


    override fun notifyPlayerConfigFragment(position: Int) {

    }

    override fun notifyAboutModifiedPlayerEntry(position: Int) {
        recyclerView?.adapter?.notifyDataSetChanged()
    }

    override fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        //TODO("Not yet implemented")
    }

    override fun notifyStatisticsButton() {

    }
}