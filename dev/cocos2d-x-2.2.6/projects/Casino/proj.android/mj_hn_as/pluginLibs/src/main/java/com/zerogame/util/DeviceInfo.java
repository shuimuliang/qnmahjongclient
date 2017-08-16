package com.zerogame.util;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.text.DecimalFormat;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Activity;
import android.view.Display;
import android.view.WindowManager;

public class DeviceInfo {

	private Activity mActivity;
	private boolean mIsLow = false;
	
	public static final int MIN_WINDOW_HEIGHT = 640;
	public static final int MIN_WINDOW_WIDTH = 800;
	public static final int MIN_RAM_G = 1;
	
	
	public DeviceInfo(Activity activity) {
		mActivity = activity;
		judgeLow();
	}
	
	public boolean judgeLow(){
		Display display = getDisplay();
		boolean isLowOnDisplay = display.getHeight() < MIN_WINDOW_HEIGHT || display.getWidth() < MIN_WINDOW_WIDTH;
		boolean isLowOnRam = getTotalRAMDouble() < MIN_RAM_G;
		
        System.out.println("hk ----------  getHeight  " + display.getHeight());
        System.out.println("hk ----------  getWidth  " + display.getWidth());

		return (isLowOnDisplay || isLowOnRam);
	}
	
	public boolean isLowDevice(){
		return mIsLow;
	}
	
	public Display getDisplay(){
		WindowManager windowManager = mActivity.getWindowManager();
		Display display = windowManager.getDefaultDisplay();
		return display;
	}
	
	public double getTotalRAMDouble(){
        RandomAccessFile reader = null;
        String load = null;
        DecimalFormat twoDecimalForm = new DecimalFormat("#.##");
        double totRam = 0;
        double gb = 0;
        try {
            reader = new RandomAccessFile("/proc/meminfo", "r");
            load = reader.readLine();

            // Get the Number value from the string
            Pattern p = Pattern.compile("(\\d+)");
            Matcher m = p.matcher(load);
            String value = "";
            while (m.find()) {
                value = m.group(1);
                // System.out.println("Ram : " + value);
            }
            reader.close();

            totRam = Double.parseDouble(value);
            // totRam = totRam / 1024;

            gb = totRam / 1048576.0;

        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            // Streams.close(reader);
        }
        
        System.out.println("hk ----------  gb  " + gb);
        return gb;
    }
	
    public String getTotalRAMString() {
        RandomAccessFile reader = null;
        String load = null;
        DecimalFormat twoDecimalForm = new DecimalFormat("#.##");
        double totRam = 0;
        String lastValue = "";
        try {
            reader = new RandomAccessFile("/proc/meminfo", "r");
            load = reader.readLine();

            // Get the Number value from the string
            Pattern p = Pattern.compile("(\\d+)");
            Matcher m = p.matcher(load);
            String value = "";
            while (m.find()) {
                value = m.group(1);
                // System.out.println("Ram : " + value);
            }
            reader.close();

            totRam = Double.parseDouble(value);
            // totRam = totRam / 1024;

            double mb = totRam / 1024.0;
            double gb = totRam / 1048576.0;
            double tb = totRam / 1073741824.0;

            if (tb > 1) {
                lastValue = twoDecimalForm.format(tb).concat(" TB");
            } else if (gb > 1) {
                lastValue = twoDecimalForm.format(gb).concat(" GB");
            } else if (mb > 1) {
                lastValue = twoDecimalForm.format(mb).concat(" MB");
            } else {
                lastValue = twoDecimalForm.format(totRam).concat(" KB");
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            // Streams.close(reader);
        }
        return lastValue;
    }
}
