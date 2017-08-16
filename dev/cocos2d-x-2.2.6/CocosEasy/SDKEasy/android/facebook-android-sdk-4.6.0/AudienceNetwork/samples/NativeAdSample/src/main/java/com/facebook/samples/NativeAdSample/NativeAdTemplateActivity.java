package com.facebook.samples.NativeAdSample;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.*;
import android.os.Build;
import android.widget.*;
import android.widget.AdapterView;
import com.facebook.ads.*;

import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class NativeAdTemplateActivity extends Activity implements AdListener {
    // Color set
    private int backgroundColor;
    private int rowBackgroundColor;
    private int titleColor;
    private int linkColor;
    private int contentColor;
    private int borderColor;

    private NativeAdView.Type viewType = NativeAdView.Type.HEIGHT_100;
    private NativeAdViewAttributes adViewAttributes = new NativeAdViewAttributes();

    private TextView statusText;
    private AdView adView;
    private NativeAd nativeAd;
    private LinearLayout nativeAdContainer;
    private Spinner backgroundColorSpinner;
    private Spinner adViewTypeSpinner;
    private ScrollView scrollView;
    private Button showCodeButton;

    private LinearLayout row1;
    private TextView row1Title;
    private TextView row1Content;
    private LinearLayout row2;
    private TextView row2Title;
    private TextView row2Subtitle1;
    private TextView row2Content1;
    private TextView row2Subtitle2;
    private TextView row2Content2;
    private LinearLayout row3;
    private TextView row3Title;
    private TextView row3Subtitle1;
    private TextView row3Content1;
    private TextView row3Subtitle2;
    private TextView row3Content2;
    private TextView row3Subtitle3;
    private TextView row3Content3;
    private TextView row3Subtitle4;
    private TextView row3Content4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_template);

        // Get Views
        row1 = (LinearLayout) findViewById(R.id.row1);
        row1Title = (TextView) findViewById(R.id.row1_title);
        row1Content = (TextView) findViewById(R.id.row1_content);

        row2 = (LinearLayout) findViewById(R.id.row2);
        row2Title = (TextView) findViewById(R.id.row2_title);
        row2Subtitle1 = (TextView) findViewById(R.id.row2_subtitle1);
        row2Content1 = (TextView) findViewById(R.id.row2_content1);
        row2Subtitle2 = (TextView) findViewById(R.id.row2_subtitle2);
        row2Content2 = (TextView) findViewById(R.id.row2_content2);

        row3 = (LinearLayout) findViewById(R.id.row3);
        row3Title = (TextView) findViewById(R.id.row3_title);
        row3Subtitle1 = (TextView) findViewById(R.id.row3_subtitle1);
        row3Content1 = (TextView) findViewById(R.id.row3_content1);
        row3Subtitle2 = (TextView) findViewById(R.id.row3_subtitle2);
        row3Content2 = (TextView) findViewById(R.id.row3_content2);
        row3Subtitle3 = (TextView) findViewById(R.id.row3_subtitle3);
        row3Content3 = (TextView) findViewById(R.id.row3_content3);
        row3Subtitle4 = (TextView) findViewById(R.id.row3_subtitle4);
        row3Content4 = (TextView) findViewById(R.id.row3_content4);

        statusText = (TextView) findViewById(R.id.status);
        nativeAdContainer = (LinearLayout) findViewById(R.id.nativeAdContainer);
        scrollView = (ScrollView) findViewById(R.id.scrollView);
        showCodeButton = (Button) findViewById(R.id.showCodeButton);

        backgroundColorSpinner = (Spinner) findViewById(R.id.backgroundColorSpinner);
        ArrayAdapter<CharSequence> backgroundColorSpinnerAdapter =
                ArrayAdapter.createFromResource(
                        this,
                        R.array.background_color_array,
                        android.R.layout.simple_spinner_item);
        backgroundColorSpinnerAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);
        backgroundColorSpinner.setAdapter(backgroundColorSpinnerAdapter);

        adViewTypeSpinner = (Spinner) findViewById(R.id.adViewTypeSpinner);
        ArrayAdapter<CharSequence> adViewTypeSpinnerAdapter = ArrayAdapter.createFromResource(this,
                R.array.adview_type_array, android.R.layout.simple_spinner_item);
        adViewTypeSpinnerAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);
        adViewTypeSpinner.setAdapter(adViewTypeSpinnerAdapter);

        setSpinnerListeners();
        setButtonListeners();

        createAndLoadNativeAd();
    }

    protected void createAndLoadNativeAd() {
        // Create a native ad request with a unique placement ID
        // (generate your own on the Facebook app settings).
        // Use different ID for each ad placement in your app.
        nativeAd = new NativeAd(NativeAdTemplateActivity.this, "YOUR_PLACEMENT_ID");

        // Set a listener to get notified when the ad was loaded.
        nativeAd.setAdListener(NativeAdTemplateActivity.this);

        // Initiate a request to load an ad.
        nativeAd.loadAd();

        statusText.setText("Loading an ad...");
    }

    @Override
    public void onAdLoaded(Ad ad) {
        if (nativeAd == null || nativeAd != ad) {
            // Race condition, load() called again before last ad was displayed
            return;
        }

        statusText.setText("Ad loaded.");
        reloadAdContainer();
    }

    @Override
    public void onError(Ad ad, AdError error) {
        statusText.setText("Ad failed to load: " + error.getErrorMessage());
    }

    @Override
    public void onAdClicked(Ad ad) {
        statusText.setText("Ad Clicked");
    }

    private void reloadAdContainer() {
        if (nativeAd != null && nativeAd.isAdLoaded()) {
            nativeAdContainer.removeAllViews();

            // Create a NativeAdViewAttributes object (e.g. adViewAttributes)
            //   to render the native ads
            // Set the attributes
            adViewAttributes.setBackgroundColor(rowBackgroundColor);
            adViewAttributes.setTitleTextColor(titleColor);
            adViewAttributes.setDescriptionTextColor(contentColor);
            adViewAttributes.setButtonBorderColor(linkColor);
            adViewAttributes.setButtonTextColor(linkColor);

            // Use NativeAdView.render to generate the ad View
            // NativeAdViewType viewType = NativeAdViewType.HEIGHT_100;
            View adView = NativeAdView.render(this, nativeAd, viewType, adViewAttributes);

            // Add adView to the container showing Ads
            nativeAdContainer.addView(adView, 0);
            nativeAdContainer.setBackgroundColor(Color.TRANSPARENT);

            showCodeButton.setText(R.string.show_code);
        }
    }

    private void setSpinnerListeners() {
        backgroundColorSpinner.setOnItemSelectedListener(
                new android.widget.AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> arg0,
                                               View view,
                                               int position,
                                               long id) {
                        int item = backgroundColorSpinner.getSelectedItemPosition();
                        switch (item) {
                            case 0:
                                backgroundColor = Color.LTGRAY;
                                rowBackgroundColor = Color.WHITE;
                                titleColor = Color.argb(0xff, 0x4e, 0x56, 0x65);
                                linkColor = Color.argb(0xff, 0x3b, 0x59, 0x98);
                                contentColor = Color.argb(0xff, 0x4e, 0x56, 0x65);
                                borderColor = Color.GRAY;
                                break;
                            case 1:
                                backgroundColor = Color.BLACK;
                                rowBackgroundColor = Color.DKGRAY;
                                titleColor = Color.WHITE;
                                linkColor = Color.GREEN;
                                contentColor = Color.WHITE;
                                borderColor = Color.GRAY;
                                break;
                            case 2:
                                backgroundColor = Color.BLUE;
                                rowBackgroundColor = Color.TRANSPARENT;
                                titleColor = Color.WHITE;
                                linkColor = Color.CYAN;
                                contentColor = Color.WHITE;
                                borderColor = Color.GRAY;
                                break;
                        }
                        resetAllColor();
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {}
                }
        );

        adViewTypeSpinner.setOnItemSelectedListener(
                new android.widget.AdapterView.OnItemSelectedListener() {
                    public void onItemSelected(AdapterView<?> arg0,
                                               View view,
                                               int position,
                                               long id) {
                        int item = adViewTypeSpinner.getSelectedItemPosition();
                        switch (item) {
                            case 0:
                                viewType = NativeAdView.Type.HEIGHT_100;
                                break;
                            case 1:
                                viewType = NativeAdView.Type.HEIGHT_120;
                                break;
                            case 2:
                                viewType = NativeAdView.Type.HEIGHT_300;
                                break;
                            case 3:
                                viewType = NativeAdView.Type.HEIGHT_400;
                                break;
                        }
                        reloadAdContainer();
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                    }
                }
        );
    }

    private void resetAllColor(){
        if (backgroundColor == Color.BLUE) {
            scrollView.setBackgroundResource(R.drawable.background_blue);
        } else {
            scrollView.setBackgroundColor(backgroundColor);
        }

        row1.setBackgroundColor(rowBackgroundColor);
        row2.setBackgroundColor(rowBackgroundColor);
        row3.setBackgroundColor(rowBackgroundColor);

        row1Title.setTextColor(titleColor);
        row2Title.setTextColor(titleColor);
        row3Title.setTextColor(titleColor);

        row2Subtitle1.setTextColor(linkColor);
        row2Subtitle2.setTextColor(linkColor);
        row3Subtitle1.setTextColor(linkColor);
        row3Subtitle2.setTextColor(linkColor);
        row3Subtitle3.setTextColor(linkColor);
        row3Subtitle4.setTextColor(linkColor);

        row1Content.setTextColor(contentColor);
        row2Content1.setTextColor(contentColor);
        row2Content2.setTextColor(contentColor);
        row3Content1.setTextColor(contentColor);
        row3Content2.setTextColor(contentColor);
        row3Content3.setTextColor(contentColor);
        row3Content4.setTextColor(contentColor);

        scrollView.invalidate();

        reloadAdContainer();
    }

    private void setButtonListeners() {
        showCodeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (showCodeButton.getText() == getResources().getString(R.string.show_ad)) {
                    reloadAdContainer();
                } else {
                    showCodeInAdContainer();
                }
            }
        });
    }

    private void showCodeInAdContainer() {
        String [] lines = getResources().getStringArray(R.array.code_snippet);
        StringBuilder codeSnippet = new StringBuilder();
        for (int i=0; i < lines.length; i++) {
            codeSnippet.append(lines[i]).append("\r\n");
        }
        nativeAdContainer.removeAllViews();
        TextView code = new TextView(this);
        code.setText(codeSnippet);
        code.setBackgroundColor(Color.WHITE);
        code.setTextColor(Color.BLACK);
        nativeAdContainer.addView(code, 0);

        showCodeButton.setText(R.string.show_ad);
    }

    public void onNothingSelected(AdapterView<?> parent) {
        // Another interface callback
    }
}
