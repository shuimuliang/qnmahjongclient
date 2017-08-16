--[[
message orderStatus {
  required string orderID = 1;    		 	// ¶©µ¥ID
  optional int32 status = 2;				// ¶©µ¥×´Ì¬ 0:succ 	1:error
  optional string errorDesc = 3;			// ´íÎóÃèÊö
  optional string jsonStr = 4;    			// json
}
--]]

ePaymentType = {
	unknown = 0,
	appstore = 1,
	iappay = 2,
	googleplay = 3,
	paypal = 4,
	paymentWall = 5,
	haima = 6,
	xy = 7,
	mol = 10,
}


local eOrderStatus = {
	finished = "finished",
	pay2Client = "pay2Client",
	pay2Server = "pay2Server",
	disabled = "disabled",
}

local LCOrderData = class("LCOrderData")
function LCOrderData:ctor(orderID,status,jsonStr,payType,createTM)
	CC_SYNTHESIZE(self,"orderID",orderID or "nil")
	CC_SYNTHESIZE(self,"status",status or eOrderStatus.disabled)
	CC_SYNTHESIZE(self,"jsonStr",jsonStr or "nil")
	CC_SYNTHESIZE(self,"payType",payType or 0)

	CC_SYNTHESIZE(self,"createTM",createTM or 0 )

	GDebug("create a new order data %s | %s | %s | %s",self.orderID,self.status,self.jsonStr,self.payType)
end

function LCOrderData:genUserContent()
	local t = {
		orderID = self.orderID,
		status = self.status,
		jsonStr = self.jsonStr,
		payType = self.payType,
		createTM = self.createTM,
	}
	return t
end

function LCOrderData:isFinished(isFirstLogin)
	if self.status == eOrderStatus.finished then
		return true
	end
	local curTM = LCNetManager:getServerTime() or 0
	if not isFirstLogin then
		if  (curTM - self.createTM) > (10*60) then
			return true
		end
	else
		if  (curTM - self.createTM) > (3*24*60*60) then
			return true
		end
	end
	return false
end

local LCPaymentMgr = class("LCPaymentMgr")

function LCPaymentMgr:start()
	-- action
	CC_SYNTHESIZE(self,"curRetryTM",1)
	CC_SYNTHESIZE(self,"maxRetryTM",60)

	CC_SYNTHESIZE_RO(self,"applyClockAction",nil)

	CC_SYNTHESIZE_RO(self,"isPreLoadAppStore",false)

	CC_SYNTHESIZE(self, "scheduler", CCDirector:sharedDirector():getScheduler())
	CC_SYNTHESIZE(self, "actTimer",nil)
	CC_SYNTHESIZE(self, "timerInterval",0)

	CC_SYNTHESIZE(self,"orderRecordMap",{})		--<uuid, < orderID, LCOrderData > >
	CC_SYNTHESIZE(self, "isFirstLogin",true)

	self:preLoadProductID()

	self:loadLocalOrderRecorder()
end

-- ======================== TIMER FUNCTION ======================== --
function LCPaymentMgr:continueTimer()
	self:resetTimer()
	self.actTimer = self.scheduler:scheduleScriptFunc(function ()
		self:timerRun()
	end ,self.timerInterval,false)
end

function LCPaymentMgr:timerRun()
	local orderList = self:getUnfinishOrderDataList()
	if #orderList > 0 then
		GError("os . time %s",os.time())

		scheduleOnce( LCSceneTransit:getRunningScene(),function( ... )
			self:orderToServer(orderList)
		end, 2)

		self.timerInterval = math.min(self.timerInterval + 5, 60 * 3 )
	end
end

function LCPaymentMgr:resetTimer()
	if self.actTimer then
		self.scheduler:unscheduleScriptEntry(self.actTimer)
		self.actTimer = nil
	end
end

function LCPaymentMgr:startTimer()
	self:resetTimer()
	self.timerInterval = 0
	self:timerRun()
end


