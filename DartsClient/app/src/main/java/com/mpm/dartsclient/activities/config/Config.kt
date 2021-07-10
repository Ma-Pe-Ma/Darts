package com.mpm.dartsclient.activities.config

import android.content.*
import android.os.Bundle
import android.util.Log
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
import com.mpm.dartsclient.activities.config.fragments.dialog.NeedPlayers
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.sqlhelper.DBHelper
import com.mpm.dartsclient.sqlhelper.SQLTables
import org.json.JSONException
import org.json.JSONObject

class Config : FragmentActivity(), BTMessageReceiver {
    var tabLayout : TabLayout? = null
    var viewPager2: ViewPager2? = null
    var previousTabIndex = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_config)
        //setSupportActionBar(findViewById(R.id.toolbar))
        //(this as AppCompatActivity).setSupportActionBar(findViewById(R.id.toolbar))
        //actionBar!!.customView.findViewById<>()

        //deleteDatabase(DBHelper.DATABASE_NAME)
        //read the players from the sqlite database
        SQLTables.PlayersTable.readPlayers()

        //setup the start button
        findViewById<FloatingActionButton>(R.id.fab).setOnClickListener { view ->
            if (PlayerProfile.chosenPlayerProfiles.size == 0) {
                var needPlayers = NeedPlayers()
                needPlayers.show(supportFragmentManager, "NEED")
            }
            else {
                val intent = Intent(this, GamePlay::class.java)
                startActivity(intent)
                //MessageHandler.requestGameStart(0)
            }
        }

        //setup the two config tabs
        setupTabs()
    }

    override fun onResume() {
        super.onResume()

        //setup the receiver for the board messages
        if (DartsClientApplication.bluetoothMode) {
            DartsClientApplication.getBluetoothCommunicator().subscribeToMessages(this)
        }
    }

    override fun onPause() {
        DartsClientApplication.getBluetoothCommunicator().unsubscribeToMessages(this)
        super.onPause()
    }

    private fun setupTabs() {
        viewPager2 = findViewById(R.id.viewPager2)
        tabLayout = findViewById(R.id.tabLayout)

        //var titles = Array<String>(2) {getString(R.string.tab1_name), getString(R.string.tab2_name)}
        var titles =  arrayOf<String>(
            getString(R.string.tab1_name), getString(
                R.string.tab2_name
            )
        )

        viewPager2!!.adapter = ViewPager2Adapter(supportFragmentManager, lifecycle)

        TabLayoutMediator(tabLayout!!, viewPager2!!) { tab, position ->
            tab.text = titles[position]
            //viewPager2!!.setCurrentItem(tab.position, true)
        }.attach()

        tabLayout!!.addOnTabSelectedListener(object : OnTabSelectedListener {
            override fun onTabSelected(tab: TabLayout.Tab) {
                if (tab.position != previousTabIndex ) {
                    when (tab.position) {
                        0-> {
                            MessageHandler.sendGameConfig()
                        }

                        1-> {
                            MessageHandler.sendPlayers()
                        }
                    }
                }

                previousTabIndex = tab.position
            }

            override fun onTabUnselected(tab: TabLayout.Tab) {}
            override fun onTabReselected(tab: TabLayout.Tab) {}
        })
    }

    //---------------------------------------------
    //Communicating between fragments
    interface FragmentCommunicator {
        fun notifyPlayerConfigFragment(position: Int)
        fun notifyAboutModifiedPlayerEntry(position: Int)
        fun notifyGameConfigFragmentAboutUpdate(position: Int)
        fun notifyStatisticsButton()
    }

    var fragments : MutableList<FragmentCommunicator> = ArrayList()

    fun subscribeToCommunicator(fragmentCommunicator: FragmentCommunicator) {
        fragments.add(fragmentCommunicator)
    }

    fun notifyPlayerConfigFragment(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyPlayerConfigFragment(position)
        }

        MessageHandler.sendPlayers()
    }

    fun notifyAboutModifiedPlayerEntry(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyAboutModifiedPlayerEntry(position)
        }

        MessageHandler.sendPlayers()
    }

    fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyGameConfigFragmentAboutUpdate(position)
        }
    }

    fun notifyStatisticsButton() {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyStatisticsButton()
        }
    }

    //------------------------------------------
    //BT message callbacks
    override fun onDump(body : JSONObject) {
        var tab = tabLayout!!.getTabAt(1)
        tab!!.select()

        MessageHandler.sendDump()
    }

    override fun onConfig(body : JSONObject) {
        DartsGameContainer.currentGame = DartsGameContainer.findGameByName(body["GAME"] as String)
        DartsGameContainer.currentGame?.parseConfigParameters(body["CONFIG"] as JSONObject)

        var tab = tabLayout!!.getTabAt(0)
        tab!!.select()

        var nrOfGame = DartsGameContainer.findNumberOfGame(DartsGameContainer.currentGame)
        notifyGameConfigFragmentAboutUpdate(nrOfGame!!)
    }

    override fun onGamePlay(body: JSONObject) {
        val intent = Intent(this, GamePlay::class.java)
        startActivity(intent)
    }
}