package com.mpm.dartsclient.fragments

import android.app.Activity
import android.graphics.Color
import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.mpm.dartsclient.R
import com.mpm.dartsclient.adapters.BTRecyclerViewAdapter
import com.mpm.dartsclient.adapters.PlayerListConfigurerRecyclerViewAdapter


class BTDeviceSelectorDialogFragment(var deviceList : MutableList<String>) : DialogFragment() {

    private var btRecyclerViewAdapter: BTRecyclerViewAdapter? = null
    var recyclerView : RecyclerView? = null
    var title : TextView? = null

    override fun onResume() {
        super.onResume()
        dialog!!.window!!.setLayout(700, 1100)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.fragment_bt_device_selector_dialog, container, false)

        recyclerView = view.findViewById(R.id.recyclerList)
        title = view.findViewById(R.id.chooserTitle)
        title!!.text = getString(R.string.btDevices)

        val itemDecoration =
            DividerItemDecoration(recyclerView!!.context, DividerItemDecoration.VERTICAL)
        val drawable = GradientDrawable(
            GradientDrawable.Orientation.BOTTOM_TOP,
            intArrayOf(Color.BLACK, Color.BLACK)
        )
        drawable.setSize(1, 1)
        itemDecoration.setDrawable(drawable)
        recyclerView!!.addItemDecoration(itemDecoration)

        btRecyclerViewAdapter = BTRecyclerViewAdapter(activity as Activity)
        btRecyclerViewAdapter!!.deviceList = deviceList

        btRecyclerViewAdapter!!.notifyDataSetChanged()

        recyclerView!!.apply {
            adapter =  btRecyclerViewAdapter
            layoutManager = LinearLayoutManager(activity)
        }

        /*recyclerView!!.addItemDecoration(
            DividerItemDecoration(
                context,
                DividerItemDecoration.VERTICAL

            )
        )*/

        // Inflate the layout for this fragment
        return view
        //return super.onCreateView(inflater, container, savedInstanceState)
    }
}