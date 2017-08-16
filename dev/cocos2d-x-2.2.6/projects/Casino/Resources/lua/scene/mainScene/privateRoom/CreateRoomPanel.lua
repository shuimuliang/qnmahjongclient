local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local TableView = require "lua/scene/sceneUtils/TableView"
local GameCell  = import(".GameCell")

local CreateRoomPanel = class("CreateRoomPanel", DialogDIY)

local MJTypeView = {
    [1] = require "lua/scene/mainScene/privateRoom/HebeiConfig",
    [2] = require "lua/scene/mainScene/privateRoom/ZhengzhouConfig",
    [3] = require "lua/scene/mainScene/privateRoom/PingdingshanConfig",
    [4] = require "lua/scene/mainScene/privateRoom/KaifengConfig",
}

local _updateBtn, _initTableView, _initConfigView, _updateConfigView, _checkGPS

local COLOR_OFF = ccc3(150, 150, 150)
local COLOR_ON = ccc3(255, 255, 255)
local COST_TYPE = {
    coin = 1,
    card = 2,
}
local CHEAT_COLOR_ON  = ccc3(169, 90, 16)
local CHEAT_COLOR_OFF = ccc3(87, 40, 0)

function CreateRoomPanel:ctor(key)
    CreateRoomPanel.super.ctor(self, key)
    self._configView = nil
    self._tableView  = nil
    self._mjTypes    = LCAppGlobal:getGameData():getJushuConfig():getAllMjType()
    -- 不支持的麻将类型不显示
    local temp = {}
    for _, t in ipairs(self._mjTypes) do
        if MJTypeView[t] then
            table.insert(temp, t)
        end
    end
    self._mjTypes = temp

    self._selectIdx  = 1
    self._mjConfig   = LCUserContext:getMJConfig()
    local switchMgr = LCAppGlobal:getGameData():getSwitchMgr()
    if switchMgr.closeWeixinLogin() then
        self._mjTypes = {2}
        self._mjConfig.cheat = false
    end
end

function CreateRoomPanel:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.createPrivateRoom, proxy, self)
    self:addChild(layer)

    local switchMgr = LCAppGlobal:getGameData():getSwitchMgr()
    if switchMgr.closeWeixinLogin() then
        self.ccb_cheatNode:setVisible(false)
    end
    _initConfigView(self)

    _initTableView(self)

    if self._mjConfig.cheat then
        self.ccb_cheatTTF:setColor(CHEAT_COLOR_ON)
        self.ccb_cheat:setVisible(true)
    else
        self.ccb_cheatTTF:setColor(CHEAT_COLOR_OFF)
        self.ccb_cheat:setVisible(false)
    end

    local proxy = CCBProxy:create()
    local tip = CCBuilderReaderLoad("configTip", proxy, self)
    layer:addChild(tip)
    tip:setVisible(false)

    return layer
end

function CreateRoomPanel:ccb_onStart(control, controlEvent)
    if not _checkGPS(self) then
        LCMessageBox:showConfirm(LCSceneTransit.getRunningScene(), {
            text = "请开启GPS定位功能。",
    		callback = function()
                CCEPlugin:requestLocationService()
    		end
        })
        return
    end
    local pb = LCNetManager:createMessage(eDefMsg.EnterRoom)
	pb._msg.roomID = 0
	pb._msg.configs = self._configView:getServerConfig()
    pb._msg.costType = control == self.ccb_coinBtn and COST_TYPE.coin or COST_TYPE.card
    pb._msg.mjType = self._mjTypes[self._selectIdx]
    pb._msg.round = self._configView:getJushu()
    pb._msg.cheat = self._mjConfig.cheat
    pb._msg.latitude = CCEPlugin:getLatitude()
    pb._msg.longitude = CCEPlugin:getLongitude()
	LCNetManager:send(pb)
	LCSceneTransit.getRunningScene():showLoading(_("正在进入房间..."))
end

function CreateRoomPanel:ccb_onCheat(control, controlEvent)
    if controlEvent == CCControlEventTouchUpInside then
        self.ccb_tipBg:setVisible(false)
        self._mjConfig.cheat = not self._mjConfig.cheat
        if self._mjConfig.cheat then
            self.ccb_cheatTTF:setColor(CHEAT_COLOR_ON)
            self.ccb_cheat:setVisible(true)
        else
            self.ccb_cheatTTF:setColor(CHEAT_COLOR_OFF)
            self.ccb_cheat:setVisible(false)
        end
    elseif controlEvent == CCControlEventTouchDown then
        local p = self.ccb_cheatTTF:convertToWorldSpace(ccp(0, 30))
        self.ccb_tipBg:setVisible(true)
        self.ccb_tipBg:setPosition(self.ccb_contentLayer:convertToNodeSpace(p))
        self.ccb_tipTxt:setString("地理位置太近或者IP相同的玩家无法进入同一个牌桌")
    elseif controlEvent == CCControlEventTouchUpOutside then
        self.ccb_tipBg:setVisible(false)
    end
