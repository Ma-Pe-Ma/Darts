package com.mpm.dartsclient.games

import androidx.fragment.app.Fragment
import com.mpm.dartsclient.activities.config.fragments.gameconfig.RoundTheClockConfig
import com.mpm.dartsclient.scoring.scoring.CricketScore
import com.mpm.dartsclient.scoring.scoring.GameScore
import org.json.JSONObject

class RoundTheClock(gameID : String, name :String) : DartsGameContainer(gameID, name) {

    override fun parseConfigParameters(jsonObject: JSONObject) {

    }

    override fun getConfigFragment(): Fragment {
        //TODO("Not yet implemented")
        return RoundTheClockConfig()
    }

    override fun serializeConfigParameters(): JSONObject {
        var config = JSONObject()




        return config
    }

    override fun getScoreObject(): GameScore {
        //TODO("Not yet implemented")
        return CricketScore()
    }
}