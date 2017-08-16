
LCPlayerAvatar = class("LCPlayerAvatar",function(...) return CCLayerColor:create(...) end)
function LCPlayerAvatar:create(...)
	local ret = LCPlayerAvatar.new()
	if not ret:init(...) then return  nil end
	return ret
end

--[[
	url,   					--头像链接
	callback,				--点击回调
	swallow,				--是否吞没事件默认false
	defaultAvatar,			--默认头像
	rect,					--头像尺寸
	playerID,				--头像玩家ID
	clipMode,				--裁切模板，默认eResImage.tests圆形
	isNotClip，				--头像是否裁切
	isShowBG，				--头像是否包框
	isShowWinBG,			--头像是否增加金色包框背景（一般用于游戏内赢钱）
	vipPoint = 100,			--玩家vip点
	vipOffset = ccp(0,-6)	--头像偏移量
]]

function LCPlayerAvatar:init(url, callback, key)
	key=key or {}
	CC_SYNTHESIZE(self,"url",			url)			-- "1" or "http://baidu.com/a.png"
	CC_SYNTHESIZE(self,"callback",		callback)  -- function(avatar,url)
	CC_SYNTHESIZE(self,"swallow", 		key.swallow)
	CC_SYNTHESIZE(self,"rect", 			key.rect or eAvatarSize)
	CC_SYNTHESIZE(self,"playerID", 		key.playerID)
	CC_SYNTHESIZE(self,"clipMode", 		key.clipMode or eResImage.tests)
	CC_SYNTHESIZE(self,"isShowBG", 		key.isShowBG)
	CC_SYNTHESIZE(self,"isShowWinBG",	key.isShowWinBG)
	CC_SYNTHESIZE(self,"isNotClip", 	key.isNotClip)
	CC_SYNTHESIZE(self,"vipPoint", 		key.vipPoint or 0)
	CC_SYNTHESIZE(self,"vipOffset", 	key.vipOffset or ccp(0,0))
	CC_SYNTHESIZE(self,"isMJ", 			key.isMJ)   --是否是麻将的avatar
	CC_SYNTHESIZE(self,"hidScore", 		key.hidScore)   --是否是麻将的avatar
	--view
	CC_SYNTHESIZE(self,"avatarSprite", nil)			--头像元素
	CC_SYNTHESIZE(self,"defaultAvatar", 1)
	CC_SYNTHESIZE(self,"downloading", false)
	CC_SYNTHESIZE(self,"isGray", false)				--是否置灰状态
	CC_SYNTHESIZE(self,"isCanTouch", true)
	CC_SYNTHESIZE(self,"menuPopupItem", nil)
	CC_SYNTHESIZE(self,"player",  nil)
	CC_SYNTHESIZE(self,"vipSprite", nil)
	CC_SYNTHESIZE(self,"vipLabel", nil)


	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)

	self:changeWidthAndHeight(self.rect.width, self.rect.height)

	self:_initView()
	self:_updateVIP()
	self:updateURL(self.url)

	return true
end

