local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local UserLicenseDialog = class("UserLicenseDialog", DialogDIY)

function UserLicenseDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.EULADialog, proxy, self)
    self:addChild(layer)

    local viewSize = self.ccb_scrollView:getViewSize()
    local contentSize = self.ccb_scrollView:getContentSize()
    self.ccb_scrollView:setContentOffset(ccp(0, viewSize.height - contentSize.height))

    return layer
end

function UserLicenseDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

return UserLicenseDialog
