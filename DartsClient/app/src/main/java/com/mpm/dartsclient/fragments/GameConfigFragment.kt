package com.mpm.dartsclient.fragments

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.Button
import android.widget.Spinner
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentTransaction
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.Config
import com.mpm.dartsclient.adapters.GameListSpinnerAdapter
import com.mpm.dartsclient.fragments.gameconfig.CricketConfig
import com.mpm.dartsclient.games.DartsGameContainer




class GameConfigFragment : Fragment(), Config.FragmentCommunicator {
    var gameButton : Button? = null
    var spinner : Spinner? = null

    private var previousFrag : Int = -1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        var view = inflater.inflate(R.layout.fragment_game_config, container, false)

        setupSpinner(view)
        //createButton(view)

        // Inflate the layout for this fragment
        return view
    }

    private fun setupSpinner(view: View) {
        spinner = view.findViewById(R.id.gameSpinner)
        spinner!!.adapter = GameListSpinnerAdapter()

        spinner!!.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {
                TODO("Not yet implemented")
            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                setProperConfigFragment(position)
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        (activity as Config).subscribeToCommunicator(this)
    }

    fun setProperConfigFragment(gameNr: Int) {
        DartsGameContainer.currentGame = DartsGameContainer.games[gameNr]

        var fragment = activity?.supportFragmentManager?.findFragmentById(R.id.gameConfigFragment)
        val fragmentTransaction: FragmentTransaction = parentFragmentManager.beginTransaction()
        fragmentTransaction.replace(R.id.gameConfigFragment, DartsGameContainer.currentGame!!.getConfigFragment())
        fragmentTransaction.commit()

        previousFrag = gameNr
        (activity as Config).sendGameConfig()
    }

    companion object {

    }

    override fun notifyPlayerConfigFragment(position: Int) {
        //TODO("Not yet implemented")
    }

    override fun notifyPlayerListConfigurerDialog(position: Int) {
        //TODO("Not yet implemented")
    }

    override fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        setProperConfigFragment(position)
    }
}

private operator fun AdapterView.OnItemSelectedListener?.invoke(function: () -> Unit) {

}
