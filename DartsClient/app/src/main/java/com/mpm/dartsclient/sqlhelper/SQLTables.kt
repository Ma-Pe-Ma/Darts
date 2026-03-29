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

        const val COLUMN_PLAYER_ID = "id"
        const val COLUMN_PLAYER_NAME = "name"
        const val COLUMN_PLAYER_NICK = "nick"
        const val COLUMN_PLAYER_BACK_COLOR = "backColor"
        const val COLUMN_PLAYER_TEXT_COLOR = "textColor"

        const val SQL_CREATE_PLAYERS =
            "CREATE TABLE IF NOT EXISTS $TABLE_NAME (" +
                "$COLUMN_PLAYER_ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "$COLUMN_PLAYER_NAME TEXT, " +
                "$COLUMN_PLAYER_NICK TEXT, " +
                "$COLUMN_PLAYER_BACK_COLOR INTEGER, " +
                "$COLUMN_PLAYER_TEXT_COLOR INTEGER)"

        private const val SQL_DELETE_ENTRIES = "DROP TABLE IF EXISTS $TABLE_NAME"

        const val listPlayers = "SELECT * FROM  $TABLE_NAME"

        fun readPlayers(): MutableList<PlayerProfile> {
            var playerProfiles: MutableList<PlayerProfile> = ArrayList()

            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbReadable = dbHelper.readableDatabase

            var cursor = dbReadable.rawQuery(listPlayers, null)

            if (cursor.moveToFirst()) {
                do {
                    val id = cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_ID))
                    val name = cursor.getString(cursor.getColumnIndex(COLUMN_PLAYER_NAME))
                    val nick = cursor.getString(cursor.getColumnIndex(COLUMN_PLAYER_NICK))
                    val backColor = cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_BACK_COLOR))
                    val textColor = cursor.getInt(cursor.getColumnIndex(COLUMN_PLAYER_TEXT_COLOR))

                    var readPlayerProfile = PlayerProfile(id, name, nick, backColor, textColor)

                    playerProfiles.add(readPlayerProfile)

                } while (cursor.moveToNext())
            }

            return playerProfiles;
        }

        fun addNewPlayer(name: String, nickname: String, textColor: Int, backgroundColor: Int): Long? {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            var contentValues = ContentValues().apply {
                put(COLUMN_PLAYER_NAME, name)
                put(COLUMN_PLAYER_NICK, nickname)
                put(COLUMN_PLAYER_BACK_COLOR, backgroundColor)
                put(COLUMN_PLAYER_TEXT_COLOR, textColor)
            }

            var newRowID : Long? = dbWritable?.insert(TABLE_NAME, null, contentValues)

            dbWritable.close()
            dbHelper.close()

            return newRowID
        }

        fun updatePlayer(id : Int, name: String, nickname: String, textColor: Int, backgroundColor: Int) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            val updatePlayer =  "UPDATE $TABLE_NAME " +
                                "SET $COLUMN_PLAYER_NICK = '${nickname}', " +
                                    "$COLUMN_PLAYER_NAME = '${name}', " +
                                    "$COLUMN_PLAYER_BACK_COLOR = ${backgroundColor}, " +
                                    "$COLUMN_PLAYER_TEXT_COLOR = ${textColor} "+
                                "WHERE $COLUMN_PLAYER_ID = $id"

            dbWritable.execSQL(updatePlayer)
            dbWritable.close()
            dbHelper.close()
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

    object PlayerMatchData {
        const val TABLE_NAME = "PlayerMatchData"
        const val COLUMN_GAME_ID = "gameId"
        const val COLUMN_PLAYER_ID = "playerId"
        const val COLUMN_PLAYER_POSITION = "position"
        const val COLUMN_PLAYER_SCORE = "score"
        const val COLUMN_ROUND_COUNT = "round"

        const val SQL_CREATE_PLAYER_MATCH_DATA =
            "CREATE TABLE IF NOT EXISTS $TABLE_NAME (" +
                "$COLUMN_GAME_ID UNSIGNED INT, " +
                "$COLUMN_PLAYER_ID UNSIGNED INT," +
                "$COLUMN_PLAYER_POSITION SMALLINT, " +
                "$COLUMN_PLAYER_SCORE INT, " +
                "$COLUMN_ROUND_COUNT INT)"

        fun addEntry(gameId : Int, playerId : Int, position : Int, score : Int, roundCount: Int) {
            var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
            var dbWritable = dbHelper.writableDatabase

            var contentValues = ContentValues().apply {
                put(COLUMN_GAME_ID, gameId)
                put(COLUMN_PLAYER_ID, playerId)
                put(COLUMN_PLAYER_POSITION, position)
                put(COLUMN_PLAYER_SCORE, score)
                put(COLUMN_ROUND_COUNT, roundCount)
            }

            var newRowID : Long? = dbWritable?.insert(TABLE_NAME, null, contentValues)

            dbWritable.close()
            dbHelper.close()
        }
    }

    fun addMatch(gamePlay: GamePlay, id: Int, type: String, subtype: String, date: String, players: List<PlayerProfile>) : SaveProgress {
        if (GamesTable.addGame(id, type, subtype, date)) {
            for (player in players) {
                PlayerMatchData.addEntry(id, player.id, player.score!!.position, player.score!!.score, player.score!!.roundCount)
            }

            return SaveProgress.success;
        }

        return SaveProgress.failure;
    }

    fun findMatches(type : String, subType : String, players : List<PlayerProfile>) : HashMap<Int, LoadedMatch> {
        val namesString = players.joinToString(separator = ", ") { "'${it.id}'" }

        var find_common_matches = "SELECT * " +
            "FROM ${GamesTable.TABLE_NAME} AS games " +
            "LEFT JOIN ${PlayerMatchData.TABLE_NAME} AS playerMatchData ON playerMatchData.${PlayerMatchData.COLUMN_GAME_ID} = games.${GamesTable.COLUMN_GAME_ID} " +
            "LEFT JOIN ${PlayersTable.TABLE_NAME} AS players ON players.${PlayersTable.COLUMN_PLAYER_ID} = playerMatchData.${PlayerMatchData.COLUMN_PLAYER_ID} " +
            "WHERE games.${GamesTable.COLUMN_GAME_ID} IN (" +
                "SELECT ${PlayerMatchData.COLUMN_GAME_ID} " +
                "FROM ${PlayerMatchData.TABLE_NAME} " +
                "WHERE ${PlayerMatchData.COLUMN_PLAYER_ID} IN (${namesString}) " +
                "AND games.${GamesTable.COLUMN_GAME_TYPE} = '${type}' " +
                "AND games.${GamesTable.COLUMN_GAME_SUBTYPE} = '${subType}'"
                "GROUP BY ${PlayerMatchData.COLUMN_GAME_ID} " +
                "HAVING COUNT(DISTINCT ${PlayerMatchData.COLUMN_PLAYER_ID}) = ${players.count()}) " +
            ") "

        var dbHelper = DBHelper(DartsClientApplication.application?.applicationContext!!)
        var dbReadable = dbHelper.readableDatabase

        var cursor = dbReadable.rawQuery(find_common_matches, null)

        var matchMap = HashMap<Int, LoadedMatch>()

        if (cursor.moveToFirst()) {
            do {
                var matchID = cursor.getInt(cursor.getColumnIndex("games.$GamesTable.COLUMN_GAME_ID"))

                //Todo create prope matchobjects

            } while (cursor.moveToNext())
        }

        cursor.close()
        dbReadable.close()
        dbHelper.close()

        return matchMap;
    }
}