local LCPlayerInfoDialog = require("lua/scene/mainScene/playerInfo/PlayerInfoDialog")
local LCInviteDialog = require("lua/scene/mainScene/friend/InviteDialog")
local animateManger = require("lua/scene/commons/AnimationLayer")
local LCGameGiftsDialog = require "lua/scene/mainScene/friend/GameGiftsDialog170"

local LCPlayerBaseLayer = class("LCPlayerBaseLayer",function() return CCLayerColor:create() end)
function LCPlayerBaseLayer:create(...)
	local ret = LCPlayerBaseLayer.new()
	if not ret:init(...) then return nil end
	return ret
end

function LCPlayerBaseLayer:init(key)
	key=key or {}
	CC_SYNTHESIZE(self,"seatPos", key.seatPos or 0)
	CC_SYNTHESIZE(self,"uiPos", key.uiPos or key.seatPos)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"deskPlayer", key.deskPlayer)
	CC_SYNTHESIZE(self,"types", key.types or ePlayerType.vertical)
	CC_SYNTHESIZE(self,"turnTimes", key.turnTimes or 12)
	CC_SYNTHESIZE(self,"deskStatusMgr", key.deskStatusMgr)
	CC_SYNTHESIZE(self,"room", key.room or self.deskStatusMgr.room)
	CC_SYNTHESIZE(self,"noGift", key.noGift or false)
	CC_SYNTHESIZE(self,"clip", key.clip)
	CC_SYNTHESIZE(self,"playerList", {})
	CC_SYNTHESIZE(self,"avatar", nil)
	CC_SYNTHESIZE(self,"size", nil)
	CC_SYNTHESIZE(self,"giftPos", {})
	CC_SYNTHESIZE(self,"namePos", {})
	CC_SYNTHESIZE(self,"goldPos", {})
	CC_SYNTHESIZE(self,"headPos", {})
	CC_SYNTHESIZE(self,"giftMenu", nil)
	CC_SYNTHESIZE(self,"giftItem", nil)
	CC_SYNTHESIZE(self,"giftSize", nil)
	CC_SYNTHESIZE(self,"isFlipX", false)
	CC_SYNTHESIZE(self, "vipLevelLabel", nil)
	CC_SYNTHESIZE(self, "touchAble", true)
	CC_SYNTHESIZE(self, "mainScene", LCSceneTransit:getRunningScene())

	CC_SYNTHESIZE_RO(self,"giftIcon", eResImage.c_avatarbox_giftbox2)
	CC_SYNTHESIZE(self, "winningEffect", nil)
	CC_SYNTHESIZE(self, "winningEffect1", nil)

	CC_SYNTHESIZE(self, "spriteNoraml", nil)
	CC_SYNTHESIZE_RO(self, "holdSeatStatus",false)

	CC_SYNTHESIZE(self,"countdown", nil)
	CC_SYNTHESIZE(self, "switcherMgr", LCAppGlobal:getGameData():getSwitchMgr())

	local ccbResourc=eResCCBI.playerLayer
	if self.types==ePlayerType.horizontal then
		ccbResourc=eResCCBI.playerLayer2
		if key.ccb then ccbResourc = key.ccb end
	end
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(ccbResourc,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    self.size = layer:getContentSize()
  	self:changeWidthAndHeight(self.size.width, self.size.height)
	self:addChild(layer)
	local nodeMap = {
		{"bgLayer","ccb_bgLayer","CCLayer"},
		{"bgSprite","ccb_bgSprite","CCSprite"},
		{"nameTTF","ccb_nameTTF","CCLabelTTF"},
		{"goldTTF","ccb_goldTTF","CCLabelTTF"},
		{"headLayer","ccb_headLayer","CCLayerColor"},
		{"giftBtn","ccb_giftBtn","CCLayer"},
		{"menuItem","ccb_menuItem","CCMenuItemImage"},
		{"addPlayerBtn","ccb_addPlayerBtn","CCLayer"},
		{"inviteIcon","ccb_inviteIcon","CCMenuItemImage"},
		{"holdSeatIcon","ccb_holdSeatIcon","CCSprite",{visible = false}},
	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)
	self.giftBtn:setVisible(false)
	self.giftPos=ccp(self.giftBtn:getPosition())
	self.namePos=ccp(self.nameTTF:getPosition())
	self.goldPos=ccp(self.goldTTF:getPosition())
	self.headPos=ccp(self.headLayer:getPosition())

	if not self.avatar then
		local isNotClip=true
		if self.clip then isNotClip=false end
		self.avatar = LCPlayerAvatar:create(1, function(avatar,url)
			GDebug("headClicked")
			self:headClicked()
		end, {isNotClip=isNotClip})
		self.avatar:setAnchorPoint(ccp(0,0))
		self.headLayer:addChild(self.avatar)
	end

	local pTitleButtonLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 30)
	local spriteN = CCScale9Sprite:createWithSpriteFrameName(eResImage.c_avatarbox_giftbox)
	self.giftMenu = CCControlButton:create(pTitleButtonLabel,spriteN)
	local function touchUpInsideAction()
		if self.deskPlayer and self.touchAble then
			LCMusicBox:playControlSounds()

			local key = {player = self.deskPlayer.player, showCurrency = true,
				callback=function ()
					if self.gameGiftDialog then self.gameGiftDialog:dismissSelf() end
				end}

			self.gameGiftDialog = LCGameGiftsDialog.new(key)
			self.gameGiftDialog:showInScene()
		end
	end
	self.giftMenu:addHandleOfControlEvent(touchUpInsideAction,CCControlEventTouchUpInside)
	local spriteIcon=CCSprite:createWithSpriteFrameName(eResImage.c_avatarbox_giftbox)
	self.giftMenu:setPreferredSize(spriteIcon:getContentSize())
	self.giftMenu:setZoomOnTouchDown(false)
	self.giftMenu:setAnchorPoint(ccp(0.5, 0.5))
	self.giftMenu:setPosition(ccp(self.giftBtn:getPosition()))
	self:addChild(self.giftMenu, 2)

	self.giftItem=CCSprite:createWithSpriteFrameName(self.giftIcon)
	self.giftSize=self.giftItem:getContentSize()
	self.giftItem:setPosition(ccp(self.giftSize.width/2, self.giftSize.height/2))
	self.giftMenu:addChild(self.giftItem)

	if self.noGift then
		self.giftBtn:setVisible(false)
		self.giftMenu:setTouchEnabled(false)
		self.giftMenu:setVisible(false)
		self.giftItem:setVisible(false)
	end

	local menuItemArray = CCArray:create()
	local spriteNoraml = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
	spriteNoraml:setPreferredSize(self.size)
	local menuPopupItem = CCMenuItemSprite:create(spriteNoraml, spriteNoraml, spriteNoraml)
	menuPopupItem:registerScriptTapHandler(function ()
		if not self.deskPlayer and self.touchAble then
			LCMusicBox:playControlSounds()
			if self.addPlayerBtn:isVisible() and self.switcherMgr:isInviteFriendValue() then
				_ANLS_EVENT(5005,self.__cname)
				if self.holdSeatStatus then
					local fromPlayerID = self.deskStatusMgr:getHoldSeatStatusByUIpos(self.uiPos)
					if fromPlayerID ~= LCAppGlobal:getLoginPlayer():getPlayerID() then
						local fromPlayer = self.deskStatusMgr:getPlayerByID(fromPlayerID)
						local key = {
							text = string.format(_("这个位置被玩家%s占了"),fromPlayer.player.nickName),
							hideClose = true
						}
						LCMessageBox:showConfirm(self.mainScene,key)
						return
					end
				end

				local _key = {
					room=self.room,
					friendType = eFriendType.inviteFriend,
					playerList=self:getGamePlayerList(),
					uiPos=self.uiPos,
					isHold = not self.deskStatusMgr:getHoldSeatStatusByUIpos(self.uiPos),
					visbleHold = self:getVisibleHold(),
				}
				LCInviteDialog.new(_key):showInScene()
			else
				if self.callback then self.callback(self.seatPos) end
			end
		end
	end)
	menuItemArray:addObject(menuPopupItem)
	local menuPopup = CCMenu:createWithArray(menuItemArray)
	menuPopup:setPosition(ccp(self.size.width/2, self.size.height/2))
	self:addChild(menuPopup)

	return true
