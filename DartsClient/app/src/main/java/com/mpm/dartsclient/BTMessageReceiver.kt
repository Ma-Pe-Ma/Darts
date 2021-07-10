package com.mpm.dartsclient

import org.json.JSONObject

interface BTMessageReceiver {

    fun onBTReceive(message: JSONObject) {
        var state = message["STATE"]
        var body = message["BODY"] as JSONObject

        when (state) {
            "DUMP" -> {onDump(body)}

            "CONFIG" -> {onConfig(body)}

            "GAMEPLAY" -> {onGamePlay(body)}
        }
    }

    fun onConfig(body : JSONObject)

    fun onDump(body : JSONObject)

    fun onGamePlay(body : JSONObject) {}

    fun onUnknown() {}

    fun onParseFail() {
        MessageHandler.requestLastMessage()
    }

}