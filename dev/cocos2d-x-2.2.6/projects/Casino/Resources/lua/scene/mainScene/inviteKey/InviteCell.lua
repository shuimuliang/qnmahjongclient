local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"
local URLSprite     = require "lua/scene/commons/URLSprite"

local InviteCell = class("InviteCell", function() return CCNode:create() end)

InviteCell.EVENT_BTN_CLICKED = "EVENT_BTN_CLICKED"

function InviteCell:ctor()
    EventProtocol.extend(self)

    self._idx = 1

    -- 这个不能放在 onEnter 里，因为 InviteDialog 的 tableview 先调用 update 再 addChild 会找不到 ccb_xxx 变量
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("inviteCell", proxy, self)
    self:addChild(layer)
end

function InviteCell:ccb_onReward(control, controlEvent)
    if control == self.ccb_btn1 then
        self:dispatchEvent{name = InviteCell.EVENT_BTN_CLICKED, idx = self._idx}
        self.ccb_btn1:setEnabled(false)
    elseif control == self.ccb_btn2 then
        self:dispatchEvent{name = InviteCell.EVENT_BTN_CLICKED, idx = self._idx + 1}
        self.ccb_btn2:setEnabled(false)
    end
end

function InviteCell:update(idx, data)
    self._idx = idx
    for i = 1, 2 do
        local info = data[i]
        if not info then
            self["ccb_layer" .. i]:setVisible(false)
            break
        else
            self["ccb_layer" .. i]:setVisible(true)
        end
        local id = info.id
        local nickname = info.nickname
        local avatar = info.avatar
        local state = info.state
        self["ccb_name" .. i]:setString(nickname)
        if state == eInviteAwardState.available then
            self["ccb_btn" .. i]:setEnabled(true)
        elseif state == eInviteAwardState.unavailable then
            self["ccb_btn" .. i]:setEnabled(false)
        end
        local avatarSize = self.ccb_avatar1:getContentSize()
        local a = URLSprite.new(avatar, avatarSize.width, avatarSize.height)
        self["ccb_avatar" .. i]:addChild(a)
        a:setPosition(avatarSize.width/2, avatarSize.height/2)
    end
end

function InviteCell.height()
    return 70
end

return InviteCell
