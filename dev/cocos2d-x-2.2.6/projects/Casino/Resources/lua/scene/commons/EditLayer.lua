
local LCEditLayer = class("LCEditLayer", function(...) return CCLayerColor:create(_APPCOLOR_RGB(0, 0, 0, 175.5)) end)

LCEditLayer.__index = LCEditLayer

function LCEditLayer:create(...)
	local ret = LCEditLayer.new()
	if not ret:init(...) then return nil end
	return ret
end

function LCEditLayer:init(scene, key)
	key=key or {}
	CC_SYNTHESIZE(self,"scene", scene)
	CC_SYNTHESIZE(self,"key", key)
	CC_SYNTHESIZE(self,"size", key.size or CCDirector:sharedDirector():getWinSize())
	CC_SYNTHESIZE(self,"textSize", key.textSize or CCSizeMake(900, 45))
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"textMaxLength", 44)
	CC_SYNTHESIZE(self,"cancle", false)
	CC_SYNTHESIZE(self,"fondSize", 19)
	CC_SYNTHESIZE(self,"pos", key.pos or ccp(self.size.width/2,
	self.size.height-self.textSize.height/2))
	self:changeWidthAndHeight(self.size.width,self.size.height)


	local function editBoxTextEventHandle(strEventName,pSender)
		local edit = tolua.cast(pSender,"CCEditBox")
		local text = edit:getText()
		if strEventName == "began" then
			self.cancle=false
		 elseif strEventName == "return" then
			if text and #text>0 and not self.cancle then
				self.scene:showLoading(_("请稍等..."))
				local playerData = LCAppGlobal:getLoginPlayer()
				local pb = LCNetManager:createMessage(eDefMsg.Broadcast)
				pb._msg.playerID = playerData.playerID
				pb._msg.content = text
				LCNetManager:send(pb)
				edit:setText("")
			else
				--self.scene:showConfirmMessage(_("亲，您还什么都没写呢哦!"))
			end
			GDebug("------ended")
			scheduleOnce(self.scene, function (...)
				if self.callback then self.callback() end
			end, 0.2)
		elseif strEventName == "ended" then
			scheduleOnce(self.scene, function (...)
				if self.callback then self.callback() end
			end, 0.2)
		elseif strEventName == "changed" then
			if CE_GET_STRING_SHOW_LENGTH(text) > self.textMaxLength then
				edit:setText(CE_STRING_CUT_OFF(text,self.textMaxLength))
			end
		end
	end

	local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_chat_whitebox)
	scale9bg:setContentSize(self.textSize)
	local editer = CCEditBox:create(self.textSize, scale9bg)
	editer:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, self.fondSize)
	editer:setFont(DEF_SYSTEM_FONT_NAME, self.fondSize)
	editer:setFontColor(_APPCOLOR_HEX("000000"))
	editer:setMaxLength(self.textMaxLength)
	editer:setReturnType(kKeyboardReturnTypeSend)
	editer:setAnchorPoint(ccp(0.5, 0.5))
	editer:setPosition(self.pos)
	editer:registerScriptEditBoxHandler(editBoxTextEventHandle)
	editer:setPlaceHolder(_("说点什么呢？"))
	self:addChild(editer)
	editer:touchDownAction(editer, CCControlEventTouchDown)

	self:registerScriptTouchHandler(function( ... )
		return self:onTouch(...)
	end, false, 1, true)
	self:setTouchEnabled(true)
	return true
end

function LCEditLayer:onTouch(eventType,x,y)
	local glPos = ccp(x, y)

	if eventType == "began" then

		return true
	elseif eventType == "moved" then

	elseif eventType == "ended" then
	GDebug("touch end")
		if self.callback then self.callback() end
	end
end

function LCEditLayer:remove()
	--local scene = LCSceneTransit:getRunningScene()
	scheduleOnce(self.scene, function (...)
		self:removeFromParentAndCleanup(true)
	end, 0.2)
end

return LCEditLayer
