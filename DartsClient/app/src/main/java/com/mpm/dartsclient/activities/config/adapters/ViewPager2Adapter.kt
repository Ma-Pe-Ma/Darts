package com.mpm.dartsclient.activities.config.adapters

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.lifecycle.Lifecycle
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.mpm.dartsclient.activities.config.fragments.GameConfigFragment
import com.mpm.dartsclient.activities.config.fragments.PlayerFragment

private const val fragmentSize = 2

class ViewPager2Adapter(fragmentManager: FragmentManager, lifecycle: Lifecycle) :
    FragmentStateAdapter(fragmentManager, lifecycle) {

    private var fragments = arrayOf<Fragment>(GameConfigFragment(), PlayerFragment())

    override fun getItemCount(): Int {
        return fragmentSize
    }

    override fun createFragment(position: Int): Fragment {
        return fragments[position]
    }

}