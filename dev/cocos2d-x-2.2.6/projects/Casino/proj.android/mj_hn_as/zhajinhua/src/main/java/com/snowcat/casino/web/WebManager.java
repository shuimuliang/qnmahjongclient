package com.snowcat.casino.web;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.os.Handler;
import android.text.TextUtils;
import android.view.Display;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;
//import android.webkit.JavascriptInterface;

import com.snowcat.casino.AppSDKManager;
import com.zerogame.pluginlibs.CJThread;

public class WebManager{
	private static WebManager instance;
	private static Activity activity;
	private RelativeLayout m_webLayout;
	private Handler myHandler = new Handler();
//	public static ProgressDialog pro;
	private WebView m_WebView;

	public static WebManager getInstance() {
		if (instance == null) {
			instance = new WebManager();
		}
		return instance;
	}
	
	class WebRunnable implements Runnable {
		private String data;
		private String m_strUrl;
		private int m_nLeft;
		private int m_nTop;
		private int m_nWidth;
		private int m_nHeight;
	    JSONObject order;
	    JSONObject newOrder = new JSONObject();  
	    
		public WebRunnable(String data, String strUrl, int nLeft,
				int nTop, int nWidth, int nHeight) {
			super();
			m_nHeight = nHeight;
			m_nWidth = nWidth;
			m_nTop = nTop;
			m_nLeft = nLeft;
			m_strUrl = strUrl;
			try {  
					JSONTokener jsonParser = new JSONTokener(data);  
					order = (JSONObject) jsonParser.nextValue();
					m_strUrl = strUrl + "?wares_id=" +  order.getString("appStoreProductId") + "&channel=" + order.getString("channel") + "&player_id=" + order.getString("playerID");

				} catch (JSONException ex) {  
				}  
		}

		/**
		 * 加载手机内置支付app
		 * @param url
		 */
		private void toLoadInnerApp(String url) {
			try {
                Intent it = new Intent(Intent.ACTION_VIEW);
				it.setData(Uri.parse(url));
				activity.startActivity(it);

//				System.out.println("hk ------------------ removeAllViews");
//				m_webLayout.removeAllViews();
            } catch (Exception e) {
				//这里需要处理 发生异常的情况
				//可能情况： 手机没有安装支付宝或者微信。或者安装支付宝或者微信但是版本过低
			}
		}

		public void run() {
			try {
				// 背景遮罩
				RelativeLayout backgroundLayout = new RelativeLayout(activity);
				backgroundLayout.setBackgroundColor(Color.BLACK);
				backgroundLayout.getBackground().setAlpha(150);

				backgroundLayout.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View arg0) {
						// TODO Auto-generated method stub

					}
				});

				LinearLayout linearLayout = new LinearLayout(activity);
				linearLayout.setOrientation(LinearLayout.VERTICAL);

