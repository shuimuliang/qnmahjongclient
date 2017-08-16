local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local DialogFullScreen = class("DialogFullScreen", DialogDIY)

local CLOSE_BTN_LEFT_MARGIN = 14
local CLOSE_BTN_TOP_MARGIN = 14
local CLOSE_BTN_WIDTH = 66
local CLOSE_BTN_HEIGHT = 62

function DialogFullScreen:ctor(key)
    key = key or {}
    DialogFullScreen.super.ctor(self, key)

    self.hideClose = key.hideClose or false
    self.showCurrency = key.showCurrency or false
    self.disableBlur = true
    self.scene = LCSceneTransit:getRunningScene()
end

function DialogFullScreen:initCommonView()
    DialogFullScreen.super.initCommonView(self)

    if not self.hideClose then
        self:initCloseBtn()
    end
    if self.showCurrency then
        self:initCurrencyLabel()
    end
end

function DialogFullScreen:initCloseBtn()
    local s = CCScale9Sprite:createWithSpriteFrameName(eResImage.c_menuicon_back)
    self.closeButton = CCControlButton:create(s)
    local btn = self.closeButton
    btn:setPreferredSize(CCSizeMake(CLOSE_BTN_WIDTH, CLOSE_BTN_HEIGHT))
    btn:setAnchorPoint(ccp(0.5, 0.5))
    btn:addHandleOfControlEvent(function ()
        self:closeButtonClicked()
    end, CCControlEventTouchUpInside)
    self:addChild(btn, 1)
end

function DialogFullScreen:relayoutView()
    DialogFullScreen.super.relayoutView(self)
    if not self.hideClose then
        local winSize = CCDirector:sharedDirector():getWinSize()
        local btnx = CLOSE_BTN_LEFT_MARGIN + CLOSE_BTN_WIDTH / 2
    	local btny = winSize.height - CLOSE_BTN_HEIGHT / 2 - CLOSE_BTN_TOP_MARGIN
        self.closeButton:setPosition(btnx, btny)
    end
end

function DialogFullScreen:initCurrencyLabel()
    local bg = CCSprite:createWithSpriteFrameName(eResImage.shop_frame_02)
    bg:setAnchorPoint(ccp(1, 1))
    local bgSize = bg:getContentSize()
    local winSize = CCDirector:sharedDirector():getWinSize()
    bg:setPosition(winSize.width, winSize.height * .96)
    self:addChild(bg, 1)

    local chip = CCSprite:createWithSpriteFrameName(eResImage.treasures[ePairKey.GOLD].smallIcon)
    bg:addChild(chip)
    local chipSize = chip:getContentSize()
    local x = chipSize.width - 5
    local y = bgSize.height/2 + 2
    chip:setPosition(x, y)

    local l =  CCLabelBMFont:create("", _APP_UI_FONT(eUIFont.fonts_money_own))
    l:setAnchorPoint(ccp(0, .5))
    l:setScale(0.28)
    l:setPosition(x + chipSize.width, y)
    self.currencyLabel = l
    bg:addChild(l)

    local goldCount = LCAppGlobal:getLoginPlayer():getGoldCount()
    self:setGoldCount(goldCount)
end

function DialogFullScreen:setGoldCount(num)
    if not num then return end
    if self.currencyLabel then
        self.currencyLabel:setString(_NICE_MONEY(num))
    end
end

return DialogFullScreen