end

function LCPlayerBaseLayer:getVisibleHold()
	---have special (eRoomType.BJRandom seatPos 0)
	return true
end

function LCPlayerBaseLayer:getGamePlayerList()
	self.playerList={}
	if self.deskStatusMgr then
		local list={}
		if self.room and self.room.roomType == eRoomType.BaccaratRoom then
			list=self.deskStatusMgr.playerIDMap
		elseif self.room and self.room.roomType == eRoomType.Zhajinhua then
			-- list=self.deskStatusMgr.playerSeatMap
			local itr = function(seat, player)
				table.insert(list, player)
			end
			self.deskStatusMgr.playerSeatMap:foreach(itr)
		else
			list=self.deskStatusMgr.playerList
		end
		for _,v in pairs(list) do
			--GDebug("---name="..v.player.nickName)
			table.insert(self.playerList, v)
		end
	else
		self.playerList=self.deskStatusMgr.playerList
	end

	return self.playerList
end

function LCPlayerBaseLayer:getWinPos()
	return CE_GET_WORLD_POSITION(self,ccp(0.5,0.5))
end

function LCPlayerBaseLayer:setGiftIcon(icon)
	self.giftIcon = icon or eResImage.c_avatarbox_giftbox2
	self:updateGiftIcon()
end

function LCPlayerBaseLayer:setGiftIconVisible(bool)
	if self.noGift then self.giftItem:setVisible(false) return end
	self.giftItem:setVisible(bool)
