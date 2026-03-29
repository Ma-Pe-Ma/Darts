package com.mpm.dartsclient

import com.mpm.dartsclient.scoring.scoring.GameScore

//Important nickname is the primary key!
class PlayerProfile(var id: Int, var name: String, var nickname: String, var backgroundColor: Int?, var textColor : Int?) {
    var score : GameScore? = null
}