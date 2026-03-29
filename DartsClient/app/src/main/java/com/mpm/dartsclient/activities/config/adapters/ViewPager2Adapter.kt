package com.mpm.dartsclient.activities.config.adapters

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.lifecycle.Lifecycle
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.mpm.dartsclient.ProfileContainer
import com.mpm.dartsclient.activities.config.fragments.GameConfigFragment
import com.mpm.dartsclient.activities.config.fragments.PlayerFragment
import com.mpm.dartsclient.games.DartsGameContainer
import com.mpm.dartsclient.loadedSQLData.MatchContainer

private const val fragmentSize = 2

class ViewPager2Adapter(fragmentManager: FragmentManager, lifecycle: Lifecycle, var profileContainer: ProfileContainer, var matchContainer: MatchContainer, var dartsGameContainer: DartsGameContainer) :
    FragmentStateAdapter(fragmentManager, lifecycle) {

    private var fragments = arrayOf<Fragment>(
        GameConfigFragment(dartsGameContainer),
        PlayerFragment(profileContainer, matchContainer, dartsGameContainer)
    )

    override fun getItemCount(): Int {
        return fragmentSize
    }

    override fun createFragment(position: Int): Fragment {
        return fragments[position]
    }

}