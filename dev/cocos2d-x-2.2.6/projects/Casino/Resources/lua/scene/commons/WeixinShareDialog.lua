local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local WeixinShareDialog = class("WeixinShareDialog", DialogDIY)

function WeixinShareDialog:ctor(key)
    WeixinShareDialog.super.ctor(self, key)
    self._title = key.title
    self._sessionCallback = key.sessionCallback
    self._timelineCallback = key.timelineCallback
end

function WeixinShareDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("weixinShare", proxy, self)
    self:addChild(layer)

    if self._title then
        self.ccb_title:setString(self._title)
    end

    return layer
end

function WeixinShareDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function WeixinShareDialog:ccb_onSession(control, controlEvent)
    if self._sessionCallback then
        self._sessionCallback()
    end
end

function WeixinShareDialog:ccb_onTimeline(control, controlEvent)
    if self._timelineCallback then
        self._timelineCallback()
    end
end

return WeixinShareDialog
