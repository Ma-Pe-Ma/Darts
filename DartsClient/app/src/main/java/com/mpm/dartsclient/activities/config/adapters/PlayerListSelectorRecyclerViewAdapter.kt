package com.mpm.dartsclient.activities.config.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.core.content.ContextCompat
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.MessageHandler
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.fragments.dialog.PlayerCreatorDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer


class PlayerListSelectorRecyclerViewAdapter(var activity : FragmentActivity, var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) : RecyclerView.Adapter<PlayerListSelectorRecyclerViewAdapter.PlayerHolder>() {
    private val maxNumberOfPlayers = 8

    override fun getItemViewType(position: Int): Int {
        return R.layout.player_config_row
    }

    override fun getItemCount(): Int {
        return profileContainer.chosenPlayerProfiles.size
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)
        return PlayerHolder(view)
    }

    override fun onBindViewHolder(holder: PlayerHolder, position: Int) {
        val currentPlayer = profileContainer.chosenPlayerProfiles[position]

        val colorBackground = if (position % 2 == 0) R.color.listBackgroundLight else R.color.listBackgroundDark
        holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, colorBackground))

        val sequenceText : String = "" + (position + 1) + "."
        holder.holderView.findViewById<TextView>(R.id.sequenceNr).text = sequenceText

        val textView = holder.holderView.findViewById<TextView>(R.id.playerName)
        textView.setTextColor(currentPlayer.textColor!!)
        textView.setBackgroundColor(currentPlayer.backgroundColor!!)
        textView.text = currentPlayer.nickname

        val editButton = holder.holderView.findViewById<ImageButton>(R.id.editPlayer)
        editButton.setOnClickListener {
            val playerCreatorDialogFragment = PlayerCreatorDialogFragment(position, profileContainer)
            playerCreatorDialogFragment.show(activity.supportFragmentManager, "PLAYERCREATOR")
        }

        val removeButton = holder.holderView.findViewById<ImageButton>(R.id.removeButton)
        removeButton.setOnClickListener {
            profileContainer.chosenPlayerProfiles.removeAt(position)
            notifyDataSetChanged()
            MessageHandler.sendPlayers(profileContainer.chosenPlayerProfiles)
            matchContainer.createStatistics(profileContainer, dartsGameContainer)
            (activity as Config).notifyStatisticsButton()
        }
    }

    class PlayerHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
    }
}