end

function LCPlayerBaseLayer:setName(str)
	if CE_GET_STRING_SHOW_LENGTH(str) <= 8 then
		self.nameTTF:setString(str)
	else
		self.nameTTF:setString(CE_STRING_CUT_OFF(str,6) .. "..")
	end
end

function LCPlayerBaseLayer:updateGiftIcon()
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.giftIcon)
	self.giftItem:setDisplayFrame(frame)
	local sc = self:getGiftIconScale()
	self.giftItem:setScale(sc)
end

function LCPlayerBaseLayer:getGiftIconScale()
	local selfSize = self.giftItem:getContentSize()
	return (self.giftSize.width/selfSize.width)
end

function LCPlayerBaseLayer:getGiftIconLayerScale()
	return self:getParent():getScale()
end

function LCPlayerBaseLayer:updateAddPlayerBtnStatus()
	if not self.switcherMgr:isInviteFriendValue() then
		self.addPlayerBtn:setVisible(false)
	elseif self.deskPlayer then
		self.addPlayerBtn:setVisible(false)
	else
		self.addPlayerBtn:setVisible(true)
	end
end

function LCPlayerBaseLayer:update(deskPlayer, seatPos, uiPos)
	if seatPos or deskPlayer then
		self.seatPos = seatPos or deskPlayer.seatPos

	end
	if uiPos then
		self.uiPos = uiPos
	end


	local hasPlayer = (deskPlayer ~= nil)
	self.deskPlayer = deskPlayer
	self.nameTTF:setVisible(hasPlayer)
	self.headLayer:setVisible(hasPlayer)
	self.goldTTF:setVisible(hasPlayer)
	if not hasPlayer then self:setGiftIcon() self.noGift=false end
	if not self.noGift then
		self.giftMenu:setVisible(hasPlayer)
	end
	if hasPlayer==false then
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(eResImage.c_avatarbox_giftbox2)
		self.giftItem:setDisplayFrame(frame)
		self.giftItem:setScale(1)
	end
	self:updateAddPlayerState()
	self:updateAddPlayerBtnStatus()

	self:updateFlipX()

	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(eResImage.c_avatar_other)
	if self.types==ePlayerType.horizontal then
		frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(eResImage.c_avatar_cross)
	end

	self:updateHoldSeatStatus()

	if not deskPlayer then
		self.bgSprite:setDisplayFrame(frame)
		self:removeTimeBar()
		return
	else
		if self.types~=ePlayerType.horizontal and LCAppGlobal:getLoginPlayer():isEqualSelfPlayer(deskPlayer.player.playerID) then
			frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(eResImage.c_avatar_player)
		end
		self.bgSprite:setDisplayFrame(frame)
	end
	self:setName(deskPlayer.player.nickName)
	--self.goldTTF:setString("$".._NICE_MONEY3(deskPlayer.carryGold))
	self:updateGoldText()

	self.avatar:updateURL(deskPlayer.player.avatar, self.deskPlayer.player)

	self:updateGiftIcon()

