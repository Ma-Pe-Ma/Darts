package com.mpm.dartsclient.games

import androidx.fragment.app.Fragment
import com.mpm.dartsclient.scoring.scoring.GameScore
import org.json.JSONObject

abstract class DartsGame(var gameID : String, var name : String) {
    var subtype : String = ""

    abstract fun parseConfigParameters(jsonObject: JSONObject)
    abstract fun serializeConfigParameters() : JSONObject
    abstract fun getConfigFragment(dartsGameContainer: DartsGameContainer) : Fragment
    abstract fun getScoreObject() : GameScore
}