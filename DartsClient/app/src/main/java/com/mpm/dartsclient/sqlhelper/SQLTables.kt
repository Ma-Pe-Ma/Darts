package com.mpm.dartsclient.sqlhelper

import android.content.ContentValues
import android.provider.BaseColumns
import android.util.Log
import com.mpm.dartsclient.DartsClientApplication
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.activities.gamePlay.GamePlay
import com.mpm.dartsclient.activities.gamePlay.SaveProgress
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.LoadedMatch
import com.mpm.dartsclient.scoring.BasicScore


object SQLTables {

    object PlayersTable : BaseColumns {
        const val TABLE_NAME = "players"
        const val COLUMN_PLAYER_NAME = "name"
        const val COLUMN_PLAYER_NICK = "nick"
        const val COLUMN_PLAYER_BACK_COLOR = "backColor"
        const val COLUMN_PLAYER_TEXT_COLOR = "textColor"

        const val SQL_CREATE_PLAYERS =
            "CREATE TABLE IF NOT EXISTS $TABLE_NAME (" +
                    "$COLUMN_PLAYER_NAME TEXT, " +
                    "$COLUMN_PLAYER_NICK TEXT PRIMARY KEY, " +
                    "$COLUMN_PLAYER_BACK_COLOR INTEGER, " +
                    "$COLUMN_PLAYER_TEXT_COLOR INTEGER)"

        private const val SQL_DELETE_ENTRIES = "DROP TABLE IF EXISTS $TABLE_NAME"

        const val listPlayers = "SELECT * FROM  $TABLE_NAME"

        fun readPlayers() {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbReadable = dbHelper.readableDatabase

            var cursor = dbReadable.rawQuery(listPlayers, null)

            if (cursor.moveToFirst()) {
                do {
                    val name = cursor.getString(cursor.getColumnIndex(COLUMN_PLAYER_NAME))
                    val nick = cursor.getString(cursor.getColumnIndex(COLUMN_PLAYER_NICK))
                    val backColor = cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_BACK_COLOR))
                    val textColor = cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_TEXT_COLOR))

                    var readPlayerProfile = PlayerProfile(name, nick, backColor, textColor)

