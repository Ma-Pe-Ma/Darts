package com.mpm.dartsclient.activities.config.adapters

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.lifecycle.Lifecycle
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.activities.config.fragments.GameConfigFragment
import com.mpm.dartsclient.activities.config.fragments.PlayerConfigFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer

class ViewPager2Adapter(fragmentManager: FragmentManager, lifecycle: Lifecycle, var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) :
    FragmentStateAdapter(fragmentManager, lifecycle) {

    private val fragmentSize = 2
    private var fragments = arrayOf<Fragment>(
        GameConfigFragment(dartsGameContainer),
        PlayerConfigFragment(profileContainer, matchContainer, dartsGameContainer)
    )

    override fun getItemCount(): Int {
        return fragmentSize
    }

    override fun createFragment(position: Int): Fragment {
        return fragments[position]
    }
}