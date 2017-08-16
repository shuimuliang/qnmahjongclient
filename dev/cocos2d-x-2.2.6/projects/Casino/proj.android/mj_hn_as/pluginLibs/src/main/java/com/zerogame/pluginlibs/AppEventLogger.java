package com.zerogame.pluginlibs;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Context;

import com.appsflyer.AFInAppEventParameterName;
import com.appsflyer.AFInAppEventType;
import com.appsflyer.AppsFlyerLib;
import com.tendcloud.tenddata.TalkingDataGA;
import com.zerogame.util.JsonUtil;

public class AppEventLogger {
	private static Context con;

	public static final String K_EVENT_SDK_INIT_FAILED = "sdk_init_failed";
	public static final String K_EVENT_SDK_LOGIN_FAILED = "sdk_login_failed";
	public static final String K_EVENT_SDK_PAY_FAILED = "sdk_pay_failed";

	public static boolean isTalkingGameInit = false;
	public static boolean isAdTrackingInit = false;
	public static boolean isAppsFlyerInit = false;

	public static void initTalkingGame(Context context, String channel,
			String talkingGameKey) {
		con = context;
		TalkingDataGA.sPlatformType = TalkingDataGA.PLATFORM_TYPE_COCOS2DX;
		TalkingDataGA.init(con, talkingGameKey, channel);
		TalkingDataGA.setVerboseLogDisabled();
		isTalkingGameInit = true;
	}

	public static void initAdTracking(Context context, String appID,
			String channelInAD) {
		isAdTrackingInit = true;
	}

	public static void initAppsFlyer(Context context, String appKey) {
		AppsFlyerLib.setAppsFlyerKey(appKey);
		AppsFlyerLib.sendTracking(context.getApplicationContext());
		isAppsFlyerInit = true;
	}

	public static void logEvent(String eventId, String eventLabel,
			String playerId, int channelId) {
	}

	public static void pageviewStart(String pageName) {
	}

	public static void pageviewEnd(String pageName) {
	}

	public static void logLogin(String parmas) {
		String playID = JsonUtil.getValueByKey(parmas, "playerID");
		if (isAdTrackingInit) {
		}
	}

	public static void logRegister(String parmas) {
		String playID = JsonUtil.getValueByKey(parmas, "playerID");
		if (isAdTrackingInit) {
		}
	}

	public static void logPurchase(final String parmas) {

		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				String playID = JsonUtil.getValueByKey(parmas, "playerID");
				String orderID = JsonUtil.getValueByKey(parmas, "orderID");
				String iapID = JsonUtil.getValueByKey(parmas, "iapID");
				String currencyType = JsonUtil.getValueByKey(parmas,
						"currencyType");
				String paymentType = JsonUtil.getValueByKey(parmas,
						"paymentType");
				String currencyAmount = JsonUtil.getValueByKey(parmas,
						"currencyAmount");
				String virtualCurrencyAmount = JsonUtil.getValueByKey(parmas,
						"virtualCurrencyAmount");

				if (currencyAmount.equals("")) {
					currencyAmount = "0";
				}

				double price = Double.parseDouble(currencyAmount);
				int priceInt = (int) (Math.ceil(price));

				if (isAdTrackingInit) {
				}

				if (isAppsFlyerInit) {
					Map<String, Object> event = new HashMap<String, Object>();
					event.put(AFInAppEventParameterName.REVENUE, currencyAmount);
					event.put(AFInAppEventParameterName.CONTENT_TYPE,
							paymentType);
					event.put(AFInAppEventParameterName.CONTENT_ID, orderID);
					event.put(AFInAppEventParameterName.CURRENCY, currencyType);
					AppsFlyerLib.trackEvent(con, AFInAppEventType.PURCHASE,
							event);
				}
			}
		});

	}

	public static void onResume(Activity context) {}

	public static void onPause(Activity context) {}

}
