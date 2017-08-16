package com.zerogame.sdkeasylibs;

import android.app.Activity;
import android.opengl.GLSurfaceView;

import com.zerogame.pluginlibs.CJThread;
import com.zerogame.pluginlibs.MyLogger;

public abstract class SDKObjectBase {

	public static final int kAppThirdLoginErrorCodeNone = 0;
	public static final int kAppThirdLoginErrorCodeCanceld = 1; // 用户取消
	public static final int kAppThirdLoginErrorCodeFailed = 2; // 失败
	public static final int kAppThirdLoginErrorCodeInfoPoor = 3; // 信息不足
	// property
	protected Activity curActivity = null;

	protected boolean isInit = false;
	protected boolean isLogin = false;

	protected String m_accountID = "";
	protected String m_accountName = "";
	protected String m_sessionID = "";

	protected MyLogger logger = MyLogger.log("街机电玩大师");

	// initManager
	public void configManager(Activity activity, GLSurfaceView value) {

		curActivity = activity;
		CJThread.init();
	}

	// return to cpp

	public String getAccountID() {
		return m_accountID;
	}

	public void setAccountID(String accountID) {
		m_accountID = accountID;
	}

	public String getAccountName() {
		return m_accountName;
	}

	public void setAccountName(String accountName) {
		m_accountName = accountName;
	}

	public String getSessionID() {
		return m_sessionID;
	}

	public void setSessionID(String sessionID) {
		m_sessionID = sessionID;
	}

	public boolean isInitSDK() {
		return isInit;
	}

	public boolean isLoginSDK() {
		return isLogin;
	}

	// native
	public native void initSdk(int code);

	public native void loginResult(boolean result);

	public native void thirdLoginResult(int code, String info);

	public native void payResult(boolean result, String order);

	public native void logoutResult();

	public native void shareResult(boolean result);

	public native String getSDKPrivateKey();

	public native String getSDKPublicKey();
	
	public native void setDeviceToken(String token);
	
	public native void videoCallBack(int status);

	// need to implement
	public abstract void initChannelSDK();

	public abstract void loginSDK();

	public abstract void thirdLogin(int loginType);

	public abstract void logoutSDK(boolean iscallback);

	public abstract void userCenterSDK();

	public abstract void paySDK(String extraData);

	public abstract void showWebPayView(String url, String extraData);

	public abstract void exitSDK();

	public abstract void share(String text);

	public abstract boolean isWXAppInstalled();

	protected abstract String getChannelStr();

	public abstract int getChannelId();

	public abstract int getDesignLanguage();

	public abstract int getLoginType();
	
	public abstract void playVideo(String extraData);
}