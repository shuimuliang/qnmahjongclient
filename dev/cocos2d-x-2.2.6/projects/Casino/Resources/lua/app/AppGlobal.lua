--[[
抽象APP类
--]]

LCAppGlobal = {}

function LCAppGlobal.start()
	math.randomseed(os.time())
	-- init saving System
	LCUserContext:start()

	-- init log file
	initLogFile()

	-- set game font
	cpf_setDefaultFont(DEF_SYSTEM_FONT_NAME,DEF_SYSTEM_FONT_FILE)

	LCDataManager:start()
	LCSceneTransit:start()
	LCNetManager:start()

	CC_SYNTHESIZE(LCAppGlobal, "deviceID",CCEPlugin:getDeviceID())
	CC_SYNTHESIZE(LCAppGlobal, "loginPlayer",LCPlayerData.new())
	CC_SYNTHESIZE(LCAppGlobal, "gameData", LCGameData.new())
	CC_SYNTHESIZE(LCAppGlobal, "userInfoData", LCUserInfoData.new())
	CC_SYNTHESIZE(LCAppGlobal, "token", "")
	CC_SYNTHESIZE(LCAppGlobal, "reconnect", 0)
	CC_SYNTHESIZE(LCAppGlobal, "highID", 0)
	CC_SYNTHESIZE(LCAppGlobal, "CSWChatID", "")  -- 客服微信号
	CC_SYNTHESIZE(LCAppGlobal, "needReconnect",true) -- auth error

	LCMusicBox:start()
	LCEventDispatcher:start()
	local LCPaymentMgr  = require("lua/model/Payment")
	LCPaymentMgr:start()

	local override = require "lua/model/ScriptOverride"
	override.start()

	if DEBUG_DISPLAY_STATUS then CCDirector:sharedDirector():setDisplayStats(true) end
	LCSceneTransit:replaceScene(eSceneName.login,nil,true)
	-- LCSceneTransit:replaceScene(eSceneName.test,nil,true)
end

function LCAppGlobal.isLogined()
	return LCAppGlobal.token and #tostring(LCAppGlobal.token) > 0
end

function LCAppGlobal:playerLogout()
	self.gameData = LCGameData.new()
	self.loginPlayer = LCPlayerData.new()
	self.userInfoData = LCUserInfoData.new()
	self.token = ""
	CAnalysis:getInstance():setPlayerID("")
	LCUserContext:saveLogin({playerID = 0})
end
