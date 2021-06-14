package com.mpm.dartsclient

import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.sqlhelper.SQLTables
import org.json.JSONObject

object MessageHandler {
    private fun createPlayerJson() : JSONObject {
        var players = JSONObject()

        for ((i, player) in PlayerProfile.chosenPlayerProfiles.withIndex()) {
            var playerJ = JSONObject()
            playerJ.put("COLOR", player.backgroundColor)
            playerJ.put("NICK", player.nickname)
            playerJ.put("NAME", player.name)
            players.put("" + (i + 1), playerJ)
        }

        return players
    }

    fun sendPlayers() {
        var jsonObject = JSONObject()
        jsonObject.put("STATE", "PLAYERS")

        var body = JSONObject()

        var players = createPlayerJson()
        body.put("PLAYERS", players)

        var gameNr : Int = SQLTables.GamesTable.findGameNr()
        body.put("GAMENR",gameNr)

        jsonObject.put("BODY", body)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    @Synchronized fun sendGameConfig() {
        var jsonObject = JSONObject()
        jsonObject.put("STATE", "CONFIG")

        var body = JSONObject()

        body.put("GAME", DartsGameContainer.currentGame!!.gameID)

        var configObject = DartsGameContainer.currentGame!!.serializeConfigParameters()
        body.put("CONFIG", configObject)

        jsonObject.put("BODY", body)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    fun sendDump() {
        var dump = JSONObject()
        dump.put("STATE", "DUMP")

        var body = JSONObject()

        var gameNr : Int = SQLTables.GamesTable.findGameNr()

        body.put("GAMENR", gameNr)
        body.put("PLAYERS", createPlayerJson())
        body.put("GAME", DartsGameContainer.currentGame!!.gameID)

        var configObject = JSONObject()

        for (game in DartsGameContainer.games) {
            configObject.put(game.gameID!!, game.serializeConfigParameters())
        }

        body.put("CONFIG", configObject)

        dump.put("BODY", body)
        DartsClientApplication.getBluetoothCommunicator().sendMessage(dump)
    }

    fun requestLastMessage() {
        var retry = JSONObject()
        retry.put("STATE","LAST")

        DartsClientApplication.getBluetoothCommunicator().sendMessage(retry)
    }

    fun requestGameStart(order : Int) {
        var jsonObject = JSONObject()
        jsonObject.put("STATE", "GAMEPLAY")

        var body = JSONObject()
        jsonObject.put("BODY", body)

        body.put("GSTATE", "START")
        var gBody = JSONObject()
        body.put("GBODY", gBody)

        gBody.put("ORDER", order)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    fun requestGameDump() {
        var dumpRequest = JSONObject()

        dumpRequest.put("STATE", "GAME")

        var body = JSONObject()
        body.put("GSTATE", "DUMP")

        var gBody = JSONObject()

        body.put("GBODY", gBody)

        dumpRequest.put("BODY", body)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(dumpRequest)
    }

    fun requestChangePlayer() {
        var playerChange = JSONObject()

        playerChange.put("STATE", "GAME")

        var body = JSONObject()
        body.put("GSTATE", "NEXT")

        var gBody = JSONObject()

        body.put("GBODY", gBody)

        playerChange.put("BODY", body)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(playerChange)
    }

    fun requestCorrectDart() {
        var jsonObject = JSONObject()
        jsonObject.put("STATE", "GAMEPLAY")
        var body = JSONObject()
        jsonObject.put("BODY", body)

        body.put("GSTATE", "CORRECT")
        var gBody = JSONObject()
        body.put("GBODY", gBody)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }

    fun requestDeleteDart(id : Int) {
        var jsonObject = JSONObject()
        jsonObject.put("STATE", "GAMEPLAY")
        var body = JSONObject()
        jsonObject.put("BODY", body)

        body.put("GSTATE", "DELETE")
        var gBody = JSONObject()
        body.put("GBODY", gBody)

        gBody.put("ID", id)

        DartsClientApplication.getBluetoothCommunicator().sendMessage(jsonObject)
    }
}