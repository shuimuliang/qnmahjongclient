local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local TableView  = require "lua/scene/sceneUtils/TableView"
local HistoryCell = import(".HistoryCell")

local HistoryDialog = class("HistoryDialog", DialogDIY)

local _initTableView, _getTitles, _getNames, _getScores, _getOwnerId
local _test

local STATE_BRIEF = 1
local STATE_DETAIL = 2

function HistoryDialog:ctor(key)
    HistoryDialog.super.ctor(self, key)

    self._brief = {}
    self._detail = {}
    self._state = STATE_BRIEF
    self._tableView = nil
end

function HistoryDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("history", proxy, self)
    self:addChild(layer)
    _initTableView(self)

    local pb = LCNetManager:createMessage(eDefMsg.GameRecord)
    LCNetManager:send(pb)
    LCSceneTransit.getRunningScene():showLoading("发送中...")

    return layer
end

function HistoryDialog:ccb_onClose()
    if self._state == STATE_BRIEF then
        self:dismissSelf()
    else
        self._state = STATE_BRIEF
        self._tableView:reloadData()
        _CBTN_CFRAME(self.ccb_closeBtn, "mj_interface_btn_close.png")
        self.ccb_detailRoomID:setVisible(false)
    end
end

function HistoryDialog:dealPacket(seed, msg)
    if seed == eDefMsg.GameRecord then

        local status = msg.status
        if status == eErrorCode.succ then
            self._brief = {}
            for _, record in ipairs(msg.records) do
                local t = {
                    roomID  = record.roomID,
                    mjType  = record.mjType,
                    round   = record.totalRound,
                    players = record.nameList,
                    owner   = 1,
                    detail  = {}
                }
                for _, roundInfo in ipairs(record.roundList) do
                    table.insert(t.detail, {
                        roundID = roundInfo.roundID,
                        time    = roundInfo.startTime,
                        score   = roundInfo.scoreList,
                    })
                end
                table.sort(t.detail, function(a, b) return a.time < b.time end)
                table.insert(self._brief, t)
            end
            LCSceneTransit.getRunningScene():hideLoading()
            self._tableView:reloadData()
        end
    end
end

-- private
_getTitles = function(self, index)
    if self._state == STATE_BRIEF then
        local luaIdx = #self._brief - index
        local info   = self._brief[luaIdx]
        local mjName = LCAppGlobal:getGameData():getJushuConfig().getNameByType(info.mjType)
        local t      = info.detail[1].time
        return {"房间号：" .. info.roomID, mjName.."("..info.round..")", t}
    else
        local luaIdx = #self._detail.detail - index
        local t      = self._detail.detail[luaIdx].time
        local mjName = LCAppGlobal:getGameData():getJushuConfig().getNameByType(self._detail.mjType)
        return {string.format("%s：%s/%s局", mjName, luaIdx, self._detail.round), "对局ID：" .. self._detail.detail[luaIdx].roundID, t}
    end
end

_getNames = function(self, index)
    if self._state == STATE_BRIEF then
        local luaIdx = #self._brief - index
        return self._brief[luaIdx].players
    else
        return self._detail.players
    end
end

_getScores = function(self, index)
    if self._state == STATE_BRIEF then
        local score = {}
        local luaIdx = #self._brief - index
        for _, detail in ipairs(self._brief[luaIdx].detail) do
            for i, s in ipairs(detail.score) do
                score[i] = (score[i] or 0) + s
            end
        end
        return score
    else
        local luaIdx = #self._detail.detail - index
        return self._detail.detail[luaIdx].score
    end
end

_getOwnerId = function(self, index)
    if self._state == STATE_BRIEF then
        local luaIdx = #self._brief - index
        return self._brief[luaIdx].owner
    else
        return self._detail.owner
    end
end

_initTableView = function(self)
    local size = self.ccb_tableLayer:getContentSize()
    local t = TableView:create(size, {
        cellHeight = function(index)
            return HistoryCell.height()
        end,
        cellAtIndex = function(index, reusedCell) -- index begin from 0
            local titles = _getTitles(self, index)
            local names = _getNames(self, index)
            local score = _getScores(self, index)
            local ownerIdx = _getOwnerId(self, index)

            if reusedCell then
                reusedCell:update(titles, names, score, ownerIdx)
                return reusedCell
            else
                local cell = HistoryCell.new()
                cell:update(titles, names, score, ownerIdx)
                return cell
            end
        end,
        numberOfCells = function()
            if self._state == STATE_DETAIL then
                return #self._detail.detail
            else
                return #self._brief
            end
        end,
        cellTouched = function(cell, cctouch, cCell)
            if self._state == STATE_DETAIL then return end
            local idx = #self._brief - cCell:getIdx()
            self._detail = self._brief[idx]
            self._state = STATE_DETAIL
            _CBTN_CFRAME(self.ccb_closeBtn, "mj_interface_btn_return.png")
            self.ccb_detailRoomID:setVisible(true)
            self.ccb_detailRoomID:setString("房间号：" .. self._detail.roomID)
            self._tableView:reloadData()
        end,
    })
    self._tableView = t
    self.ccb_tableLayer:addChild(t)
end

_test = function(self)
    LCSceneTransit.getRunningScene():showLoading("发送中...")
    performWithDelay(self, function()
        self._brief = {
            {
                roomID = 1,
                mjType = 1,
                round = 8,
                players = {"杨晋", "阿斯蒂芬", "扣款可能", "asdf fkds"},
                owner = 3,
                detail = {
                    {roundID = 1,time = 99999999, score = {1, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {2, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {3, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {4, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {5, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {6, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {7, 100, -50, -50}},
                    {roundID = 1,time = 99999999, score = {8, 100, -50, -50}},
                }
            },
            {
                roomID = 2,
                mjType = 1,
                round = 16,
                players = {"放到晋", "阿斯蒂芬", "扣款可能", "asdf fkds"},
                owner = 1,
                detail = {
                    {roundID = 1,time = 99999999, score = {0, 1, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 2, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 3, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 4, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 5, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 6, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 7, -50, -50}},
                    {roundID = 1,time = 99999999, score = {0, 8, -50, -50}},
                }
            },
        }
        LCSceneTransit.getRunningScene():hideLoading()
        self._tableView:reloadData()
    end, .5)
end

return HistoryDialog