-- ======================== LOCAL RECORDER FUNCTION ======================== --
function LCPaymentMgr:loadLocalOrderRecorder()
	local recorder = LCUserContext:getOrderRecorder()
	for uuid,orderTable in pairs(recorder) do
		self.orderRecordMap[uuid] = {}
		local tmpTable = self.orderRecordMap[uuid]
		for orderID,orderDataTable in pairs(orderTable) do
			local status = orderDataTable.status
			local jsonStr = orderDataTable.jsonStr
			local payType = orderDataTable.payType
			local createTM = orderDataTable.createTM or 0
			local orderData = LCOrderData.new(orderID,status,jsonStr,payType,createTM)
			tmpTable[orderID] = orderData
		end
	end
end

function LCPaymentMgr:saveLocalOrderRecorder()
	local recorder = LCUserContext:getOrderRecorder()
	for uuid,orderTable in pairs(self.orderRecordMap) do
		for orderID,orderData in pairs(orderTable) do
			recorder[uuid] = recorder[uuid] or {}
			recorder[uuid][orderID] = orderData:genUserContent()
		end
	end
	LCUserContext:saveOrderRecorder(recorder)
end
-- ======================== PAYMENT PASS ON FUNCTION ======================== --
function LCPaymentMgr:getUuid(orderID)
	return LCAppGlobal:getDeviceID()
end

-- ======================== PAYMENT PRE LOAD FUNCTION ======================== --
function LCPaymentMgr:preLoadProductID()
	-- check
	if self.isPreLoadAppStore then return end
	self.isPreLoadAppStore = true
	-- load
	local appIDList = {}
	local storeMap = LCDataManager:getShopGemInfoMgr():getInfoList()
	for _,info in pairs(storeMap) do
		local appid = tostring(info.productID)
		if #appid > 0 then
			table.insert(appIDList,appid)
		end
	end
	CSDKManager:getInstance():preLoadGoods(cjson.encode(appIDList))
end

-- ======================== PAYMENT RECORDER MAP FUNCTION ======================== --

function LCPaymentMgr:getOrderByID(orderID)
	if not orderID then return end
	orderID = tostring(orderID)
	local uuid = self:getUuid()
	if not self.orderRecordMap[uuid] then return end
	return self.orderRecordMap[uuid][orderID]
end

function LCPaymentMgr:saveOrderData(orderData)
	if not orderData then return end
	local orderID = orderData.orderID
	local uuid = self:getUuid()
	self.orderRecordMap[uuid] = self.orderRecordMap[uuid] or {}
	self.orderRecordMap[uuid][orderID] = orderData
	self:saveLocalOrderRecorder()
end

function LCPaymentMgr:getUnfinishOrderDataList()
	local list = {}
	local uuid = self:getUuid()
	local orderDataMap = self.orderRecordMap[uuid] or {}
	for _,data in pairs(orderDataMap) do
		if not data:isFinished(self.isFirstLogin) then
			local order = {
				payType = tonumber(data.payType),
				jsonStr = data.jsonStr,
			}
			if order.payType then table.insert(list,order) end
		end
	end
	self.isFirstLogin = false
	return list
end

function LCPaymentMgr:showAd(adType)
	CSDKManager:getInstance():registerVideoScriptHandler(function (status)

		local text = _("暂时没有可领取的免费筹码!")
		if status == eADPlayStatus.succ then
			text = _("您已完成任务，请耐心等待筹码到账")

		elseif status == eADPlayStatus.failed then

		elseif status == eADPlayStatus.half then
			text = _("您取消了视频奖励")
		end

		local curScene = LCSceneTransit:getRunningScene()
		if curScene and status ~= eADPlayStatus.succ and curScene.showConfirmMessage then
			scheduleOnce( LCSceneTransit:getRunningScene(),function( ... )
				curScene:showConfirmMessage(text)
			end, 0.5)
		end
	end)
	local data = cjson.encode({
		playerID = LCAppGlobal:getLoginPlayer():getId(),
	})
	CSDKManager:getInstance():playVideo(data)
end