-- private
function LCPlayerAvatar:_initView()
	local avatarSprite = CCSprite:create()
	-- avatarSprite:setAnchorPoint(ccp(0,0))
	avatarSprite:setPosition(ccp(self.rect.width/2,self.rect.height/2))
	self.avatarSprite = avatarSprite
	local pic = self.defaultAvatar
	if not pic then
		self.defaultAvatar=1
		pic=eAvatarRec[tonumber(self.defaultAvatar)].icon
	end
	self:_setDisplayFrameName(pic)

	if not self.isNotClip then
		self:addChild(_CREATE_STENCILED_IMAGE(self.clipMode, avatarSprite))
	else
		self:addChild(avatarSprite)
	end

	if self.isShowBG and not self.isShowWinBG then
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("c_avatar_other.png")
		local bg = CCSprite:create()
		bg:setDisplayFrame(frame)
		bg:setPosition(avatarSprite:getPosition())
		self:addChild(bg)
	end

	if self.isShowWinBG then
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("c_avatar_other_win.png")
		local bg = CCSprite:create()
		bg:setDisplayFrame(frame)
		bg:setPosition(avatarSprite:getPosition())
		self:addChild(bg)
	end

	if self.isMJ then
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_head_bg_score.png")
		local bg = CCSprite:create()
		bg:setDisplayFrame(frame)
		bg:setAnchorPoint(0,1)
		bg:setPosition(0, 30)
		bg:setZOrder(avatarSprite:getZOrder() - 1)
		self:addChild(bg)

		-- self.goldLable = CCLabelBMFont:create("1000", _APP_UI_FONT(eUIFont.fonts_money_own))
		self.goldLable = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 70)
		self.goldLable:setColor(_APPCOLOR_HEX("f9fad3"))
		self.goldLable:enableStroke(ccc3(255, 255, 255), 1, true)
		bg:addChild(self.goldLable)
		self.goldLable:setPosition(bg:getContentSize().width/2,bg:getContentSize().height/2 - 20)
		self.goldLable:setScale(0.8)

		local frame2 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_head_frame.png")
		local bg2 = CCSprite:create()
		bg2:setDisplayFrame(frame2)
		bg2:setPosition(avatarSprite:getPosition())
		self:addChild(bg2)

		local frame3 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_head_zhuang.png")
		self.bankerIcon = CCSprite:create()
		self.bankerIcon:setDisplayFrame(frame3)
		self.bankerIcon:setPosition(220,220)
		self:addChild(self.bankerIcon)
		self.bankerIcon:setVisible(false)

		local frame4 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("messga.png")
		self.voiceIcon = CCSprite:create()
		self.voiceIcon:setDisplayFrame(frame4)
		self.voiceIcon:setPosition(120,260)
		self:addChild(self.voiceIcon)
		self.voiceIcon:setVisible(false)
		self.voiceIcon:setScale(0.6)
		if self.hidScore then
			bg:setVisible(false)
		else
			bg:setVisible(true)
		end

		local frame5 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_pao0.png")
		self.paoScoreIcon = CCSprite:create()
		self.paoScoreIcon:setDisplayFrame(frame5)
		self.paoScoreIcon:setScale(3.2)
		self.paoScoreIcon:setPosition(150,40)
		self.paoScoreIcon:setVisible(false)
		self:addChild(self.paoScoreIcon)
	end

	self:touchInit()
end

function LCPlayerAvatar:setVoiceVisible(isVisible)
	isVisible = isVisible or false
	self.voiceIcon:setVisible(isVisible)
end

function LCPlayerAvatar:setMJBanker(isBanker)
	if not self.isMJ or not self.bankerIcon then return end
	isBanker = isBanker or false
	self.bankerIcon:setVisible(isBanker)
end

function LCPlayerAvatar:setMJScore(gold)
	if not self.isMJ or not self.goldLable then return end
	self.goldLable:setString(gold)
end

function LCPlayerAvatar:setMJPao(isVisible,paoScore)
	if not self.paoScoreIcon or not self.paoScoreIcon then return end
	isVisible = isVisible or false
	self.paoScoreIcon:setVisible(isVisible)

	local name = "mj_pao"..paoScore..".png"
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(name)
	self.paoScoreIcon:setDisplayFrame(frame)
end

function LCPlayerAvatar:_setDisplayFrameName(frameName)
	self.avatarSprite:setScale(1)
	local iconSpriteFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(frameName)
	if not iconSpriteFrame then
		GError("should not here")
		return
	end
	if not self.swallow and self.menuPopupItem then
		self.menuPopupItem:setPosition(ccp(self.rect.width/2  ,self.rect.height/2 ))
	end
	self.avatarSprite:setDisplayFrame(iconSpriteFrame)
end

function LCPlayerAvatar:_getRemoteAvatar(url)
	if not url then return end
	local fileName = string.gsub(url,"/","")
	fileName = string.gsub(fileName,":","")

	local avatarPath = CCFileUtils:sharedFileUtils():getWritablePath() .. fileName ..".png"
	local file,err = io.open(avatarPath,"r+")
	if file ~= nil then
		self:_setTexture(CCTextureCache:sharedTextureCache():addImage(avatarPath))
		file:close()
	else
		local httpRequest = HttpRequestExt:create()
		self.downloading = true
		httpRequest:sendGet(url,function(code,tag,strError,spr)
			self.downloading = false
			if spr == nil then
				GDebug(strError)
			else
				local sprite = tolua.cast(spr,"CCSprite")
				if self._setTexture then self:_setTexture(sprite:getTexture()) end
			end
		end)
	end
end

