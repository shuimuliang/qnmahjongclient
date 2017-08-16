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
package com.snowcat.casino;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.snowcat.casino.config.GameConfig;
import com.snowcat.casino.web.WebManager;
import com.tendcloud.tenddata.TalkingDataGA;
import com.zerogame.pluginlibs.AppEventLogger;
import com.zerogame.pluginlibs.AppPlugin;
import com.zerogame.sdkeasylibs.BaseClient;

public class Casino extends BaseClient {

	static final AppPlugin pluginManager = AppPlugin.getInstance();
	public static WebManager webManager;
	private static RelativeLayout m_webLayout;
	private static Activity activity;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		activity = this;

		AppEventLogger.initTalkingGame(this, initChannel(),
				GameConfig.TALKING_GAME_KEY);
		AppEventLogger.initAdTracking(this, GameConfig.AD_TRACKING_KEY,
				"GooglePlay");

		final AppSDKManager manager = AppSDKManager.getInstance();
		manager.configManager(this, Cocos2dxGLSurfaceView.getInstance());

		pluginManager.configWithAcitivity(this);

		webManager = WebManager.getInstance();

		if (m_webLayout == null) {
			m_webLayout = new RelativeLayout(this);
			addContentView(m_webLayout, new LayoutParams(
					LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub

		pluginManager.selectImageResult(requestCode, resultCode, data);

		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public String initChannel() {
		return AppSDKManager.getInstance().getChannelStr();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		AppSDKManager.getInstance().exitSDK();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		TalkingDataGA.onResume(this);
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		TalkingDataGA.onPause(this);
	}

	public static void openURL1(String url, String data) {
		webManager.loadUrl(activity, m_webLayout, data, url, 50, 50, 1136, 640);
	}
}