-- ============== DEAL LOCAL ORDER ============== --
function LCPaymentMgr:checkMissOrder()
	local jsonOrderString = CSDKManager:getInstance():getMissOrderList()
	GDebug("checkMissOrder get miss list : %s",jsonOrderString)
	local channel = CSDKManager:getInstance():getChannelID()
	if jsonOrderString and  #jsonOrderString > 0 then
		local orderMapList = cjson.decode(jsonOrderString)
		for i=1,#orderMapList do
			local orderID = orderMapList[i]
			local jsonStr = CSDKManager:getInstance():getOrderInfoAndRemove(orderID)
			local orderData = nil
			if channel == SDK_CHANNEL_GOOGLE_PLAY then
				orderData = LCOrderData.new(orderID,eOrderStatus.pay2Client,jsonStr,ePaymentType.googleplay,LCNetManager:getServerTime())
			else
				orderData = LCOrderData.new(orderID,eOrderStatus.pay2Client,jsonStr,ePaymentType.appstore,LCNetManager:getServerTime())
			end
			self:saveOrderData(orderData)
		end
	end
	self:startTimer()
end

function LCPaymentMgr:getCurrencyType()
	return "CNY"
end

-- ============== PAY PROCESS ============== --
function LCPaymentMgr:pay(gemData, orderCallBack)
	if not gemData then
		GError("invalid arguments")
		return
	else
		GDebug("start pay (id %s) to clent SDK ... ",gemData.gemID)
	end
	self:payToChannel(gemData , orderCallBack)
end

function LCPaymentMgr:payToChannel(gemData, orderCallBack)
	GDebug("pay to channel ... begin")
	local gemInfo = gemData:getInfo()
	local payType = gemData.payType

	-- set callback
	CSDKManager:getInstance():registerBuyScriptHandler(function (result, jsonStr)

		if orderCallBack then orderCallBack(result) end

		if not result then
					self:startTimer()

			GDebug("pay to channel ... failed")
			return
		else
			GDebug("pay to channel ... success")
		end
		if #jsonStr == 0 then
			GDebug("pay to channel ... failed : json string len 0")
			return
		end

		local data = cjson.decode(jsonStr)
		local orderID = data["transaction_id"]

		if self:getOrderByID(orderID) then
			GDebug("order(%s) data is exist",orderID or "nil")
			return
		end

		local orderData = LCOrderData.new(orderID,eOrderStatus.pay2Client,jsonStr,payType,LCNetManager:getServerTime())
		self:saveOrderData(orderData)
		GDebug("pay to channel ... end")
		-- show
		local curScene = LCSceneTransit:getRunningScene()
		if curScene.showLoading then
			curScene:showLoading(_ID2WORD(11006))
		end
		self:startTimer()
	end)

	-- open SDK
	local data = cjson.encode({
		ID = gemData.gemID,
		name =  gemData:getName(),
		price = gemData.money,
		desc = gemData:getDesc(),
		appStoreProductId =  gemData.productID,
		playerID = LCAppGlobal:getLoginPlayer():getId(),
		userID = LCAppGlobal:getLoginPlayer():getId(),
		channel = CSDKManager:getInstance():getChannelID(),
		currencyType = self:getCurrencyType(),
		payType = payType,

		accountID = CSDKManager:getInstance():getAccountID(),
		accoutName = CSDKManager:getInstance():getAccountName(),
		gameName = DEF_GAME_NAME,

	})


	if payType == ePaymentType.appstore then
		CSDKManager:getInstance():pay(data)
	elseif payType == ePaymentType.iappay then
		CSDKManager:getInstance():showWebPayView(DEF_IAPPAY_PATH,data)
	elseif payType == ePaymentType.paypal then
		CSDKManager:getInstance():payWithPayPal(data)
	else
		CSDKManager:getInstance():pay(data)
		--GError("payType error (%s)",payType and payType or "nil")
	end
end


function LCPaymentMgr:orderToServer(orderList)
	GDebug("pay to server ... begin")
	local pb = LCNetManager:createMessage(eDefMsg.OrderApplyNew)
	pb._msg.orderList = orderList

	if #pb._msg.orderList > 0 then
		LCNetManager:sendToGameServer(pb)
	end
