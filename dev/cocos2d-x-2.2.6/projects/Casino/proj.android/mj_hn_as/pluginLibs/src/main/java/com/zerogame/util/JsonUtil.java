package com.zerogame.util;

import org.json.JSONException;
import org.json.JSONObject;

import android.text.TextUtils;

public class JsonUtil {
	public static String getValueByKey(String jsonString, String keyString) {
		String payIDStr = "";
		if (!TextUtils.isEmpty(jsonString)) {
			try {
				JSONObject json = new JSONObject(jsonString);
				payIDStr = json.getString(keyString);
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		if (payIDStr == null) {
			payIDStr = "";
		}
		return payIDStr;
	}
}
