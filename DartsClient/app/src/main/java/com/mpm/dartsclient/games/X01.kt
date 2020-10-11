package com.mpm.dartsclient.games

import android.app.Activity
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.fragments.gameconfig.X01Config
import com.mpm.dartsclient.scoring.CricketScore
import com.mpm.dartsclient.scoring.Score
import org.json.JSONObject

class X01(gameID : String, name :String) : DartsGameContainer(gameID, name) {

    override fun parseConfigParameters(jsonObject: JSONObject, activity: Activity) {

    }

    override fun getConfigFragment(): Fragment {
        return X01Config()
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