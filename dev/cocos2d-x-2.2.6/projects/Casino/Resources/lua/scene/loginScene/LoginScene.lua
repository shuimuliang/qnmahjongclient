local UserLicenseDialog = require "lua/scene/loginScene/UserLicenseDialog"
local VersionCheck      = require "lua/scene/loginScene/VersionCheck"

local LoginScene = class("LoginScene", LCSceneBase)

local _initVersionCheck, _registerWechatCallback, _initProcessTimer

function LoginScene:ctor(key)
	local key = key or {}
	if not LoginScene.super.ctor(self, key) then return end

	_initVersionCheck(self)

	self.disableAutoLogin = key.disableAutoLogin
	self._ipPort          = nil
	self._agree           = true
	self._bar             = nil
end

function LoginScene:onEnter()
	LoginScene.super.onEnter(self)

	LCNetManager:closeGameSocket()
	LCAppGlobal:setToken("")

	local proxy = CCBProxy:create()
 	local layer = CCBuilderReaderLoad(eResCCBI.loginSceneZJH, proxy, self)
    self:addChild(layer)

	self.ccb_wechatBtn:setVisible(true)
	self.ccb_wechatBtn:setEnabled(true)
	self.ccb_quickLoginBtn:setVisible(false)
	self.ccb_quickLoginBtn:setEnabled(false)

	self.ccb_versionLabel:setString(DEF_GAME_VERSION)
	_registerWechatCallback(self)
	_initProcessTimer(self)

	if DEBUG_ON_OFF then
		self._ipPort = require "lua/scene/loginScene/IpPortView".new()
		self:addChild(self._ipPort)
	end

	self:showLoading("检查版本...")
	self._versionCheck:request()
end

function LoginScene:onExit()
	_MUSIC_STOP(true)
	self:cleanGameSocket()
	LoginScene.super.onExit(self)
end

function LoginScene:ccb_onEULA(control, controlEvent)
	UserLicenseDialog.new():showInScene()
end

function LoginScene:ccb_onCheck(control, controlEvent)
	self._agree = not self._agree
	self.ccb_check:setVisible(self._agree)
	self.ccb_wechatBtn:setEnabled(self._agree)
end

function LoginScene:autoLogin()
	local id           = LCUserContext:getLogin().playerID
	local refreshToken = LCUserContext:getLogin().refreshToken
	if id == 0 then return end
	self:sendLogin(nil, id, refreshToken)
end

function LoginScene:enterMainScene()
	LCSceneTransit:replaceScene(eSceneName.main,{firstIn = true})
end

function LoginScene:cleanGameSocket( ... )
	-- is need close or clear event
	local gameSocket = LCNetManager:getGameSocket()
	if gameSocket then
		local tag = self.__cname;
		gameSocket:unregisterEventDisconnect(tag)
		gameSocket:unregisterEventFailedConnect(tag)
		gameSocket:unregisterEventOnConnect(tag)
	end
end

function LoginScene:openGameSocket(ip,port)
	self.reconnectTimes = 0
	self.gameIPIndex = 1
	LCAppGlobal:setNeedReconnect(true)
	self:cleanGameSocket()
	LCNetManager:openGameSocket(tostring(ip),tonumber(port))
	local gameSocket = LCNetManager:getGameSocket()
	if gameSocket then
		gameSocket:registerEventOnConnect(self.__cname, function(  )
			GDebug("connect succ")
			-- if self.reconnecting then
			-- 	self.reconnecting = false
				self:sendDirty()
			-- end
		end)
		gameSocket:registerEventDisconnect(self.__cname, function( )
			self:checkConnect()
			GDebug("dissconnect ")
		end)
		gameSocket:registerEventFailedConnect(self.__cname, function( )
			GDebug("failed connect")
			self:checkConnect()
		end)
	end
end

