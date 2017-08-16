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

import com.snowcat.casino.AppSDKManager;
import com.zerogame.pluginlibs.CJThread;

public class WebManager{
	private static WebManager instance;
	private static Activity activity;
	private RelativeLayout m_webLayout;
	final Handler myHandler = new Handler();
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
		///	m_strUrl= "file:///android_asset/test.html";
			data = data;

			try {  
					JSONTokener jsonParser = new JSONTokener(data);  
					order = (JSONObject) jsonParser.nextValue();  
				    
					newOrder.put("player_id", order.getString("playerID"));  
				    newOrder.put("wares_id", order.getString("appStoreProductId"));  
				    newOrder.put("channel", order.getString("channel"));        
				} catch (JSONException ex) {  
				}  
		//	System.out.println("-------data="+data);
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
				m_webLayout.addView(backgroundLayout, new LayoutParams(
						LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

				//pro = ProgressDialog.show(activity, null, null);
				m_WebView = new WebView(activity);
				WebSettings webSetting = m_WebView.getSettings();
				webSetting.setDomStorageEnabled(true);

				webSetting.setJavaScriptEnabled(true);
				webSetting.setSupportZoom(true);
				webSetting.setBuiltInZoomControls(false);
				webSetting.setUseWideViewPort(false);
				webSetting.setLoadWithOverviewMode(true);
				webSetting.setCacheMode(WebSettings.LOAD_NO_CACHE);
				webSetting.setUserAgentString("CasinoApp:yes Version:1.0.0");
				webSetting.setSavePassword(false);
				webSetting.setSaveFormData(false);
				m_WebView.setWebChromeClient(new WebCallBackChromeClient());
				m_WebView.addJavascriptInterface(new DemoJavaScriptInterface(activity), "external");


				LinearLayout linearLayout = new LinearLayout(activity);
				linearLayout.setOrientation(LinearLayout.VERTICAL);
				
				m_WebView.setWebViewClient(new WebCallBackClient(
						newOrder.toString()));
				m_WebView.loadUrl(m_strUrl);
				m_WebView.requestFocus();
			//	m_WebView.setBackgroundColor(110);

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
			//Log.i("aaaa", "create DemoJavaScriptInterface");
		}

		/**
		 * This is not called on the UI thread. Post a runnable to invoke
		 * loadUrl on the UI thread.
		 */
		public void iappayFinished(String noUser, String jsonStr) {
		//	System.out.println("-------iappayFinished");
		//	System.out.println("-------jsonStr="+jsonStr);
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
	     				//	System.out.println("-------c_succeff="+c_succeff);
	     				//	System.out.println("-------c_transID="+c_transID);
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
		public void iappayFailed() {
			System.out.println("-------iappayFailed");
			myHandler.post(new Runnable() {
	             @Override
	             public void run() {
	                 // This gets executed on the UI thread so it can safely modify Views
	            	 m_webLayout.removeAllViews();
	             }
	         });
		}
	}
	
	public class WebCallBackChromeClient extends WebChromeClient {

		public void openFileChooser(ValueCallback<Uri> uploadFile,
				String acceptType, String capture) {
			mUploadMessage = uploadFile;
			
		}
	}
	
	
	static ValueCallback<Uri> mUploadMessage = null;

	static public class WebCallBackClient extends WebViewClient {
		public String newOrder;
		private boolean hasSend=false;
		public WebCallBackClient(String data) {
			super();
			newOrder = data;
		}

		public WebCallBackClient() {

		}

		public boolean data(WebView view, final String url) {

			Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
				}
			});
			System.out.println("-------start loadUrl");
			return true;
		}

		public void onPageFinished(WebView view, String url) {
			super.onPageFinished(view, url);
//			if (pro != null) {
//				pro.dismiss();
//			}
			if (hasSend==false) {
				hasSend=true;
				view.loadUrl("javascript:casinoIpayH5.pay('"+newOrder+"')");
			}
			
			System.out.println("--------------onNotifyOpenUrlComplete");
			
		}
		
		@Override
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			// TODO Auto-generated method stub
			System.out.println("----------shouldOverrideUrlLoading");
			if (url.substring(0, 6).equalsIgnoreCase("weixin")) {
				
				List<Map<String, Object>> list=getInstalledApps();
				
				boolean isHave=false;
				for (int i = 0; i < list.size(); i++) {
					for (Object v : list.get(i).values()) {  
						// System.out.println("value= " + v);  
						if (v.toString().length()>=14 && (v.toString().substring(0, 14)).equalsIgnoreCase("com.tencent.mm")){
							isHave=true;
							Uri uri = Uri.parse(url); 
							Intent it = new Intent(Intent.ACTION_VIEW, uri); 
							activity.startActivity(it); 
						}
					}  
				}
				
				if (isHave==false) {
					Toast.makeText(activity, "亲，您还没有安装微信！", Toast.LENGTH_SHORT).show();
				}
				
				return true;
			}
			else
			{
				return super.shouldOverrideUrlLoading(view, url);
			}
		}

		public void onReceivedError(WebView view, int errorCode,
				String description, String failingUrl) {
//			if (pro != null) {
//				pro.dismiss();
//			}
			System.out.println("----------onNotifyOpenUrlError");

		}

		public void onPageStarted(WebView view, String url, Bitmap favicon) {
			// 显示等待
			// pro = ProgressDialog.show(m_WeiLe, null, null);
			System.out.println("----------onPageStarted");
			
			super.onPageStarted(view, url, favicon);
		}

	}
	
	public void loadUrl(Activity activity, RelativeLayout m_webLayout, String data, String strUrl, int nLeft,
			int nTop, int nWidth, int nHeight) {
		this.activity = activity;
		this.m_webLayout=m_webLayout;
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
