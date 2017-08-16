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

package com.facebook.samples.NativeAdSample;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;
import com.facebook.ads.*;

public class NativeAdHscrollActivity extends Activity implements NativeAdsManager.Listener {
    private NativeAdsManager manager;
    private NativeAdScrollView scrollView;
    private Button reloadButton;
    private LayoutInflater inflater;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.hscroll);

        manager = new NativeAdsManager(this, "YOUR_PLACEMENT_ID", 5);
        manager.setListener(this);
        manager.loadAds(NativeAd.MediaCacheFlag.ALL);

        inflater = (LayoutInflater)this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        reloadButton = (Button)findViewById(R.id.reloadHscroll);
        reloadButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                manager.loadAds();
            }
        });
    }

    @Override
    public void onAdsLoaded() {
        Toast.makeText(this, "Ads loaded", Toast.LENGTH_SHORT).show();

        if (scrollView != null) {
            ((LinearLayout) findViewById(R.id.hscrollContainer)).removeView(scrollView);
        }

        scrollView = new NativeAdScrollView(NativeAdHscrollActivity.this, manager,
                NativeAdView.Type.HEIGHT_400);

        // Custom view example:
        /* scrollView = new NativeAdScrollView(NativeAdHscrollActivity.this, manager,
            new NativeAdScrollView.CustomAdView() {
                @Override
                public View createView(NativeAd nativeAd) {
                    View adView = inflater.inflate(R.layout.ad_unit, null);
                    NativeAdSampleActivity.inflateAd(nativeAd, adView,
                        NativeAdHscrollActivity.this);
                    return adView;
                }

                @Override
                public void destroyView(NativeAd nativeAd, View view) {
                    nativeAd.unregisterView();
                }
        }); */

        ((LinearLayout) findViewById(R.id.hscrollContainer)).addView(scrollView);
    }

    @Override
    public void onAdError(AdError error) {
        Toast.makeText(this, "Ad error: " + error.getErrorMessage(), Toast.LENGTH_SHORT).show();
    }
}