function LoginScene:sendLogin(sessionID, playerID, refreshToken)
	self:showLoading(_("登录中..."))

	if self._ipPort then
		DEF_LOGIN_SERVER_IP = self._ipPort:getLoginIp()
		GDebug("wz : DEF_LOGIN_SERVER_IP -> " .. DEF_LOGIN_SERVER_IP)
	end

	local pb             = LCNetManager:createMessage(eDefMsg.Login)
	pb._msg.channel      = CSDKManager:getInstance():getChannelID()
 	pb._msg.version      = DEF_INTERNAL_VERSION
 	pb._msg.session      = sessionID
 	pb._msg.loginType    = 1
 	pb._msg.machineID    = LCAppGlobal:getDeviceID()
	pb._msg.playerID     = playerID
	pb._msg.refreshToken = refreshToken

 	local request = LCHttpClient:playerLogin(pb)
 	LCHttpClient:sendPbWithHandler(request,function(seed,msg)
 		self:dealPacket(seed, msg)
    end,function ()
		self:hideLoading()
		_ANLS_EVENT(8004,self.__cname)
		LCMessageBox:showConfirm(self,
		{text = _ID2WORD(12006),
		buttonText = _ID2WORD(11001),})
    end)
end

function LoginScene:sendDirty()
	self:showLoading(_("加载中..."))
	local pb = LCNetManager:createMessage(eDefMsg.Dirty)
	LCNetManager:send(pb)
end

----==================  process data from server    ===================
function LoginScene:loginRev(msg)
	if msg.status == eErrorCode.succ then
		LCAppGlobal:setReconnect(0)
		DEF_GAME_SERVER_IP_LIST = {msg.logicServer}
		self:openGameSocket(msg.logicServer)
	else
		self:hideLoading()
		LCUserContext:saveLogin({playerID = 0})
		LCAppGlobal:playerLogout()
		self:showConfirmMessage(_ID2WORD(msg.status))
	end
end

function LoginScene:dealPacket(seed, msg, uiUpdater)
	if seed == eDefMsg.Login then
		self:loginRev(msg)
	elseif seed == eDefMsg.Dirty then
		self:hideLoading()
		if msg.roomID ~= 0 then
			self:sendEnterRoom(msg.roomID, true)
		else
			self:enterMainScene()
		end
	elseif seed == eDefMsg.EnterRoom then
		if msg.status == eErrorCode.succ then
			self:processEnterRoom(msg)
		else
			self:enterMainScene()
		end
	elseif seed == eDefMsg.ErrorCode then
		self:hideLoading()
	end
	LoginScene.super.dealPacket(self, seed, msg, uiUpdater)
end

function LoginScene:forceUpdate(event)
	self:hideLoading()
	local url         = event.url
	GDebug("url = " .. url)

	LCMessageBox:showConfirm(self, {
		text = "新版本发布，请前往应用商店下载！",
		hideClose = true,
		callback = function()
			CCEPlugin:openURL(url)
			self:forceUpdate(event)
		end
	})
end

function LoginScene:ccb_wechatBtnClicked(sender, eventType)
	if self._ipPort then
		self._ipPort:save()
	end
	if CSDKManager:getInstance():isWXAppInstalled() then
		CSDKManager:getInstance():thirdLogin(1)
	else
		if DEBUG_ON_OFF then
			self:sendLogin("1")
		else
			LCToastHUD.new("请先安装微信"):showInScene()
		end
	end
end

function LoginScene:ccb_quickLoginBtnClicked(sender, eventType)
	self:sendLogin("yangjin")
end

-- private
_registerWechatCallback = function(self)
	CSDKManager:getInstance():registerThirdLoginScriptHandler(function(errorCode, jsonStr)
		if errorCode == 0 then
			local data = cjson.decode(jsonStr)
			local sessionID = data["tokenString"]
			if sessionID then
				if DEF_TARGET_PLATFORM == DEF_PLATFORM_ANDROID then
					--解决android回调纹理加载错误
					scheduleOnce( LCSceneTransit:getRunningScene(),function( ... )
						self:sendLogin(sessionID)
					end, 0.1)
				else
					self:sendLogin(sessionID)
				end

			end
		else
			if DEF_TARGET_PLATFORM == DEF_PLATFORM_ANDROID then
				--解决android回调纹理加载错误
				scheduleOnce( LCSceneTransit:getRunningScene(),function( ... )
					LCSceneTransit:getRunningScene():showConfirmMessage(_ID2WORD(12008))
				end, 0.1)
			else
				LCSceneTransit:getRunningScene():showConfirmMessage(_ID2WORD(12008))
			end
		end
	end)