end

function LCPlayerBaseLayer:updateGoldText()
	self.goldTTF:setString("$".._NICE_MONEY3(self.deskPlayer.player.goldCount))
end

function LCPlayerBaseLayer:updateAddPlayerState()

end

function LCPlayerBaseLayer:updateFlipX()

end

function LCPlayerBaseLayer:headClicked()
	if not self.deskPlayer or not self.touchAble then return end
	local key = {
		title = self.deskPlayer.player.nickName,
		playerID = self.deskPlayer.player.playerID,
		infoTypes = eRoleInfoType.roomInfo,
		player = self.deskPlayer.player,
		seatPos = self.seatPos,
		uiPos = self.uiPos,
		}
	LCPlayerInfoDialog.new(key):showInScene()
end

function LCPlayerBaseLayer:ccb_addPlayerClicked(control,controlEvent)
	if not self.room then return end

end

function LCPlayerBaseLayer:ccb_giftClicked(control,controlEvent)
	if not self.deskPlayer or not self.touchAble then return end
	LCGameGiftsDialog:showInScene(self.mainScene, {player = self.deskPlayer.player})
end

function LCPlayerBaseLayer:getPosByType(types)
	local pos=ccp(self:getPosition())
	if types == ePosType.center then
		return ccp(pos.x+self.size.width/2, pos.y+self.size.height/2)
	elseif types == ePosType.left then
		return ccp(pos.x, pos.y+self.size.height/2)
	elseif types == ePosType.right then
		return ccp(pos.x+self.size.width, pos.y+self.size.height/2)
	elseif types == ePosType.top then
		return ccp(pos.x+self.size.width/2, pos.y+self.size.height)
	elseif types == ePosType.bottom then
		return ccp(pos.x+self.size.width/2, pos.y)
	elseif types == ePosType.leftTop then
		return ccp(pos.x, pos.y+self.size.height)
	elseif types == ePosType.rightTop then
		return ccp(pos.x+self.size.width, pos.y+self.size.height)
	elseif types == ePosType.leftBottom then
		return ccp(pos.x, pos.y)
	elseif types == ePosType.rightBottom then
		return ccp(pos.x+self.size.width, pos.y)
	end
	return pos
end

function LCPlayerBaseLayer:setFlipX(isFlipX)
	if self.isFlipX==false and isFlipX then
		self.giftMenu:setPositionX(self.size.width-self.giftPos.x)
		if self.types==ePlayerType.horizontal then
			self.nameTTF:setPositionX(self.size.width-self.namePos.x)
			self.goldTTF:setPositionX(self.size.width-self.goldPos.x)
			self.headLayer:setPositionX(self.size.width-self.headPos.x)
		end
	elseif isFlipX==false then
		self.giftMenu:setPositionX(self.giftPos.x)
		self.nameTTF:setPositionX(self.namePos.x)
		self.goldTTF:setPositionX(self.goldPos.x)
		self.headLayer:setPositionX(self.headPos.x)
	end
	self.isFlipX=isFlipX
end

function LCPlayerBaseLayer:getGiftWorldPosition()
	local node = self.giftMenu
	return CE_GET_WORLD_POSITION(node)
end

function LCPlayerBaseLayer:getHoldSeatWorldPosition()
	local node = self.holdSeatIcon
	return CE_GET_WORLD_POSITION(node)
end

function LCPlayerBaseLayer:getHeadCenterWorldPosition()
	local node = self.avatar and self.avatar or self.headLayer

	local size = node:getContentSize()
	local width,height = size.width,size.height
	local scale = self:getGiftIconLayerScale()
	local pos = CE_GET_WORLD_POSITION(node)
	local posX,posY = pos.x,pos.y
	return ccp(pos.x + width/2 * scale,pos.y + height/2 * scale)
end

