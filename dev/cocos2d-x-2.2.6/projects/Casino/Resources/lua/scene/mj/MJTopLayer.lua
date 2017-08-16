local animateManger   = require("lua/scene/commons/AnimationLayer")
local LCGameChatLayer = require("lua/scene/commons/GameChatLayer")

local OP_EFFECT = {}
OP_EFFECT[eMJOperation.peng]  = "peng"
OP_EFFECT[eMJOperation.hu]    = "hu"
OP_EFFECT[eMJOperation.chi]   = "chi"
OP_EFFECT[eMJOperation.ming]  = "gang"
OP_EFFECT[eMJOperation.an]    = "angang"
OP_EFFECT[eMJOperation.ziHu]  = "zimohu"
OP_EFFECT[eMJOperation.paoHu] = "hu"


local LCMJTopLayer = class( "LCMJTopLayer", function() return CCLayer:create() end )

function LCMJTopLayer:create(...)
	local ret = LCMJTopLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJTopLayer:init(deskView,deskStatusMgr)
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjTopLayer,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	local nodeMap = {
		{ "voicepanel", 		"ccb_voicePanel", 		"CCLayer" },
		{ "voiceTipLab", 		"ccb_voiceTipLab", 		"CCLabelTTF" },
		{ "voiceTimeLab", 		"ccb_voiceTimeLab", 	"CCLabelTTF" },
		{ "voiceImg", 		    "ccb_voiceImg", 	    "CCSprite" },
		{ "voiceFinishImg",     "ccb_voiceFinishImg", 	"CCSprite" },
	}

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self.scene = LCSceneTransit:getRunningScene() 
	self.deskView = deskView
	self.deskStatusMgr = deskStatusMgr
	self.effVolume = 0

	self:initAnimateManger()

	if CIMManager then
		CIMManager:getInstance():registerUploadScriptHandler(function(fileID)
			self:onUploadCallBack(fileID)
		end)

		CIMManager:getInstance():registerDownloadScriptHandler(function(fileID)
			self:onDownloadCallBack()
		end)
	end

	self.voicepanel:setVisible(false)
	return true
end

function LCMJTopLayer:initAnimateManger(  )
	self.animateManger = animateManger:create()
	self.deskView:addChild(self.animateManger)
end

function LCMJTopLayer:showGuide()
	if not self.guideEff then
		self.guideEff = LCEffectObject:create("shou")
		self.guideEff:setAutoPlay(false)
		self.guideEff:setAutoDelete(false)
		self.guideEff:play("shou")
		self.guideEff:setScale(0.34)
		self.deskView:addChild(self.guideEff)

		local guideNode = self.deskView:getGuideNode()
		local p = CE_GET_WORLD_POSITION(guideNode)
		self.guideEff:setPosition(p)

	end
	self.guideEff:setVisible(true)
end

function LCMJTopLayer:hideGuide()
	if self.guideEff then self.guideEff:setVisible(false) end
end

function LCMJTopLayer:playOpEff(pType,opType)
	local effectName = OP_EFFECT[opType]
	local eff = LCEffectObject:create(effectName)
	eff:setAutoPlay(false)
	eff:setAutoDelete(false)
	eff:play(effectName,function( ... )
		eff.armature:pause()
		scheduleOnce(self.scene,function() eff:removeFromParentAndCleanup(true) end, 1)		
	end)
	self.deskView:addChild(eff)

	local node = self.deskView["showNode"..pType]
	local p = CE_GET_WORLD_POSITION(node)
	eff:setPosition(p)
	if opType == eMJOperation.an or opType == eMJOperation.ming or opType == pengGang then
		scheduleOnce(self.scene,function() self:playAward(self.deskView,p) end, 0.7)		
	end
end

function LCMJTopLayer:playLoseEff()
	local effectName = "woshule"
	local eff = LCEffectObject:create(effectName)
	eff:setAutoPlay(false)
	eff:setAutoDelete(false)
	eff:play(effectName,function( ... )
		eff.armature:pause() 
		scheduleOnce(self.scene,function() eff:removeFromParentAndCleanup(true) end, 3)
	end)
	self.deskView:addChild(eff)

	local node = self.deskView:getTimerLayer()
	local p = CE_GET_WORLD_POSITION(node)	
	eff:setPosition(p)
