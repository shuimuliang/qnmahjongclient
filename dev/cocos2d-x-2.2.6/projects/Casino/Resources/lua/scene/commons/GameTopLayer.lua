local LCStoreDialog = require("lua/scene/mainScene/shop/StoreDialog")
local LCGameChatLayer = require("lua/scene/commons/GameChatLayer")
local LCEditLayer = require("lua/scene/commons/EditLayer")
local LeaveLayer = require("lua/scene/commons/LeaveLayer")

local LCGameTopLayer = class("LCGameTopLayer", function() return CCNode:create() end)

function LCGameTopLayer:ctor(scene,key)
	key = key or {}
	local function onEvent(event)
		if event == "enter" then self:onEnter() end
	end
	self:registerScriptHandler(onEvent)

	self.scene = scene
	CC_SYNTHESIZE(self,"lobbyCallback",key.lobbyCallback)
	CC_SYNTHESIZE(self,"changeCallback",key.changeCallback)
	self._onQuit = key.onQuit
end

-- virtual
function LCGameTopLayer:onEnter()
	local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.gameTopLayer,proxy,self)
	self:addChild(layer)

	local switcherMgr = LCAppGlobal:getGameData():getSwitchMgr()
	if not switcherMgr:isGameStoreValue() then
		self.ccb_gameStore:setVisible(false)
	end

	local size = CCDirector:sharedDirector():getVisibleSize()
	GDebug("winSize : w, h = %s, %s", size.width, size.height)
	self:setPositionY(eUIConst.screenHeight)
	self:updateTalkCard()

	local pTitleButtonLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 30)
	local spriteN = CCScale9Sprite:createWithSpriteFrameName(eResImage.c_menuicon_chat)
	local controlButton = CCControlButton:create(pTitleButtonLabel, spriteN)
	controlButton:addHandleOfControlEvent(handler(self, self.chatBtnClicked), CCControlEventTouchUpInside)
	controlButton:setPreferredSize(CCSizeMake(71, 71))
	controlButton:setPosition(50, size.height*0.25)
	self.scene:addChild(controlButton)
end

function LCGameTopLayer:menuItemClicked(index)
	if index == 1 then
		if self.lobbyCallback then self.lobbyCallback() end
	elseif index == 2 then
		if self.changeCallback then
			self.changeCallback()
		end
	end
end

function LCGameTopLayer:menuBtnClicked()
	if self._onQuit then self._onQuit() end
	-- self.LeaveLayer = LeaveLayer:create({
	-- 	callback=function (index)
	-- 		if self.LeaveLayer then
	-- 			self.LeaveLayer:removeFromParentAndCleanup(true)
	-- 			self.LeaveLayer=nil
	-- 		end
	-- 		if index<=0 then return end
	-- 		self:menuItemClicked(index)
	-- 	end,
	-- 	disableChangeRoomBtn = self.disableChangeRoomBtn
	-- })
	--
	-- local size = CCDirector:sharedDirector():getWinSize()
	-- local content = self.LeaveLayer:getContentSize()
	-- self.LeaveLayer:setPosition((size.width-content.width)/2, (size.height-content.height)/2)
	-- self.scene:addChild(self.LeaveLayer)
end

function LCGameTopLayer:ccb_btnClicked1()
	self:menuBtnClicked()
end

function LCGameTopLayer:ccb_btnClicked2()
	local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST)
	if not haveCount or haveCount<=0 then
		LCMessageBox:showConfirm1({text=_("小喇叭数量不足！"), buttonType=eButtonType.yellow, textColor=_APPCOLOR_HEX("99FFFF"),
		callback = function ()
			local LCStoreDialog = require("lua/scene/mainScene/shop/StoreDialog")
			LCStoreDialog.new({showCurrency = true}):showInScene()
		end,})
		return
	end
	self.editLayer=LCEditLayer:create(self.scene, {callback=function (...)
		self.editLayer:removeFromParentAndCleanup(true)
		self.editLayer=nil
		self.scene:hideLoading()
	end})
	self.scene:addChild(self.editLayer, 2)
end

function LCGameTopLayer:ccb_btnClicked3()
	_ANLS_EVENT(5003,self.__cname)
	LCStoreDialog.new({showCurrency = true}):showInScene()
end

function LCGameTopLayer:chatBtnClicked(sliderDownBtn)
	_ANLS_EVENT(5004,self.__cname)
	LCMusicBox:playControlSounds()
	local messList=self.scene:getMessList()

	local key={recordList=clone(messList), callback=function ()
				if self.gameChatLayer then
					self.gameChatLayer:removeFromParentAndCleanup(true)
				end
				self.gameChatLayer = nil
				self.scene.gameChatLayer=nil
			end}
	self.gameChatLayer = LCGameChatLayer:create(self.scene, key)
	self.scene:addChild(self.gameChatLayer)
	self.scene.gameChatLayer = self.gameChatLayer
end

function LCGameTopLayer:dealPacket(seed, msg, uiUpdater)
	if seed == eDefMsg.Broadcast or seed == eDefMsg.BuyGoods then
		self:updateTalkCard()
	end
end

function LCGameTopLayer:updateTalkCard()
	local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST)
	self.ccb_countLabel:setString(haveCount)
end

function LCGameTopLayer:hideStoreBtn()
	self.ccb_gameStore:setVisible(false)
end

return LCGameTopLayer
