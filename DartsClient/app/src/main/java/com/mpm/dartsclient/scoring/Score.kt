package com.mpm.dartsclient.scoring

abstract class Score {
    var score : Int = 0
    var winning : Boolean = false

    abstract fun score(dart : Dart)

}