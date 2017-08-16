local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local FeedbackDialog = class("FeedbackDialog", DialogDIY)

local _initTextArea, _clearFeedback

function FeedbackDialog:ctor(key)
    FeedbackDialog.super.ctor(self, key)
    self._textLabel = nil
    self._editor    = nil
    self._picPath   = ""
end

function FeedbackDialog:initContentView()
    GDebug("wz : 1")
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.feedback, proxy, self)
    self:addChild(layer)

    _initTextArea(self)

    return layer
end

function FeedbackDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function FeedbackDialog:ccb_onPic(control, controlEvent)
    ImagePicker:getInstance():selectPicAndUpload(function(code, result, tag, error, spr)
        GDebug("%s ; %s ;  %s ;  %s", code , result or "", tag or "", DEF_UPLOAD_PATH)
        if result and #result > 0 then
            local splitResult = splitStr(result, '=')
            self._picPath = DEF_AVATAR_BASE_PATH..splitResult[2]
        end
        if spr then
            local sprite = tolua.cast(spr, "CCSprite")
            local oldSprite = self.ccb_pic:getChildByTag(1)
            if oldSprite then oldSprite:removeFromParentAndCleanup(true) end
            self.ccb_pic:addChild(sprite, 1, 1)
            local realSize = self.ccb_picBtn:getContentSize()
            local selfSize = sprite:getContentSize()
            local containerSize = self.ccb_pic:getContentSize()
            sprite:setScaleX(realSize.width/selfSize.width)
            sprite:setScaleY(realSize.height/selfSize.height)
            sprite:setPosition(containerSize.width/2, containerSize.height/2)
        end
    end, DEF_UPLOAD_PATH, DEF_STORAGE_TYPE)
end

function FeedbackDialog:ccb_onSubmit(control, controlEvent)
    local text = self._textLabel:getString()
	if text == "" and self._picPath == "" then
		-- LCMessageBox:showConfirm(LCSceneTransit:getRunningScene(), {text = _("亲，您还什么都没写呢哦!")})
        LCToastHUD.new("亲，您还什么都没写呢！"):showInScene()
		return
	end
    local pb = LCNetManager:createMessage(eDefMsg.Feedback)
    pb._msg.playerID = LCAppGlobal:getLoginPlayer():getId()
    if self._picPath ~= "" then
        pb._msg.imgUrl = self._picPath
    end
    pb._msg.text = text
    LCNetManager:send(pb)

	-- local channel = CSDKManager:getInstance():getChannelID()
	-- local version = DEF_INTERNAL_VERSION
	-- local request = LCHttpClient:feedback(LCAppGlobal:getLoginPlayer():getId(), version, channel, self._picPath, text)
	-- LCHttpClient:sendWithHandler(request, function()
	-- 	GDebug("succ")
	-- 	LCMessageBox:showConfirm(LCSceneTransit:getRunningScene(), {text = _("反馈成功，我们会尽快处理您的问题!"), callback = function ()
    --         _clearFeedback(self)
	-- 	end})
	-- end,function()
	-- 	GDebug("failed")
	-- 	LCMessageBox:showConfirm(LCSceneTransit:getRunningScene(), {text = _("请求失败，请稍后再试")})
	-- end)

    LCSceneTransit.getRunningScene():showLoading("发送中...")
end

function FeedbackDialog:dealPacket(seed, msg)
    if seed == eDefMsg.Feedback then
        LCSceneTransit.getRunningScene():hideLoading()
        if msg.status == 0 then
            _clearFeedback(self)
            LCToastHUD.new("提交成功"):showInScene()
        end
    end
end

-- private
_clearFeedback = function(self)
    local oldSprite = self.ccb_pic:getChildByTag(1)
	if oldSprite then oldSprite:removeFromParentAndCleanup(true) end
	self._picPath = ""
	if self._textLabel then self._textLabel:setString("") end
end

_initTextArea = function(self)
    local scale9bgContent = self.ccb_textBg:getContentSize()
    self._textLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 24)
	self._textLabel:setHorizontalAlignment(kCCTextAlignmentLeft)
	self._textLabel:setDimensions(CCSize(scale9bgContent.width-10, scale9bgContent.height-10))
	self.ccb_textBg:addChild(self._textLabel, 2)
    self._textLabel:setVisible(false)
    self._textLabel:setColor(ccc3(138, 101, 65))
    self._textLabel:setPosition(scale9bgContent.width/2, scale9bgContent.height/2)

    local function editBoxTextEventHandle(strEventName, pSender)
        if strEventName == "began" then
            local text=""
            if self._textLabel then
                text = self._textLabel:getString()
                self._textLabel:setVisible(false)
            end
            self._editor:setText(text)
        elseif strEventName == "changed" then
            if self._textLabel then self._textLabel:setString(self._editor:getText()) end
        elseif strEventName == "ended" or strEventName == "return" then
            if self._textLabel then
                self._textLabel:setVisible(true)
            end
            self._editor:setText("")
        end
    end

    local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
	scale9bg:setContentSize(scale9bgContent)
	self._editor = CCEditBox:create(scale9bgContent, scale9bg)
	self._editor:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, 24)
	self._editor:setFont(DEF_SYSTEM_FONT_NAME, 24)
	self._editor:setFontColor(ccc3(138, 101, 65))
	self._editor:setMaxLength(90)
	self._editor:setReturnType(kKeyboardReturnTypeDone)
	self._editor:setAnchorPoint(0, .5)
	self._editor:setContentSize(scale9bgContent)
	self._editor:setPosition(0, scale9bgContent.height*0.5)
	self._editor:registerScriptEditBoxHandler(editBoxTextEventHandle)
	self.ccb_textBg:addChild(self._editor)
end

return FeedbackDialog
