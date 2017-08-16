local LCPaymentMgr        = require("lua/model/Payment")
local LCBroadcastLayer170 = require("lua/scene/commons/BroadcastLayer170")
local URLSprite           = require "lua/scene/commons/URLSprite"
local InviteKeyDialog     = require "lua/scene/mainScene/inviteKey/InviteKeyDialog"
local LCActivityDialog    = require "lua/scene/mainScene/activity/ActivityDialog"
local LCPageScorllView    = require "lua/scene/sceneUtils/PageScrollView"

local HallScene = class("HallScene", LCSceneBase)

local _registerSocketEvent, _unregisterSocketEvent, _sendDirty
local _registerDataEvent, _unregisterDataEvent, _isBindInviteCode, _initWithSwitches, _btnAction, _broadcast, _isShowActivity

function HallScene:ctor(key)
    key = key or {}
    HallScene.super.ctor(self, key)

    self._firstIn      = key.firstIn
    self._toastString  = key.toastString
    self._handle       = {}
    self._broadcastIdx = 1
end

function HallScene:onEnter()
    HallScene.super.onEnter(self)

    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("HallScene", proxy, self)
    self:addChild(layer)

    _registerSocketEvent(self)

    local s = self.ccb_avatarBg:getContentSize()
    local p = LCAppGlobal:getLoginPlayer()
    local avatar = URLSprite.new(p:getAvatar(), s.width, s.height)
    self.ccb_avatarBg:addChild(avatar)
    avatar:setPosition(s.width/2, s.height/2)
    self.ccb_coinCount:setString(p:getCoins())
    self.ccb_cardCount:setString(p:getCards())

    _registerDataEvent(self, p)
    _initWithSwitches(self)

    self.ccb_broadcastView:setTouchEnabled(false)
    local container = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 24)
    container:setColor(ccc3(236, 217, 158))
    self.ccb_broadcastView:setContainer(container)
    _broadcast(self)

    local s = LCAppGlobal:getGameData():getSwitchMgr()
    local quickLogin = s.closeWeixinLogin()
    if quickLogin then
        --苹果审核广播不可见
        self.ccb_broadcastBg:setVisible(false)
    end

    if self._firstIn then
        LCPaymentMgr:checkMissOrder()
       -- if _isShowActivity(self) then LCActivityDialog.new():showInScene() end
    end

    if self._toastString then
        LCToastHUD.new(self._toastString):showInScene()
    end
    -- _btnAction(self)
    local ad1 = CCSprite:createWithSpriteFrameName("s_buttontransparent.png")
    ad1:setAnchorPoint(0, 0)
    local ad2 = CCSprite:createWithSpriteFrameName("s_buttontransparent.png")
    ad2:setAnchorPoint(0, 0)
    local pageView = LCPageScorllView.new({ad1, ad2}, {})
    self.ccb_adBg:addChild(pageView)
    local pageSize = ad1:getContentSize()
    local adBgSize = self.ccb_adBg:getContentSize()
    pageView:setPosition((adBgSize.width-pageSize.width)/2, (adBgSize.height-pageSize.height)/2+5)
    local curPage = 1
    local step = 1
    local maxPage, minPage = 2, 1

    if not quickLogin then
        --苹果审核则公告栏是静态的
        schedule(self.ccb_adBg, function()
            local nextPage = curPage + step
            if nextPage > maxPage or nextPage < minPage then step = -step end
            curPage = curPage + step
            pageView:scrollToPage(curPage)
        end, 5)
    else
        pageView:setTouchEnabled(false)
    end
end

function HallScene:onExit()
    _unregisterSocketEvent(self)
    _unregisterDataEvent(self)
    HallScene.super.onExit(self)
end

function HallScene:dealPacket(seed, msg, uiUpdater)
    if seed == eDefMsg.EnterRoom then
        self:hideLoading()
        self:processEnterRoom(msg)
    elseif seed == eDefMsg.ShopList then
        self:hideLoading()
    elseif seed == eDefMsg.Dirty then
        self:hideLoading()
    end
    HallScene.super.dealPacket(self, seed, msg, uiUpdater)
end

function HallScene:ccb_onAvatar(control, controlEvent)
    local p = LCAppGlobal:getLoginPlayer()
    require "lua/scene/mainScene/playerInfo/PlayerInfoDialog".new({id = p:getId(), avatar = p:getAvatar(), nickname = p:getNickname(), ip = p:getIp()}):showInScene()
end

function HallScene:ccb_onShop(control, controlEvent)
    if _isBindInviteCode() then
        require "lua/scene/mainScene/shop/ShopDialog".new():showInScene()
    else
        InviteKeyDialog.new():showInScene()
    end
end

function HallScene:ccb_onGuide(control, controlEvent)
    require "lua/scene/mainScene/help/HelpDialog".new():showInScene()
end

function HallScene:ccb_onHistory(control, controlEvent)
    require "lua/scene/mainScene/history/HistoryDialog".new():showInScene()
end

function HallScene:ccb_onService(control, controlEvent)
    require "lua/scene/mainScene/setting/FeedbackDialog".new():showInScene()
end

function HallScene:ccb_onSetting(control, controlEvent)
    require "lua/scene/mainScene/setting/SettingDialog".new():showInScene()
end

function HallScene:ccb_onActivity(control, controlEvent)
    LCActivityDialog.new():showInScene()
end

function HallScene:ccb_onInvite(control, controlEvent)
    if _isBindInviteCode() then
        require "lua/scene/mainScene/inviteKey/InviteDialog".new():showInScene()
    else
        InviteKeyDialog.new():showInScene()
    end
end

function HallScene:ccb_onOrganize(control, controlEvent)
    require "lua/scene/mainScene/privateRoom/CreateRoomPanel".new():showInScene()
