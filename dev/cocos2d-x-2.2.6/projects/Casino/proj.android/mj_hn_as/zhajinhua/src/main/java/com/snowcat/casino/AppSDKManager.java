package com.snowcat.casino;

import android.app.Activity;
import android.opengl.GLSurfaceView;

import com.snowcat.casino.config.GameConfig;
import com.snowcat.casino.weixin.WeiXinAdapter;
import com.zerogame.pluginlibs.AppPlugin;
import com.zerogame.pluginlibs.CJThread;
import com.zerogame.pluginlibs.MyLogger;
import com.zerogame.sdkeasylibs.SDKObjectBase;

public class AppSDKManager extends SDKObjectBase {

	private static AppSDKManager instance;
	private WeiXinAdapter mWXAdapter;
	private String mWXState = "";
	private boolean mIsLogining = false;


	public static AppSDKManager getInstance() {
		if (instance == null) {
			instance = new AppSDKManager();
		}
		return instance;
	}

	@Override
	public void configManager(Activity activity, GLSurfaceView value) {
		super.configManager(activity, value);
		mWXAdapter = new WeiXinAdapter(activity);
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
		//return "channel_" + getChannelId();
		return "千胜麻将_Android";
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
		if (mWXAdapter != null){
			CJThread.runOnMainThread(new Runnable() {
				@Override
				public void run() {
					mWXAdapter.login();
				}
			});
		}
	}

	@Override
	public int getLoginType() {
		return GameConfig.LOGIN_TYPE;
	}

	@Override
	public void share(String text) {
		// TODO Auto-generated method stub
		final String textF = text;
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				mWXAdapter.share(textF);
			}
		});
	}

    @Override
    public boolean isWXAppInstalled() {
        return mWXAdapter.isWXAppInstalled();
    }


    @Override
	public void playVideo(final String extraData) {
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
			}
		});
	}

    public String getMissOrderList(){
     return "";
    }

	public void onPause() {
	}

	public void onResume() {
	}

	public String getWXState(){
		return mWXState;
	}

	public void setWXState(String wxState){
		mWXState = wxState;
	}

	public boolean getIsLogining(){
		return mIsLogining;
	}

	public void setIsLogining(boolean isLogining){
		mIsLogining = isLogining;
	}
}