end


function LCMJTopLayer:playAward(node,pos)
	if not node or not pos then return end
	local effectName = "casino_baozha1"
	local eff = LCEffectObject:create(effectName)
	eff:setAutoPlay(true)
	eff:setAutoDelete(true)
	eff:setScale(0.5)
	node:addChild(eff)
	eff:setPosition(pos)
end

function LCMJTopLayer:playLiuju()
	local effectName = "liuju"
	local eff = LCEffectObject:create(effectName)
	eff:setAutoPlay(false)
	eff:setAutoDelete(true)
	eff:play(effectName,function( ... )
	end)
	self.deskView:addChild(eff)

	local node = self.deskView:getTimerLayer()
	local p = CE_GET_WORLD_POSITION(node)	
	eff:setPosition(p)
end


function LCMJTopLayer:onVoiceBtnClicked(control,controlEvent)
	if self.visibleSchedule then
		return
	end

	if self.touchEvent == controlEvent then return end

	if controlEvent == CCControlEventTouchDown then
		self:onTouchDown()
    elseif controlEvent == CCControlEventTouchDragOutside then
    	self:onTouchDragOutside()
    elseif controlEvent == CCControlEventTouchDragInside then
    	if self.touchEvent == CCControlEventTouchDown then return end
    	self:onTouchDragInside()
    elseif controlEvent == CCControlEventTouchUpOutside then
    	self:onTouchUpOutside()
    elseif controlEvent == CCControlEventTouchUpInside then
    	self:onTouchUpInside()
    end

    self.touchEvent = controlEvent
end

function LCMJTopLayer:onChatBtnClicked(control,controlEvent)
	if controlEvent == CCControlEventTouchUpInside then
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
end

function LCMJTopLayer:audioStop()
	SimpleAudioEngine:sharedEngine():pauseBackgroundMusic()
	local curVolum = LCUserContext:getGameSettings().soundEffectVolume or SimpleAudioEngine:sharedEngine():soundEffectVolume()
	self.effVolume = curVolum
	SimpleAudioEngine:sharedEngine():setEffectsVolume(0)
end

function LCMJTopLayer:audioStart()
 	SimpleAudioEngine:sharedEngine():resumeBackgroundMusic()
	SimpleAudioEngine:sharedEngine():setEffectsVolume(self.effVolume)
end

function LCMJTopLayer:onTouchDown()
	self:audioStop()

	self.voicepanel:setVisible(true)
	self.voiceTimeLab:setVisible(true)
	self.voiceFinishImg:setVisible(false)

	self.voiceTipLab:setString(_("手指上滑 取消发送"))
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("small-mak.png")
	self.voiceImg:setDisplayFrame(frame)

	if CIMManager then CIMManager:getInstance():beginRecord() end

	if not self.timeSchedule then
		self.recordTime = 10
		self.voiceTimeLab:setString(self.recordTime)
		self.timeSchedule = schedule(self.scene,function()
			self.recordTime = self.recordTime - 1
			self.voiceTimeLab:setString(self.recordTime)
			if self.recordTime <= 0 then
				self:onTouchUpInside()
			end
		end,1)
	end
end

function LCMJTopLayer:onTouchDragOutside()
	self.voiceTipLab:setString(_("松开手指 取消发送"))
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("go.png")
	self.voiceImg:setDisplayFrame(frame)
end

function LCMJTopLayer:onTouchDragInside()
	self:onTouchDown()
end

function LCMJTopLayer:onTouchUpOutside()
	self:audioStart()
	self.voicepanel:setVisible(false)

  	if self.timeSchedule then
  		unschedule(self.scene, self.timeSchedule)
  		self.timeSchedule = nil
  	end

  	if CIMManager then CIMManager:getInstance():stopRecord(false) end
end

function LCMJTopLayer:onTouchUpInside()
	self:audioStart()

	if self.recordTime > 9 then
		self:onTouchUpOutside()
		return
	end

	self.voiceFinishImg:setVisible(true)
	self.voiceTipLab:setString(_("已发送"))

	if CIMManager then CIMManager:getInstance():stopRecord(true) end

	if self.timeSchedule then
  		unschedule(self.scene, self.timeSchedule)
  		self.timeSchedule = nil
  	end

  	self.voiceTimeLab:setVisible(false)

  	if not self.visibleSchedule then
	  	self.visibleSchedule = scheduleOnce(self.scene, function()
	  		self.voicepanel:setVisible(false)
	  		self.visibleSchedule = nil
	  	end,1)
  	end
