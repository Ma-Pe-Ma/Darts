package com.mpm.dartsclient.games

import android.app.Activity
import android.content.Context
import android.content.SharedPreferences
import android.util.Log
import androidx.fragment.app.Fragment
import com.mpm.dartsclient.R
import com.mpm.dartsclient.fragments.gameconfig.CricketConfig
import com.mpm.dartsclient.scoring.CricketScore
import com.mpm.dartsclient.scoring.Score
import org.json.JSONObject

class Cricket(gameID : String, name :String) : DartsGameContainer(gameID, name) {

    override fun parseConfigParameters(config: JSONObject, activity : Activity) {
        var cricketSharedPrefEditor : SharedPreferences.Editor =  activity.getSharedPreferences(
            activity.getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE).edit()

        Log.i("DARTS", "Mes: "+config)

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

        Log.i("DARTS", "Set crick config!");

        cricketSharedPrefEditor.commit()
    }

    override fun serializeConfigParameters(activity: Activity): JSONObject {
        var configObject = JSONObject()

        var sharedPreferences = activity.getSharedPreferences(activity.getString(R.string.cricketSharedPreference), Context.MODE_PRIVATE)

        val defaultType = activity.getString(R.string.cricketTypeM1)
        val cricketType = sharedPreferences!!.getString("CricketType", defaultType)!!
        configObject.put("CricketType", cricketType)

        val defaultSet = activity.getString(R.string.cricketSetM1)
        val cricketNumberSet = sharedPreferences!!.getString("CricketNumberSet", defaultSet)!!
        configObject.put("CricketNumberSet", cricketNumberSet)

        val defaultCustom = activity.getString(R.string.customSetM1)
        val cricketCustomSet = sharedPreferences!!.getString("CricketCustomSet", defaultCustom)!!
        configObject.put("CricketCustomSet", cricketCustomSet)

        val numberOfNumbersValue : Int = sharedPreferences!!.getInt("CricketNr", 6)
        val intervalStartingNrValue : Int = sharedPreferences!!.getInt("CricketStart", 15)
        configObject.put("CricketNr", numberOfNumbersValue)
        configObject.put("CricketStart", intervalStartingNrValue)

        //Log.i("DARTS", "CRIC: $configObject")

        return configObject
    }

    override fun getConfigFragment(): Fragment {
        return CricketConfig()
    }

    override fun getScoreObject(): Score {
        return CricketScore()
    }


}