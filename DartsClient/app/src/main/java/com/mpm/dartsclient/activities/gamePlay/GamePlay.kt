package com.mpm.dartsclient.activities.gamePlay

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.icu.util.Calendar
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
import com.mpm.dartsclient.*
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.gamePlay.fragments.PlayerScoreDialogFragment
import com.mpm.dartsclient.activities.gamePlay.fragments.PostConfigDialog
import com.mpm.dartsclient.activities.gamePlay.fragments.WinningMessageDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.nativeElements.CustomGLSurfaceView
import com.mpm.dartsclient.scoring.Dart
import com.mpm.dartsclient.sqlhelper.SQLTables
import org.json.JSONException
import org.json.JSONObject
import java.util.*

class GamePlay : AppCompatActivity(), BTMessageReceiver {
    private var mGLView: GLSurfaceView? = null

    private var dartMap = mutableMapOf<Int, TextView>()
    private var multiplierMap = mutableMapOf<Int, String>()
    var playerName : TextView? = null
    private var playerScore : TextView? = null
    private var dartID : Int = 0
    private var flashing : Runnable? = null

    private var broadcastReceiver : BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            var message = intent?.getStringExtra("message")

            try {
                onBTReceive(message as JSONObject)
            }
            catch (e: JSONException) {
                onParseFail()
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game_play)

