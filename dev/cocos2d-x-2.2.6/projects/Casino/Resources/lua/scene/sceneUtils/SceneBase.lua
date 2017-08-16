LCSceneBase = class("LCSceneBase",function() return CCScene:create() end)
function LCSceneBase:ctor(key)
	CC_SYNTHESIZE(self,"key",key)

	CC_SYNTHESIZE(self,"dialogs",{})
	CC_SYNTHESIZE(self,"progressHUD",nil)

	CC_SYNTHESIZE(self,"broadcastLayer",nil)
	CC_SYNTHESIZE(self,"inviteLayer",nil)

	CC_SYNTHESIZE(self,"gameIPIndex", 1)
	CC_SYNTHESIZE(self,"reconnectTimes", 0)
	CC_SYNTHESIZE(self, "reconnecting", false)

	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()
		elseif event == "enterTransitionFinish" then self:onEnterTransitionDidFinish()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)

	schedule(self,function()
		self:update()
	end,0.5)
	return true
end

-- virtual
function LCSceneBase:onEnter()
end

function LCSceneBase:switchGameIP()
	local ipList = DEF_GAME_SERVER_IP_LIST
	if not ipList or #ipList <= 1 then return end
	local ipListNum = #ipList
	self.gameIPIndex = (self.gameIPIndex) % ipListNum + 1
	if self.gameIPIndex > ipListNum then
		self.gameIPIndex = 1
	end
	local gameIP = ipList[self.gameIPIndex]
	local gameSocket = LCNetManager:getGameSocket()
	gameSocket:updateIP(gameIP)
	GDebug("game server ip : %s   %s",gameIP,self.gameIPIndex)
end

function LCSceneBase:checkConnect()
	local callback = function( ... )
		-- logout game
		local curScene = LCSceneTransit:getRunningScene()
   		if curScene and curScene.__cname == "LCLoginScene" then return end

		LCAppGlobal:setNeedReconnect(true)
		LCAppGlobal:playerLogout()
		LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
	end

	GDebug("11111111")
	self.reconnectTimes = self.reconnectTimes + 1
	local ipList = DEF_GAME_SERVER_IP_LIST or {}
	local ipNum = #ipList + 1
	if self.reconnectTimes > ipNum then
		GDebug("11111111222")
		LCAppGlobal:setNeedReconnect(false)
		LCMessageBox:showConfirm(self,{text = _("连接服务器失败,请检查您的网络..."),hideClose = true,callback = callback})
	end

	if LCAppGlobal:getNeedReconnect() then
		GDebug("1111111122233333")
		self:switchGameIP()
		self:showLoading(_("正在连接服务器..."))
		LCAppGlobal:setReconnect(1)
		self.reconnecting = true
	else
		GDebug("11111111222333334444")
		self:hideLoading()
		LCNetManager:closeGameSocket()
		LCAppGlobal:setReconnect(0)
		self.reconnecting = false
	end
end

function LCSceneBase:onAppBackEvent()
	-- 游戏从后台切回来的时候
end

function LCSceneBase:onEnterTransitionDidFinish()
end

function LCSceneBase:onExit()
end

-- hud
function LCSceneBase:showLoading(text)
	if self.progressHUD then
		self.progressHUD:setShowText(text)
		return
	end
	self.progressHUD = LCLoadingHUD.new(text):showInScene() -- LCProgressHUD:showInScene(text)

end
function LCSceneBase:hideLoading()
	if not self.progressHUD then return end
	self.progressHUD:dismissSelf()
	self.progressHUD = nil
end

-- message box
function LCSceneBase:showConfirmMessage(text,callback)
	if not text then return end
	LCMessageBox:showConfirm(self,{text = text, callback = callback})
end


function LCSceneBase:sendEnterRoom(deskID,relink)
	local pb = LCNetManager:createMessage(eDefMsg.EnterRoom)
	pb._msg.roomID = deskID or 0
	pb._msg.configs = {}
	if relink then
		pb._msg.relink = 1
	end
	LCNetManager:send(pb)
	self:showLoading(_("正在进入房间..."))
end

-- -- room related
-- function LCSceneBase:sendEnterRoom(gameType, roomID, deskID, proxy,inviteID)
-- 	if not gameType then GError("should not here") return end
--   	if not LCAppGlobal.isLogined() then
-- 		LCMessageBox:showConfirm(self,{text = _ID2WORD(1003),hideClose = true,callback = function( ... )
-- 			LCAppGlobal:playerLogout()
-- 			LCSceneTransit:replaceScene(eSceneName.login)
-- 		end})
-- 		return
--     end

-- 	self:showLoading(_("正在进入房间..."))
-- 	local pb = LCNetManager:createMessage(eDefMsg.EnterRoom)
-- 	pb._msg.room = {roomType = gameType, roomID = roomID or 0, deskID = deskID or 0 }
-- 	pb._msg.enterProxy = proxy or 0
-- 	pb._msg.inviteID = inviteID
-- 	LCNetManager:send(pb)
-- end

function LCSceneBase:processEnterRoom(enterRoomRecv)
	if not enterRoomRecv then GError("should not here") return end
	if enterRoomRecv.status == eErrorCode.succ then
		LCSceneTransit:replaceScene(eSceneName.MJ,{enterRoomRecv = enterRoomRecv})
	elseif enterRoomRecv.status == 2106 then
		LCMessageBox:showConfirm(self, {
            text = "进入防作弊房间，请开启GPS定位功能。",
    		callback = function()
                CCEPlugin:requestLocationService()
    		end
        })
	else
		local text=_ID2WORD(enterRoomRecv.status)
		self:showConfirmMessage(text)
	end
end

-- network messages

function LCSceneBase:dealPacket(seed, msg, uiUpdater)
	-- process common error message
	if seed == eDefMsg.Error then
		local code = msg.status
		local function playerLogout( ... )
			LCNetManager:closeGameSocket()
			-- logout game
			LCAppGlobal:setToken("")
		end

		local callback = function( ... )
			LCAppGlobal:playerLogout()
			LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
		end
		if code == 301 then
			playerLogout()
			LCMessageBox:showConfirm(self,{text = _("您的登录已经过期，请重新登录！"),hideClose = true,callback = callback})
		else
			playerLogout()
			LCMessageBox:showConfirm(self,{text = _ID2WORD(code),hideClose = true,callback = callback})
		end
		self:hideLoading()
		return
	end
	if seed == eDefMsg.GetAdGold then
		if msg.status ~= eErrorCode.succ then return end
		local text =  string.format( _("恭喜获得免费筹码 %s"), msg.goldCount)
		LCMessageBox:showConfirm(self,{text = text,hideClose = true,callback = callback})
	end

	if self.broadcastLayer then self.broadcastLayer:dealPacket(seed, msg, uiUpdater) end
	-- dialog deal
	for dialogTag, dialog in pairs(self.dialogs) do
		dialog:dealPacket(seed, msg, uiUpdater)
	end
end

function LCSceneBase:update()
	if CIMManager then
		CIMManager:getInstance():update(1)
	end
end