end

function HallScene:ccb_onJoin(control, controlEvent)
    require "lua/scene/mainScene/privateRoom/JoinRoomPanel".new():showInScene()
end

-- private
_sendDirty = function(self)
    self:showLoading(_ID2WORD(12005))
    local pb = LCNetManager:createMessage(eDefMsg.Dirty)
    LCNetManager:send(pb,LCPackOption.new(DEF_INSTANT_TIMEOUT,nil,function( ... )
        self:hideLoading()
        if not self.erroBox then
            self.erroBox = LCMessageBox:showConfirm(self,{text = _("请求失败，请稍后再试"),
            buttonText = _ID2WORD(11001),
            hideClose = true,
            callback = function( ... )
                self.erroBox = nil
            end})
        end
    end))
end

_registerSocketEvent = function(self)
    local gameSocket = LCNetManager:getGameSocket()
	if gameSocket then
		gameSocket:registerEventOnConnect(self.__cname,function( ... )
			self:hideLoading()
			if self.reconnecting then
				self.reconnecting = false
				_sendDirty(self)
			end
		end)
		gameSocket:registerEventDisconnect(self.__cname,function( ... )
			self:checkConnect()
		end)
		gameSocket:registerEventFailedConnect(self.__cname,function( ... )
			self:checkConnect()
		end)
	end
end

_unregisterSocketEvent = function(self)
    local gameSocket = LCNetManager:getGameSocket()
	local tag = self.__cname;
	if gameSocket then
		gameSocket:unregisterEventDisconnect(tag)
		gameSocket:unregisterEventFailedConnect(tag)
		gameSocket:unregisterEventOnConnect(tag)
	end
end

_registerDataEvent = function(self, p)
    self._handle.updateCoinsAfter = p:addEventListener(LCPlayerData.EVENT_UPDATE_COINS_AFTER, function(event)
        self.ccb_coinCount:setString(event.coins)
    end)
    self._handle.updateCardsAfter = p:addEventListener(LCPlayerData.EVENT_UPDATE_CARDS_AFTER, function(event)
        self.ccb_cardCount:setString(event.cards)
    end)
    local broadcastMgr = LCAppGlobal:getGameData():getBroadCastMgr()
    self._handle.broadcast = broadcastMgr:addEventListener(broadcastMgr.EVENT_BROADCAST, function(event)
        _broadcast(self)
    end)
end

_unregisterDataEvent = function(self)
    if self._handle.updateCoinsAfter then
        LCAppGlobal:getLoginPlayer():removeEventListener(LCPlayerData.EVENT_UPDATE_COINS_AFTER, self._handle.updateCoinsAfter)
        self._handle.updateCoinsAfter = nil
    end
    if self._handle.updateCardsAfter then
        LCAppGlobal:getLoginPlayer():removeEventListener(LCPlayerData.EVENT_UPDATE_CARDS_AFTER, self._handle.updateCardsAfter)
        self._handle.updateCardsAfter = nil
    end
    if self._handle.broadcast then
        local broadcastMgr = LCAppGlobal:getGameData():getBroadCastMgr()
        broadcastMgr:removeEventListener(broadcastMgr.EVENT_BROADCAST, self._handle.broadcast)
        self._handle.broadcast = nil
    end
end

_isBindInviteCode = function()
    local s = LCAppGlobal:getGameData():getSwitchMgr()
    if s.closeWeixinLogin() then return true end
    return LCAppGlobal:getHighID() ~= 0
end

_isShowActivity = function(self)
    local s = LCAppGlobal:getGameData():getSwitchMgr()
    if s.closeWeixinLogin() then return false end
    local wchatID = LCAppGlobal:getCSWChatID()
    if wchatID ~= "" then
        return true
    end
    return false
end

_initWithSwitches = function(self)
    local s = LCAppGlobal:getGameData():getSwitchMgr()
    self.ccb_activityBtn:setVisible(_isShowActivity(self))
    self.ccb_inviteBtn:setVisible(not s.closeInvite())
end

_btnAction = function(self)
    local x, y = self.ccb_enterBtn:getPosition()
    self.ccb_enterBtn:runAction(CCRepeatForever:create(Transition.sequence{
        CCEaseOut:create(CCMoveTo:create(2.5, ccp(x, y+6)), 1),
        CCMoveTo:create(2, ccp(x, y)),
    }))
    x, y = self.ccb_joinBtn:getPosition()
    self.ccb_joinBtn:runAction(CCRepeatForever:create(Transition.sequence{
        CCEaseOut:create(CCMoveTo:create(2.5, ccp(x, y+6)), 1),
        CCMoveTo:create(2, ccp(x, y)),
    }))
end

_broadcast = function(self)
    local textArr = LCAppGlobal:getGameData():getBroadCastMgr().getText()
    if table.isEmpty(textArr) then return end
    if self._broadcastIdx > #textArr then
        self._broadcastIdx = 1
    end
    local container = self.ccb_broadcastView:getContainer()
    container:setString(textArr[self._broadcastIdx])
    local speed = 100
    local viewSize = self.ccb_broadcastView:getViewSize()
    local labelSize = container:getContentSize()
    container:stopAllActions()
    container:setPosition(viewSize.width, (viewSize.height-labelSize.height)/2)

    local time = (viewSize.width + labelSize.width) / speed
    container:runAction(Transition.sequence{
        CCMoveTo:create(time, ccp(-labelSize.width, (viewSize.height-labelSize.height)/2)),
        CCCallFunc:create(function() _broadcast(self) end),
    })
    self._broadcastIdx = self._broadcastIdx + 1
    if self._broadcastIdx > #textArr then
        self._broadcastIdx = 1
    end
end

return HallScene
