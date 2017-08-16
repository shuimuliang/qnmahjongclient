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
package com.zerogame.sdkeasylibs;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxWebActivity;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;

import com.tendcloud.tenddata.TalkingDataGA;
//import com.umeng.analytics.MobclickAgent;
import com.zerogame.pluginlibs.AppEventLogger;
import com.zerogame.pluginlibs.AppPlugin;

public abstract class BaseClient extends Cocos2dxWebActivity {
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		AppPlugin.getInstance().configWithAcitivity(this);
	}

	public Cocos2dxGLSurfaceView onCreateGLSurfaceView() {
		return new LuaGLSurfaceView(this);
	}

	static {
		System.loadLibrary("cocos2dlua");
	}

	public abstract String initChannel();

	@Override
	protected void onResume() {
		super.onResume();
		AppEventLogger.onResume(this);
	}

	@Override
	protected void onPause() {
		super.onPause();
		AppEventLogger.onPause(this);
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {

		if (event.getKeyCode() == KeyEvent.KEYCODE_BACK
				&& event.getAction() == KeyEvent.ACTION_DOWN
				&& event.getRepeatCount() == 0) {
			Log.i("@zz@", "KEYCODE_BACK");
			new Builder(this)
					.setMessage(getString(R.string.exit_ask))
					.setTitle(getString(R.string.exit_tip))
					.setPositiveButton(getString(R.string.exit_sure),
							new OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									new Handler().post(new Runnable() {

										@Override
										public void run() {
											// TODO Auto-generated method stub
											TalkingDataGA.onKill();
//											MobclickAgent.onKillProcess(BaseClient.this);
											android.os.Process
													.killProcess(android.os.Process
															.myPid());
										}
									});
								}
							})
					.setNegativeButton(getString(R.string.exit_cancel),
							new OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
								}
							}).create().show();
		}
		return super.dispatchKeyEvent(event);
	}
}

class LuaGLSurfaceView extends Cocos2dxGLSurfaceView {

	public LuaGLSurfaceView(Context context) {
		super(context);
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// exit program when key back is entered
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			android.os.Process.killProcess(android.os.Process.myPid());
		}
		return super.onKeyDown(keyCode, event);
	}

}
