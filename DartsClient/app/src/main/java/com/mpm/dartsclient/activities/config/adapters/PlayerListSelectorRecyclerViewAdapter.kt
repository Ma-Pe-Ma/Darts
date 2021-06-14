package com.mpm.dartsclient.activities.config.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.core.content.ContextCompat
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.MessageHandler
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.fragments.dialog.PlayerCreatorDialogFragment
import com.mpm.dartsclient.activities.config.fragments.dialog.PlayerListConfigurerDialogFragment
import com.mpm.dartsclient.sqlhelper.SQLTables


class PlayerListSelectorRecyclerViewAdapter(var activity : FragmentActivity) : RecyclerView.Adapter<PlayerListSelectorRecyclerViewAdapter.PlayerHolder>() {
    private val maxNumberOfPlayers = 8

    override fun getItemViewType(position: Int): Int {
        return if (position == PlayerProfile.chosenPlayerProfiles.size) R.layout.add_player_row else R.layout.player_config_row
    }

    override fun getItemCount(): Int {
        return if (PlayerProfile.chosenPlayerProfiles.size + 1 > maxNumberOfPlayers) {
            maxNumberOfPlayers
        } else {
            PlayerProfile.chosenPlayerProfiles.size + 1
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)
        return PlayerHolder(view)
    }

    override fun onBindViewHolder(holder: PlayerHolder, position: Int) {
        var imageID : Int? = null
        var textID : Int? = null

        when(getItemViewType(position)) {
            R.layout.player_config_row -> {
                imageID = R.id.removeButton
                textID = R.id.playerName
            }
            R.layout.add_player_row -> {
                imageID = R.id.addPlayer
                textID = R.id.newPlayerText
            }
            else -> {}
        }

        var imageButton = holder.holderView.findViewById<ImageButton>(imageID!!)
        var textView = holder.holderView.findViewById<TextView>(textID!!)

        if (position % 2 == 0) {
            holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, R.color.listBackgroundLight))
        }
        else {
            holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, R.color.listBackgroundDark))
        }

        when (position) {
            PlayerProfile.chosenPlayerProfiles.size -> {
                imageButton.setOnClickListener {

                    var dialogExisting = false

                    val fragments: List<Fragment> = activity.supportFragmentManager.fragments
                    for (fragment in fragments) {
                        if (fragment is DialogFragment) {
                            dialogExisting = true
                        }
                    }

                    if (!dialogExisting) {
                        PlayerListConfigurerDialogFragment.chosenPlayers = PlayerProfile.chosenPlayerProfiles
                        var playerListConfigurerDialogFragment = PlayerListConfigurerDialogFragment()
                        playerListConfigurerDialogFragment.show(activity.supportFragmentManager, "PlayerConfigDialog")
                    }

                    //notifyItemInserted(players.size + 1)
                    //notifyDataSetChanged()
                }

                //textView.setText("uj jatekos")
            }
            else -> {
                var sequencNr = holder.holderView.findViewById<TextView>(R.id.sequenceNr)
                var sequenceText : String = "" + (position + 1) + "."
                sequencNr.text = sequenceText

                textView.setTextColor(PlayerProfile.chosenPlayerProfiles[position].textColor!!)
                textView.setBackgroundColor(PlayerProfile.chosenPlayerProfiles[position].backgroundColor!!)

                var editButton = holder.holderView.findViewById<ImageButton>(R.id.editPlayer)

                var i : Int = 0
                for (player in PlayerProfile.playerProfiles) {
                    if (player == PlayerProfile.chosenPlayerProfiles[position] ) {
                        break;
                    }
                    i++
                }

                editButton.setOnClickListener {
                    var playerCreatorDialogFragment = PlayerCreatorDialogFragment(i)
                    playerCreatorDialogFragment.show(activity.supportFragmentManager, "PLAYERCREATOR")
                }

                imageButton.setOnClickListener {
                    PlayerProfile.chosenPlayerProfiles.removeAt(position)
                    notifyDataSetChanged()
                    MessageHandler.sendPlayers()
                    SQLTables.createStatistics()
                    (activity as Config).notifyStatisticsButton()
                }

                textView.text = PlayerProfile.chosenPlayerProfiles[position].nickname
            }
        }
    }

    class PlayerHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
    }
}