end

function LCMJTopLayer:onMsgVoiceChat(msg)
	if msg.status == 0 then
		local fileID = msg.text
		self.voicePlayerID = msg.playerID
		self.voiceTime = 10 - msg.messID
		if CIMManager then CIMManager:getInstance():downloadRecord(fileID) end
	end
end

function LCMJTopLayer:onDownloadCallBack(fileID)
	if not self.voicePlayerID then return end
	local pos = self.deskStatusMgr:getPosByPlayerID(self.voicePlayerID)
	if not pos then return end
	local avatar = self.deskView:getAvatarByPos(pos)
	if not avatar then return end

	avatar:setVoiceVisible(true)
	self:audioStop()

	scheduleOnce(self.scene,function()
		avatar:setVoiceVisible(false)
		self:audioStart()
	end,self.voiceTime + 0.5)
end

function LCMJTopLayer:onUploadCallBack(fileID)
	self.fileID = fileID
	local pb = LCNetManager:createMessage(eDefMsg.GameChat)
	pb._msg.types 	 = 4
	pb._msg.messID 	 = self.recordTime
	pb._msg.text 	 = self.fileID
	-- pb._msg.textType = 0
	LCNetManager:send(pb)
end


function LCMJTopLayer:chatShow(msg)
	if msg.types==eChatType.icon1 then
		local pos = self.deskStatusMgr:getPosByPlayerID(msg.playerID)
		if not pos then return end
		local avatar = self.deskView:getAvatarByPos(pos)
		if not avatar then return end

		local data=eResImage.s_chatIcon[eChatType.icon1]
		self.animateManger:playGiftEffect(avatar, data[msg.messID].effName, function ()
		end, 1.6, eResGameCommon.effect.face)

	elseif msg.types==eChatType.talk or msg.types==eChatType.edit then
		local pos, direct, scale=self:getMessPos(msg.playerID)
		if not pos then return end
		local sex = ePlayerGender.male
		local player = self.deskStatusMgr:getPlayerByID(msg.playerID)
		if player then
			sex = player.gender
		end
		self.animateManger:talkTextShow(pos, msg, direct, sex ,function ()
		end, scale)
		
	elseif msg.types==eChatType.voice then
		self:onMsgVoiceChat(msg)
	end
end

function LCMJTopLayer:onInviteBtnClicked()
	if CSDKManager:getInstance():isWXAppInstalled() then
		local deskID = self.deskStatusMgr:getRoomID()
		CSDKManager:getInstance():share(cjson.encode({
        	title = "千胜河南麻将",
        	description = "千胜麻将，好友约局必备神器，我在 " .. deskID .. " 房间等你~",
        	webpageUrl = "http://oow62av5c.bkt.clouddn.com/hnmj.html",
        	type = "link",
        	scene = "session" -- "timeline"
    	}))
	else
		LCToastHUD.new("您未安装微信"):showInScene()
	end
end

function LCMJTopLayer:getMessPos(playerID)
	local pos = self.deskStatusMgr:getPosByPlayerID(playerID)
	if not pos then return end
	local avatar = self.deskView:getAvatarByPos(pos)
	if not avatar then return end

	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	local content=avatar:getContentSize()
    local p = CE_GET_WORLD_POSITION(avatar)

    local offX = 0
    local offY = 0
    local direct = -1

    local switch = {
	   [eMJPType.bottom] = function() direct = -1 offX = -70 offY = 30 end,
		[eMJPType.right] = function() direct =  1 offX = -70 offY = 30 end,
		  [eMJPType.top] = function() direct = -1 offX = -70 offY = 30 end,
		 [eMJPType.left] = function() direct = -1 offX = -70 offY = 30 end,
	}
	local f = switch[pType] 
	if f then f() end

    p.x = p.x + content.width /2 + offX
    p.y = p.y + content.height/2 + offY
	return p, direct
end

return LCMJTopLayer
