package com.mpm.dartsclient.activities.config.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.Config
import com.mpm.dartsclient.activities.config.fragments.dialog.DeleteProfileDialogFragment
import com.mpm.dartsclient.activities.config.fragments.dialog.PlayerCreatorDialogFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer

class PlayerListConfigurerRecyclerViewAdapter(var activity : Config, var profileContainer : ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer)
    : RecyclerView.Adapter<PlayerListConfigurerRecyclerViewAdapter.PlayerHolder>(), Filterable {

    var filteredPlayers : List<PlayerProfile> = profileContainer.playerProfiles
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)

        return PlayerHolder(view)
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.player_config_row
    }

    override fun getItemCount(): Int {
        return filteredPlayers.size
    }

    override fun getItemId(position: Int): Long {
        return position as Long
    }
    override fun onBindViewHolder(holder: PlayerHolder, position: Int) {        var backgroundColor = if (position % 2 == 0) R.color.listBackgroundLight else R.color.listBackgroundDark
        holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, backgroundColor))

        var sequencNr = holder.holderView.findViewById<TextView>(R.id.sequenceNr)
        var sequenceText : String = "" + (position + 1) + "."
        sequencNr.text = sequenceText

        var textView = holder.holderView.findViewById<TextView>(R.id.playerName)
        textView.setTextColor(filteredPlayers[position].textColor!!)
        textView.setBackgroundColor(filteredPlayers[position].backgroundColor!!)
        textView.text = filteredPlayers[position].nickname

        var deleteButton = holder.holderView.findViewById<ImageButton>(R.id.removeButton)
        deleteButton.setImageDrawable(activity.getDrawable(R.drawable.trash))

        var editButton = holder.holderView.findViewById<ImageButton>(R.id.editPlayer)
        editButton.setOnClickListener {
            var playerCreatorDialogFragment = PlayerCreatorDialogFragment(filteredPlayers[position], profileContainer)
            playerCreatorDialogFragment.show(activity.supportFragmentManager, "PLAYERCREATOR")
        }

        if (profileContainer.chosenPlayerProfiles.contains(filteredPlayers[position])) {
            holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, R.color.listBackgroundInactiveLight))

            textView.setOnClickListener {
                Toast.makeText(activity.applicationContext, activity.getString(R.string.playerAlreadyChosen1), Toast.LENGTH_SHORT).show()
            }

            deleteButton.alpha = 0.5f
            deleteButton.setOnClickListener {
                Toast.makeText(activity.applicationContext, activity.getString(R.string.playerAlreadyChosen2), Toast.LENGTH_SHORT).show()
            }
        }
        else {
            textView.setOnClickListener {
                activity.notifyPlayerConfigFragment(position)
            }

            deleteButton.setOnClickListener {
                deleteProfile(position)
            }
        }
    }

    private fun deleteProfile(position: Int) {
        DeleteProfileDialogFragment(profileContainer.playerProfiles[position], profileContainer).show(activity.supportFragmentManager, "PlayerDeleteDialog")
    }

    class PlayerHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
    }

    override fun getFilter(): Filter {
        return object : Filter () {
            override fun performFiltering(constraint: CharSequence?): FilterResults {
                val searchStr = constraint?.toString()?.lowercase()

                val results = if (searchStr.isNullOrBlank()) {
                    profileContainer.playerProfiles
                } else {
                    profileContainer.playerProfiles.filter {
                        it.nickname.lowercase().contains(searchStr) ||
                        it.name.lowercase().contains(searchStr)
                    }
                }

                return FilterResults().apply {
                    values = results
                    count = results.size
                }
            }

            override fun publishResults(constraint: CharSequence?, results: FilterResults?) {
                filteredPlayers = results?.values as ArrayList<PlayerProfile>
                notifyDataSetChanged()
            }
        }
    }
}