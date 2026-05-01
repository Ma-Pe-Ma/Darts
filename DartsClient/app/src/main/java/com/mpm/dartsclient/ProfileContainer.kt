package com.mpm.dartsclient

class ProfileContainer {
    companion object {
        @Volatile
        private var instance: ProfileContainer? = null

        fun getInstance(): ProfileContainer {
            return instance ?: synchronized(this) {
                instance ?: ProfileContainer().also { instance = it }
            }
        }
    }public

    var playerProfiles : MutableList<PlayerProfile> = ArrayList()
    var chosenPlayerProfiles : MutableList<PlayerProfile> = ArrayList()
    var currentPlayer : PlayerProfile? = null
    var currentCursor : Int = 0

    fun setPlayerOrder(mode : Int) {
        when(mode) {
            0 -> {
                val newOrder = chosenPlayerProfiles.drop(1).toMutableList()
                newOrder.add(chosenPlayerProfiles[0])
                chosenPlayerProfiles = newOrder
            }
            1 -> {
                chosenPlayerProfiles = chosenPlayerProfiles.reversed().toMutableList()
            }
            2 -> {}
        }
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

    fun checkTakenNickname(existingPlayer: PlayerProfile?, newName: String): Boolean {
        var checkableUsers =
            if (existingPlayer != null) {
                playerProfiles.filter { it.id != existingPlayer.id }
            }
            else {
                playerProfiles;
            }

        return checkableUsers.any {it.nickname == newName};
    }
}