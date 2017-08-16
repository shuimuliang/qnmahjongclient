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

import android.app.ListActivity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import com.facebook.ads.*;

import java.util.ArrayList;
import java.util.List;

public class NativeAdListActivity extends ListActivity
    implements NativeAdsManager.Listener, AdListener {

    private ListView listView;
    private ListViewAdapter adapter;
    private NativeAdsManager listNativeAdsManager;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        listNativeAdsManager = new NativeAdsManager(this, "YOUR_PLACEMENT_ID", 2);
        listNativeAdsManager.setListener(this);
        listNativeAdsManager.loadAds();

        listView = getListView();
        adapter = new ListViewAdapter(getApplicationContext());
        listView.setAdapter(adapter);
    }

    @Override
    public void onAdClicked(Ad ad) {
        Toast.makeText(this, "Ad Clicked", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onAdLoaded(Ad ad) { }

    @Override
    public void onAdsLoaded() {
        NativeAd ad = this.listNativeAdsManager.nextNativeAd();
        ad.setAdListener(this);
        adapter.addNativeAd(ad);
    }

    @Override
    public void onAdError(AdError error) {
        Toast.makeText(this, "Native ads manager failed to load: " +  error.getErrorMessage(),
            Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onError(Ad ad, AdError error) {
        Toast.makeText(this, "Ad failed to load: " +  error.getErrorMessage(), Toast.LENGTH_SHORT).show();
    }

    class ListViewAdapter extends BaseAdapter {

        private LayoutInflater inflater;
        private List<Object> list;

        private NativeAd ad;
        private static final int AD_INDEX = 2;

        public ListViewAdapter(Context context) {
            list = new ArrayList<Object>();
            inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            for (int i = 1; i <= 35; i++) {
                list.add("ListView Item #" + i);
            }
        }

        @Override
        public int getCount() {
            return list.size();
        }

        @Override
        public Object getItem(int position) {
            return list.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (position == AD_INDEX && ad != null) {
                // Return the native ad view
                return (View) list.get(position);
            } else {
                TextView view; // Default item type (non-ad)
                if (convertView != null && convertView instanceof TextView) {
                    view = (TextView) convertView;
                } else {
                    view = (TextView) inflater.inflate(R.layout.list_item, parent, false);
                }
                view.setText((String) list.get(position));
                return view;
            }
        }

        public synchronized void addNativeAd(NativeAd ad) {
            if (ad == null) {
                return;
            }
            if (this.ad != null) {
                // Clean up the old ad before inserting the new one
                this.ad.unregisterView();
                this.list.remove(AD_INDEX);
                this.ad = null;
                this.notifyDataSetChanged();
            }
            this.ad = ad;
            View adView = inflater.inflate(R.layout.ad_unit, null);
            NativeAdSampleActivity.inflateAd(ad, adView, NativeAdListActivity.this);
            list.add(AD_INDEX, adView);
            this.notifyDataSetChanged();
        }
    }
}
