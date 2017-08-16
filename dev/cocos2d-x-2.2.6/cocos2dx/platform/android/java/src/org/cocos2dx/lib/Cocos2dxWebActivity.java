/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.webkit.CookieManager;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class Cocos2dxWebActivity extends Cocos2dxActivity {

	public static Activity actInstance;
	private LinearLayout _webLayout;
	private WebView _webView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		actInstance = this;
		_webLayout = new LinearLayout(this);
		actInstance.addContentView(_webLayout, new LayoutParams(
				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
	}

	public static Object getJavaActivity() {
		return actInstance;
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && _webView != null) {
			return false;
		}
		return super.onKeyDown(keyCode, event);
	}

	public void displayWebView(final int x, final int y, final int width,
			final int height) {
		this.runOnUiThread(new Runnable() {
			public void run() {
				_webView = new WebView(actInstance);
				_webLayout.addView(_webView);

				LinearLayout.LayoutParams linearParams = (LinearLayout.LayoutParams) _webView
						.getLayoutParams();
				linearParams.leftMargin = x;
				linearParams.topMargin = y;
				linearParams.width = width;
				linearParams.height = height;
				_webView.setLayoutParams(linearParams);

				_webView.setBackgroundColor(0);
				_webView.getSettings().setCacheMode(WebSettings.LOAD_NO_CACHE);
				_webView.getSettings().setAppCacheEnabled(false);
				_webView.getSettings().setJavaScriptEnabled(true);
				CookieManager.getInstance().setAcceptCookie(true);

				_webView.setWebViewClient(new WebViewClient() {
					@Override
					public boolean shouldOverrideUrlLoading(WebView view,
							String url) {
						return false;
					}
				});
			}
		});
	}

	public void updateURL(final String url) {
		String urlString = url;
		if (url != null && url.length() != 0 && url.startsWith("file")
				&& url.contains("assets")) {
			urlString = urlString.replace("assets", "android_asset");
		}
		final String finalString = urlString;

		this.runOnUiThread(new Runnable() {
			public void run() {
				_webView.loadUrl(finalString);
			}
		});
	}

	public void removeWebView() {
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (_webView != null) {
					_webLayout.removeView(_webView);
					_webView.destroy();
					_webView = null;
				}
			}
		});
	}
}