end

_initVersionCheck = function(self)
	self._versionCheck = VersionCheck.new()
	self._versionCheck:addEventListener(VersionCheck.EVENT_ERROR, function()
		GError("版本检查错误")
		self:hideLoading()
		LCMessageBox:showConfirm(self, {
			text = "版本检查失败，请重试",
			hideClose = true,
			callback = function()
				self:showLoading("检查版本...")
				self._versionCheck:request()
			end
		})
		LCAnalysisMgr:event("检查更新失败", DEF_INTERNAL_VERSION)
	end)
	self._versionCheck:addEventListener(VersionCheck.EVENT_PASS, function()
		GDebug("已经是最新版本")
		self:hideLoading()
		if not self.disableAutoLogin then
			self:autoLogin()
		end
		LCAnalysisMgr:event("检查更新成功", DEF_INTERNAL_VERSION)
	end)
	self._versionCheck:addEventListener(VersionCheck.EVENT_FORCE_UPDATE, handler(self, self.forceUpdate))
	self._versionCheck:addEventListener(VersionCheck.EVENT_HOTFIX_BEGIN, function(event)
		local fileSize = event.fileSize
		self:hideLoading()
		self.ccb_wechatBtn:setVisible(false)
		self.ccb_checkNode:setVisible(false)
		self.ccb_quickLoginBtn:setVisible(false)
		self.ccb_eulaBtn:setVisible(false)
		self.ccb_barBg:setVisible(true)
		self.ccb_statusLabel:setString(string.format("资源更新中：0M/%.2fM", fileSize/1024/1024))
		if self._bar then
			self._bar:setPercentage(0)
		end
	end)
	self._versionCheck:addEventListener(VersionCheck.EVENT_HOTFIX_PERCENT, function(event)
		local percent  = event.percent
		local fileSize = event.fileSize
		if self._bar then
			self._bar:setPercentage(percent)
		end
		local msize = fileSize/1024/1024
		self.ccb_statusLabel:setString(string.format("资源更新中：%.2fM/%.2fM", percent/100*msize, msize))
	end)
	self._versionCheck:addEventListener(VersionCheck.EVENT_MODULE, function(event)
		local module = cjson.decode(event.module)
		print_lua_table(module)
		local switchMgr = LCAppGlobal:getGameData():getSwitchMgr()
		switchMgr.update(module)
		if switchMgr.closeWeixinLogin() then
			self.ccb_wechatBtn:setVisible(false)
			self.ccb_wechatBtn:setEnabled(false)
			self.ccb_quickLoginBtn:setVisible(true)
			self.ccb_quickLoginBtn:setEnabled(true)
		end
	end)
	self._versionCheck:addEventListener(VersionCheck.EVENT_OPTIONAL_UPDATE, function(event)
		self:hideLoading()
	end)
end

_initProcessTimer = function(self)
	if self._bar then return end
	self.ccb_bar:retain()
	self.ccb_bar:removeFromParentAndCleanup(true)
	local x, y = self.ccb_bar:getPosition()
	self._bar = CCProgressTimer:create(self.ccb_bar)
	self._bar:setBarChangeRate(ccp(1, 0))
	self._bar:setType(kCCProgressTimerTypeBar)
	self._bar:setMidpoint(ccp(0, 0.5))
	self.ccb_barBg:addChild(self._bar)
	self.ccb_bar:release()
	self._bar:setPosition(x, y)
end

return LoginScene
