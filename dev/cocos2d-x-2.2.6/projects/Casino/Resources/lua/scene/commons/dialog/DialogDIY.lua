local DialogDIY = class("DialogDIY", function() return CCLayerColor:create() end)

local maskColor = ccc3(0, 0, 0)

function DialogDIY:ctor(key)
    key                 = key or {}

    self.showMask       = key.showMask
    self.disableBlur    = key.disableBlur or true
    self.tapToDismiss   = key.tapToDissmiss or false
    self.maskColor      = key.maskColor or maskColor
    self.dismissCallback = key.dismissCallback
    self.closeButton    = nil
    self.contentView_   = nil
    self.isRemove       = false
    self.isAdd          = false

	if self.showMask == nil then self.showMask = true end

    local function onEvent(event)
        if event == "enter" then self:onEnter()
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onEvent)
end

function DialogDIY:onEnter()
    self.isAdd = true
    self.isRemove = false
    local scene = LCSceneTransit:getRunningScene()
	scene.dialogs[self] = self
    self:initCommonView()
    self.contentView_ = self:initContentView()
    self:relayoutView()
end

function DialogDIY:onExit()
	if not self then return end
    self.isRemove = true
    self.isAdd = false
end

function DialogDIY:createBlurSprite()
    local renderTexture = _APP_RENDER_TEXTURE()
    renderTexture:begin()
    CCDirector:sharedDirector():getRunningScene():visit()
    renderTexture:endToLua()
    local sprite = renderTexture:doFastBlur(20,3)
    return sprite
end

function DialogDIY:initCommonView()
    local function onTouch(eventType,x,y)
		if eventType == "ended" then
            GDebug("wz onTouch")
			if not self.tapToDismiss then return end
			self:dismissSelf()
		end
		return true
	end
	self:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
	self:setTouchEnabled(true)

    if self.showMask then
        self:setColor(self.maskColor)
        self:setOpacity(100)
    end
    if not self.disableBlur then
        local winSize = CCDirector:sharedDirector():getWinSize()
        local blurSprite = self:createBlurSprite()
        blurSprite:setPosition(winSize.width/2, winSize.height/2)
        self:addChild(blurSprite)
    end
end

function DialogDIY:initContentView()
    GDebug("need to override")
end

function DialogDIY:relayoutView()
    if not self.contentView_ then GError("no contentView.") return end
    local contentSize = self.contentView_:getContentSize()
    local winSize = CCDirector:sharedDirector():getWinSize()
    self.contentView_:setPosition((winSize.width - contentSize.width) / 2, (winSize.height - contentSize.height) / 2)
end

function DialogDIY:closeButtonClicked(sender)
	LCMusicBox:playControlSounds()
	self:dismissSelf()
end

function DialogDIY:showInScene(zOrder)
    if self.isAdd then GError("dialog has been added.") return self end
    local scene = LCSceneTransit:getRunningScene()
    if type(zOrder) ~= "number" then zOrder = 0 end
	scene:addChild(self, zOrder or 0)
    return self
end

function DialogDIY:dismissSelf()
    local scene = LCSceneTransit:getRunningScene()
    scene.dialogs[self] = nil
    if not self.isRemove then
        if self.dismissCallback then
            self.dismissCallback(self)
        end
        self:removeFromParentAndCleanup(true)
    end
end

function DialogDIY:dealPacket(...)
    GDebug("need to override")
end

return DialogDIY