        if (DartsClientApplication.bluetoothMode) {
            LocalBroadcastManager.getInstance(this).registerReceiver(
                broadcastReceiver,
                IntentFilter("boardMessage")
            )
        }

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
        if (DartsClientApplication.bluetoothMode) {
            LocalBroadcastManager.getInstance(this).unregisterReceiver(broadcastReceiver)
        }
        super.onDestroy()
    }

    override fun onResume() {
        super.onResume()
        mGLView?.onResume();
        //this is a special case, dump needed when the activity is shown, 1 when starting 2 when resumed,
        getGameDump()

        //TODO:
        var debugGameDump = debugGameDump()
        onGameDump(debugGameDump)
        var debugRoundDump = debugRoundDump()
        //onRound(debugRoundDump)
        var debugDartDump = debugDartDump()
        //onHit(debugDartDump)
    }

    override fun onPause() {
        super.onPause()
        mGLView?.onPause()
    }

    //setting the proper GUI Elements
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
    }

    //create maps for gui elements
    private fun initializeMaps() {
        dartMap[0] = findViewById(R.id.dart1)
        dartMap[1] = findViewById(R.id.dart2)
        dartMap[2] = findViewById(R.id.dart3)

        multiplierMap[0] = "-"
        multiplierMap[1] = " "
        multiplierMap[2] = "D"
        multiplierMap[3] = "T"

        //setup the correct listeners
        for ((i, dart) in dartMap.values.withIndex()) {
            dart.setOnClickListener {
                MessageHandler.requestCorrectDart()
            }

            dart.setOnLongClickListener {
                MessageHandler.requestDeleteDart(i)
                return@setOnLongClickListener true
            }
        }
    }

    //--------------------------------- GAMEPLAY ELEMENTS--------------------------

    //change player with client
    private fun changePlayer() {
        MessageHandler.requestChangePlayer()
    }

    private fun getGameDump() {
        var postConfigDialog  = supportFragmentManager.findFragmentByTag("POSTCONF") as PostConfigDialog?
        postConfigDialog?.dismiss()

        var winningMessage  = supportFragmentManager.findFragmentByTag("WINNING") as WinningMessageDialogFragment?
        winningMessage?.dismiss()

        MessageHandler.requestGameDump()
    }

    //----------------------------RECEIVING MESSAGES FROM BOARD -------------------------------------------
    override fun onConfig(body: JSONObject) {
        val intent = Intent(this, Config::class.java)
        //intent.putExtra("body", body.toString())
        startActivity(intent)
    }

    override fun onDump(body: JSONObject) {
        val intent = Intent(this, Config::class.java)
        startActivity(intent)
    }

    //receiving message about gameplay
    override fun onGamePlay(body: JSONObject) {
        var gstate = body["GSTATE"] as String
        var gbody = body["GBODY"] as JSONObject

        when (gstate) {
            "START" -> onGameStart(gbody)
            "HIT" -> onHit(gbody)
            "ROUND" -> onRound(gbody)
            "DUMP" -> onGameDump(gbody)
        }
    }

    private fun onGameStart(body: JSONObject) {
        WinningMessageDialogFragment.saveProgress = SaveProgress.inProgress
        getGameDump()
    }

    //when a dart hit
    private fun onHit(throwObject: JSONObject) {
        dartID = throwObject["ID"] as Int
        var multiplier = throwObject["M"] as Int
        var sector = throwObject["S"] as Int

        //playerNameTextHandler.removeCallbacks(newRoundIntroEnd)
        //playerNameTextHandler.removeCallbacks(flashing)
        scoreTextHandler.removeCallbacks(dartHitEnd)

        //show the current hit dart info
        PlayerProfile.currentPlayer?.score?.score = throwObject["SC"] as Int

        var thrownText = "" + (dartID + 1) + ": " + multiplierMap[multiplier]+sector
        dartMap[dartID]?.text = thrownText

        playerScore!!.setBackgroundColor(getColor(R.color.PINK))
        playerScore!!.text = thrownText

        (mGLView as CustomGLSurfaceView).customRenderer?.highlightSector(multiplier, sector)

        //remove the current hit dart info 1,5 ms later!
        scoreTextHandler.postDelayed(dartHitEnd, 1500)
    }

    //finishing showing hit dart info
    private var scoreTextHandler = Handler()
    private var dartHitEnd = Runnable {
        playerScore?.text = PlayerProfile.currentPlayer!!.score!!.score.toString()
        playerScore?.setBackgroundColor(getColor(R.color.WHITE))

        (mGLView as CustomGLSurfaceView).customRenderer?.highlightSector(0, 0)

        //if the last dart info was shown then start flashing
        if (dartID == 2) {
            setRoundEndFlashing()
        }
    }

    fun debugDartDump() : JSONObject {
        var dart = JSONObject()

        dart.put("ID", 2)
        dart.put("M", 2)
        dart.put("S", 25)

        dart.put("SC", 177)



        return dart
    }

    //start flashing the result
    private fun setRoundEndFlashing() {
        flashing = object : Runnable {
            var counter = 0

            override fun run() {

                if ((++counter) % 2 == 1) {
                    runOnUiThread() {
                        //playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.textColor!!)
                        //playerName?.setTextColor(PlayerProfile.currentPlayer!!.backgroundColor!!)
                        playerName?.setBackgroundColor((0xFFFFFF - PlayerProfile.currentPlayer!!.backgroundColor!!).or(0xFF000000.toInt()))
                        playerName?.setTextColor((0xFFFFFF - PlayerProfile.currentPlayer!!.textColor!!).or(0xFF000000.toInt()))
                    }
                }
                else {
                    runOnUiThread() {
                        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.backgroundColor!!)
                        playerName?.setTextColor(PlayerProfile.currentPlayer!!.textColor!!)

                        //inverse  = (0xFFFFFF - textColor!!).or(0xFF000000.toInt())
                    }
                }

                playerNameTextHandler.postDelayed(this, 1000)
            }
        }

        playerNameTextHandler.postDelayed(flashing!!, 1000)
    }

    fun debugRoundDump() : JSONObject {
        var round = JSONObject()

        round.put("DART", 1)
        round.put("ROUND", 0)

        var darts = JSONObject()
        round.put("DARTS", darts)

        var dart1 = JSONObject()
        dart1.put("M", 1)
        dart1.put("S", 7)
        dart1.put("DEL", false)

        darts.put("0", dart1)
        darts.put("1", dart1)
        darts.put("2", dart1)

        var player = JSONObject()
        round.put("PLAYER", player)

        player.put("NICK", "MPM")
        player.put("SCORE", 500)
        player.put("ROUND", 0)
        player.put("POSITION", 7)
        player.put("STATUS", JSONObject())

        return round
    }

    private fun onRound(gBody: JSONObject) {
        var roundCounter = gBody["ROUND"] as Int
        var dartID = gBody["DART"] as Int
        var darts = gBody["DARTS"] as JSONObject


        for (i in 0..2) {
            var dart = darts[i.toString()] as JSONObject

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

        var playerObject = gBody["PLAYER"] as JSONObject
        var player  = parsePlayerObject(playerObject)

        //set the cursor
        for ((i, playerNumbered) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
            if (player == playerNumbered) {
                PlayerProfile.currentCursor = i
            }
        }

        PlayerProfile.currentPlayer = player

        //remove earlier callbacks if they exists (by some error)
        playerNameTextHandler.removeCallbacks(newRoundIntroEnd)
        playerNameTextHandler.removeCallbacks(flashing)

        playerScore!!.text = PlayerProfile.currentPlayer!!.score!!.score.toString()
        playerName?.text = "P${PlayerProfile.currentCursor + 1} - ${PlayerProfile.currentPlayer!!.nickname}"

        (mGLView as CustomGLSurfaceView).customRenderer?.setCurrentColor(player!!.backgroundColor!!)
        (mGLView as CustomGLSurfaceView).customRenderer?.highlightSector(0, 0)
        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.backgroundColor!!)
        playerName?.setTextColor(PlayerProfile.currentPlayer!!.textColor!!)

        when(dartID) {
            0 -> {
                //Show New Round details
                playerName?.setBackgroundColor(getColor(R.color.BLACK))
                playerName?.setTextColor(getColor(R.color.WHITE))
                playerName?.text = "R${PlayerProfile.currentPlayer!!.score!!.roundCount} - ${PlayerProfile.currentCursor}"
                //Show regular dart waiting screen 1,5 sec later
                playerNameTextHandler.postDelayed(newRoundIntroEnd, 1500)
            }

            1 -> {

            }

            2 -> {
                //remove if they exist by error
                setRoundEndFlashing()
            }
        }

        if (player!!.score!!.position > 0) {
            val winningEarlier : WinningMessageDialogFragment? = supportFragmentManager.findFragmentByTag("WINNING") as WinningMessageDialogFragment?
            winningEarlier?.dismiss()

            var winningMessage = WinningMessageDialogFragment(player)
            winningMessage.show(supportFragmentManager, "WINNING")

            setRoundEndFlashing()
        }
    }

    //new round info showing finished
    private var playerNameTextHandler = Handler()
    private var newRoundIntroEnd  = Runnable {
        playerScore?.text = PlayerProfile.currentPlayer!!.score!!.score.toString()
        playerName?.setBackgroundColor(PlayerProfile.currentPlayer!!.backgroundColor!!)
        playerName?.setTextColor(PlayerProfile.currentPlayer!!.textColor!!)
        playerName?.text = "P${PlayerProfile.currentCursor + 1} - ${PlayerProfile.currentPlayer!!.nickname}"
    }

    fun debugGameDump() : JSONObject {
        var dump = JSONObject()

        dump.put("CURRENT", "MPM")
        dump.put("GAMENR", 6)

        var players = JSONObject()
        dump.put("PLAYERS", players)

        var mpm = JSONObject()
        players.put ("0", mpm)

        mpm.put("NICK", "MPM")
        mpm.put("SCORE", 500)
        mpm.put("ROUND", 7)
        mpm.put("POSITION", 1)

        mpm.put("STATUS", JSONObject())


        var mzs = JSONObject()
        players.put ("1", mzs)

        mzs.put("NICK", "MZS")
        mzs.put("SCORE", 400)
        mzs.put("ROUND", 6)
        mzs.put("POSITION", 2)

        mzs.put("STATUS", JSONObject())

        Log.i("DARTS", "DUMP: "+dump)

        return dump
    }

    private fun onGameDump(gBody: JSONObject) {
        var currentNick = gBody["CURRENT"] as String

        var playersObject = gBody["PLAYERS"] as JSONObject
        var keys : Iterator<String> = playersObject.keys()

        var newPlayerList : MutableList<PlayerProfile?> = MutableList(2) {null}

        Log.i("DARTS", "P-Length: "+playersObject.length()+ ", new: " + newPlayerList.size)

        var newResultPlayer : PlayerProfile? = null

        while (keys.hasNext()) {
            var key : String = keys.next()
            var slot = key.toInt()

            var playerObject = playersObject[key] as JSONObject
            var player = parsePlayerObject(playerObject)

            if (player!!.nickname == currentNick) {
                newResultPlayer = player
            }

            newPlayerList[slot] = player

            if (currentNick == player.nickname) {
                PlayerProfile.currentCursor = slot
                PlayerProfile.currentPlayer = player
            }
        }

        //not elegant
        var nonNullList : MutableList<PlayerProfile> = mutableListOf()

        for (player in newPlayerList) {
            nonNullList.add(player!!)
        }

        PlayerProfile.chosenPlayerProfiles = nonNullList

        //finding how many players are still playing
        var activePlayers = 0
        for (player in PlayerProfile.chosenPlayerProfiles) {
            if (player.score!!.position == 0) {
                activePlayers++
                break
            }
        }

        //game ended, save game data to database
        if (activePlayers == 0) {
            var gameNr = gBody["GAMENR"] as Int

            var dateTime = GregorianCalendar()
            var dateTimeString = "" + dateTime.get(Calendar.YEAR) +  "-" + dateTime.get(Calendar.MONTH) + "-" + dateTime.get(
                Calendar.DAY_OF_MONTH
            ) + " "+ dateTime.get(Calendar.HOUR_OF_DAY)+":"+ dateTime.get(Calendar.MINUTE)+":"+dateTime.get(
                Calendar.SECOND
            )

            SQLTables.addMatch(
                this,
                gameNr,
                DartsGameContainer.currentGame!!.gameID,
                DartsGameContainer.currentGame!!.subtype,
                dateTimeString,
                PlayerProfile.chosenPlayerProfiles
            )

            val winningEarlier : WinningMessageDialogFragment? = supportFragmentManager.findFragmentByTag("WINNING") as WinningMessageDialogFragment?
            winningEarlier?.dismiss()

            var winningMessage = WinningMessageDialogFragment(null)
            winningMessage.show(supportFragmentManager, "WINNING")
        }
        else if (newResultPlayer != null && newResultPlayer.score!!.position > 0) {
            val winningEarlier : WinningMessageDialogFragment? = supportFragmentManager.findFragmentByTag("WINNING") as WinningMessageDialogFragment?
            winningEarlier?.dismiss()

            var winningMessage = WinningMessageDialogFragment(newResultPlayer)
            winningMessage.show(supportFragmentManager, "WINNING")
        }
    }

    private fun parsePlayerObject(playerObject: JSONObject) : PlayerProfile? {
        var player = PlayerProfile.findPlayerByNick(playerObject["NICK"] as String)

        player?.score?.score = playerObject["SCORE"] as Int
        player?.score?.roundCount = playerObject["ROUND"] as Int
        player?.score?.position = playerObject["POSITION"] as Int

        //game specific additional info
        var status = playerObject["STATUS"] as JSONObject
        player?.score?.setState(status)

        return player
    }

    fun notifyWinningFragment(saveProgress: SaveProgress) {
        runOnUiThread {
            WinningMessageDialogFragment.saveProgress = saveProgress

            val winningFragment : WinningMessageDialogFragment? = supportFragmentManager.findFragmentByTag("WINNING") as WinningMessageDialogFragment?
            winningFragment?.setCorrectMessageOutside(saveProgress)
        }
    }
}