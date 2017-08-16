local TabsView = require "lua/scene/commons/TabsView"

local InnerTabView = class("InnerTabView", TabsView)

-- constants
local DEFAULT_WIDTH = 984
local BTN_HEIGHT = 49
local COVER_HEIGHT = 44
local COLOR_GREY = ccc3(159, 159, 159)
local COLOR_WHITE = ccc3(255, 255, 255)

function InnerTabView:ctor(nameList, totalWidth)
    self.totalWidth = totalWidth or DEFAULT_WIDTH
    self.curIdx = 1
    InnerTabView.super.ctor(self, nameList)
    local bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.shop_label_01)
    bg:setContentSize(self.totalWidth-2, BTN_HEIGHT)
    bg:setAnchorPoint(0.5, 0)
    self:addChild(bg)
end

function InnerTabView:initTab(idx, name, tabNum)
    local btnw, btnh = self.totalWidth / tabNum, BTN_HEIGHT
    local spacex = btnw - 1
    local s9s = CCScale9Sprite:createWithSpriteFrameName(eResImage.shop_label_01)
    local btn = CCControlButton:create(s9s)
    btn:setPreferredSize(CCSizeMake(btnw, btnh))
    local x = -spacex * tabNum / 2 + (idx - .5) * spacex
    btn:setPosition(x, btnh/2)
    btn:setZoomOnTouchDown(false)
    btn:addHandleOfControlEvent(function( ... )
        self.cover:setPositionX(x)
        --[[ -- 移动太卡
        self.cover:stopAllActions()
        self.cover:runAction(CCMoveTo:create(.1, ccp(x, btnh/2)))
        --]]
        LCMusicBox:playControlSounds()
        if idx ~= self.curIdx then
            self.labMap[self.curIdx]:setColor(COLOR_GREY)
            self.curIdx = idx
            self.labMap[idx]:setColor(COLOR_WHITE)
        end
        self:dispatchEvent{name = InnerTabView.EVENT_TAB_CLICKED, index = idx}
    end, CCControlEventTouchUpInside)
    self:addChild(btn)

    local l = CCLabelTTF:create(name, DEF_SYSTEM_FONT_NAME, 22)
    self:addChild(l, 3)
    l:setPosition(x, btnh/2)
    l:enableShadow(CCSizeMake(0, -2), 1, 0, true)
    if idx ~= self.curIdx then
        l:setColor(COLOR_GREY)
    end
    self.labMap[idx] = l

    if not self.cover then
        self.cover = CCScale9Sprite:createWithSpriteFrameName(eResImage.shop_label_02)
        self:addChild(self.cover, 2)
        self.cover:setContentSize(spacex-4, COVER_HEIGHT)
        self.cover:setPosition(x, btnh/2)
    end
    if idx ~= tabNum then
        local gapSprite = CCSprite:createWithSpriteFrameName(eResImage.shop_label_03)
        gapSprite:setPosition(-(self.totalWidth-2)/2 + btnw * idx, btnh/2+2)
        self:addChild(gapSprite, 1)
    end
end

return InnerTabView
