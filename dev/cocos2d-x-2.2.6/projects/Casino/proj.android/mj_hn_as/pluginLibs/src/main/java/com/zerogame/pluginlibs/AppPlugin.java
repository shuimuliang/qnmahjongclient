package com.zerogame.pluginlibs;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.R.bool;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.KeyguardManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Entity;
import android.content.Intent;
import android.location.Location;
import android.location.LocationManager;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.ClipboardManager;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.os.Environment;
import android.os.Environment.*;

import com.zerogame.pluginlibs.notification.AlarmNotification;

import static android.os.Environment.DIRECTORY_DCIM;

public class AppPlugin extends CJThread {

	private static AppPlugin instance;
	private static Activity mainActivity;
	protected Handler sMainThreadHandler = null;
	private String udidStr = "";
	public static ImageMgr imageMgr;
	public LocationManager locationManager;
	public LocationMgr locationMgr;

	public native void callMsgHandler(int handler, int flag);

	public native void callMsgHandler2(int index);

	static public native void onNotifySelectImage(String path);

	public static AppPlugin getInstance() {
		if (instance == null) {
			instance = new AppPlugin();
		}
		return instance;
	}

	public void configWithAcitivity(Activity activity) {
		mainActivity = activity;
		mainActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
		udidStr = getDeviceUniqueID();
		AlarmNotification.Ince().setActivity(activity);
		imageMgr = ImageMgr.getInstance();
		imageMgr.configWithAcitivity(mainActivity);
		locationMgr = new LocationMgr(activity);
	}

	// ///////////////////////////////////////////////////////////////////////

	public static void openURL(String url) {
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setData(Uri.parse(url));
		mainActivity.startActivity(i);
	}

