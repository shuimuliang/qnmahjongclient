package com.snowcat.casino;

import android.content.Intent;
import android.os.Bundle;

import com.zerogame.sdkeasylibs.BaseSplashActivity;

public class SplashActivity extends BaseSplashActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// 执行BaseSplashActivity里的布局
		super.onCreate(savedInstanceState);

		initLogo(new initLogoCallBack() {
			@Override
			public void onFinished() {
				Intent intent = new Intent(SplashActivity.this, Casino.class);
				SplashActivity.this.startActivity(intent);
				SplashActivity.this.finish();
			}
		});
	}
}