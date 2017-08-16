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
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.*;
import android.view.View.OnTouchListener;
import android.widget.*;
import com.facebook.ads.*;

public class NativeAdSampleActivity extends Activity implements AdListener {

    protected static final String TAG = NativeAdSampleActivity.class.getSimpleName();
    private TextView nativeAdStatus;
    private LinearLayout nativeAdContainer;

    private Button showNativeAdButton;
    private Button showNativeAdListButton;
    private Button showNativeAdHScrollButton;
    private Button showNativeAdTemplateButton;

    private LinearLayout adView;
    private NativeAd nativeAd;
    private AdChoicesView adChoicesView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_ad_demo);

        nativeAdContainer = (LinearLayout) findViewById(R.id.nativeAdContainer);

        LayoutInflater inflater = (LayoutInflater)this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        adView = (LinearLayout) inflater.inflate(R.layout.ad_unit, nativeAdContainer);

        nativeAdStatus = (TextView) findViewById(R.id.nativeAdStatus);
        showNativeAdButton = (Button)findViewById(R.id.loadNativeAdButton);
        showNativeAdListButton = (Button)findViewById(R.id.loadNativeAdListButton);
        showNativeAdHScrollButton = (Button)findViewById(R.id.loadNativeAdHscroll);
        showNativeAdTemplateButton = (Button)findViewById(R.id.loadNativeAdTemplateButton);

        showNativeAdButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                nativeAdStatus.setText("Requesting an ad...");

                // Create a native ad request with a unique placement ID (generate your own on the Facebook app settings).
                // Use different ID for each ad placement in your app.
                nativeAd = new NativeAd(NativeAdSampleActivity.this, "YOUR_PLACEMENT_ID");

                // Set a listener to get notified when the ad was loaded.
                nativeAd.setAdListener(NativeAdSampleActivity.this);

                // When testing on a device, add its hashed ID to force test ads.
                // The hash ID is printed to log cat when running on a device and loading an ad.
                // AdSettings.addTestDevice("THE HASHED ID AS PRINTED TO LOG CAT");

                // Initiate a request to load an ad.
                nativeAd.loadAd(NativeAd.MediaCacheFlag.ALL);
            }
        });

        showNativeAdListButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(NativeAdSampleActivity.this, NativeAdListActivity.class);
                NativeAdSampleActivity.this.startActivity(intent);
            }
        });

        showNativeAdHScrollButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(NativeAdSampleActivity.this,
                        NativeAdHscrollActivity.class);
                NativeAdSampleActivity.this.startActivity(intent);
            }
        });

        showNativeAdTemplateButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(NativeAdSampleActivity.this,
                        NativeAdTemplateActivity.class);
                NativeAdSampleActivity.this.startActivity(intent);
            }
        });
    }

    @Override
    public void onError(Ad ad, AdError error) {
        nativeAdStatus.setText("Ad failed to load: " + error.getErrorMessage());
    }

    @Override
    public void onAdClicked(Ad ad) {
        Toast.makeText(this, "Ad Clicked", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onAdLoaded(Ad ad) {
        if (nativeAd == null || nativeAd != ad) {
            // Race condition, load() called again before last ad was displayed
            return;
        }

        // Unregister last ad
        nativeAd.unregisterView();

        nativeAdStatus.setText("");

        inflateAd(nativeAd, adView, this);

        // Using the ad choices view is optional, but your native ad unit should
        // be clearly delineated from the rest of your app content. See
        // https://developers.facebook.com/docs/audience-network/guidelines/native-ads#native
        // for details. We recommend using the AdChoicesView
        if (adChoicesView == null) {
            adChoicesView = new AdChoicesView(this, nativeAd);
            adView.addView(adChoicesView, 0);
        }

        // Registering a touch listener to log which ad component receives the touch event.
        // We always return false from onTouch so that we don't swallow the touch event (which
        // would prevent click events from reaching the NativeAd control).
        // The touch listener could be used to do animations.
        nativeAd.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    switch (view.getId()) {
                        case R.id.nativeAdCallToAction:
                            Log.d(TAG, "Call to action button clicked");
                            break;
                        case R.id.nativeAdMedia:
                            Log.d(TAG, "Main image clicked");
                            break;
                        default:
                            Log.d(TAG, "Other ad component clicked");
                    }
                }
                return false;
            }
        });
    }

    public static void inflateAd(NativeAd nativeAd, View adView, Context context) {
        // Create native UI using the ad metadata.

        ImageView nativeAdIcon = (ImageView) adView.findViewById(R.id.nativeAdIcon);
        TextView nativeAdTitle = (TextView) adView.findViewById(R.id.nativeAdTitle);
        TextView nativeAdBody = (TextView) adView.findViewById(R.id.nativeAdBody);
        MediaView nativeAdMedia = (MediaView) adView.findViewById(R.id.nativeAdMedia);
        TextView nativeAdSocialContext = (TextView) adView.findViewById(R.id.nativeAdSocialContext);
        Button nativeAdCallToAction = (Button) adView.findViewById(R.id.nativeAdCallToAction);

        // Setting the Text
        nativeAdSocialContext.setText(nativeAd.getAdSocialContext());
        nativeAdCallToAction.setText(nativeAd.getAdCallToAction());
        nativeAdCallToAction.setVisibility(View.VISIBLE);
        nativeAdTitle.setText(nativeAd.getAdTitle());
        nativeAdBody.setText(nativeAd.getAdBody());

        // Downloading and setting the ad icon.
        NativeAd.Image adIcon = nativeAd.getAdIcon();
        NativeAd.downloadAndDisplayImage(adIcon, nativeAdIcon);

        // Downloading and setting the cover image.
        NativeAd.Image adCoverImage = nativeAd.getAdCoverImage();
        int bannerWidth = adCoverImage.getWidth();
        int bannerHeight = adCoverImage.getHeight();
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        Display display = wm.getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        int screenWidth = metrics.widthPixels;
        int screenHeight = metrics.heightPixels;
        nativeAdMedia.setLayoutParams(new LinearLayout.LayoutParams(
                screenWidth,
                Math.min((int) (((double) screenWidth / (double) bannerWidth) * bannerHeight), screenHeight / 3)
        ));
        nativeAdMedia.setNativeAd(nativeAd);

        // Wire up the View with the native ad, the whole nativeAdContainer will be clickable
        nativeAd.registerViewForInteraction(adView);

        // Or you can replace the above call with the following function to specify the clickable areas.
        // nativeAd.registerViewForInteraction(adView,
        //     Arrays.asList(nativeAdCallToAction, nativeAdMedia));
    }
}