	// ///////////////////////////////////////////////////////////////////////
	public static void setClipboardTxt(final String pTtxt) {
		mainActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {

				ClipboardManager cmb = (ClipboardManager) mainActivity
						.getSystemService(Context.CLIPBOARD_SERVICE);
				cmb.setText(pTtxt.trim());
			}
		});

	}

	// ///////////////////////////////////////////////////////////////////////
	public static void messageBox(final String title, final String msg,
			final String button1Title, final String button2Title,
			final int handler) {

		runOnMainThread(new Runnable() {
			public void run() {
				AlertDialog.Builder mBuilder = new AlertDialog.Builder(
						mainActivity);
				mBuilder.setTitle(title).setMessage(msg);
				if ((button1Title != null)
						&& (!button1Title.equalsIgnoreCase(""))) {
					mBuilder.setPositiveButton(button1Title,
							new OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									if (handler != 0) {
										runOnGLThread(new Runnable() {
											public void run() {
												AppPlugin mAppPlugin = AppPlugin
														.getInstance();
												mAppPlugin.callMsgHandler(
														handler, 1);

											}
										});
									}
								}
							});
				}
				if ((button2Title != null)
						&& (!button2Title.equalsIgnoreCase(""))) {
					mBuilder.setNegativeButton(button2Title,
							new OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									if (handler != 0) {
										runOnGLThread(new Runnable() {
											public void run() {

												AppPlugin mAppPlugin = AppPlugin
														.getInstance();
												mAppPlugin.callMsgHandler(
														handler, 2);
											}
										});
									}
								}
							});
				}
				AlertDialog dialog = mBuilder.create();
				dialog.setCancelable(false);
				dialog.show();
			}
		});

	}

	// ///////////////////////////////////////////////////////////////////////
	public static void messageBox2(final String title, final String msg,
			final String button1Title, final String button2Title) {

		runOnMainThread(new Runnable() {
			public void run() {
				AlertDialog.Builder mBuilder = new AlertDialog.Builder(
						mainActivity);
				mBuilder.setTitle(title).setMessage(msg);
				if ((button1Title != null)
						&& (!button1Title.equalsIgnoreCase(""))) {
					mBuilder.setPositiveButton(button1Title,
							new OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									runOnGLThread(new Runnable() {
										public void run() {
											AppPlugin mAppPlugin = AppPlugin
													.getInstance();
											mAppPlugin.callMsgHandler2(1);

										}
									});
								}
							});
				}
				if ((button2Title != null)
						&& (!button2Title.equalsIgnoreCase(""))) {
					mBuilder.setNegativeButton(button2Title,
							new OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									runOnGLThread(new Runnable() {
										public void run() {
											AppPlugin mAppPlugin = AppPlugin
													.getInstance();
											mAppPlugin.callMsgHandler2(2);
										}
									});
								}
							});
				}
				AlertDialog dialog = mBuilder.create();
				dialog.setCancelable(false);
				dialog.show();
			}
		});

	}

	// ///////////
	public void selectImageResult(int requestCode, int resultCode, Intent data) {
		if (imageMgr != null) {
			imageMgr.selectImageResult(requestCode, resultCode, data);
		}
	}

	// ///////////////////////////////////////////////////////////////////////
	public String getUDID() {
		return udidStr;
	}

	public String getDeviceModel() {
		String deviceModel = "";
		deviceModel = android.os.Build.MODEL;
		return deviceModel;
	}

	public int getOSVersion() {
		int osVersionInt = 0;
		String osVersionString;
		osVersionString = android.os.Build.VERSION.RELEASE.replace(".", "");
		osVersionInt = Integer.parseInt(osVersionString);
		return osVersionInt;
	}

	public int getIsRoot() {
		if (isRoot()) {
			return 1;
		}
		return 0;
	}

	/** 判断手机是否root，不弹出root请求框<br/> */
	public static boolean isRoot() {
		String binPath = "/system/bin/su";
		String xBinPath = "/system/xbin/su";
		if (new File(binPath).exists() && isExecutable(binPath))
			return true;
		if (new File(xBinPath).exists() && isExecutable(xBinPath))
			return true;
		return false;
	}

	private static boolean isExecutable(String filePath) {
		Process p = null;
		try {
			p = Runtime.getRuntime().exec("ls -l " + filePath);
			// 获取返回内容
			BufferedReader in = new BufferedReader(new InputStreamReader(
					p.getInputStream()));
			String str = in.readLine();
			if (str != null && str.length() >= 4) {
				char flag = str.charAt(3);
				if (flag == 's' || flag == 'x')
					return true;
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (p != null) {
				p.destroy();
			}
		}
		return false;
	}

	public String getDeviceUniqueID() {

		// imei
		TelephonyManager TelephonyMgr = (TelephonyManager) this.mainActivity
				.getSystemService(Context.TELEPHONY_SERVICE);
		String m_szImei = "";
		try {
			m_szImei = TelephonyMgr.getDeviceId();

		} catch (Exception e) {
			m_szImei = "000000000000000";
			e.printStackTrace();
		}
		//
		String m_szDevIDShort = "35"
				+ // we make this look like a valid IMEI
				Build.BOARD.length() % 10 + Build.BRAND.length() % 10
				+ Build.CPU_ABI.length() % 10 + Build.DEVICE.length() % 10
				+ Build.DISPLAY.length() % 10 + Build.HOST.length() % 10
				+ Build.ID.length() % 10 + Build.MANUFACTURER.length() % 10
				+ Build.MODEL.length() % 10 + Build.PRODUCT.length() % 10
				+ Build.TAGS.length() % 10 + Build.TYPE.length() % 10
				+ Build.USER.length() % 10; // 13 digits

		// android id
		String m_szAndroidID = Secure.getString(
				this.mainActivity.getContentResolver(), Secure.ANDROID_ID);

		// wifi address
		WifiManager wm = (WifiManager) this.mainActivity.getApplicationContext()
				.getSystemService(Context.WIFI_SERVICE);
		String m_szWLANMAC = wm.getConnectionInfo().getMacAddress();

		// blue tooth
		BluetoothAdapter m_BluetoothAdapter = null; // Local Bluetooth adapter
		m_BluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		String m_szBTMAC = "000000000000000";
		try {
			m_szBTMAC = m_BluetoothAdapter.getAddress();

		} catch (Exception e) {
			e.printStackTrace();
		}

		String m_szLongID = m_szImei + m_szDevIDShort + m_szAndroidID
				+ m_szWLANMAC + m_szBTMAC;
		// compute md5
		MessageDigest m = null;
		try {
			m = MessageDigest.getInstance("MD5");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		m.update(m_szLongID.getBytes(), 0, m_szLongID.length());
		// get md5 bytes
		byte p_md5Data[] = m.digest();
		// create a hex string
		String m_szUniqueID = new String();
		for (int i = 0; i < p_md5Data.length; i++) {
			int b = (0xFF & p_md5Data[i]);
			// if it is a single digit, make sure it have 0 in front (proper
			// padding)
			if (b <= 0xF)
				m_szUniqueID += "0";
			// add number to string
			m_szUniqueID += Integer.toHexString(b);
		} // hex string to uppercase
		m_szUniqueID = m_szUniqueID.toUpperCase();

		return m_szUniqueID;

	}

	public static void setAlarmNotification(int type, int repeatDays,
			int timeByMin, String title, String msg, int id) {
		AlarmNotification.Ince().setAlarmNotification(type, repeatDays,
				timeByMin, title, msg, id);
	}

	public static void closeAlarmNotification(int id) {
		AlarmNotification.Ince().closeAlarmNotification(id);
	}
	
	public static void setIdleTimerDisabled(boolean disabled){
		ContentResolver mContentResolver = mainActivity.getContentResolver();
		 android.provider.Settings.System.putInt(mContentResolver,
				 android.provider.Settings.Secure.LOCK_PATTERN_ENABLED, disabled ? 0 : 1);
	}
	
	public static void playVibration(){
		mainActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
//				Vibrator vibrator = (Vibrator)mainActivity.getSystemService(Service.VIBRATOR_SERVICE);
//				vibrator.vibrate(500);
			}
		});
	}

	public static String getMacAddress() {
		WifiManager wifi = (WifiManager) mainActivity.getApplicationContext()
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();

		if (info != null && info.getMacAddress() != null) {
			return info.getMacAddress();
		}

		return "";
	}

	public static void selectImageNotification(int clipWidth) {
		final int clip = clipWidth;
		mainActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				imageMgr.selectImage(clip);
			}
		});
	}

	public String getSDCardPath(){
        String path = Environment.getExternalStoragePublicDirectory(DIRECTORY_DCIM).toString() + "／";
		return path;
	}

	/**
	 * 判断手机GPS是否开启
	 * @param
	 * @return
	 */
	public boolean locationServicesEnabled() {
//		//通过GPS卫星定位,定位级别到街
//		boolean gps = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
//		//通过WLAN或者移动网络确定位置
//		boolean network = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
//
//		if (gps || network) {
//			return true;
//		}
//		return false;
		return true;
	}

	public double getLatitude(){
		return locationMgr.mLatitude;
	}

	public double getLongitude(){
		return  locationMgr.mLongitude;
	}

	/**
	 * 开启手机GPS
	 */
	public static void requestLocationService() {
		CJThread.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				// 转到手机设置界面，用户设置GPS
				Intent intent = new Intent(
						Settings.ACTION_LOCATION_SOURCE_SETTINGS);
				mainActivity.startActivityForResult(intent, 0); // 设置完成后返回到原来的界面
			}
		});
	}
}