end

function LCPaymentMgr:payFinished(result,errorDesc,orderData)
	GDebug("pay finished result %s, %s",result and "true" or "false",errorDesc or "")
	if result then
		orderData.status = eOrderStatus.finished
	end
	self:saveOrderData(orderData)
	return
end



function LCPaymentMgr:serverCallback(msg)

	local isSuccess = false

	local function dealOrder(orderStatus)
		GDebug("orderStatus orderID %s",orderStatus.orderID)
		GDebug("orderStatus status %s",orderStatus.status)
		GDebug("orderStatus errorDesc %s",orderStatus.errorDesc)
		GDebug("orderStatus jsonStr %s",orderStatus.jsonStr)
		local orderID = orderStatus.orderID
		local gemID = orderStatus.gemID

		local shopMgr = LCAppGlobal:getGameData():getShopMgr()
		local gemData,gemInfo
		if gemID and gemID > 0 then
			gemData = shopMgr:getGemDataByID(gemID )
			if not gemData then  -- gem data have not get from server
				local tempInfo = LCDataManager:getShopGemInfoMgr():getInfo(gemID)
				local key = {
					gemID = gemID,
					payType = tempInfo.payType,
					gemCount = tempInfo.count,
					money = tempInfo.price/100,
					productID = tempInfo.productID,
					sort = 1,
				}
				gemData = LCGemData:createByPB(key)
			end
			gemInfo = gemData and gemData:getInfo() or LCDataManager:getShopGemInfoMgr():getInfo(gemID)
		end
		if (orderStatus.status == 0 and orderID) then
			-- Analysis
			-- if gemData then
			-- 	local gemInfo = gemData:getInfo()
			-- 	local iapId = gemInfo:getProductID()
			-- 	local currencyAmount = gemData:getMoney()
			-- 	local currencyType = self:getCurrencyType()
			-- 	local virtualCurrencyAmount = gemData:getGemCount()
			-- 	local paymentType = gemData:getPayType()


			-- 	GDebug(" talking data data:::: %s  %s  %s   %s  %s  %s ",orderID,iapId,currencyAmount,currencyType,virtualCurrencyAmount,paymentType)

			-- 	LCAnalysisMgr:logChargeReq(orderID,
			-- 							   iapId,
			-- 							   currencyAmount,
			-- 							   currencyType,
			-- 							   virtualCurrencyAmount,
			-- 							   paymentType)
			-- end
			-- LCAnalysisMgr:logChargeSucc(orderID)

			local shopMgr = LCAppGlobal:getGameData():getShopMgr()
			shopMgr:setCharged()
		end

		-- isSuccess
		if not isSuccess then
			isSuccess = (orderStatus.status == 0)
		end

		-- check order invalid
		local orderData = self:getOrderByID(orderID)
		if not orderData then
			if gemData then
				orderData = LCOrderData.new(orderID,eOrderStatus.finished,orderStatus.jsonStr,gemData.payType,LCNetManager:getServerTime())
			else
				if gemInfo then
					orderData = LCOrderData.new(orderID,eOrderStatus.finished,orderStatus.jsonStr,gemInfo.payType,LCNetManager:getServerTime())
				else
					orderData = LCOrderData.new(orderID,eOrderStatus.finished,orderStatus.jsonStr,ePaymentType.unknown,LCNetManager:getServerTime())
				end
			end
		end
		self:payFinished((orderStatus.status == 0),orderStatus.errorDesc,orderData)
	end

	for _,orderStatus in pairs(msg.orderList) do
		dealOrder(orderStatus)
	end

	self:continueTimer()

	local curScene = LCSceneTransit:getRunningScene()
	if curScene.hideLoading then
		curScene:hideLoading()
	end
	if curScene and isSuccess then
		if curScene.showConfirmMessage then
			curScene:showConfirmMessage(_("充值成功!"))
		end
	end
end


return LCPaymentMgr
