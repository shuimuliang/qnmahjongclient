package com.snowcat.casino;

import android.app.Activity;
import android.opengl.GLSurfaceView;

import com.snowcat.casino.config.GameConfig;
import com.zerogame.pluginlibs.AppPlugin;
import com.zerogame.pluginlibs.CJThread;
import com.zerogame.pluginlibs.MyLogger;
import com.zerogame.sdkeasylibs.SDKObjectBase;
//import com.snowcat.casino.push.UmengMsgAdapter;
//import com.umeng.message.PushAgent;

public class AppSDKManager extends SDKObjectBase {

	private static AppSDKManager instance;
	
	public static AppSDKManager getInstance() {
		if (instance == null) {
			instance = new AppSDKManager();
		}
		return instance;
	}

	@Override
	public void configManager(Activity activity, GLSurfaceView value) {
		super.configManager(activity, value);
		MyLogger.logFlag = true;
	}

	@Override
	public void loginSDK() {
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				String deviceId = AppPlugin.getInstance().getDeviceUniqueID();
				m_accountName = deviceId;
				m_accountID = deviceId;
				m_sessionID = deviceId;
				CJThread.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						isLogin = true;
						loginResult(true);
					}
				});
			}
		});
	}

	@Override
	public void initChannelSDK() {
//		UmengMsgAdapter.getInstance().init(curActivity, getChannelId() + "");
				
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				// do channel init()
				isInit = true;
			}
		});
	}

	@Override
	public String getChannelStr() {
		return "老虎机_官网_Android";
	}

	@Override
	public void paySDK(String extraData) {
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
			}
		});
	}

	@Override
	public void showWebPayView(final String url, final String extraData) {
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Casino.openURL1(url, extraData);
			}
		});
	}

	@Override
	public void exitSDK() {
		// do channel init()
	}

	@Override
	public int getDesignLanguage() {
		return GameConfig.DESIGN_LANGUAGE;
	}

	@Override
	public int getChannelId() {
		return GameConfig.CAHNNEL_ID;
	}

	@Override
	public void userCenterSDK() {
	}

	@Override
	public void logoutSDK(boolean iscallback) {
		// TODO Auto-generated method stub
		if (iscallback) {
			CJThread.runOnMainThread(new Runnable() {
				@Override
				public void run() {
					// do channel init()
				}
			});
		}
	}

	@Override
	public void thirdLogin(int loginType) {
		// TODO Auto-generated method stub
	}

	@Override
	public int getLoginType() {
		return GameConfig.LOGIN_TYPE;
	}

	@Override
	public void shareSDK() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void playVideo(String extraData) {
		// TODO Auto-generated method stub
		
	}

}
