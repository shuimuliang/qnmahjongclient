package com.zerogame.cocosEasy;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;  
import java.io.File; 
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;

public class javaCocosUDID extends Activity{
	
	private static javaCocosUDID instance;

    public static javaCocosUDID getInstance() {
        return instance;
    }

    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		instance = this;
	}
	
	public synchronized static String getUDID() {
		if (instance == null) {
			instance = new javaCocosUDID();
		}
		return instance.getDeviceUniqueID();
	}
	
	private synchronized String getDeviceUniqueID() {
		//imei
		TelephonyManager TelephonyMgr = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE); 
		String m_szImei = TelephonyMgr.getDeviceId(); 
		//
		String m_szDevIDShort = "35" + //we make this look like a valid IMEI 
		Build.BOARD.length()%10 + 
		Build.BRAND.length()%10 + 
		Build.CPU_ABI.length()%10 + 
		Build.DEVICE.length()%10 + 
		Build.DISPLAY.length()%10 + 
		Build.HOST.length()%10 + 
		Build.ID.length()%10 + 
		Build.MANUFACTURER.length()%10 + 
		Build.MODEL.length()%10 + 
		Build.PRODUCT.length()%10 + 
		Build.TAGS.length()%10 + 
		Build.TYPE.length()%10 + 
		Build.USER.length()%10 ; //13 digits
		
		//android id
		String m_szAndroidID = Secure.getString(getContentResolver(), Secure.ANDROID_ID);

		//wifi address
		WifiManager wm = (WifiManager)getSystemService(Context.WIFI_SERVICE); 
		String m_szWLANMAC = wm.getConnectionInfo().getMacAddress();
		
		//blue tooth
		BluetoothAdapter m_BluetoothAdapter = null; // Local Bluetooth adapter      
		m_BluetoothAdapter = BluetoothAdapter.getDefaultAdapter();      
		String m_szBTMAC = m_BluetoothAdapter.getAddress();
		
		
		String m_szLongID = m_szImei + m_szDevIDShort 
			    + m_szAndroidID+ m_szWLANMAC + m_szBTMAC;      
			// compute md5     
			 MessageDigest m = null;   
			try {
			 m = MessageDigest.getInstance("MD5");
			 } catch (NoSuchAlgorithmException e) {
			 e.printStackTrace();   
			}    
			m.update(m_szLongID.getBytes(),0,m_szLongID.length());   
			// get md5 bytes   
			byte p_md5Data[] = m.digest();   
			// create a hex string   
			String m_szUniqueID = new String();   
			for (int i=0;i<p_md5Data.length;i++) {   
			     int b =  (0xFF & p_md5Data[i]);    
			// if it is a single digit, make sure it have 0 in front (proper padding)    
			    if (b <= 0xF) 
			        m_szUniqueID+="0";    
			// add number to string    
			    m_szUniqueID+=Integer.toHexString(b); 
			   }   // hex string to uppercase   
			m_szUniqueID = m_szUniqueID.toUpperCase();
			
			return m_szUniqueID;
		
	}
	
}
