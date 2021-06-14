package com.mpm.dartsclient.loadedSQLData

import android.util.Log
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.scoring.BasicScore

class LoadedMatch(var ID : Int, var type : String, var subType : String, var date : String) {
    companion object {
        var loadedMatches = mutableListOf<LoadedMatch>()
        var playerStatistics = HashMap<PlayerProfile, Pair<AverageScore, HashMap<Int,AverageScore>>>()

        fun processLoadedMatches() {
            playerStatistics = HashMap()
            var matchCount = loadedMatches.size

            for (player in PlayerProfile.chosenPlayerProfiles) {
                var sumScore = 0.0f
                var sumRound = 0.0f
                var sumPosition = 0.0f

                var resultEntry = HashMap<Int, AverageScore>()

                for ((i, player) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
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

                for ((i, player) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
                    resultEntry[i + 1]!!.averageRound /= resultEntry[i + 1]!!.averagePosition
                    resultEntry[i + 1]!!.averageScore /= resultEntry[i + 1]!!.averagePosition
                    resultEntry[i + 1]!!.averagePosition /= matchCount
                }

                playerStatistics[player] = Pair(averageScore, resultEntry)
            }
        }
    }

    var playerResults = HashMap<PlayerProfile, BasicScore>()

    fun addPlayerResult(player: PlayerProfile, basicScore : BasicScore) {
        playerResults[player] = basicScore
    }
}