package com.mpm.dartsclient.scoring

open class BasicScore() {
    var score : Int = 0
    var position : Int = 0
    var roundCount : Int = 0

    constructor(position : Int, score : Int, roundCount : Int) : this() {
        this.position = position
        this.score = score
        this.roundCount = roundCount
    }
}