package com.mpm.dartsclient.activities

import android.app.AlertDialog
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
import com.mpm.dartsclient.DartsClientApplication
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.adapters.ViewPager2Adapter
import com.mpm.dartsclient.fragments.PlayerCreatorDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import org.json.JSONException
import org.json.JSONObject

class Config : FragmentActivity() {
    var tabLayout : TabLayout? = null
    var viewPager2: ViewPager2? = null

    private var broadcastReceiver : BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            var message = intent?.getStringExtra("message")

            processMessage(message)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_config)
        //setSupportActionBar(findViewById(R.id.toolbar))
        //(this as AppCompatActivity).setSupportActionBar(findViewById(R.id.toolbar))
        //actionBar!!.customView.findViewById<>()

        findViewById<FloatingActionButton>(R.id.fab).setOnClickListener { view ->

            if (PlayerProfile.chosenPlayerProfiles.size == 0) {
                showNeedPlayers()
            }
            else {
                messageAboutGameStart()
                val intent = Intent(this, GamePlay::class.java)
                startActivity(intent)
            }
        }

        LocalBroadcastManager.getInstance(this).registerReceiver(
            broadcastReceiver,
            IntentFilter("boardMessage")
        );

        setupTabs()
    }

    override fun onDestroy() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(broadcastReceiver)
        super.onDestroy()
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
                Log.i("DARTS", "TabPos: "+tab.position)

                when (tab.position) {
                    0-> {
                        sendGameConfig()
                    }

                    1-> {
                        sendPlayers()
                    }
                }
            }

            override fun onTabUnselected(tab: TabLayout.Tab) {}
            override fun onTabReselected(tab: TabLayout.Tab) {}
        })
    }

    var prevPlayerProfile : PlayerProfile? = null
    fun createPlayerCreator(position: Int, previous: PlayerProfile?) {
        var playerCreator = PlayerCreatorDialogFragment()
        var bundle = Bundle()
        bundle.putInt("position", position)
        if (previous != null) {
            bundle.putInt("color", previous.color)
            bundle.putString("name", previous.name)
            bundle.putString("nickname", previous.nickname)
        }
        playerCreator.arguments = bundle
        playerCreator.show(supportFragmentManager, "PlayerCreatorDialog")
        supportFragmentManager.executePendingTransactions()

        playerCreator.dialog?.setOnCancelListener {
            var builder = AlertDialog.Builder(this)

            builder.setTitle("Biztos?")
            builder.setPositiveButton(
                "Jah",
                DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                    prevPlayerProfile = null
                })

            builder.setNegativeButton(
                "Nope",
                DialogInterface.OnClickListener { dialogInterface: DialogInterface, i: Int ->
                    if (position != -1) {
                        createPlayerCreator(position, prevPlayerProfile)
                    }
                })

            builder.setOnCancelListener {
                prevPlayerProfile = null
            }

            if (prevPlayerProfile?.name != PlayerProfile.playerProfiles[position].name
                || prevPlayerProfile?.nickname != PlayerProfile.playerProfiles[position].nickname
                || prevPlayerProfile?.color != PlayerProfile.playerProfiles[position].color
            ) {
                builder.create().show()
            }
            else {
                prevPlayerProfile = null
                //Find the currently focused view, so we can grab the correct window token from it.
            }
        }
    }

    private fun showNeedPlayers() {
        var builder = AlertDialog.Builder(this)

        builder.setTitle("Nem adott meg játékosokat")
        builder.setMessage("Definiáljon játékosokat!")
        builder.setPositiveButton(
            "Jah",
            DialogInterface.OnClickListener { _, _ -> }
        )
        builder.create().show()
    }

    //Communicating between fragments
    var fragments : MutableList<FragmentCommunicator> = ArrayList()

    fun subscribeToCommunicator(fragmentCommunicator: FragmentCommunicator) {
        fragments.add(fragmentCommunicator)
    }

    fun notifyPlayerConfigFragment(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyPlayerConfigFragment(position)
        }
        sendPlayers()
    }

    fun notifyPlayerListConfigurerDialog(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyPlayerListConfigurerDialog(position)
        }
        sendPlayers()
    }

    fun notifyGameConfigFragmentAboutUpdate(position: Int) {
        for(fragmentCommunicator in fragments) {
            fragmentCommunicator.notifyGameConfigFragmentAboutUpdate(position)
        }
    }

    interface FragmentCommunicator {
        fun notifyPlayerConfigFragment(position: Int)
        fun notifyPlayerListConfigurerDialog(position: Int)
        fun notifyGameConfigFragmentAboutUpdate(position: Int)
    }

    fun processMessage(message: String?) {
        Log.i("DARTS", "Size: " + message?.length + " Message: " + message)
        var jsonObject: JSONObject

        try {
            jsonObject = JSONObject(message)
        }
        catch (e : JSONException) {
            var retry = JSONObject()
            retry.put("MENU","LAST")

            DartsClientApplication.getBluetoothCommunicator().sendMessage(retry)
            return
        }

        var menu = jsonObject["MENU"] as String
        var nrOfGame = DartsGameContainer.findNumberOfGame(DartsGameContainer.currentGame)

        if (menu == "DUMP") {
            var dump = JSONObject()
            dump.put("PLAYERS", createPlayerJson())
            dump.put("MENU", "DUMP")
            dump.put("GAME", DartsGameContainer.currentGame!!.gameID)

            var configObject = JSONObject()

            for (game in DartsGameContainer.games) {
                configObject.put(game.gameID!!, game.serializeConfigParameters(this))
            }

            dump.put("CONFIG", configObject)
            DartsClientApplication.getBluetoothCommunicator().sendMessage(dump)

            var tab = tabLayout!!.getTabAt(1)
            tab!!.select()
        }
        else if (menu == "CONFIG") {
            DartsGameContainer.currentGame = DartsGameContainer.findGameByName(jsonObject["GAME"] as String)
            DartsGameContainer.currentGame?.parseConfigParameters(
                jsonObject["CONFIG"] as JSONObject,
                this
            )

            var tab = tabLayout!!.getTabAt(0)
            tab!!.select()
            Log.i("DARTS", " TAB CHANGED!")
            notifyGameConfigFragmentAboutUpdate(nrOfGame!!)
        }
        else if (menu == "GAMEPLAY") {
            //viewPager2.
        }
    }

    fun sendPlayers() {
        var jsonObject = JSONObject()
        jsonObject.put("MENU", "PLAYERCONFIG")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)
        jsonObject.put("PLAYERS", createPlayerJson())
        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    fun sendGameConfig() {
        var jsonObject = JSONObject()
        jsonObject.put("MENU", "CONFIG")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)

        var configObject = DartsGameContainer.currentGame!!.serializeConfigParameters(this)

        jsonObject.put("CONFIG", configObject)
        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    private fun createPlayerJson() : JSONObject {
        var players = JSONObject()

        for ((i, player) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
            var playerJ = JSONObject()
            playerJ.put("COLOR", player.color)
            playerJ.put("NICK", player.nickname)
            playerJ.put("NAME", player.name)
            players.put("" + (i + 1), playerJ)
        }

        return players
    }

    private fun messageAboutGameStart() {
        var jsonObject = JSONObject()
        jsonObject.put("MENU", "GAMEPLAY")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)
        jsonObject.put("NEXT", 0)
        jsonObject.put("DEL", 4)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }
}