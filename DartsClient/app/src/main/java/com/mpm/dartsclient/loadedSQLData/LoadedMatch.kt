package com.mpm.dartsclient.loadedSQLData

import android.util.Log
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.scoring.BasicScore

class LoadedMatch(var ID : Int, var type : String, var subType : String, var date : String) {
    var playerResults = HashMap<PlayerProfile, BasicScore>()

    fun addPlayerResult(player: PlayerProfile, basicScore : BasicScore) {
        playerResults[player] = basicScore
    }
}