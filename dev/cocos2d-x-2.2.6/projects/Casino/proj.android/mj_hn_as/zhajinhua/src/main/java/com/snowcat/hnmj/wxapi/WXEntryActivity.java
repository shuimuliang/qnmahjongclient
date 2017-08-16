package com.snowcat.hnmj.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

import com.snowcat.casino.AppSDKManager;
import com.snowcat.casino.config.GameConfig;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.zerogame.pluginlibs.CJThread;

import org.json.JSONException;
import org.json.JSONObject;


/**
 * Created by kevin on 2017/1/17.
 */

public class WXEntryActivity extends Activity  implements IWXAPIEventHandler{
    private IWXAPI api;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //注册API
        api = WXAPIFactory.createWXAPI(this,  GameConfig.WEIXIN_APP_ID);
        api.handleIntent(getIntent(), this);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        api.handleIntent(intent, this);
    }

    @Override
    public void onReq(BaseReq baseReq) {

    }

    @Override
    public void onResp(BaseResp baseResp) {
        if(baseResp instanceof SendAuth.Resp){
            AppSDKManager.getInstance().setIsLogining(false);
            String state = AppSDKManager.getInstance().getWXState();
            SendAuth.Resp authResp = (SendAuth.Resp)baseResp;
            //获取微信传回的code
            if (authResp.errCode == 0 && state.endsWith(authResp.state)){
                final JSONObject jsonObject = new JSONObject();
                try {
                    jsonObject.put("tokenString",authResp.code);
                } catch (JSONException e) {
                    e.printStackTrace();
                }

                CJThread.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        AppSDKManager.getInstance().thirdLoginResult(0,jsonObject.toString());
                    }
                });
            }else{
//                CJThread.runOnGLThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        AppSDKManager.getInstance().thirdLoginResult(1,"");
//                    }
//                });
            }
        }else if(baseResp instanceof SendMessageToWX.Resp){
            if (baseResp.errCode == 0){
                CJThread.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        AppSDKManager.getInstance().shareResult(true);
                    }
                });
            }
        }

        finish();
    }
}
