local Iappay = {}

function Iappay.pay(id, channel, goodsID, orderCallBack)
	GDebug("Iappay begin.")

	CSDKManager:getInstance():registerBuyScriptHandler(function (result, jsonStr)

		if orderCallBack then orderCallBack(result) end

		if not result then
			GDebug("pay to channel ... failed")
			return
		else
			GDebug("pay to channel ... success")
		end
		if #jsonStr == 0 then
			GDebug("pay to channel ... failed : json string len 0")
			return
		end
		
        GDebug("pay to channel ... end")
	end)

    CSDKManager:getInstance():showWebPayView(DEF_IAPPAY_PATH.."?player_id="..id.."&channel="..channel.."&wares_id="..goodsID, cjson.encode{})
end

return Iappay
