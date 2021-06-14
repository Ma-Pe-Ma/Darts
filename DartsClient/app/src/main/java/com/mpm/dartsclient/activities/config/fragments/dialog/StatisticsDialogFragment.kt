package com.mpm.dartsclient.activities.config.fragments.dialog

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.R
import com.mpm.dartsclient.activities.config.adapters.StatisticsPlayerAdapter
import com.mpm.dartsclient.loadedSQLData.LoadedMatch

class StatisticsDialogFragment : DialogFragment() {

    var recyclerView : RecyclerView? = null

    override fun onResume() {
        super.onResume()

        dialog!!.window!!.setLayout(700, 700)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.fragment_statistics, container, false)

        recyclerView = view.findViewById(R.id.statisticsRecycler)

        var commonMatchNumber = view.findViewById<TextView>(R.id.commonMatchNumber)
        commonMatchNumber.text =requireActivity().getString(R.string.commonMatchNumber) +  " " + LoadedMatch.loadedMatches.size.toLong()

        var statisticsAdapter = StatisticsPlayerAdapter(requireActivity())

        recyclerView!!.apply {
            adapter = statisticsAdapter
            layoutManager = LinearLayoutManager(activity)
        }



        return view
    }

}