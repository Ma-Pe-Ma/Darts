package com.mpm.dartsclient

import com.mpm.dartsclient.scoring.scoring.GameScore

//Important nickname is the primary key!
class PlayerProfile(var name : String, var nickname : String, var backgroundColor : Int?, var textColor : Int?) {
    companion object {
        var playerProfiles : MutableList<PlayerProfile> = ArrayList()
        var chosenPlayerProfiles : MutableList<PlayerProfile> = ArrayList()
        var currentPlayer : PlayerProfile? = null
        var currentCursor : Int = 0

        fun setPlayerOrder(mode : Int) {
            when(mode) {
                0 -> orderCyclicModification()
                1 -> orderReverseModification()
                2 -> {}
            }
        }

        fun orderCyclicModification() {
            val newOrder = mutableListOf<PlayerProfile>()

            for (i in 1 until chosenPlayerProfiles.size) {
                newOrder.add(chosenPlayerProfiles[i])
            }

            newOrder.add(chosenPlayerProfiles[0])

            chosenPlayerProfiles = newOrder
        }

        fun orderReverseModification() {
            var newOrder = mutableListOf<PlayerProfile>()

            for (i in chosenPlayerProfiles.size - 1..0) {
                newOrder.add(chosenPlayerProfiles[i])
            }

            chosenPlayerProfiles = newOrder
        }

        var roundCounter = 0

        fun findNextPlayer() : PlayerProfile {
            while (true) {
                if (++currentCursor == chosenPlayerProfiles.size) {
                    currentCursor = 0
                    roundCounter++
                }

                var checkable = chosenPlayerProfiles[currentCursor]

                if (checkable.score!!.position < 0) {
                    currentPlayer = checkable
                    return checkable
                }
            }
        }

        fun findPlayerByNick(nick : String) : PlayerProfile? {
            for (player in chosenPlayerProfiles) {
                if (player.nickname == nick) {
                    return player
                }
            }

            return null
        }
    }

    var score : GameScore? = null
}