function LCPlayerBaseLayer:startTimeBar(callback, time)
	--GDebug("-----seatPos="..self.deskPlayer.seatPos)
	if not time or time<=0 then time=self.turnTimes end
	self:removeTimeBar()
	if not self.deskPlayer then return end
	local pcName = eResImage.c_avatar_countdown
	if self.types~=ePlayerType.vertical then
		pcName = eResImage.c_avatar_cross_countdown
	end
	self.countdown = CCProgressTimer:create(CCSprite:createWithSpriteFrameName(pcName))
	self.countdown:setAnchorPoint(0, 0)
	self.countdown:setColor(_APPCOLOR_HEX("4eff00"))
	self:addChild(self.countdown)

	local to = CCProgressFromTo:create(time, 100, 0)
	self.countdown:setType(kCCProgressTimerTypeRadial)
	self.countdown:setMidpoint(ccp(0.5, 0.5))
	self.countdown:setBarChangeRate(ccp(0, 1))
	self.countdown:setReverseProgress(true)
	self.countdown:setOpacity(0xFF)
	self.countdown:runAction(CCSequence:createWithTwoActions(to, CCCallFunc:create(function ()
		GDebug("LCTimebarLayer ok")
		if callback then callback() end
	end)))

	self:timeCall(callback)
end

function LCPlayerBaseLayer:timeCall(callback)
	local mainScene = LCSceneTransit:getRunningScene()
	scheduleOnce(mainScene, function (...)
		if self.countdown then
			local percent=math.ceil(self.countdown:getPercentage())
			--GDebug("-----0---0="..self.countdown:getPercentage())
			--GDebug("-----seatPos="..self.deskPlayer.seatPos)
			if percent>60 then
				self.countdown:setColor(_APPCOLOR_HEX("4eff00"))
			elseif percent<=60 and percent>55 then
				self.countdown:setColor(_APPCOLOR_HEX("9cff00"))
			elseif percent<=55 and percent>50 then
				if percent==50 and LCAppGlobal:getLoginPlayer():isEqualSelfPlayer(self.deskPlayer.player.playerID) then LCMusicBox:playTimeWarm() end
				self.countdown:setColor(_APPCOLOR_HEX("9cff00"))
			elseif percent<=50 and percent>45 then
				self.countdown:setColor(_APPCOLOR_HEX("c6ff00"))
			elseif percent<=45 and percent>40 then
				self.countdown:setColor(_APPCOLOR_HEX("fffc00"))
			elseif percent<=40 and percent>35 then
				self.countdown:setColor(_APPCOLOR_HEX("ffc000"))
			elseif percent<=35 and percent>30 then
				self.countdown:setColor(_APPCOLOR_HEX("ffa800"))
			elseif percent<=30 and percent>25 then
				self.countdown:setColor(_APPCOLOR_HEX("ff8400"))
			elseif percent<=25 and percent>20 then
				self.countdown:setColor(_APPCOLOR_HEX("ff6000"))
			elseif percent<=20 and percent>15 then
				self.countdown:setColor(_APPCOLOR_HEX("ff3600"))
			elseif percent==3 then
				self.countdown:setColor(_APPCOLOR_HEX("ff0000"))
				self.countdown:stopAllActions()
				local array = CCArray:create()
				array:addObject(CCFadeIn:create(0.5))
				array:addObject(CCFadeOut:create(0.5))
				array:addObject(CCFadeIn:create(0.5))
				array:addObject(CCFadeOut:create(0.5))
				array:addObject(CCFadeIn:create(0.5))
				array:addObject(CCCallFunc:create(function (...)
					if callback then callback() end
				end))
				self.countdown:runAction(CCSequence:create(array))
				return
			else
				self.countdown:setColor(_APPCOLOR_HEX("ff0000"))
			end
			if percent>0 then self:timeCall() end
		end
	end, 0.1)
end

function LCPlayerBaseLayer:removeTimeBar()
	if self.countdown then
		self.countdown:stopAllActions()
		self.countdown:removeFromParentAndCleanup(true)
		self.countdown = nil
	end
end

function LCPlayerBaseLayer:setHoldSeatStatus(bool)
	self.holdSeatStatus = bool
	self:updateHoldSeatStatus()
end

function LCPlayerBaseLayer:updateHoldSeatStatus()
	if self.holdSeatStatus and (not self.deskPlayer) then
		self.holdSeatIcon:setVisible(true)
		self.inviteIcon:setVisible(false)
	else
		self.holdSeatIcon:setVisible(false)
		self.inviteIcon:setVisible(true)
	end
end

return LCPlayerBaseLayer
