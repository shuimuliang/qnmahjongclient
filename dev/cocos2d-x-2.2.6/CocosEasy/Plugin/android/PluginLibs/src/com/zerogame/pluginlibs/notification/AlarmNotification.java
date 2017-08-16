package com.zerogame.pluginlibs.notification;


import java.util.Calendar;
import java.util.HashMap;

import android.R.integer;
import android.R.string;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

public class AlarmNotification {
	private static AlarmNotification ince = null;
	private Activity mainActivity = null;
	
	private HashMap<String, PendingIntent> pendingMap = new HashMap<String, PendingIntent>();
	
	public static AlarmNotification Ince(){
		if(ince == null)
		{
			ince = new AlarmNotification();
		}
		return ince;
	}
	
	public void setActivity(Activity activity){
		mainActivity = activity;
	}
	
	
	public void setAlarmNotification(int type, int repeatDays, int timeByMin , String title , String msg,int tagId){
		
		Intent intent = new Intent("MyReceiver");
		intent.setAction("MyReceiver");
		intent.putExtra(NotiConstant.strTitle,title);
		intent.putExtra(NotiConstant.strMsg, msg);
		intent.putExtra("id", tagId);
		intent.putExtra("type",type);
		
		PendingIntent pendingIntent = PendingIntent.getBroadcast(mainActivity, 0, intent, tagId);
		String id = String.valueOf(tagId);
		pendingMap.put(id,pendingIntent);
				
		AlarmManager am = (AlarmManager)mainActivity.getSystemService(Context.ALARM_SERVICE);
		if(type == 1){
			SharedPreferences preferences = mainActivity.getSharedPreferences("day",Activity.MODE_PRIVATE);
			SharedPreferences.Editor editor = preferences.edit();
			Calendar c = Calendar.getInstance(); 
			int day = c.get(Calendar.DAY_OF_YEAR);
			editor.putInt("day", day);
			editor.commit();
			
			Calendar calendar = Calendar.getInstance();
			calendar.add(Calendar.HOUR, 24);
			calendar.set(Calendar.HOUR_OF_DAY, timeByMin/60);
			calendar.set(Calendar.MINUTE, 00);
			calendar.set(Calendar.SECOND, 00);
			am.setRepeating(AlarmManager.RTC_WAKEUP,calendar.getTimeInMillis(), AlarmManager.INTERVAL_DAY, pendingIntent);
		}else
		{
			am.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis() + timeByMin*60*1000, pendingIntent);
		}
	}
		
	public void closeAlarmNotification(int id){
		String idString = String.valueOf(id);
		PendingIntent pendingIntent = pendingMap.get(idString);
		if (pendingIntent != null){
			AlarmManager am = (AlarmManager)mainActivity.getSystemService(Context.ALARM_SERVICE);
			am.cancel(pendingIntent);
			pendingIntent = null;
			pendingMap.put(idString, null);
		}
	}
}
