package com.zerogame.util;

import java.io.IOException;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.net.DatagramPacket;

public class CJSocketClient {
	private static CJSocketClient client;
	private static DatagramSocket udpClientSocket;
	
	public static final String ANALYSIS_SERVER_IP = "thlog.dev.chaoshenxy.cn";
	public static final int ANALYSIS_SERVER_PORT = 7777;
	
	DatagramPacket udpSendPacket;
	InetAddress hostAddress;
	
	private CJSocketClient(){
		try {
			udpClientSocket = new DatagramSocket();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static CJSocketClient getInstance(){
		if (client == null) {
			client = new CJSocketClient();
		}
		return client;
	}
	
	public void sendMsg(String msg){
		if (udpClientSocket != null) {
			try {
				hostAddress = InetAddress.getByName(ANALYSIS_SERVER_IP);
				byte buf[] = msg.getBytes();
				udpSendPacket = new DatagramPacket(buf, buf.length, hostAddress,
						ANALYSIS_SERVER_PORT);
				udpClientSocket.send(udpSendPacket);
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
