package com.mpm.dartsclient.games.cricket

import android.content.Context
import android.content.SharedPreferences
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.DartsClientApplication
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.fragments.gameconfig.CricketConfig
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.scoring.scoring.CricketScore
import com.mpm.dartsclient.scoring.scoring.GameScore
import org.json.JSONObject

class Cricket(gameID : String, name :String) : DartsGameContainer(gameID, name) {
    companion object {
        var scoreMap = Array(21) {0}
    }

    override fun parseConfigParameters(config: JSONObject) {
        var context = DartsClientApplication.context

        var cricketSharedPrefEditor : SharedPreferences.Editor =
            context?.getSharedPreferences(context.getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE)!!
            .edit()

        var cricketNr = config["CricketNr"] as Int;
        var cricketStart = config["CricketStart"] as Int;

        var cricketType  = config["CricketType"] as String;
        var cricketNumberSet = config["CricketNumberSet"] as String;
        var cricketCustomSet = config["CricketCustomSet"] as String;

        cricketSharedPrefEditor.putString("CricketType", cricketType)
        cricketSharedPrefEditor.putString("CricketNumberSet", cricketNumberSet)
        cricketSharedPrefEditor.putString("CricketCustomSet", cricketCustomSet)

        cricketSharedPrefEditor.putInt("CricketNr", cricketNr)
        cricketSharedPrefEditor.putInt("CricketStart", cricketStart)

        cricketSharedPrefEditor.apply()

        currentGame!!.subtype = cricketType

        var rawMap = config["MAP"] as JSONObject
        scoreMap = Array(21) {0}

        var keys = rawMap.keys()

        while (keys.hasNext()) {
            var key = keys.next()

            var sector : Int = key.toInt()
            var value : Int = rawMap.get(key) as Int

            scoreMap[sector] = value
        }
    }

    override fun serializeConfigParameters() : JSONObject {
        var configObject = JSONObject()

        var context = DartsClientApplication.context

        var sharedPreferences = context?.getSharedPreferences(context.getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE)

        val defaultType = SubCricket.SCORE.toString()
        val cricketType = sharedPreferences!!.getString("CricketType", defaultType)!!
        configObject.put("CricketType", cricketType)

        val defaultSet = CricketSet.CLASSIC.toString()
        val cricketNumberSet = sharedPreferences.getString("CricketNumberSet", defaultSet)!!
        configObject.put("CricketNumberSet", cricketNumberSet)

        val defaultCustom = CricketCustomSet.INTERVAL.toString()
        val cricketCustomSet = sharedPreferences.getString("CricketCustomSet", defaultCustom)!!
        configObject.put("CricketCustomSet", cricketCustomSet)

        val numberOfNumbersValue : Int = sharedPreferences.getInt("CricketNr", 6)
        val intervalStartingNrValue : Int = sharedPreferences.getInt("CricketStart", 15)
        configObject.put("CricketNr", numberOfNumbersValue)
        configObject.put("CricketStart", intervalStartingNrValue)

        return configObject
    }

    override fun getConfigFragment() : Fragment {
        return CricketConfig()
    }

    override fun getScoreObject() : GameScore {
        return CricketScore()
    }
}