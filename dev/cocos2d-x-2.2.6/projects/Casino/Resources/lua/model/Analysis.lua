local eEvent = {
	newPlayerGuide = "NPG",	-- ID
	story_enter = "STORY_ENTER", -- script name
	story_exit = "STORY_EXIT", -- script name
	battleRound = "BR", -- level_stageID_round(enter)
	login = "LOGIN",
	pveFailedJump = "PVE_FAILED_JUMP",
	pveWinClick = "PVE_WIN_CLICK_PAGE",
}

LCAnalysisMgr = class("LCAnalysisMgr")

function LCAnalysisMgr:isEnabled()
	if DEBUG_DISABLE_ANALYSIS then return false end
	return DEF_TARGET_PLATFORM == DEF_PLATFORM_IOS or DEF_TARGET_PLATFORM == DEF_PLATFORM_ANDROID
end


function LCAnalysisMgr:logRegister(player)
	if not self:isEnabled() then return end
	if not player then return end
	local data = cjson.encode({
		playerID = player.id,
		})
	CAnalysis:getInstance():logRegister(data)
end
function LCAnalysisMgr:logLogin( autoRegister )
	if not self:isEnabled() then return end
	self:logPlayer()
	local player = LCAppGlobal:getLoginPlayer()
	-- 首次注册 上报注册
	if autoRegister and autoRegister == 1 then
		self:logRegister(player)
	end

	--登陆上报登陆
	local data = cjson.encode({
		playerID = player.id,
		})
	CAnalysis:getInstance():logLogin(data)
end

function LCAnalysisMgr:logPlayer()
	if not self:isEnabled() then return end

	local player = LCAppGlobal:getLoginPlayer()
	if not player.id then return end

	TDGAAccount:setAccount(player.id)
	TDGAAccount:setAccountName(player.nickname)
	TDGAAccount:setAccountType(11)
	-- TDGAAccount:setLevel(2)
	TDGAAccount:setGender(player.gender)
end

function LCAnalysisMgr:logChargeReq(orderId,iapID,currencyAmount,currencyType,virtualCurrencyAmount,paymentType)
	if not self:isEnabled() then return end
	local player = LCAppGlobal:getLoginPlayer()

	TDGAVirtualCurrency:onChargeRequest(orderId, iapID, currencyAmount, currencyType, virtualCurrencyAmount, paymentType)
	-- for apps flyer and adtraking
	local data = cjson.encode({
		playerID = player.id,
		orderID =  orderId,
		iapID = iapID,
		currencyType = currencyType,
		paymentType =  paymentType,
		currencyAmount = currencyAmount,
		virtualCurrencyAmount = virtualCurrencyAmount,
	})
	CAnalysis:getInstance():logPurchase(data)
end
function LCAnalysisMgr:logChargeSucc(orderId)
	if not self:isEnabled() then return end
	TDGAVirtualCurrency:onChargeSuccess(orderId)
end

--local eventData = {key1="value1", key2="value2", key3="value3"}
function LCAnalysisMgr:event(eventID, content)
	if not self:isEnabled() then return end
	if not eventID or not content then GError("invalid arguments") return end
	if type(content) ~= "table" then
		content = {
			content = content,
		}
	end
	TalkingDataGA:onEvent(eventID, content)
end

function _ANLS_EVENT(eventID,content)
	--GDebug("TalkingData %s - %s",eventID,content or "")
	local infoMgr = LCDataManager:getTalkingGameInfoMgr()
	if not infoMgr then return end
	local info = infoMgr:getInfo(eventID)
	if not info then return end
	LCAnalysisMgr:event(info.talkName, content)
end
