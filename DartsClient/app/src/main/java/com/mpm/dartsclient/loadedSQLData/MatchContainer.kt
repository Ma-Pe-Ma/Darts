package com.mpm.dartsclient.loadedSQLData

import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.sqlhelper.SQLTables

class MatchContainer {

    companion object {
        @Volatile
        private var instance: MatchContainer? = null

        fun getInstance(): MatchContainer {
            return instance ?: synchronized(this) {
                instance ?: MatchContainer().also { instance = it }
            }
        }
    }public

    var loadedMatches = mutableListOf<LoadedMatch>()
    var playerStatistics = HashMap<PlayerProfile, Pair<AverageScore, HashMap<Int,AverageScore>>>()

    fun createStatistics(profileContainer: ProfileContainer, dartsGameContainer: DartsGameContainer) {
        if (profileContainer.chosenPlayerProfiles.size == 0) {
            loadedMatches = mutableListOf()
            return
        }

        SQLTables.findMatches(dartsGameContainer.currentGame!!.gameID, dartsGameContainer.currentGame!!.subtype, profileContainer.chosenPlayerProfiles)

        playerStatistics = HashMap()
        var matchCount = loadedMatches.size

        for (player in profileContainer.chosenPlayerProfiles) {
            var sumScore = 0.0f
            var sumRound = 0.0f
            var sumPosition = 0.0f

            var resultEntry = HashMap<Int, AverageScore>()

            for ((i, player) in profileContainer.chosenPlayerProfiles.withIndex()) {
                resultEntry[i + 1] = AverageScore(0.0f,0.0f,0.0f)
            }

            for (match in loadedMatches) {
                var matchResult = match.playerResults[player]!!

                //Log.i("DARTS", player.nickname +" MATCH POS: "+ matchResult.position)

                sumScore += matchResult.score
                sumRound += matchResult.roundCount
                sumPosition += matchResult.position

                resultEntry[matchResult.position]!!.averageScore += matchResult.score
                resultEntry[matchResult.position]!!.averageRound += matchResult.roundCount
                resultEntry[matchResult.position]!!.averagePosition += 1
            }

            if (matchCount == 0) {
                playerStatistics[player] = Pair(AverageScore(0f, 0f, 0f), resultEntry)
                continue
            }

            //Log.i("DARTS", player.nickname +" sumpos: "+sumPosition + ", av: "+sumPosition / matchCount)

            var averageScore = AverageScore(sumScore / matchCount,sumPosition / matchCount,  sumRound / matchCount)

            for ((i, player) in profileContainer.chosenPlayerProfiles.withIndex()) {
                resultEntry[i + 1]!!.averageRound /= resultEntry[i + 1]!!.averagePosition
                resultEntry[i + 1]!!.averageScore /= resultEntry[i + 1]!!.averagePosition
                resultEntry[i + 1]!!.averagePosition /= matchCount
            }

            playerStatistics[player] = Pair(averageScore, resultEntry)
        }
    }
}