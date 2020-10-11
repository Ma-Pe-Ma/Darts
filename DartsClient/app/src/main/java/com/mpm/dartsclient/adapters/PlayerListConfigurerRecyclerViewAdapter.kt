package com.mpm.dartsclient.adapters

import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.PlayerProfile
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.Config
import com.mpm.dartsclient.fragments.DeleteProfileDialogFragment

class PlayerListConfigurerRecyclerViewAdapter(var activity : Config, var chosenPlayers : List<PlayerProfile>)
    : RecyclerView.Adapter<PlayerListConfigurerRecyclerViewAdapter.PlayerHolder>(), Filterable {

    var filteredPlayers : MutableList<PlayerProfile> = PlayerProfile.playerProfiles
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlayerHolder {
        val inflater = LayoutInflater.from(parent.context)
        var view: View = inflater.inflate(viewType, parent, false)

        return PlayerHolder(view)
    }

    override fun getItemViewType(position: Int): Int {
        return if (position == filteredPlayers.size) R.layout.add_player_row else R.layout.player_row
    }

    override fun getItemCount(): Int {
        return (filteredPlayers.size + 1)
    }

    override fun getItemId(position: Int): Long {
        return position as Long
    }

    //@SuppressLint("UseCompatLoadingForDrawables")
    override fun onBindViewHolder(holder: PlayerHolder, position: Int) {
        var imageID : Int? = null
        var textID : Int? = null

        when(getItemViewType(position)) {
            R.layout.player_row -> {
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
            filteredPlayers?.size -> {

                imageButton.setOnClickListener {
                    activity.createPlayerCreator(-1, null)
                }

                textView.text = activity.getString(R.string.createNewPlayer)
            }
            else -> {
                var sequencNr = holder.holderView.findViewById<TextView>(R.id.sequenceNr)
                var sequenceText : String = "" + (position + 1) + "."
                sequencNr.text = sequenceText

                textView.setTextColor(filteredPlayers[position].inverseColor)
                textView.setBackgroundColor(filteredPlayers[position].color)

                imageButton.setImageDrawable(activity.getDrawable(R.drawable.trash))

                var editButton = holder.holderView.findViewById<ImageButton>(R.id.editPlayer)

                editButton.setOnClickListener {

                    var i : Int = 0
                    for (player in PlayerProfile.playerProfiles) {
                        if (player == filteredPlayers[position] ) {
                            break;
                        }
                        i++
                    }

                    activity.createPlayerCreator(i, null)
                }

                if (chosenPlayers.contains(filteredPlayers[position])) {
                    holder.holderView.setBackgroundColor(ContextCompat.getColor(activity, R.color.listBackgroundInactiveLight))

                    textView.setOnClickListener {
                        Toast.makeText(activity.applicationContext, activity.getString(R.string.playerAlreadyChosen1), Toast.LENGTH_SHORT).show()
                    }

                    imageButton.alpha = 0.5f

                    imageButton.setOnClickListener {
                        Toast.makeText(activity.applicationContext, activity.getString(R.string.playerAlreadyChosen2), Toast.LENGTH_SHORT).show()
                    }
                }

                else {
                    textView.setOnClickListener {
                        activity.notifyPlayerConfigFragment(position)
                    }

                    imageButton.setOnClickListener {
                        deleteProfile(position)
                    }
                }

                textView.text = filteredPlayers[position].nickname
            }
        }
    }

    private fun deleteProfile(position: Int) {
        DeleteProfileDialogFragment(position).show(activity.supportFragmentManager, "PlayerDeleteDialog")
    }

    class PlayerHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var holderView = itemView
    }

    override fun getFilter(): Filter {
        return object : Filter () {
            override fun performFiltering(constraint: CharSequence?): FilterResults {
                val filterResults = FilterResults()
                if (constraint == null || constraint.length == 0) {
                    //filterResults.count = towns.size();
                    //filterResults.values = towns;
                    filterResults.count = PlayerProfile.playerProfiles.size
                    filterResults.values = PlayerProfile.playerProfiles
                }
                else {
                    val resultsModel: MutableList<PlayerProfile> = ArrayList()
                    val searchStr = constraint.toString().toLowerCase()

                    for (playerProfile in PlayerProfile.playerProfiles) {

                        val lowerCasePlayerName : String = playerProfile.name.toLowerCase()
                        val lowerCasePlayerNickname : String = playerProfile.nickname.toLowerCase()

                        if (lowerCasePlayerName.length >= searchStr.length) {
                            if (lowerCasePlayerName.substring(0, searchStr.length) == searchStr) {
                                resultsModel.add(playerProfile)
                                continue;
                            }
                        }

                        if (lowerCasePlayerNickname.length >= searchStr.length) {
                            if (lowerCasePlayerNickname.substring(0, searchStr.length) == searchStr) {
                                resultsModel.add(playerProfile)
                                continue;
                            }
                        }
                    }

                    filterResults.count = resultsModel.size
                    filterResults.values = resultsModel
                }

                return filterResults
            }

            override fun publishResults(constraint: CharSequence?, results: FilterResults?) {
                filteredPlayers = results?.values as ArrayList<PlayerProfile>
                notifyDataSetChanged()
            }

        }
    }
}