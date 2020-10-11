package com.mpm.dartsclient.adapters

import android.annotation.SuppressLint
import android.database.DataSetObserver
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SpinnerAdapter
import android.widget.TextView
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.R

class GameListSpinnerAdapter : SpinnerAdapter {
    override fun isEmpty(): Boolean {
        if (DartsGameContainer.games.size == 0) {
            return true
        }
        return false
    }

    @SuppressLint("ViewHolder")
    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        //TODO("Not yet implemented")
        val inflater = LayoutInflater.from(parent!!.context)
        var view: View = inflater.inflate(R.layout.game_name_row, parent, false)

        var text = view.findViewById<TextView>(R.id.gameNameRow)

        text.apply {
            text.text = DartsGameContainer.games[position].gameID
            //text.textSize = 20F
        }

        return view
    }

    override fun registerDataSetObserver(observer: DataSetObserver?) {
        //TODO("Not yet implemented")
    }

    override fun getItemViewType(position: Int): Int {
        return R.layout.game_name_row
    }

    override fun getItem(position: Int): Any {
        //TODO("Not yet implemented")

        return DartsGameContainer.games[position]
    }

    override fun getViewTypeCount(): Int {
        return 1
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun hasStableIds(): Boolean {
        //TODO("Not yet implemented")
        return true
    }

    override fun getDropDownView(position: Int, convertView: View?, parent: ViewGroup?): View {
        //TODO("Not yet implemented")
        val inflater = LayoutInflater.from(parent!!.context)
        var view : View = inflater.inflate(R.layout.game_name_row, parent, false)

        var text = view.findViewById<TextView>(R.id.gameNameRow)

        text.apply {
            text.text = DartsGameContainer.games[position].gameID
            //text.textSize = 20F
        }

        return view
    }

    override fun unregisterDataSetObserver(observer: DataSetObserver?) {
        //TODO("Not yet implemented")
    }

    override fun getCount(): Int {
        return DartsGameContainer.games.size
    }
}