				m_webLayout.addView(backgroundLayout, new LayoutParams(
						LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

				m_WebView = new WebView(activity);
				WebSettings webSetting = m_WebView.getSettings();
				webSetting.setDomStorageEnabled(true);
                webSetting.setSupportZoom(true);
                webSetting.setBuiltInZoomControls(false);
                webSetting.setUseWideViewPort(false);
                webSetting.setJavaScriptEnabled(true);
                webSetting.setLoadWithOverviewMode(true);
                webSetting.setSavePassword(false);
                webSetting.setSaveFormData(false);
                webSetting.setCacheMode(WebSettings.LOAD_NO_CACHE);
				m_WebView.setWebChromeClient(new WebChromeClient());
				m_WebView.addJavascriptInterface(new DemoJavaScriptInterface(activity), "external");

				m_WebView.setWebViewClient(new WebViewClient() {
					@Override
					public boolean shouldOverrideUrlLoading(WebView view, String url) {
						final String urlF = url;
						if (!TextUtils.isEmpty(urlF)) {
							if (!urlF.startsWith("http") && !urlF.startsWith("https")) {{
								//加载手机内置支付
								myHandler.post(new Runnable() {
									@Override
									public void run() {
										// This gets executed on the UI thread so it can safely modify Views
										toLoadInnerApp(urlF);
										}
								});
								return true;
							}
							} else {
								return false;
							}
						}
						return  false;
					}
				});

				myHandler.postDelayed(new Runnable() {

					@Override
					public void run() {
                        m_WebView.loadUrl(m_strUrl);
					}
				}, 100);

				Display mDisplay = activity.getWindowManager()
						.getDefaultDisplay();
				int w = mDisplay.getWidth();
				int h = mDisplay.getHeight();

				RelativeLayout.LayoutParams weblatouParmas = null;
				
				int nImgW =w;
				int nImgH = h;
				RelativeLayout.LayoutParams latouParmas = new RelativeLayout.LayoutParams(
						nImgW, nImgH);
				latouParmas.addRule(RelativeLayout.CENTER_IN_PARENT);

				weblatouParmas = new RelativeLayout.LayoutParams(nImgW, nImgH);

				weblatouParmas.addRule(RelativeLayout.CENTER_IN_PARENT);
				m_WebView.setLayoutParams(weblatouParmas);
				m_webLayout.addView(m_WebView);

			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	}
	
	final class DemoJavaScriptInterface {
		DemoJavaScriptInterface(Activity activity) {
		}

		/**
		 * This is not called on the UI thread. Post a runnable to invoke
		 * loadUrl on the UI thread.
		 */
//		@JavascriptInterface
		public void iappayFinished(String noUser, String jsonStr) {
			JSONObject jsonRes;
			String transID="";
			boolean succ=false;
			if (jsonStr!=null) {
				try {
					jsonRes = new JSONObject(jsonStr);
					transID = jsonRes
							.getString("TransId");
				} catch (Exception e) {
					// TODO: handle exception
				}
			}
			if (transID.equalsIgnoreCase("")==false) {
				succ=true;
			}
			final boolean c_succeff=succ;
			final String c_transID=transID;
			
			myHandler.post(new Runnable() {
	             @Override
	             public void run() {
	                 // This gets executed on the UI thread so it can safely modify Views
	            	 CJThread.runOnGLThread(new Runnable() {
	     				public void run() {
	     					JSONObject order = new JSONObject();
	     					try {
								order.put("transaction_id", c_transID);
							} catch (JSONException e) {
								e.printStackTrace();
							} 
	     					AppSDKManager.getInstance().payResult(c_succeff, order.toString());
	     				}
	     			});
	            	 m_webLayout.removeAllViews();
	             }
	         });

		}
//		@JavascriptInterface
		public void iappayFailed() {
			System.out.println("-------iappayFailed");
			myHandler.post(new Runnable() {
	             @Override
	             public void run() {
	            	 m_webLayout.removeAllViews();
	             }
	         });
		}

//		@JavascriptInterface
		public void userDidCancelPayment() {
			System.out.println("-------userDidCancelPayment");
			myHandler.post(new Runnable() {
				@Override
				public void run() {
					m_webLayout.removeAllViews();
				}
			});
		}
	}

	public void loadUrl(Activity activity, RelativeLayout m_webLayout, String data, String strUrl, int nLeft,
			int nTop, int nWidth, int nHeight) {
		this.activity = activity;
		this.m_webLayout=m_webLayout;
        this.myHandler = new Handler();
		this.activity.runOnUiThread(new WebRunnable(data, strUrl, nLeft,
				nTop, nWidth, nHeight));
	}
	
	private static List<Map<String, Object>> getInstalledApps() {  
		 List<PackageInfo> packages =activity.getPackageManager().getInstalledPackages(0);  
	        List<Map<String, Object>> listMap = new ArrayList<Map<String,Object>>(packages.size());  
	       for (int j = 0; j < packages.size(); j++) {  
	            Map<String, Object> map = new HashMap<String, Object>();  
	           PackageInfo packageInfo = packages.get(j);  
	            if((packageInfo.applicationInfo.flags&ApplicationInfo.FLAG_SYSTEM)==0){  
	                map.put("img", packageInfo.applicationInfo.loadIcon(activity.getPackageManager()).getCurrent());  
	                map.put("name", packageInfo.applicationInfo.loadLabel(activity.getPackageManager()).toString());  
	                map.put("desc", packageInfo.packageName);  
	                listMap.add(map);  
	           }  
	        }  
	        return listMap;  
	   }
}
