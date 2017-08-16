local LCGameSceneBase = class("LCGameSceneBase", LCSceneBase)

function LCGameSceneBase:ctor(key)
	if not key then return false end
	if not LCGameSceneBase.super.ctor(self,key) then return false end
	CC_SYNTHESIZE(self, "cacheMsgs", key.cacheMsgs or {})
	-- CC_SYNTHESIZE(self, "viewsMgr", key.viewsMgr)
	CC_SYNTHESIZE(self, "deskStatusMgr", key.deskStatusMgr)
	CC_SYNTHESIZE(self,"gameChatLayer",nil)

	CC_SYNTHESIZE(self, "messList", {})

	CC_SYNTHESIZE(self, "messagesMgr",key.messagesMgr)
	if (not self.messagesMgr)
	or (not self.deskStatusMgr) then
		GError("LCGameSceneBase init mgrs failed")
		return false
	end
	return true
end


function LCGameSceneBase:sendDirty()
	self:showLoading(_ID2WORD(12005))

	local pb = LCNetManager:createMessage(eDefMsg.Dirty)
	LCNetManager:send(pb,LCPackOption.new(DEF_INSTANT_TIMEOUT, function( ... )
    	self:hideLoading()
    	LCMessageBox:showConfirm(self,{text = _("请求失败，请稍后再试"),
			 buttonText = _ID2WORD(11001),
			 callback = function( ... )
				 --self:sendDirty()
			end})
    end))
end

function LCGameSceneBase:onEnter()
	LCSceneBase.onEnter(self)
	-- self.viewsMgr:initAnimateManger()
	CCEPlugin:setIdleTimerDisabled(true)
	local gameSocket = LCNetManager:getGameSocket()
	if not gameSocket then return end
	gameSocket:registerEventOnConnect(self, function( ... )
		self:hideLoading()
		GDebug("connect succ")
		if self.reconnecting then
			self.reconnecting = false
			self:sendDirty()
		end
	end)
	gameSocket:registerEventDisconnect(self, function( ... )
		self:checkConnect()
		GDebug("dissconnect ")
	end)
	gameSocket:registerEventFailedConnect(self, function( ... )
		self:checkConnect()
		GDebug("failed connect")
	end)
end

function LCGameSceneBase:onEnterTransitionDidFinish( )
	-- process cahe messages
	for i=1,#self.cacheMsgs do
		local tMsg = self.cacheMsgs[i]
		self:dealPacket(tMsg.seed,tMsg.msg,tMsg.uiUpdater)
	end
end

function LCGameSceneBase:onExit()
	self.viewsMgr = nil
	self.deskStatusMgr = nil
	self.gameChatLayer=nil
	self.messList = {}

	CCEPlugin:setIdleTimerDisabled(false)
	local gameSocket = LCNetManager:getGameSocket()
	if gameSocket then
		gameSocket:unregisterEventDisconnect(self)
		gameSocket:unregisterEventFailedConnect(self)
		gameSocket:unregisterEventOnConnect(self)
	end
	LCSceneBase.onExit(self)
end


-- ======================== COMMON GAME FUNCS ======================== --

function LCGameSceneBase:gameChat(seed, msg, uiUpdater)
	if msg.status == eErrorCode.succ then
		self.messagesMgr:pushMessageToQueue(seed,msg,uiUpdater)

		local mess = {
			status   	= msg.status,
			playerID 	= msg.playerID,
			nickName 	= msg.nickName,
			types	 	= msg.types,
			messID	 	= msg.messID,
			text	 	= msg.text,
			avatar   	= msg.avatar,
			broadTypes  = eBroadcastType.player,
		}
		table.insert(self.messList, mess)
	else
		self:showConfirmMessage(_ID2WORD(msg.status))
	end
end

-- ======================== NET FUNCS ======================== --
function LCGameSceneBase:dealPacket(seed, msg, uiUpdater)
	LCSceneBase.dealPacket(self,seed, msg, uiUpdater)
	if seed==eDefMsg.InGameGift then
	elseif seed==eDefMsg.ShopInGame then
	elseif seed==eDefMsg.GameChat then
		self:gameChat(seed, msg, uiUpdater)
	elseif seed == eDefMsg.Dirty then
		self:hideLoading()
		if msg.roomID ~= 0 then
			-- enter room
			self:sendEnterRoom(msg.roomID,true)
		else
			LCMessageBox:showConfirm(self,{text = _ID2WORD(3906),
									  hideClose = true,
									  callback = function ( ... )
									  	LCSceneTransit:replaceScene(eSceneName.main)
									  end})
		end
	elseif seed == eDefMsg.EnterRoom then
		self:hideLoading()
		if msg.status ~= eErrorCode.succ then
			LCMessageBox:showConfirm(self,{text = _ID2WORD(3906),
									  hideClose = true,
									  callback = function ( ... )
									  	LCSceneTransit:replaceScene(eSceneName.main)
									  end})
			return
		end

		self:processEnterRoom(msg)
	else
		self.messagesMgr:pushMessageToQueue(seed,msg,uiUpdater)
	end
	if self.gameChatLayer then self.gameChatLayer:dealPacket(seed, msg, uiUpdater) end
	-- if self.viewsMgr.gameTopLayer then self.viewsMgr.gameTopLayer:dealPacket(seed, msg, uiUpdater) end
end

function LCGameSceneBase:_onReconnect(msg)
end


return LCGameSceneBase
