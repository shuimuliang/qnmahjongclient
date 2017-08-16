local animateManger = require("lua/scene/commons/AnimationLayer")

local LCViewsMgr = class("LCViewsMgr")

function LCViewsMgr:ctor(scene)
	self.scene = scene
	CC_SYNTHESIZE(self, "animateManger", nil)
	CC_SYNTHESIZE(self, "deskStatusMgr",nil)
	CC_SYNTHESIZE(self, "playersLayer", nil)
end

-- ======================== COMMON FUNCS ======================== --
function LCViewsMgr:initAnimateManger(  )
	self.animateManger = animateManger:create(self)
	self.scene:addChild(self.animateManger)
end

function LCViewsMgr:getPlayerCenterPosByID(playerID)
	GError("should not here ,subclass should implement it")
end

function LCViewsMgr:getPlayerIDBySeatPos(seatPos)
	local deskPlayer = self.deskStatusMgr:getPlayerBySeatPos(seatPos)
	if not deskPlayer then return end
	return deskPlayer.player.playerID
end

function LCViewsMgr:getHeadCenterWorldPosition(playerID)
	if not playerID then return end
	local deskPlayer = self.deskStatusMgr:getPlayerByID(playerID)
	if not deskPlayer then return end
	local seatPos = deskPlayer.seatPos
	local playerBaseLayer
	if self.playersLayer then
		playerBaseLayer = self.playersLayer:getSingleLayerBySeatPos(seatPos)
	elseif self.tableView and self.tableView.playerSeatMap then
		playerBaseLayer = self.tableView.playerSeatMap[seatPos]
	end
	if not playerBaseLayer then return ccp(-1000,-1000) , 0 end
	return playerBaseLayer:getHeadCenterWorldPosition(), seatPos
end

function LCViewsMgr:getGiftWorldPosition(playerID)
	if not playerID then return end
	local deskPlayer = self.deskStatusMgr:getPlayerByID(playerID)
	if not deskPlayer then return end
	local seatPos = deskPlayer.seatPos
	local playerBaseLayer
	if self.playersLayer then
		playerBaseLayer = self.playersLayer:getSingleLayerBySeatPos(seatPos)
	elseif self.tableView and self.tableView.playerSeatMap then
		playerBaseLayer = self.tableView.playerSeatMap[seatPos]
	end
	if not playerBaseLayer then return ccp(-1000,-1000) , 0 end
	return playerBaseLayer:getGiftWorldPosition(),seatPos
end
function LCViewsMgr:getMessPos(playerID)
	GError("should not here ,subclass should implement it")
end

function LCViewsMgr:updateView()
	GError("should not here ,subclass should implement it")
end

function LCViewsMgr:updatePlayerGold(playerID)
	GDebug("updatePlayerGold: %s   ",playerID)
	local deskPlayer = self.deskStatusMgr:getPlayerByID(playerID)
	if self.playersLayer then  	GDebug("updatePlayerGold: %s   ",playerID)
 self.playersLayer:updateOneByPlayer(deskPlayer) end
end

function LCViewsMgr:configTableBoards(levelInfo)
	--GError("should not here ,subclass should implement it (levelID : %s)",levelID)
end

function LCViewsMgr:loadView(ignoreSound)
	local name = self.scene.__cname
	self.deskStatusMgr = self.scene:getDeskStatusMgr()
	if not ignoreSound then LCMusicBox:playEnterRoom() end
end

-- ======================== EASY FUNCS ======================== --
function LCViewsMgr:holdSeat(isHold,fromPos, toPos,callback,arriveFunc)
	if not fromPos or not toPos then return false end

	if isHold then
		local singleLayer_from,singleLayer_to
		if self.playersLayer then
			singleLayer_from = self.playersLayer:getSingleLayerBySeatPos(fromPos)
			singleLayer_to = self.playersLayer:getSingleLayerBySeatPos(toPos)
		elseif self.tableView and self.tableView.playerSeatMap then
			singleLayer_from = self.tableView.playerSeatMap[fromPos]
			singleLayer_to = self.tableView.playerSeatMap[toPos]
		end

		if not singleLayer_from or not singleLayer_to then return end
		self.animateManger:spriteFly(
			eResImage.common_holdseat,
			singleLayer_from:getHoldSeatWorldPosition(),
			singleLayer_to:getHoldSeatWorldPosition(),
			callback,nil,nil,arriveFunc)
	else
		if callback then callback() end
		if arriveFunc then arriveFunc() end
	end
	return true
end

