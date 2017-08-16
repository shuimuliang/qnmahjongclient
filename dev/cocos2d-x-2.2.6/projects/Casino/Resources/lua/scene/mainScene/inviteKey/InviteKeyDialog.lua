local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local InviteKeyDialog = class("InviteKeyDialog", DialogDIY)

function InviteKeyDialog:ctor(key)
    InviteKeyDialog.super.ctor(self, key)
    self._editBox = nil
    self._keys = nil
end

function InviteKeyDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("inviteKeyDialog", proxy, self)
    self:addChild(layer)

    local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
    local layerSize = self.ccb_editBg:getContentSize()
    scale9bg:setContentSize(layerSize)
    local editBox = CCEditBox:create(layerSize, scale9bg)
    editBox:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, 30)
    editBox:setPlaceHolder("点击输入邀请码")
    local color = ccc3(138, 101, 65)
    editBox:setPlaceholderFontColor(color)
    editBox:setFont(DEF_SYSTEM_FONT_NAME, 30)
    editBox:setFontColor(color)
    editBox:setMaxLength(40)
    editBox:setInputMode(kEditBoxInputModeNumeric)
    editBox:setReturnType(kKeyboardReturnTypeDone)
    editBox:setAnchorPoint(ccp(0, 0.5))
    editBox:setContentSize(scale9bg:getContentSize())
    editBox:setPosition(0, layerSize.height / 2)
    self.ccb_editBg:addChild(editBox)
    self._editBox = editBox

    return layer
end

local error_code = {
    [601] = "邀请码无效",
    [602] = "绑定邀请码失败",
}
function InviteKeyDialog:dealPacket(seed, msg)
    if seed == eDefMsg.InviteCode then
        LCSceneTransit.getRunningScene():hideLoading()
        local status = msg.status
        local highID = msg.highID
        local coins = msg.coins
        if status == eErrorCode.succ then
            LCAppGlobal:setHighID(highID)
            self:dismissSelf()
            LCToastHUD.new("成功绑定"):showInScene()
            LCAppGlobal:getLoginPlayer():setCoins(coins)
        else
            LCMessageBox:showConfirm(LCSceneTransit.getRunningScene(), {
                text = error_code[status] or _ID2WORD(status),
            })
        end
    end
end

function InviteKeyDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function InviteKeyDialog:ccb_onSubmit(control, controlEvent)
    local key = self._editBox:getText()
    GDebug("%s", key)
    if key == "" then
        LCToastHUD.new("请先填写邀请码"):showInScene()
        return
    end
    if tonumber(key) == LCAppGlobal:getLoginPlayer():getId() then
        LCToastHUD.new("不能填写自己"):showInScene()
        return
    end
    LCSceneTransit.getRunningScene():showLoading("绑定中...")
    local pb = LCNetManager:createMessage(eDefMsg.InviteCode)
    pb._msg.code = key
    LCNetManager:send(pb)
end

return InviteKeyDialog
