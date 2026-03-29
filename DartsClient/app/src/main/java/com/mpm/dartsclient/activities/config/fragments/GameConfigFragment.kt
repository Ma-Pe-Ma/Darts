package com.mpm.dartsclient.activities.config.fragments

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.Button
import android.widget.Spinner
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentTransaction
import com.mpm.dartsclient.MessageHandler
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.adapters.GameListSpinnerAdapter
import com.mpm.dartsclient.games.DartsGameContainer

class GameConfigFragment(var dartsGameContainer: DartsGameContainer) : Fragment(), Config.FragmentCommunicator {
    var spinner : Spinner? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        val view = inflater.inflate(R.layout.fragment_game_config, container, false)
        setupSpinner(view)

        return view
    }

    private fun setupSpinner(view: View) {
        val gameIds: List<String> = dartsGameContainer.games.map { it.gameID }

        spinner = view.findViewById(R.id.gameSpinner)
        spinner!!.adapter = GameListSpinnerAdapter(gameIds)

        spinner!!.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

            override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
                setProperConfigFragment(position)
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        (activity as Config).subscribeToCommunicator(this)
    }

    //called when spinner changed or new data comes from dartboard
    fun setProperConfigFragment(gameNr: Int) {
        dartsGameContainer.currentGame = dartsGameContainer.games[gameNr]
        dartsGameContainer.currentGame!!.subtype = "DEFAULT"

        val fragmentTransaction: FragmentTransaction = parentFragmentManager.beginTransaction()
        fragmentTransaction.replace(R.id.gameConfigFragment, dartsGameContainer.currentGame!!.getConfigFragment(dartsGameContainer))
        fragmentTransaction.commit()
    }

    override fun notifyPlayerConfigFragment(position: Int) {

    }

    override fun notifyAboutModifiedPlayerEntry(position: Int) {

    }

    override fun notifyGameConfigFragmentAboutUpdate(position: Int) {

    }

    override fun notifyStatisticsButton() {

    }
}