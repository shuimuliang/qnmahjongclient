package com.snowcat.casino.push;

import android.app.Activity;
import android.app.Application;
import android.app.Notification;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.widget.Toast;

import com.snowcat.casino.AppSDKManager;
import com.umeng.message.IUmengRegisterCallback;
import com.umeng.message.IUmengUnregisterCallback;
import com.umeng.message.PushAgent;
import com.umeng.message.UTrack;
import com.umeng.message.UmengMessageHandler;
import com.umeng.message.UmengNotificationClickHandler;
import com.umeng.message.UmengRegistrar;
import com.umeng.message.entity.UMessage;

public class UmengMsgAdapter {

	private static UmengMsgAdapter mInstance;
	private PushAgent mPushAgent;
	private Application mApplication;
	public static final String CALLBACK_RECEIVER_ACTION = "callback_receiver_action";
	public static IUmengRegisterCallback mRegisterCallback;
	public static IUmengUnregisterCallback mUnregisterCallback;

	public static UmengMsgAdapter getInstance() {
		if (mInstance == null) {
			mInstance = new UmengMsgAdapter();
		}
		return mInstance;
	}

	public void registApplication(Application application) {

		mApplication = application;
		mPushAgent = PushAgent.getInstance(mApplication);
		mPushAgent.setDebugMode(true);

		/**
		 * 该Handler是在IntentService中被调用，故 1.
		 * 如果需启动Activity，需添加Intent.FLAG_ACTIVITY_NEW_TASK 2.
		 * IntentService里的onHandleIntent方法是并不处于主线程中，因此，如果需调用到主线程，需如下所示;
		 * 或者可以直接启动Service
		 * */
		UmengMessageHandler messageHandler = new UmengMessageHandler() {
			@Override
			public void dealWithCustomMessage(final Context context,
					final UMessage msg) {
				new Handler(mApplication.getMainLooper()).post(new Runnable() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						UTrack.getInstance(mApplication.getApplicationContext())
								.trackMsgClick(msg);
						Toast.makeText(context, msg.custom, Toast.LENGTH_LONG)
								.show();
					}
				});
			}

			@Override
			public Notification getNotification(Context context, UMessage msg) {
				switch (msg.builder_id) {
				case 1:

				default:
					// 默认为0，若填写的builder_id并不存在，也使用默认。
					return super.getNotification(context, msg);
				}
			}
		};
		mPushAgent.setMessageHandler(messageHandler);

		/**
		 * 该Handler是在BroadcastReceiver中被调用，故
		 * 如果需启动Activity，需添加Intent.FLAG_ACTIVITY_NEW_TASK
		 * */
		UmengNotificationClickHandler notificationClickHandler = new UmengNotificationClickHandler() {
			@Override
			public void dealWithCustomAction(Context context, UMessage msg) {
				Toast.makeText(context, msg.custom, Toast.LENGTH_LONG).show();
			}
		};
		mPushAgent.setNotificationClickHandler(notificationClickHandler);

		mRegisterCallback = new IUmengRegisterCallback() {

			@Override
			public void onRegistered(String registrationId) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(CALLBACK_RECEIVER_ACTION);
				mApplication.sendBroadcast(intent);
			}

		};
		mPushAgent.setRegisterCallback(mRegisterCallback);

		mUnregisterCallback = new IUmengUnregisterCallback() {

			@Override
			public void onUnregistered(String registrationId) {
				Intent intent = new Intent(CALLBACK_RECEIVER_ACTION);
				mApplication.sendBroadcast(intent);
			}
		};
		mPushAgent.setUnregisterCallback(mUnregisterCallback);
	}
	
	public void init(Activity activity,String channelString){
		mPushAgent = PushAgent.getInstance(activity);
		mPushAgent.enable(mRegisterCallback);
		mPushAgent.setMessageChannel(channelString);
		mPushAgent.onAppStart();
		
		String device_token = UmengRegistrar.getRegistrationId(activity);
		AppSDKManager.getInstance().setDeviceToken(device_token);
	}
}
