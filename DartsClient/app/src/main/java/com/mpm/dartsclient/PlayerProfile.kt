package com.mpm.dartsclient

import com.mpm.dartsclient.scoring.Score

class PlayerProfile(var name : String, var nickname : String, var color : Int) {
    var inverseColor = (0xFFFFFF - color).or(0xFF000000.toInt())

    companion object {
        var playerProfiles : MutableList<PlayerProfile> = ArrayList()
        var chosenPlayerProfiles : MutableList<PlayerProfile> = ArrayList()
        var currentPlayer : PlayerProfile? = null
        var currentCursor : Int = 0

        init {
            playerProfiles.add(PlayerProfile("PETER", "MPM", -11462))
            playerProfiles.add(PlayerProfile("ZS", "MZS", -16711936))
            playerProfiles.add(PlayerProfile("PETER2", "MPM2", -11462))
            playerProfiles.add(PlayerProfile("PETER3", "MPM3", -11462))
            playerProfiles.add(PlayerProfile("PETER4", "MPM4", -11462))
            playerProfiles.add(PlayerProfile("PETER5", "MPM5", -11462))
            playerProfiles.add(PlayerProfile("PETER6", "MPM6", -11462))
            playerProfiles.add(PlayerProfile("PETER7", "MPM7", -11462))
            playerProfiles.add(PlayerProfile("PETER8", "MPM8", -11462))
            playerProfiles.add(PlayerProfile("PETER9", "MPM9", -11462))
            playerProfiles.add(PlayerProfile("PETER10", "MPM10", -11462))
            playerProfiles.add(PlayerProfile("PETER11", "MPM11", -11462))
        }
    }

    var score : Score? = null
}