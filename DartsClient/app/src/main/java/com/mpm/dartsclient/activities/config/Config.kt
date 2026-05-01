package com.mpm.dartsclient.activities.config

import android.content.*
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.fragment.app.FragmentActivity
import androidx.localbroadcastmanager.content.LocalBroadcastManager
import androidx.viewpager2.widget.ViewPager2
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.tabs.TabLayout
import com.google.android.material.tabs.TabLayout.OnTabSelectedListener
import com.google.android.material.tabs.TabLayoutMediator
import com.mpm.dartsclient.*
import com.mpm.dartsclient.activities.gamePlay.GamePlay
import com.mpm.dartsclient.activities.config.adapters.ViewPager2Adapter
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer
import com.mpm.dartsclient.sqlhelper.SQLTables
import org.json.JSONObject

class Config : FragmentActivity() {
    var tabLayout : TabLayout? = null
    var viewPager2: ViewPager2? = null

    var profileContainer: ProfileContainer = ProfileContainer.getInstance()

    var matchContainer: MatchContainer = MatchContainer.getInstance()

    var dartsGameContainer: DartsGameContainer = DartsGameContainer.getInstance()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_config)

        profileContainer.playerProfiles = SQLTables.PlayersTable.readPlayers()

        //setup the start button
        findViewById<FloatingActionButton>(R.id.fab).setOnClickListener { view ->
            if (profileContainer.chosenPlayerProfiles.size == 0) {
                Toast.makeText(applicationContext, getString(R.string.neededPlayers), Toast.LENGTH_SHORT).show()
            }
            else {
                //val intent = Intent(this, GamePlay::class.java)
                //startActivity(intent)
                Toast.makeText(applicationContext, getString(R.string.unfinished), Toast.LENGTH_SHORT).show()
            }
        }

        viewPager2 = findViewById(R.id.viewPager2)
        tabLayout = findViewById(R.id.tabLayout)

        var titles =  arrayOf<String>(
            getString(R.string.tab1_name),
            getString(R.string.tab2_name)
        )

        viewPager2!!.adapter = ViewPager2Adapter(supportFragmentManager, lifecycle, profileContainer, matchContainer, dartsGameContainer)

        TabLayoutMediator(tabLayout!!, viewPager2!!) { tab, position ->
            tab.text = titles[position]
        }.attach()
    }

    //---------------------------------------------
    //Communicating between fragments
        interface FragmentCommunicator {
        fun notifyPlayerConfigFragment(position: Int)
        fun notifyAboutModifiedPlayerEntry()
    }

    var fragments : MutableList<FragmentCommunicator> = ArrayList()

    fun subscribeToCommunicator(fragmentCommunicator: FragmentCommunicator) {
        fragments.add(fragmentCommunicator)
    }

    fun notifyPlayerConfigFragment(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyPlayerConfigFragment(position)
        }
    }

    fun notifyAboutModifiedPlayerEntry() {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyAboutModifiedPlayerEntry()
        }
    }
}