package com.snowcat.casino.weixin;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.snowcat.casino.AppSDKManager;
import com.snowcat.casino.config.GameConfig;
import com.snowcat.hnmj.R;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.zerogame.util.JsonUtil;
import java.io.ByteArrayOutputStream;
import java.util.UUID;

import android.graphics.Bitmap.CompressFormat;
import android.graphics.Canvas;
import android.graphics.Rect;


/**
 * Created by kevin on 2016/12/27.
 */

public class WeiXinAdapter {
    private IWXAPI api;
    private Activity mActivity;

    public String mUUID;

    public WeiXinAdapter(Activity activity){
        mActivity = activity;
        api = WXAPIFactory.createWXAPI(mActivity, GameConfig.WEIXIN_APP_ID,true);
        api.registerApp(GameConfig.WEIXIN_APP_ID);
    }


    public void share(String text){

        String type = JsonUtil.getValueByKey(text,"type");
        if (type.equals("text")){
            String content = JsonUtil.getValueByKey(text,"text");

            WXTextObject textObject = new WXTextObject();
            textObject.text = content;

            WXMediaMessage msg = new WXMediaMessage();
            msg.mediaObject = textObject;
            msg.description = content;

            SendMessageToWX.Req req = new SendMessageToWX.Req();
            req.transaction = String.valueOf(System.currentTimeMillis());
            req.message = msg;

            api.sendReq(req);
        }else{
            String title = JsonUtil.getValueByKey(text,"title");
            String description = JsonUtil.getValueByKey(text,"description");
            String url = JsonUtil.getValueByKey(text,"webpageUrl");
            String scene = JsonUtil.getValueByKey(text,"scene");
            shareLink(url,title,description,scene);
        }
    }

    public void shareLink(String url, String title, String description, String scene){
        WXWebpageObject webpage = new WXWebpageObject();
        webpage.webpageUrl = url;

        WXMediaMessage msg = new WXMediaMessage(webpage);
        msg.title = title;
        msg.description = description;
        Bitmap thumb = BitmapFactory.decodeResource(mActivity.getResources(), R.drawable.small_icon);
        Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, 100, 100, true);
        thumb.recycle();
        msg.thumbData = bmpToByteArray(thumbBmp, true);


        SendMessageToWX.Req req = new SendMessageToWX.Req();
        req.transaction = buildTransaction("webpage");
        req.message = msg;

        if(scene.compareTo("timeline") == 0){
            req.scene = SendMessageToWX.Req.WXSceneTimeline;
        }else{
            req.scene = SendMessageToWX.Req.WXSceneSession;
        }
        api.sendReq(req);
    }

    public void login(){
        if (AppSDKManager.getInstance().getIsLogining()){
            return;
        }
        mUUID = UUID.randomUUID().toString();
        AppSDKManager.getInstance().setWXState(mUUID);
        final SendAuth.Req req = new SendAuth.Req();
        req.scope = "snsapi_userinfo";
        req.state = mUUID;
        api.sendReq(req);
        AppSDKManager.getInstance().setIsLogining(true);
    }

    private String buildTransaction(final String type) {
        return (type == null) ? String.valueOf(System.currentTimeMillis())
                :type + System.currentTimeMillis();
    }

    public static byte[] bmpToByteArray(final Bitmap bmp, final boolean needRecycle) {
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        bmp.compress(CompressFormat.PNG, 100, output);
        if (needRecycle) {
            bmp.recycle();
        }

        byte[] result = output.toByteArray();
        try {
            output.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return result;
    }

    public boolean isWXAppInstalled(){
        return api.isWXAppInstalled();
//        return true;
    }
}








