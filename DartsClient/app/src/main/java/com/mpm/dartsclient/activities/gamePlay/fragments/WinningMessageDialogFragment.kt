package com.mpm.dartsclient.activities.gamePlay.fragments

import android.content.DialogInterface
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.gamePlay.SaveProgress
import com.mpm.dartsclient.activities.gamePlay.adapters.PlayerScoreRecyclerViewAdapter

class WinningMessageDialogFragment(var finishingPlayer : PlayerProfile?) : DialogFragment() {
    companion object {
        var saveProgress = SaveProgress.inProgress
    }

    var messageText : TextView? = null
    var titleText : TextView? = null
    var partialResultText : TextView? = null
    var okButton : Button? = null
    var recyclerView : RecyclerView? = null
    var explainingRow : ConstraintLayout? = null

    var message : String = ""
    var title : String = ""
    var partial : String = ""

    private var finishedPlayers = mutableListOf<PlayerProfile>()

    init {
        finishedPlayers = mutableListOf()

        for (player in PlayerProfile.chosenPlayerProfiles) {
            if (player.score!!.position > 0) {
                finishedPlayers.add(player)
            }
        }

        finishedPlayers = finishedPlayers.sortedWith(compareBy { it.score!!.position }).toMutableList()
    }

    override fun onResume() {
        super.onResume()
        var height = 400 + 200 * finishedPlayers.size
        if (height > 1000) {
            height = 1000
        }
        dialog!!.window!!.setLayout(700, height)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val dialogView: View = inflater.inflate(R.layout.fragment_winning, null)

        titleText = dialogView.findViewById(R.id.winningTitle)

        partialResultText = dialogView.findViewById(R.id.partialResult)

        messageText = dialogView.findViewById(R.id.winningMessage)

        okButton = dialogView.findViewById(R.id.winningOK)
        okButton!!.text = "RENDBEN"

        //disable OK Button when saving
        if (saveProgress == SaveProgress.inProgress) {
            //okButton!!.isEnabled = false
        }

        //if button is enabled (when saving is not in progress)
        okButton!!.setOnClickListener {
            dismiss()

            //if game is finishing and active
            if (PlayerProfile.chosenPlayerProfiles.size == finishedPlayers.size) {
                var postConfigDialog = PostConfigDialog()
                postConfigDialog.show(parentFragmentManager, "POSTCONF")
            }
        }

        var playerList = dialogView.findViewById<ConstraintLayout>(R.id.playerResultList)

        var explainingRow = playerList.findViewById<ConstraintLayout>(R.id.explainingRow)
        setExplainingRow(explainingRow)

        recyclerView = playerList.findViewById(R.id.playerRecyclerList)
        recyclerView?.apply {
            var highlightPosition = if (PlayerProfile.chosenPlayerProfiles.size == finishedPlayers.size) -1 else finishedPlayers.size
            recyclerView?.adapter = PlayerScoreRecyclerViewAdapter(requireActivity(), highlightPosition, true, finishedPlayers)
            layoutManager = LinearLayoutManager(activity)
        }

        assignCorrectTexts(finishingPlayer)

        return dialogView
    }

    override fun onCancel(dialog: DialogInterface) {
        super.onCancel(dialog)

        //if game is finishing
        if (PlayerProfile.chosenPlayerProfiles.size == finishedPlayers.size) {
            when (saveProgress) {
                //when saving not finished reshow this window
                SaveProgress.inProgress -> {
                    var newWinningMessage = WinningMessageDialogFragment(finishingPlayer)
                    newWinningMessage.show(parentFragmentManager, "WINNING")
                }

                //show post config, ask for rematch!
                else -> {
                    var postConfigDialog = PostConfigDialog()
                    postConfigDialog.show(parentFragmentManager, "POSTCONF")
                }
            }
        }
    }

    fun setCorrectMessageOutside(saveProgress: SaveProgress) {
        message = findMessageByProgress(saveProgress)

        if (saveProgress != SaveProgress.inProgress) {
            okButton!!.isEnabled = true
        }

        messageText!!.text = message
    }

    private fun findMessageByProgress(saveProgress: SaveProgress) : String {
        return when (Companion.saveProgress) {
            SaveProgress.inProgress -> {
                getString(R.string.savingInProgress)
            }

            SaveProgress.success -> {
                getString(R.string.savingSuccess)
            }

            SaveProgress.failure -> {
                getString(R.string.savingFailure)
            }
        }
    }

    private fun assignCorrectTexts(finishingPlayer: PlayerProfile?) {
        //if 1 player is playing or last one remaining show game finished dialog
        if (finishedPlayers.size == PlayerProfile.chosenPlayerProfiles.size) {
            title = getString(R.string.gameFinishedTitle)
            partial = getString(R.string.finalPartial)
            message = findMessageByProgress(saveProgress)
        }
        //when game continues (when at least 3 players playing and and at least 2 players remain)
        else {
            title = getString(R.string.winningDefaultTitle)
            partial = getString(R.string.defaultPartial)
            message  = finishingPlayer!!.nickname + " helyezése: " + finishingPlayer.score!!.position + "."
        }

        titleText!!.text = title
        partialResultText!!.text = partial
        messageText!!.text = message
    }

    private fun setExplainingRow(explainingRow : View) {
        var sequenceNr = explainingRow!!.findViewById<TextView>(R.id.scoreSequenceNr)
        sequenceNr.text = requireActivity().getString(R.string.posAbbr)

        var nick = explainingRow!!.findViewById<TextView>(R.id.scorePlayerName)
        nick.text = requireActivity().getString(R.string.nick)

        var score = explainingRow!!.findViewById<TextView>(R.id.scoreScore)
        score.text = requireActivity().getString(R.string.points)

        var round = explainingRow!!.findViewById<TextView>(R.id.scoreRoundCounter)
        round.text = requireActivity().getString(R.string.round)

        var position = explainingRow!!.findViewById<TextView>(R.id.scorePosition)
        position.text = requireActivity().getString(R.string.id)
    }
}