                    PlayerProfile.playerProfiles.add(readPlayerProfile)

                } while (cursor.moveToNext())
            }
        }

         fun addNewPlayer(playerProfile : PlayerProfile) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            var contentValues = ContentValues().apply {
                put(COLUMN_PLAYER_NAME, playerProfile.name)
                put(COLUMN_PLAYER_NICK, playerProfile.nickname)
                put(COLUMN_PLAYER_BACK_COLOR, playerProfile.backgroundColor)
                put(COLUMN_PLAYER_TEXT_COLOR, playerProfile.textColor)
            }

            var newRowID : Long? = dbWritable?.insert(TABLE_NAME, null, contentValues)

            if (newRowID?.compareTo(-1) == 0) {
                //Log.i("DARTS", "NEW PLAYER INJECTION FAILED!")
            }
            else {
                //Log.i("DARTS", "NEW PLAYER INJECTION success at: " + newRowID)
            }

            dbWritable.close()
            dbHelper.close()
        }

        fun updatePlayer(modifiedProfile : PlayerProfile, oldKey : String) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            val updatePlayer =  "UPDATE $TABLE_NAME " +
                                "SET $COLUMN_PLAYER_NICK = '${modifiedProfile.nickname}', " +
                                    "$COLUMN_PLAYER_NAME = '${modifiedProfile.name}', " +
                                    "$COLUMN_PLAYER_BACK_COLOR = ${modifiedProfile.backgroundColor}, " +
                                    "$COLUMN_PLAYER_TEXT_COLOR = ${modifiedProfile.textColor} "+
                                "WHERE $COLUMN_PLAYER_NICK = '$oldKey'"

            dbWritable.execSQL(updatePlayer)
            dbWritable.close()
            dbHelper.close()

            if (modifiedProfile.nickname != oldKey) {
                GamePlayerTable.changePlayerNick(oldKey, modifiedProfile.nickname)
            }
        }
    }

    object GamesTable {
        const val TABLE_NAME = "games"
        const val COLUMN_GAME_ID = "id"
        const val COLUMN_GAME_TYPE = "type"
        const val COLUMN_GAME_SUBTYPE = "subtype"
        const val COLUMN_GAME_DATE = "date"

        const val SQL_CREATE_GAMES =
            "CREATE TABLE IF NOT EXISTS $TABLE_NAME (" +
                    "$COLUMN_GAME_ID UNSIGNED INT PRIMARY KEY, " +
                    "$COLUMN_GAME_TYPE TEXT, " +
                    "$COLUMN_GAME_SUBTYPE TEXT, " +
                    "$COLUMN_GAME_DATE DATETIME)"

        private const val SQL_FIND_GAME_NR = "SELECT $COLUMN_GAME_ID " +
                                    "FROM $TABLE_NAME " +
                                    "WHERE $COLUMN_GAME_ID = (SELECT MAX($COLUMN_GAME_ID) FROM $TABLE_NAME)"

        fun findGameNr() : Int {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbReadable = dbHelper.readableDatabase

            var gameNr : Int = 0

            var cursor = dbReadable.rawQuery(SQL_FIND_GAME_NR, null)

            if (cursor.moveToFirst()) {
                gameNr = cursor.getInt(cursor.getColumnIndex(COLUMN_GAME_ID))
            }

            cursor.close()

            dbReadable.close()
            dbHelper.close()

            return gameNr
        }

        fun addGame(id : Int, type : String, subtype : String, date : String) : Boolean {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            var contentValues = ContentValues().apply {
                put(COLUMN_GAME_ID, id)
                put(COLUMN_GAME_TYPE, type)
                put(COLUMN_GAME_SUBTYPE, subtype)
                put(COLUMN_GAME_DATE, date)
            }

            var newRowID : Long? = dbWritable?.insert(TABLE_NAME, null, contentValues)

            var success = false

            if (newRowID?.compareTo(-1) == 0) {
                //Log.i("DARTS", "NEW GAME INJECTION FAILED!")
            }
            else {
                success = true
                //Log.i("DARTS", "NEW GAME INJECTION success at: " + newRowID)
            }

            dbWritable.close()
            dbHelper.close()

            return success
        }

        fun listGames() {
            val SQL_LIST_GAMES = "SELECT * FROM $TABLE_NAME"

            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbReadable = dbHelper.readableDatabase

            var cursor = dbReadable.rawQuery(SQL_LIST_GAMES, null)

            if (cursor.moveToFirst()) {
                do {
                    /*Log.i("DARTS", "---------------------------")
                    Log.i("DARTS", "GAMEID: "+cursor.getInt(cursor.getColumnIndex(COLUMN_GAME_ID)))
                    Log.i("DARTS", "TYPE: "+cursor.getString(cursor.getColumnIndex(COLUMN_GAME_TYPE)))
                    Log.i("DARTS", "SUB: "+cursor.getString(cursor.getColumnIndex(COLUMN_GAME_SUBTYPE)))
                    Log.i("DARTS", "DATE: "+cursor.getString(cursor.getColumnIndex(COLUMN_GAME_DATE)))*/
                } while (cursor.moveToNext())
            }

            cursor.close()

            dbReadable.close()
            dbHelper.close()

        }
    }

    object GamePlayerTable {
        const val TABLE_NAME = "GamePlayerRelation"
        const val COLUMN_GAME_ID = "gameId"
        const val COLUMN_PLAYER_NICK = "nick"
        const val COLUMN_PLAYER_POSITION = "position"
        const val COLUMN_PLAYER_SCORE = "score"
        const val COLUMN_ROUND_COUNT = "round"

        const val SQL_CREATE_GAME_PLAYER =
            "CREATE TABLE IF NOT EXISTS $TABLE_NAME (" +
                    "$COLUMN_GAME_ID UNSIGNED INT, " +
                    "$COLUMN_PLAYER_NICK TEXT," +
                    "$COLUMN_PLAYER_POSITION SMALLINT, " +
                    "$COLUMN_PLAYER_SCORE INT, " +
                    "$COLUMN_ROUND_COUNT INT)"

        fun addEntry(gameID : Int, nick : String, position : Int, score : Int, roundCount: Int) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            var contentValues = ContentValues().apply {
                put(COLUMN_GAME_ID, gameID)
                put(COLUMN_PLAYER_NICK, nick)
                put(COLUMN_PLAYER_POSITION, position)
                put(COLUMN_PLAYER_SCORE, score)
                put(COLUMN_ROUND_COUNT, roundCount)
            }

            var newRowID : Long? = dbWritable?.insert(TABLE_NAME, null, contentValues)

            if (newRowID?.compareTo(-1) == 0) {
                //Log.i("DARTS", "NEW GAME PLAYER INJECTION FAILED!")
            }
            else {
                //Log.i("DARTS", "NEW GAME PLAYER INJECTION success at: " + newRowID)
            }

            dbWritable.close()
            dbHelper.close()
        }

        fun changePlayerNick(oldNick : String, newNick : String) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            val updatePlayer =  "UPDATE $TABLE_NAME " +
                    "SET $COLUMN_PLAYER_NICK = '$newNick' " +
                    "WHERE $COLUMN_PLAYER_NICK = '$oldNick'"

            dbWritable.execSQL(updatePlayer)
            dbWritable.close()
            dbHelper.close()
        }

        fun listGamePlayers() {
            val SQL_LIST_GAME_PLAYER = "SELECT * FROM $TABLE_NAME"

            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbReadable = dbHelper.readableDatabase

            var cursor = dbReadable.rawQuery(SQL_LIST_GAME_PLAYER, null)

            if (cursor.moveToFirst()) {
                do {
                    /*Log.i("DARTS", "---------------------------")
                    Log.i("DARTS", "GAMEID: "+cursor.getInt(cursor.getColumnIndex(COLUMN_GAME_ID)))
                    Log.i("DARTS", "NICK: "+cursor.getString(cursor.getColumnIndex(COLUMN_PLAYER_NICK)))
                    Log.i("DARTS", "POS: "+cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_POSITION)))
                    Log.i("DARTS", "SCO: "+cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_SCORE)))
                    Log.i("DARTS", "ROU: "+cursor.getInt(cursor.getColumnIndex(COLUMN_ROUND_COUNT)))*/
                } while (cursor.moveToNext())
            }

            cursor.close()

            dbReadable.close()
            dbHelper.close()
        }
    }

    fun addMatch(gamePlay: GamePlay, id : Int, type : String, subtype: String, date : String, players : List<PlayerProfile>) {
        //if new game creation is successful then add players data too!

        var savingThread = Thread() {
            if (GamesTable.addGame(id, type, subtype, date)) {
                for (player in players) {
                    GamePlayerTable.addEntry(id, player.nickname, player.score!!.position, player.score!!.score, player.score!!.roundCount)
                }

                gamePlay.notifyWinningFragment(SaveProgress.success)

                var thread2 = Thread() {
                    createStatistics()
                }

                thread2.start()
            }
            else {
                gamePlay.notifyWinningFragment(SaveProgress.failure)
            }
        }

        savingThread.start()
    }

    fun createStatistics() {
        if (PlayerProfile.chosenPlayerProfiles.size == 0) {
            LoadedMatch.loadedMatches = mutableListOf()
            return
        }

        findMatches(DartsGameContainer.currentGame!!.gameID, DartsGameContainer.currentGame!!.subtype, PlayerProfile.chosenPlayerProfiles)

        LoadedMatch.processLoadedMatches()
    }

    private fun findMatches(type : String, subType : String, players : List<PlayerProfile>) {
        var matchMap : HashMap<Int, LoadedMatch> = findProperGames(type, subType)
        //findProperPlayers(matchMap, players)
        findPlayersForFilteredMatches(matchMap)

        LoadedMatch.loadedMatches = matchMap.entries.map { it.value }.toMutableList()

        Log.i("DARTS", "common matches: " + LoadedMatch.loadedMatches.size)

        for ((i,match) in LoadedMatch.loadedMatches.withIndex()) {
            Log.i("DARTS", i.toString()+".th match ID: "+match.ID)
        }
    }

    //method to find the games by the given type and subtype
    private fun findProperGames(type : String, subType : String) : HashMap<Int, LoadedMatch>{
        var SQL_FIND_MATCHES_BY_TYPE = "SELECT * " +
                "FROM ${GamesTable.TABLE_NAME} " +
                "WHERE (${GamesTable.COLUMN_GAME_TYPE} = '${type}' AND ${GamesTable.COLUMN_GAME_SUBTYPE} = '${subType}')"

        var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
        var dbReadable = dbHelper.readableDatabase

        var cursor = dbReadable.rawQuery(SQL_FIND_MATCHES_BY_TYPE, null)

        var matchMap = HashMap<Int, LoadedMatch>()

        if (cursor.moveToFirst()) {
            do {
                var matchID = cursor.getInt(cursor.getColumnIndex(GamesTable.COLUMN_GAME_ID))
                var date = cursor.getString(cursor.getColumnIndex(GamesTable.COLUMN_GAME_DATE))
                var newLoadedMatch = LoadedMatch(matchID, type, subType, date)
                matchMap[matchID] = newLoadedMatch
            } while (cursor.moveToNext())
        }

        cursor.close()
        dbReadable.close()
        dbHelper.close()

        return matchMap
    }

    fun findPlayersForFilteredMatches(matchMap : HashMap<Int, LoadedMatch>) {
        if (matchMap.size == 0) {
            return
        }

        var SQL_MATCH_CONDITION = ""

        var playerMap = HashMap<String, PlayerProfile>()
        for (player in PlayerProfile.playerProfiles) {
            playerMap[player.nickname] = player
        }

        var matchIndex = 0
        for (match in matchMap) {
            var partialCondition = "(${GamePlayerTable.COLUMN_GAME_ID} = ${match.key})"

            if (++matchIndex != matchMap.size) {
                partialCondition += " OR "
            }
            SQL_MATCH_CONDITION += partialCondition
        }

        var SQL_FIND_PROPER_PLAYERS = "SELECT * " +
                "FROM ${GamePlayerTable.TABLE_NAME} " +
                "WHERE " +
                "($SQL_MATCH_CONDITION)"

        var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
        var dbReadable = dbHelper.readableDatabase

        var cursor = dbReadable.rawQuery(SQL_FIND_PROPER_PLAYERS, null)

        if (cursor.moveToFirst()) {
            do {
                var matchID = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_GAME_ID))

                if (matchMap.containsKey(matchID)) {
                    var nick = cursor.getString(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_NICK))
                    var player = playerMap[nick]

                    var loadedMatch = matchMap[matchID]

                    var position = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_POSITION))
                    var score = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_SCORE))
                    var round = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_ROUND_COUNT))

                    var loadedPlayerResult = BasicScore(position, score, round)

                    loadedMatch?.addPlayerResult(player!!, loadedPlayerResult)
                }

            } while (cursor.moveToNext())
        }

        //remove matches where not (only) current players played
        var matchIterator : MutableIterator<Map.Entry<Int, LoadedMatch>> = matchMap.iterator()
        while (matchIterator.hasNext()) {
            var loadedMatch = matchIterator.next().value

            //when players doesn't much the current player count remove it
            if (loadedMatch.playerResults.size != PlayerProfile.chosenPlayerProfiles.size) {
                matchIterator.remove()
                continue
            }

            //when a current player is not found in a match remove it
            for (player in PlayerProfile.chosenPlayerProfiles) {
                if (loadedMatch.playerResults[player] == null) {
                    matchIterator.remove()
                    continue
                }
            }
        }
    }

    //method to sort the result (given by findProperGames) by the playing players
    private fun findProperPlayers(matchMap : Map<Int, LoadedMatch>, players : List<PlayerProfile>) {
        var SQL_NICK_CONDITION = ""

        var playerMap = HashMap<String, PlayerProfile>()
        for ((playerNr, player) in players.withIndex()) {
            val nick = player.nickname
            playerMap[player.nickname] = player

            var partialCondition = "(${GamePlayerTable.COLUMN_PLAYER_NICK} = '$nick')"

            if (playerNr != players.size - 1) {
                partialCondition += " OR "
            }

            SQL_NICK_CONDITION += partialCondition
        }

        val SQL_FIND_COMMON_MATCHES = "SELECT * " +
                "FROM  ${GamePlayerTable.TABLE_NAME} " +
                "WHERE " +
                "($SQL_NICK_CONDITION)"

        var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
        var dbReadable = dbHelper.readableDatabase

        var cursor = dbReadable.rawQuery(SQL_FIND_COMMON_MATCHES, null)

        if (cursor.moveToFirst()) {
            do {
                var matchID = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_GAME_ID))

                if (matchMap.containsKey(matchID)) {
                    var nick = cursor.getString(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_NICK))
                    var player = playerMap[nick]

                    var loadedMatch = matchMap[matchID]

                    var position = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_POSITION))
                    var score = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_PLAYER_SCORE))
                    var round = cursor.getInt(cursor.getColumnIndex(GamePlayerTable.COLUMN_ROUND_COUNT))

                    var loadedPlayerResult = BasicScore(position, score, round)

                    loadedMatch?.addPlayerResult(player!!, loadedPlayerResult)
                }
            } while (cursor.moveToNext())
        }

        cursor.close()
        dbReadable.close()
        dbHelper.close()
    }
}