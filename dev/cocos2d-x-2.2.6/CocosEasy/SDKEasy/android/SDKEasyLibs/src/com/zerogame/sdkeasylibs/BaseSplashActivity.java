package com.zerogame.sdkeasylibs;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

public class BaseSplashActivity extends Activity {

	private static final long SPLASH_DELAY_MILLIS = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splash);
		printVersion();
	}

	protected void initLogo(final initLogoCallBack mCallBack) {
		new Handler().postDelayed(new Runnable() {

			@Override
			public void run() {
				if (mCallBack != null) {

					mCallBack.onFinished();
				}
			}
		}, SPLASH_DELAY_MILLIS);

	}

	public static abstract interface initLogoCallBack {
		public abstract void onFinished();
	}

	private void printVersion() {
		String appVersion;
		int appCode;
		PackageManager manager = this.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(this.getPackageName(), 0);
			appVersion = info.versionName; // 版本名
			appCode = info.versionCode;
			Log.i("casinohk", "versionName:" + appVersion + "	versionCode:" + appCode);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
	}
}