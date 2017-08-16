local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local LCActivityDialog = class("LCActivityDialog", DialogDIY)

function LCActivityDialog:ctor(...)
    LCActivityDialog.super.ctor(self, ...)
end

function LCActivityDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("activityDialog", proxy, self)
    self:addChild(layer)

    local wchatID = LCAppGlobal:getCSWChatID()
    local str = "联系微信："..wchatID
    self.ccb_wchatIDLab:setString(str)

    return layer
end

function LCActivityDialog:ccb_onClose()
    self:dismissSelf()
end

return LCActivityDialog
