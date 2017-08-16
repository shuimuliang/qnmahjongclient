local IpPortView = class("IpPortView", function() return CCNode:create() end)

local _initEditbox, _updateCheckbox

function IpPortView:ctor()
    local function onEvent(event)
		if event == "enter" then self:onEnter()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)

    self._editBox = nil
    self._idx = 1
end

function IpPortView:onEnter()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("IpPortView", proxy, self)
    self:addChild(layer)

    _initEditbox(self)

    local config = LCUserContext:getIpAndPort()
    self._editBox:setText(config.ip or "")
    self._idx = config.idx

    _updateCheckbox(self)

    return layer
end

function IpPortView:getLoginIp()
    if self._idx == 1 then
        if self._editBox then
            return self._editBox:getText()
        end
    elseif self._idx == 2 then
        return "mjlogindev.kszjh.info"
    elseif self._idx == 3 then
        return "mjlogin.kszjh.info"
    end
    return ""
end

function IpPortView:getLogicIp()
    if self._idx == 1 then
        if self._editBox then
            return string.gsub(self._editBox:getText(), "5001", "5002")
        end
    elseif self._idx == 2 then
        return "mjlogic.kszjh.info"
    elseif self._idx == 3 then
        return "mjlogic.kszjh.info"
    end
    return ""
end

function IpPortView:onExit()
    LCUserContext:saveIpAndPort({idx = self._idx, ip = self._editBox:getText()})
end

function IpPortView:save()
    LCUserContext:saveIpAndPort({idx = self._idx, ip = self._editBox:getText()})
end

function IpPortView:ccb_onSelect(control, controlEvent)
    if control == self.ccb_btnDiy then
        self._idx = 1
    elseif control == self.ccb_btnDev then
        self._idx = 2
    elseif control == self.ccb_btnRelease then
        self._idx = 3
    end
    _updateCheckbox(self)
end

-- private
_initEditbox = function(self)
    local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
    local layerSize = self.ccb_ipBg:getContentSize()
    scale9bg:setContentSize(layerSize)
    local editBox = CCEditBox:create(layerSize, scale9bg)
    editBox:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, 30)
    local color = ccc3(255, 255, 255)
    editBox:setPlaceholderFontColor(color)
    editBox:setFont(DEF_SYSTEM_FONT_NAME, 30)
    editBox:setFontColor(color)
    editBox:setMaxLength(40)
    editBox:setReturnType(kKeyboardReturnTypeDone)
    editBox:setAnchorPoint(ccp(0, 0.5))
    editBox:setContentSize(scale9bg:getContentSize())
    editBox:setPosition(0, layerSize.height / 2)
    self.ccb_ipBg:addChild(editBox)
    self._editBox = editBox
end

_updateCheckbox = function(self)
    self.ccb_diy:setVisible(self._idx == 1)
    self.ccb_dev:setVisible(self._idx == 2)
    self.ccb_release:setVisible(self._idx == 3)
end

return IpPortView
