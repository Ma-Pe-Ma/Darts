package com.mpm.dartsclient.games

import android.app.Activity
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.fragments.gameconfig.RoundTheClockConfig
import com.mpm.dartsclient.scoring.CricketScore
import com.mpm.dartsclient.scoring.Score
import org.json.JSONObject

class RoundTheClock(gameID : String, name :String) : DartsGameContainer(gameID, name) {

    override fun parseConfigParameters(jsonObject: JSONObject, activity: Activity) {

    }

    override fun getConfigFragment(): Fragment {
        //TODO("Not yet implemented")
        return RoundTheClockConfig()
    }

    override fun serializeConfigParameters(activity: Activity): JSONObject {
        var config = JSONObject()




        return config
    }

    override fun getScoreObject(): Score {
        //TODO("Not yet implemented")
        return CricketScore()
    }
}