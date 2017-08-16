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
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

public class InterstitialFragment extends Fragment implements InterstitialAdListener {

    private TextView interstitialAdStatusLabel;
    private Button loadInterstitialButton;
    private Button showInterstitialButton;
    private InterstitialAd interstitialAd;

    private String statusLabel = "";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_interstitial, container, false);

        interstitialAdStatusLabel = (TextView)view.findViewById(R.id.interstitialAdStatusLabel);
        loadInterstitialButton = (Button)view.findViewById(R.id.loadInterstitialButton);
        showInterstitialButton = (Button)view.findViewById(R.id.showInterstitialButton);

        loadInterstitialButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (interstitialAd != null) {
                    interstitialAd.destroy();
                    interstitialAd = null;
                }
                setLabel("Loading interstitial ad...");

                // Create the interstitial unit with a placement ID (generate your own on the Facebook app settings).
                // Use different ID for each ad placement in your app.
                interstitialAd = new InterstitialAd(InterstitialFragment.this.getActivity(), "YOUR_PLACEMENT_ID");

                // Set a listener to get notified on changes or when the user interact with the ad.
                interstitialAd.setAdListener(InterstitialFragment.this);

                // Load a new interstitial.
                interstitialAd.loadAd();
            }
        });

        showInterstitialButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (interstitialAd == null || !interstitialAd.isAdLoaded()) {
                    // Ad not ready to show.
                    setLabel("Ad not loaded. Click load to request an ad.");
                } else {
                    // Ad was loaded, show it!
                    interstitialAd.show();
                    setLabel("");
                }
            }
        });
        return view;
    }

    @Override
    public void onDestroy() {
        if (interstitialAd != null) {
            interstitialAd.destroy();
            interstitialAd = null;
        }
        super.onDestroy();
    }

    @Override
    public void onError(Ad ad, AdError error) {
        if (ad == interstitialAd) {
            setLabel("Interstitial ad failed to load: " + error.getErrorMessage());
        }
    }

    @Override
    public void onAdLoaded(Ad ad) {
        if (ad == interstitialAd) {
            setLabel("Ad loaded. Click show to present!");
        }
    }

    @Override
    public void onInterstitialDisplayed(Ad ad) {
        Toast.makeText(this.getActivity(), "Interstitial Displayed", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onInterstitialDismissed(Ad ad) {
        Toast.makeText(this.getActivity(), "Interstitial Dismissed", Toast.LENGTH_SHORT).show();

        // Cleanup.
        interstitialAd.destroy();
        interstitialAd = null;
    }

    @Override
    public void onAdClicked(Ad ad) {
        Toast.makeText(this.getActivity(), "Interstitial Clicked", Toast.LENGTH_SHORT).show();
    }

    private void setLabel(String label) {
        statusLabel = label;
        if (interstitialAdStatusLabel != null) {
            interstitialAdStatusLabel.setText(statusLabel);
        }
    }
}
