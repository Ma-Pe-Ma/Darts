package com.mpm.dartsclient.activities.config

import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.View
import android.view.animation.Animation
import android.view.animation.Transformation
import androidx.cardview.widget.CardView
import androidx.constraintlayout.widget.ConstraintLayout

class ExpandableCardView(context: Context, attributeSet: AttributeSet) : CardView(context, attributeSet) {
    var collapsedHeight = 0
    var expansionHeight = 0

    var resultRecyclerListWithHeader : ConstraintLayout? = null

    fun expand() {
        //resultRecyclerList?.visibility = View.VISIBLE
        val a: Animation = object : Animation() {
            override fun applyTransformation(interpolatedTime: Float, t: Transformation?) {
                resultRecyclerListWithHeader?.visibility = View.VISIBLE
                if (interpolatedTime == 1f) {
                    // Do this after expanded
                }
                layoutParams.height = (collapsedHeight + expansionHeight * interpolatedTime).toInt()
                requestLayout()
            }

            override fun willChangeBounds(): Boolean {
                return true
            }
        }

        a.duration = 500
        //a.duration = expansionHeight.toLong()
        startAnimation(a)
    }

    fun collapse() {
        val a: Animation = object : Animation() {
            override fun applyTransformation(interpolatedTime: Float, t: Transformation?) {
                layoutParams.height = (collapsedHeight  + expansionHeight - expansionHeight * interpolatedTime).toInt()

                requestLayout()
                if (interpolatedTime == 1f) {
                    resultRecyclerListWithHeader?.visibility = View.INVISIBLE
                }
            }

            override fun willChangeBounds(): Boolean {
                return true
            }
        }
        
        a.duration = 500
        //a.duration = expansionHeight.toLong()
        startAnimation(a)
    }
}