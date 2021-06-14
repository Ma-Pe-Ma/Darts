package com.mpm.dartsclient.scoring.scoring

import com.mpm.dartsclient.scoring.Dart
import org.json.JSONObject

class CricketScore : GameScore() {

    var state : Array<Int> = Array<Int>(21) {0}

    override fun score(dart: Dart) {

    }

    override fun setState(body : JSONObject) {
        /*var states = body["STATES"] as JSONObject
        var keys = states.keys()

        while (keys.hasNext()) {
            var key = keys.next()

            var sector : Int = key.toInt()
            var value : Int = states.get(key) as Int

            if (sector == 25) {
                sector = 0
            }

            state[sector] = value
        }*/
    }
}