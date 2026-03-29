package com.mpm.dartsclient.sqlhelper

import android.content.Context
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import java.sql.SQLException

class DBHelper(context: Context) : SQLiteOpenHelper(context, DATABASE_NAME, null, DATABASE_VERSION) {
    companion object {
        // If you change the database schema, you must increment the database version.
        const val DATABASE_VERSION = 2
        const val DATABASE_NAME = "DARTS.db"
    }

    override fun onCreate(db: SQLiteDatabase?) {
        try{
            db?.execSQL(SQLTables.PlayersTable.SQL_CREATE_PLAYERS)
            db?.execSQL(SQLTables.GamesTable.SQL_CREATE_GAMES)
            db?.execSQL(SQLTables.PlayerMatchData.SQL_CREATE_PLAYER_MATCH_DATA)
        }
        catch (e : SQLException) {

        }
    }

    override fun onUpgrade(db: SQLiteDatabase?, oldVersion: Int, newVersion: Int) {
        //TODO("Not yet implemented")
        //Log.i("DARTS", "db upgraded, from "+oldVersion+", to: "+newVersion)
    }
}