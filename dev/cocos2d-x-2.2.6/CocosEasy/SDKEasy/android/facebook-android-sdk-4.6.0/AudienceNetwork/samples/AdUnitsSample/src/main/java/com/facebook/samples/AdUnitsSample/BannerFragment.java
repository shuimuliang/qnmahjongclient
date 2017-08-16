/**
 * Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
 *
 * You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
 * copy, modify, and distribute this software in source code or binary form for use
 * in connection with the web services and APIs provided by Facebook.
 *
 * As with any software that integrates with the Facebook platform, your use of
 * this software is subject to the Facebook Developer Principles and Policies
 * [http://developers.facebook.com/policy/]. This copyright notice shall be
 * included in all copies or substantial portions of the software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

package com.facebook.samples.AdUnitsSample;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;

public class BannerFragment extends Fragment implements AdListener {

    private RelativeLayout adViewBannerContainer;
    private TextView adBannerStatusLabel;
    private AdView adViewBanner;

    private String statusLabel = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Initialize status label
        statusLabel = getString(R.string.loading_status);

        // Create a banner's ad view with a unique placement ID (generate your own on the Facebook app settings).
        // Use different ID for each ad placement in your app.
        boolean isTablet = getResources().getBoolean(R.bool.is_tablet);
        adViewBanner = new AdView(this.getActivity(), "YOUR_PLACEMENT_ID",
                isTablet ? AdSize.BANNER_HEIGHT_90 : AdSize.BANNER_HEIGHT_50);

        // Set a listener to get notified on changes or when the user interact with the ad.
        adViewBanner.setAdListener(this);

        // Initiate a request to load an ad.
        adViewBanner.loadAd();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_banner, container, false);

        adViewBannerContainer = (RelativeLayout)view.findViewById(R.id.adViewContainer);

        adBannerStatusLabel = (TextView)view.findViewById(R.id.adStatusLabel);
        setLabel(statusLabel);
        // Reposition the ad and add it to the view hierarchy.
        adViewBannerContainer.addView(adViewBanner);

        return view;
    }

    @Override
    public void onDestroyView() {
        adViewBannerContainer.removeView(adViewBanner);
        super.onDestroyView();
    }

    @Override
    public void onDestroy() {
        adViewBanner.destroy();
        adViewBanner = null;
        super.onDestroy();
    }

    @Override
    public void onError(Ad ad, AdError error) {
        if (ad == adViewBanner) {
            setLabel("Ad failed to load: " + error.getErrorMessage());
        }
    }

    @Override
    public void onAdLoaded(Ad ad) {
        if (ad == adViewBanner) {
            setLabel("");
        }
    }

    @Override
    public void onAdClicked(Ad ad) {
        Toast.makeText(this.getActivity(), "Ad Clicked", Toast.LENGTH_SHORT).show();
    }

    private void setLabel(String status) {
        statusLabel = status;
        if (adBannerStatusLabel != null) {
            adBannerStatusLabel.setText(statusLabel);
        }
    }
}
