package com.zerogame.pluginlibs;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.util.Log;

public class CJThread {
	protected static GLSurfaceView m_sGLSurfaceView = null;
	protected static Handler m_sMainThreadHandler = null;

	public static void runOnGLThread(Runnable r) {
		if (null == m_sGLSurfaceView) {
			Log.e("runOnGLThread", "runOnGLThread sGLSurfaceView is null");
			return;
		}
		m_sGLSurfaceView.queueEvent(r);
	}

	public static void runOnMainThread(Runnable r) {
		if (null == m_sMainThreadHandler) {
			Log.e("runOnGLThread", "sMainThreadHandler is null");
			return;
		}
		m_sMainThreadHandler.post(r);
	}

	public static void init() {
		m_sGLSurfaceView = Cocos2dxGLSurfaceView.getInstance();
		if (null == m_sMainThreadHandler) {
			m_sMainThreadHandler = new Handler();
		}
		return;
	}
}