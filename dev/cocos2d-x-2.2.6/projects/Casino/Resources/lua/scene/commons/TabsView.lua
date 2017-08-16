local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local TabsView = class("TabsView", function()
    local node = CCNode:create()
    node:setAnchorPoint(.5, 0)
    return node
end)

-- events
TabsView.EVENT_TAB_CLICKED = "EVENT_TAB_CLICKED"

-- constants
local TOTAL_WIDTH = 875
local BTN_HEIGHT = 50

-- {"name1", "name2", "name3", "name4"}

function TabsView:ctor(params)
    if not params then GError("no params in tabs view.") return true end
    EventProtocol.extend(self)
    self.cover = nil
    self.labMap = {}
    self.newTags = {}

    self.tabNum = #params

    for i, name in ipairs(params) do
        self:initTab(i, name, self.tabNum)
    end
    self:selectLabel(1)
end

function TabsView:selectLabel(index,isCover)
    for i,v in pairs(self.labMap) do
        local color = _APPCOLOR_HEX("8d8c8c")
        if i == index then
            color = _APPCOLOR_HEX("FFFFFF")
        end
        self.labMap[i]:setColor(color)

        --切换tab时，字体颜色变化的同时，背景色也跟着变化
        if isCover and self.cover then
            local btnw, btnh = TOTAL_WIDTH / self.tabNum, BTN_HEIGHT
            local spacex = btnw - 5
            local x = -spacex * self.tabNum / 2 + (index - .5) * spacex
            self.cover:setPositionX(x)
        end
    end
end

function TabsView:initTab(idx, name, tabNum)
    local btnw, btnh = TOTAL_WIDTH / tabNum, BTN_HEIGHT
    local spacex = btnw - 5
    local s9s = CCScale9Sprite:createWithSpriteFrameName(eResImage.tabsView.label_01)
    local btn = CCControlButton:create(s9s)
    btn:setPreferredSize(CCSizeMake(btnw, btnh))
    btn:setAnchorPoint(ccp(.5, .5))
    local x = -spacex * tabNum / 2 + (idx - .5) * spacex
    btn:setPosition(x, btnh/2)
    btn:setZoomOnTouchDown(false)
    btn:addHandleOfControlEvent(function( ... )
        self.cover:setPositionX(x)
        self:selectLabel(idx)
        --[[ -- 移动太卡
        self.cover:stopAllActions()
        self.cover:runAction(CCMoveTo:create(.1, ccp(x, btnh/2)))
        --]]
        LCMusicBox:playControlSounds()
        self:dispatchEvent{name = TabsView.EVENT_TAB_CLICKED, index = idx}
    end, CCControlEventTouchUpInside)
    self:addChild(btn)

    local l = CCLabelTTF:create(name, DEF_SYSTEM_FONT_NAME, 19)
    self:addChild(l, 2)
    l:setPosition(x, btnh/2)
    l:enableShadow(CCSizeMake(0, -2), 1, 0, true)
    self.labMap[idx] = l

    if not self.cover then
        self.cover = CCScale9Sprite:createWithSpriteFrameName(eResImage.tabsView.label_02)
        self:addChild(self.cover, 1)
        self.cover:setContentSize(btnw, btnh)
        self.cover:setPosition(x, btnh/2)
    end
end

function TabsView:addTo(parent, zOrder, tag)
    parent:addChild(self, zOrder or 0, tag or 0)
    return self
end

function TabsView:pos(x, y)
    self:setPosition(x, y)
    return self
end

function TabsView:setItemFlagVisible(index,visible)
    if not index then GDebug("no index") return end
    local tabNum = #self.labMap
    if index <= 0 or index > tabNum then
        GDebug("error index " .. index)
        return
    end
    if not self.newTags[index] then
        local btnw, btnh = TOTAL_WIDTH / tabNum, BTN_HEIGHT
        local spacex = btnw - 5
        local btnx = -spacex * tabNum / 2 + (index - .5) * spacex
        local flagx = btnx - btnw / 4
        local sp = CCSprite:createWithSpriteFrameName(eResImage.c_mb_new)
        self:addChild(sp, 2)
        sp:setAnchorPoint(1, 1)
        sp:setPosition(flagx, btnh)
        self.newTags[index] = sp
    end
	if visible~=nil then
		self.newTags[index]:setVisible(visible)
	end
    -- local itemCount = #self.tabs
    -- if not index or index > itemCount  then return end
    -- local flag = self.bubbleFlgMap[index]
    -- if (not flag and not visible) or
    --     (flag and visible) then
    --     return
    -- end
    -- if not flag and visible then
    --     flag = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_exclamation2)
    --     local itemWidth = (self.selfSize.width - eUIConst.kTabHeaderLeftGap*2)/math.max(itemCount,self.minSplit)
    --     flag:setPosition(ccp(eUIConst.kTabHeaderLeftGap+(index-0.1)*itemWidth,self.selfSize.height/2))
    --     self:addChild(flag)
    --     self.bubbleFlgMap[index] = flag
    -- end

    -- if flag and not visible then
    --     flag:removeFromParentAndCleanup(true)
    --     self.bubbleFlgMap[index] = nil
    -- end
end

return TabsView