end

function CreateRoomPanel:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function CreateRoomPanel:onExit()
    CreateRoomPanel.super.onExit(self)
    local curType = self._mjTypes[self._selectIdx]
    self._mjConfig.selectedType = curType
    -- self._mjConfig[curType] = self._configView:getLocalConfig()
    LCUserContext:saveMJConfig(self._mjConfig)
end

-- private
_updateBtn = function(self, count)
    local jushuConfig = LCAppGlobal:getGameData():getJushuConfig()
    local mjType = self._mjTypes[self._selectIdx]
    local coinsCost = jushuConfig.getCoinsByTypeJushu(mjType, count)
    local cardsCost = jushuConfig.getCardsByTypeJushu(mjType, count)
    self.ccb_coinsTTF:setString(coinsCost)
    self.ccb_cardsTTF:setString(cardsCost)
    local me = LCAppGlobal:getLoginPlayer()
    local myCoins = me:getCoins()
    local myCards = me:getCards()
    local coinBtnEnabled = myCoins >= coinsCost
    local cardBtnEnabled = myCards >= cardsCost
    self.ccb_coinBtn:setEnabled(coinBtnEnabled)
    self.ccb_cardBtn:setEnabled(cardBtnEnabled)
    local coinColor = coinBtnEnabled and COLOR_ON or COLOR_OFF
    local cardColor = cardBtnEnabled and COLOR_ON or COLOR_OFF
    self.ccb_coinBtn:setColor(coinColor)
    self.ccb_cardBtn:setColor(cardColor)
    self.ccb_coinsTTF:setColor(coinColor)
    self.ccb_cardsTTF:setColor(cardColor)
    self.ccb_coins:setColor(coinColor)
    self.ccb_cards:setColor(cardColor)
end

_updateConfigView = function(self, selectedType)
    self._configView = MJTypeView[selectedType].new(self._mjConfig[selectedType], selectedType)
    self.ccb_configLayer:addChild(self._configView)
    self._configView:addEventListener(self._configView.class.EVENT_JUSHU, function(event)
        local count = event.count
        _updateBtn(self, count)
    end)
    self._configView:addEventListener(self._configView.class.EVENT_UPDATE_LOCAL_CONFIG, function(event)
        local config = event.config
        local type   = event.type
        self._mjConfig[type] = config
    end)
    local count = self._configView:getJushu()
    _updateBtn(self, count)
end

_initTableView = function(self)
    local size = self.ccb_tableView:getContentSize()
    local t = TableView:create(size, {
        cellHeight = function(index)
            return GameCell.height()
        end,
        cellAtIndex = function(index, reusedCell) -- index begin from 0
            local luaIdx     = #self._mjTypes - index
            local mjType     = self._mjTypes[luaIdx]
            local name       = LCAppGlobal:getGameData():getJushuConfig().getNameByType(mjType)
            local isSelected = self._selectIdx == luaIdx
            GDebug("%s, %s", mjType, name)
            if reusedCell then
                reusedCell:setName(name)
                reusedCell:setSelected(isSelected)
                return reusedCell
            else
                local cell = GameCell.new()
                cell:setName(name)
                cell:setSelected(isSelected)
                return cell
            end
        end,
        numberOfCells = function()
            return #self._mjTypes
        end,
        cellTouched = function(cell, cctouch, cCell)
            local luaIdx = #self._mjTypes - cCell:getIdx()
            if self._selectIdx == luaIdx then return end
            cell:setSelected(true)
            local lastIdx = #self._mjTypes - self._selectIdx
            self._selectIdx = luaIdx
            self._tableView:updateCellAtIndex(lastIdx)
            local selectedType = self._mjTypes[self._selectIdx]
            self._configView:removeFromParentAndCleanup(true)
            _updateConfigView(self, selectedType)
        end,
    })
    self._tableView = t
    self.ccb_tableView:addChild(t)
end

_initConfigView = function(self)
    local selectedType = self._mjConfig.selectedType
    self._selectIdx    = table.indexof(self._mjTypes, selectedType)
    if not self._selectIdx then -- 没有找到上一次选中的玩法，默认选第一个
        self._selectIdx = 1
        selectedType = self._mjTypes[1]
        self._mjConfig.selectedType = selectedType
    end
    _updateConfigView(self, selectedType)
end

_checkGPS = function(self)
    if self._mjConfig.cheat then
        return CCEPlugin:locationServicesEnabled()
    end
    return true
end

return CreateRoomPanel
