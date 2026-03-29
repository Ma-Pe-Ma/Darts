package com.mpm.dartsclient.games

import com.mpm.dartsclient.games.cricket.Cricket

class DartsGameContainer {
    companion object {
        @Volatile
        private var instance: DartsGameContainer? = null

        fun getInstance(): DartsGameContainer {
            return instance ?: synchronized(this) {
                instance ?: DartsGameContainer().also { instance = it }
            }
        }
    }public

    var games : MutableList<DartsGame> = ArrayList()
    var currentGame : DartsGame? = null

    init {
        games.add(Cricket("CRICKET", "Cricket"))
        games.add(RoundTheClock("RTC", "Round the Clock"))
        games.add(X01("X01", "X01"))
    }

    fun findGameByName (ID : String) : DartsGame?{
        for (game in games) {
            if (game.gameID == ID) {
                return game
            }
        }

        return currentGame
    }

    fun findNumberOfGame(searchedGame : DartsGame?) : Int? {
        for ((numberG, game) in games.withIndex()) {
            if (game == searchedGame) {
                return numberG
            }
        }

        return null
    }
}