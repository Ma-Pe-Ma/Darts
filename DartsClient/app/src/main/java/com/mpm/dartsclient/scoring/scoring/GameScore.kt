package com.mpm.dartsclient.scoring.scoring

import com.mpm.dartsclient.scoring.BasicScore
import com.mpm.dartsclient.scoring.Dart
import org.json.JSONObject

abstract class GameScore : BasicScore() {
    //the object holding the current scoring
    abstract fun score(dart : Dart)
    //Custom state setter as status of closeness of numbers in cricket!
    abstract fun setState(body : JSONObject)
}