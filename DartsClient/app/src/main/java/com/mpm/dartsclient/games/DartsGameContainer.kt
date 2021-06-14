package com.mpm.dartsclient.games

import androidx.fragment.app.Fragment
import com.mpm.dartsclient.games.cricket.Cricket
import com.mpm.dartsclient.scoring.scoring.GameScore
import org.json.JSONObject

abstract class DartsGameContainer(var gameID : String, var name : String) {

    var subtype : String = ""

    companion object {
        var games : MutableList<DartsGameContainer> = ArrayList()
        var currentGame : DartsGameContainer? = null

        init {
            games.add(Cricket("CRICKET", "Cricket"))
            games.add(RoundTheClock("RTC", "Round the Clock"))
            games.add(X01("X01", "X01"))
        }

        fun findGameByName (ID : String) : DartsGameContainer?{
            for (game in games) {
                if (game.gameID == ID) {
                    return game
                }
            }

            return currentGame
        }

        fun findNumberOfGame(searchedGame : DartsGameContainer?) : Int? {
            for ((numberG, game) in games.withIndex()) {
                if (game == searchedGame) {
                    return numberG
                }
            }

            return null
        }
    }

    abstract fun parseConfigParameters(jsonObject: JSONObject)
    abstract fun serializeConfigParameters() : JSONObject
    abstract fun getConfigFragment() : Fragment
    abstract fun getScoreObject() : GameScore
}