function LCPlayerAvatar:touchInit()
	local contentSize = self.rect
	if self.swallow == nil or self.swallow then
		local pTitleButtonLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 30)
		pTitleButtonLabel:setDimensions(contentSize)
		local spriteN = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
		local controlButton = CCControlButton:create(pTitleButtonLabel,spriteN)
		local function touchUpInsideAction()
			if not self.isCanTouch then return end
			LCMusicBox:playControlSounds()
			if self.callback then self.callback(self,self.url, self.player) end
		end
		controlButton:addHandleOfControlEvent(touchUpInsideAction,CCControlEventTouchUpInside)
		controlButton:setPreferredSize(contentSize)
		controlButton:setZoomOnTouchDown(false)
		controlButton:setAnchorPoint(ccp(0.5, 0.5))
		controlButton:setPosition(ccp(contentSize.width/2, contentSize.height/2))
		self:addChild(controlButton)
	else
		local menuItemArray = CCArray:create()
		local spriteNoraml = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
		local menuPopupItem = CCMenuItemSprite:create(spriteNoraml, spriteNoraml, spriteNoraml)
		menuPopupItem:ignoreAnchorPointForPosition(false)
		menuPopupItem:setContentSize(contentSize)
		menuPopupItem:setAnchorPoint(ccp(0.5, 0.5))
		menuPopupItem:setPosition(ccp(contentSize.width/2, contentSize.height/2))
		menuPopupItem:registerScriptTapHandler(function ()
			if not self.isCanTouch then return end
			LCMusicBox:playControlSounds()
			if self.callback then self.callback(self,self.url, self.player) end
		end)
		menuItemArray:addObject(menuPopupItem)
		local menuPopup = CCMenu:createWithArray(menuItemArray)
		menuPopup:setContentSize(self.rect)
		self.menuPopupItem = menuPopupItem
		menuPopup:ignoreAnchorPointForPosition(false)
		menuPopup:setAnchorPoint(ccp(0.5, 0.5))
		menuPopup:setPosition(ccp(self.rect.width/2, self.rect.height/2))
		self:addChild(menuPopup)
	end
end

function LCPlayerAvatar:_setTexture(texture)
	self.avatarSprite:setTexture(texture)
	local textureSize = texture:getContentSize()
	local scaleWidth = self.rect.width/textureSize.width
	local scaleHeight = self.rect.height/textureSize.height
	local scale = math.min(scaleWidth, scaleHeight)
	self.avatarSprite:setScale(scale)
	self.avatarSprite:setTextureRect(CCRectMake(0,0,textureSize.width,textureSize.height))
	if not self.swallow and self.menuPopupItem then
		local deltaX = (self.rect.width - textureSize.width)
		local deltaY = (self.rect.height - textureSize.height)

		self.menuPopupItem:setPosition(ccp(self.rect.width/2 + deltaX/2 ,self.rect.height/2 + deltaY/2 ))
		-- self.menuPopupItem:setPosition(ccp(textureSize.width/2,textureSize.height/2))
	end
end

function LCPlayerAvatar:_updateVIP()
end

-- public
function LCPlayerAvatar:updateURL(url, player,vipValue)
	self.player=player
	if player then
		self.playerID=player.playerID
		self.vipPoint = player.vipValue
		self:_updateVIP()
	end
	if vipValue then
		self.vipPoint = vipValue
		self:_updateVIP()
	end
	url=tostring(url or "")
	if not url or url == "" or #url <= 0 then url = self.defaultAvatar end
	-- if url == self.url then return end
	if self.downloading then return end
	self.url = url
	local isRemote = string.find(url, 'http')
	local souLen   = #eAvatarRec-1
	local gender   = 1
	if isRemote then
		if self.playerID and LCAppGlobal:getGameData():getReportData():isHave(self.playerID) then
			self:_setDisplayFrameName(eAvatarRec[self.defaultAvatar].icon)
		else
			self:_getRemoteAvatar(url)
		end
	else
		if not url then GError("LCPlayerAvatar 128 url not nil") end
		local pic=tonumber(url)
		if pic then
			if pic>souLen or pic<0 then
				if player and player.gender and player.gender>0 and player.gender<souLen then
					pic=player.gender
				else
					pic=1
				end
			end
			self:_setDisplayFrameName(eAvatarRec[pic].icon)
		else
			self:_setDisplayFrameName(url)
		end
	end
end

function LCPlayerAvatar:setAvatarScale(scale)
	if not scale then return end
	self:setScale(scale)
end

function LCPlayerAvatar:setGrayEnabled( enabled )
	self.isGray = enabled
	if enabled then self.avatarSprite:enableGrey()
	else self.avatarSprite:defaultShader() end
end
function LCPlayerAvatar:setTouchEnabled( enabled )
	self.isCanTouch = enabled
end

-- virtual
function LCPlayerAvatar:onEnter()

end

function LCPlayerAvatar:onExit()
end

return LCPlayerAvatar