function LCViewsMgr:updateHoldSeatViewStatus(seatPos)
	local seatStatus = self.deskStatusMgr:getHoldSeatStatusBySeatpos(seatPos)
	local singleplayer
	if self.playersLayer then
		singleLayer = self.playersLayer:getSingleLayerBySeatPos(seatPos)
	elseif self.tableView and self.tableView.playerSeatMap then
		singleLayer = self.tableView.playerSeatMap[seatPos]
	end

	if not singleLayer then return end

	if seatStatus then
		singleLayer:setHoldSeatStatus(true)
	else
		singleLayer:setHoldSeatStatus(false)
	end
end

function LCViewsMgr:sendKick(fromID, playerID,callback)
	local fromPos = self:getHeadCenterWorldPosition(fromID)
	local toPos = self:getHeadCenterWorldPosition(playerID)
	if not fromPos or not toPos then return false end
	self.animateManger:spriteFly(eResImage.common_foot, fromPos, toPos, callback)
	return true
end

function LCViewsMgr:sendGift(fromID, playerID, giftInfo)
	local giftResources=eResImage.treasures[giftInfo.key]
	local fromPos=self:getHeadCenterWorldPosition(fromID)
	local toPos, seatPos=self:getGiftWorldPosition(playerID)
	--[[if giftResources.effName=="play1" then
		toPos, seatPos=self:getHeadCenterWorldPosition(playerID)
	end--]]
	local image=giftResources.icon
	if not fromPos or not toPos then return false end

	local singleplayer
	if self.playersLayer then
		singleplayer = self.playersLayer:getSingleLayerBySeatPos(seatPos)
	elseif self.tableView and self.tableView.playerSeatMap then
		singleplayer = self.tableView.playerSeatMap[seatPos]
	end
	if not singleplayer then return end
	singleplayer:setGiftIcon(image)
	singleplayer:setGiftIconVisible(false)
	local scale = singleplayer:getGiftIconScale() * singleplayer:getGiftIconLayerScale()

	local function playerEff(node, giftScale)
		if not node then node=singleplayer end
		self.animateManger:playGiftEffect(node, giftResources.effName, function ()
			singleplayer:setGiftIconVisible(true)
		end, giftScale)
	end

	if giftResources.effName and #giftResources.effName>0 and giftResources.effName~="play7" then

		local function playerEff2(...)
			self.animateManger:playGiftEffect(singleplayer, giftResources.effName, function ()
				self.animateManger:playGiftEffect(singleplayer, giftResources.effName2, function ()
					singleplayer:setGiftIconVisible(true)
				end)
			end)
		end

		if LCAppGlobal:getLoginPlayer():isEqualSelfPlayer(fromID)
			and LCAppGlobal:getLoginPlayer():isEqualSelfPlayer(playerID) then
			if giftResources.effName=="play1" then
				playerEff2()
			else
				playerEff()
			end
		else
			local toCenterPos=self:getHeadCenterWorldPosition(playerID)
			self.animateManger:spriteFly(image, fromPos, toCenterPos, function ()
				if giftResources.effName=="play1" then
					playerEff2()
				else
					playerEff()
				end
			end, scale, true)
		end
	else
		if giftResources.effName and giftResources.effName=="play7" then
			self.animateManger:spriteFly(image, fromPos, toPos, function ()
				playerEff(singleplayer.spriteNoraml, scale)
			end, scale, true)
		else
			self.animateManger:spriteFly(image, fromPos, toPos, function ()
				singleplayer:setGiftIconVisible(true)
			end,scale)
		end
	end

	return true
end

function LCViewsMgr:chatShow(msg)
	if msg.types==eChatType.icon1 then
		local pos, seatPos=self:getHeadCenterWorldPosition(tostring(msg.playerID))
		if not pos then return end
		--[[self.animateManger:talkIconShow(pos, msg, function ()

		end)--]]

		local singleplayer
		if self.playersLayer then
			singleplayer = self.playersLayer:getSingleLayerBySeatPos(seatPos)
		elseif self.tableView and self.tableView.playerSeatMap then
			singleplayer = self.tableView.playerSeatMap[seatPos]
		end
		if not singleplayer then return end
	--	GDebug("----msg.types="..msg.types)
		local data=eResImage.s_chatIcon[eChatType.icon1]
		self.animateManger:playGiftEffect(singleplayer, data[msg.messID].effName, function ()

		end, 1, eResGameCommon.effect.face)
	elseif msg.types==eChatType.talk or msg.types==eChatType.edit then
		local pos, direct, scale=self:getMessPos(tostring(msg.playerID))
		if not pos then return end
		local sex = ePlayerGender.female
		local deskPlayer = self.deskStatusMgr:getPlayerByID(msg.playerID)
		if deskPlayer then
			local player = deskPlayer.player
			sex = player.gender
		end
		self.animateManger:talkTextShow(pos, msg, direct, sex ,function ()

		end, scale)
	elseif msg.types==eChatType.voice then
		self:onRecvVoiceChat(msg)
	end
end

function LCViewsMgr:onRecvVoiceChat(msg)
end

return LCViewsMgr
