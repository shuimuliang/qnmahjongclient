local DialogDIY  = require "lua/scene/commons/dialog/DialogDIY"
local TableView  = require "lua/scene/sceneUtils/TableView"
local InviteCell = import ".InviteCell"

local InviteDialog = class("InviteDialog", DialogDIY)

local _initTableView

function InviteDialog:ctor(key)
    InviteDialog.super.ctor(self, key)
    self._players = {}
    self._tableView = nil
end

function InviteDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("invite", proxy, self)
    self:addChild(layer)

    _initTableView(self)

    -- 获取下线列表
    local pb = LCNetManager:createMessage(eDefMsg.InviteList)
    LCNetManager:send(pb)
    LCSceneTransit.getRunningScene():showLoading("发送中...")

    self.ccb_myKey:setString(LCAppGlobal:getLoginPlayer():getId())

    return layer
end

function InviteDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function InviteDialog:ccb_onInvite(control, controlEvent)
    CSDKManager:getInstance():share(cjson.encode({
        title = "邀请有好礼",
        description = "我的邀请码：".. (LCAppGlobal:getLoginPlayer():getId()) .."，输入后即可获得奖励哦！",
        webpageUrl = "http://oow62av5c.bkt.clouddn.com/hnmj.html",
        type = "link",
        scene = "session" -- "timeline"
    }))
end

function InviteDialog:dealPacket(seed, msg)
    if seed == eDefMsg.InviteList then
        LCSceneTransit.getRunningScene():hideLoading()
        local status = msg.status
        if status == eErrorCode.succ then
            self._players = msg.players
            self._tableView:reloadData()
        else
            -- 这条协议没有错误码
        end
    elseif seed == eDefMsg.InviteAward then
        LCSceneTransit.getRunningScene():hideLoading()
        local status = msg.status
        if status == eErrorCode.succ then
            local coins = msg.coins
            local id    = msg.id
            LCAppGlobal:getLoginPlayer():setCoins(coins)
            if id then
                for _, p in ipairs(self._players) do
                    if p.id == id then
                        p.state = 1
                    end
                end
            end
        else
            LCMessageBox:showConfirm(LCSceneTransit.getRunningScene(), {
                text = _ID2WORD(status),
            })
        end
    end
end

-- private
_initTableView = function(self)
    local size = self.ccb_tableLayer:getContentSize()
    local t = TableView:create(size, {
        cellHeight = function(index)
            return InviteCell.height()
        end,
        cellAtIndex = function(index, reusedCell) -- index begin from 0
            local luaIdx = math.ceil(#self._players / 2) - index
            local data = {}
            if #self._players >= luaIdx * 2 then
                data = {self._players[luaIdx * 2 - 1], self._players[luaIdx * 2]}
            elseif #self._players >= luaIdx * 2 - 1 then
                data = {self._players[luaIdx * 2 - 1]}
            end
            if reusedCell then
                reusedCell:update(luaIdx * 2 - 1, data)
                return reusedCell
            else
                local cell = InviteCell.new()
                cell:update(luaIdx * 2 - 1, data)
                cell:addEventListener(InviteCell.EVENT_BTN_CLICKED, function(event)
                    local id = self._players[event.idx] and self._players[event.idx].id
                    if id then
                        GDebug("领奖 id : " .. id)
                    else
                        GError("领奖 id 错误")
                        return
                    end
                    local pb = LCNetManager:createMessage(eDefMsg.InviteAward)
                    pb._msg.id = id
                    LCNetManager:send(pb)
                    LCSceneTransit.getRunningScene():showLoading("发送中...")
                end)
                return cell
            end
        end,
        numberOfCells = function()
            return math.ceil(#self._players / 2)
        end,
        cellTouched = function(cell, cctouch, cCell)
        end,
    })
    self._tableView = t
    self.ccb_tableLayer:addChild(t)
end

return InviteDialog
