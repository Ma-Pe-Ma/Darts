package com.mpm.dartsclient.activities

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.os.Handler
import android.util.DisplayMetrics
import android.util.Log
import android.widget.FrameLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.localbroadcastmanager.content.LocalBroadcastManager
import com.mpm.dartsclient.DartsClientApplication
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.fragments.PlayerScoreDialogFragment
import com.mpm.dartsclient.fragments.PostConfigDialog
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.nativeElements.CustomGLSurfaceView
import com.mpm.dartsclient.scoring.Dart
import org.json.JSONException
import org.json.JSONObject


class GamePlay : AppCompatActivity() {
    private var mGLView: GLSurfaceView? = null

    private var dartMap = mutableMapOf<Int, TextView>()
    private var multiplierMap = mutableMapOf<Int, String>()
    var playerName : TextView? = null
    private var playerScore : TextView? = null
    private var roundCounter : Int = 0
    private var dartID : Int = 0
    private var flashing : Runnable? = null

    private var broadcastReceiver : BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            var message = intent?.getStringExtra("message")

            processMessage(message)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game_play)

        LocalBroadcastManager.getInstance(this).registerReceiver(
            broadcastReceiver,
            IntentFilter("boardMessage")
        )

        mGLView = findViewById(R.id.customGLView)
        initializeLayout()

        val displayMetrics = DisplayMetrics()
        windowManager.defaultDisplay.getMetrics(displayMetrics)
        val height = displayMetrics.heightPixels
        val width = displayMetrics.widthPixels

        var glContainer = findViewById<FrameLayout>(R.id.customGLContainer)
        glContainer.layoutParams = ConstraintLayout.LayoutParams(width, width)

        (mGLView as CustomGLSurfaceView).customRenderer?.init(assets)
    }

    override fun onDestroy() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(broadcastReceiver)
        super.onDestroy()
    }

    override fun onResume() {
        super.onResume()
        mGLView?.onResume();
    }

    override fun onPause() {
        super.onPause()
        mGLView?.onPause()
    }

    private fun initializeLayout() {
        playerName = findViewById(R.id.playerNameGame)
        playerName!!.setOnClickListener {
            changePlayer()
        }
        playerScore = findViewById(R.id.playerScoreGame)
        playerScore!!.setOnClickListener {
            PlayerScoreDialogFragment().show(supportFragmentManager, "PLAYERSORE")
        }

        initializeMaps()
        startGame()
    }

    private fun startGame() {
        PlayerProfile.currentCursor = PlayerProfile.chosenPlayerProfiles.size - 1
        nextPlayer()
    }

    private fun initializeMaps(){
        dartMap[0] = findViewById(R.id.dart1)
        dartMap[1] = findViewById(R.id.dart2)
        dartMap[2] = findViewById(R.id.dart3)

        multiplierMap[0] = "-"
        multiplierMap[1] = " "
        multiplierMap[2] = "D"
        multiplierMap[3] = "T"

        for ((i, dart) in dartMap.values.withIndex()) {
            dart.setOnClickListener {
                correctDart(i, 0)
            }

            dart.setOnLongClickListener {
                correctDart(i, 1)
                return@setOnLongClickListener true
            }
        }
    }

    //change player with client
    private fun changePlayer() {
        nextPlayer()

        var jsonObject = JSONObject()
        jsonObject.put("MENU", "GAMEPLAY")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)
        jsonObject.put("NEXT", 1)
        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    //only this called when player changed on board
    private fun nextPlayer() {
        var currentPlayer = findNextPlayer()

        //if (flashing != null) {
            hitHandler.removeCallbacks(flashing)
            //flashing = null
        //}
        (mGLView as CustomGLSurfaceView).customRenderer?.setCurrentColor(currentPlayer.color)
        (mGLView as CustomGLSurfaceView).customRenderer?.highLightSector(0,0)

        saveRound()

        playerScore?.text = ""
        playerName?.setBackgroundColor(getColor(R.color.BLACK))
        playerName?.setTextColor(getColor(R.color.WHITE))
        playerName?.text = "Round " + roundCounter + " - P"+(PlayerProfile.currentCursor + 1)
        //reseting
        for (dart in dartMap) {
            dart.value.text = "" + (dart.key +1) + ": -"
            dart.value.setBackgroundColor(getColor(R.color.WHITE))
        }

        newRoundHandler.postDelayed(newRound, 1500)
    }

    private var newRoundHandler = Handler()
    private var newRound  = Runnable {
        playerScore?.text = PlayerProfile.currentPlayer!!.score!!.score.toString()
        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.color)
        playerName?.setTextColor(getColor(R.color.BLACK))
        playerName?.text = "P"+(PlayerProfile.currentCursor + 1)+" - "+PlayerProfile.currentPlayer!!.nickname
    }

    private var hitHandler = Handler()
    private var dartHit = Runnable {
        playerScore?.text = PlayerProfile.currentPlayer!!.score!!.score.toString()
        playerScore?.setBackgroundColor(getColor(R.color.WHITE))

        (mGLView as CustomGLSurfaceView).customRenderer?.highLightSector(0,0)

        if (dartID == 2) {
            runOnUiThread() {
                playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.color)
                playerName?.setTextColor(PlayerProfile.currentPlayer!!.inverseColor)
            }

             flashing = object : Runnable {
                var counter = 0

                override fun run() {
                    if ((++counter) % 2 == 1) {
                        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.inverseColor)
                        playerName?.setTextColor(PlayerProfile.currentPlayer!!.color)
                    }
                    else {
                        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.color)
                        playerName?.setTextColor(PlayerProfile.currentPlayer!!.inverseColor)
                    }

                    hitHandler.postDelayed(this, 1000)
                }
            }

            hitHandler.postDelayed(flashing, 1000)
        }
    }

    private fun saveRound() {

    }

    private fun findNextPlayer() : PlayerProfile {
        while (true) {
            if (++PlayerProfile.currentCursor == PlayerProfile.chosenPlayerProfiles.size) {
                PlayerProfile.currentCursor = 0
                roundCounter++
            }

            var checkable = PlayerProfile.chosenPlayerProfiles[PlayerProfile.currentCursor]

            if (!checkable.score!!.winning) {
                PlayerProfile.currentPlayer = checkable
                return checkable
            }
        }
    }

    fun processMessage(message: String?) {
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

        var menu = jsonObject["MENU"]

        when (menu) {
            "CONFIG" -> {
                //back to previous activity
            }

            "GAMEPLAY" -> {
                //parsing boolean a bit complicated
                var method = jsonObject["MET"] as String

                when (method) {
                    "NEXT" -> {
                        nextPlayer()
                    }
                    "DUMP" -> {
                        var data = jsonObject["DAT"] as JSONObject
                        processDartDump(data)
                    }
                    "HIT" -> {
                        var data = jsonObject["DAT"] as JSONObject
                        processHit(data)
                    }
                }


            }

            "POSTCONFIG" -> {

                var setting = jsonObject["SET"] as Int

                when (setting) {
                    -1 -> PostConfigDialog().show(supportFragmentManager, "POSTCONFIG")
                    else -> {
                        setPostProcess(setting, false)
                        //startGame()
                    }

                }
            }

            "SOUND" -> {

            }
        }
    }

    fun setPostProcess(mode: Int, send: Boolean) {
        var jsonObject = JSONObject()

        jsonObject.put("MENU", "POSTCONFIG")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)

        if (mode == 0) {
            jsonObject.put("ORDER", 0)

            val newOrder = mutableListOf<PlayerProfile>()

            for (i in 1 until PlayerProfile.chosenPlayerProfiles.size) {
                newOrder.add(PlayerProfile.chosenPlayerProfiles[i])
            }

            newOrder.add(PlayerProfile.chosenPlayerProfiles[0])

            PlayerProfile.chosenPlayerProfiles = newOrder
        }

        if (mode == 1) {
            jsonObject.put("ORDER", 1)

            var newOrder = mutableListOf<PlayerProfile>()

            for (i in PlayerProfile.chosenPlayerProfiles.size - 1..0) {
                newOrder.add(PlayerProfile.chosenPlayerProfiles[i])
            }

            PlayerProfile.chosenPlayerProfiles = newOrder
        }

        if (mode == 2) {
            jsonObject.put("ORDER", 2)
        }

        if (send) {
            DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
        }
    }

    fun correctDart(id: Int, correct: Int) {
        var jsonObject = JSONObject()
        jsonObject.put("MENU", "GAMEPLAY")
        jsonObject.put("GAME", DartsGameContainer.currentGame!!.gameID)
        jsonObject.put("NEXT", 0)

        var fixObject = JSONObject()
        fixObject.put("DART", id)
        fixObject.put("MET", correct)

        jsonObject.put("FIX", fixObject)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    fun processHit(throwObject: JSONObject) {
        dartID = throwObject["ID"] as Int
        var multiplier = throwObject["M"] as Int
        var sector = throwObject["S"] as Int

        PlayerProfile.currentPlayer?.score?.score = throwObject["SC"] as Int

        var thrownText = "" + (dartID + 1) +": " + multiplierMap[multiplier]+sector
        dartMap[dartID]?.text = thrownText

        playerScore!!.setBackgroundColor(getColor(R.color.PINK))
        //playerScore!!.text = multiplierMap[multiplier] + sector
        playerScore!!.text = thrownText

        (mGLView as CustomGLSurfaceView).customRenderer?.highLightSector(multiplier, sector)

        hitHandler.postDelayed(dartHit, 1500)
    }

    private fun processDartDump(dumpObject: JSONObject) {
        for (i in 0..2) {
            var dart = dumpObject[i.toString()] as JSONObject
            var multiplier = dart["M"] as Int
            var sector = dart["S"] as Int
            var delState = dart["DEL"] as Boolean
            var dartObj = Dart(multiplier, sector)

            var thrownText = "" + (i + 1) +": " + multiplierMap[dartObj.multiplier]
            if (dartObj.sector != 0) {
                thrownText += dartObj.sector
            }
            dartMap[i]?.text = thrownText
            var color : Int = if (delState) getColor(R.color.RED) else getColor(R.color.WHITE)
            dartMap[i]?.setBackgroundColor(color)
        }

        var finalScore = dumpObject["SC"] as Int

        PlayerProfile.currentPlayer!!.score!!.score = finalScore
        playerScore!!.text = finalScore.toString()
    }
}