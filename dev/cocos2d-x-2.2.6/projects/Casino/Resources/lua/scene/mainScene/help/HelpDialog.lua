local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local TableView = require "lua/scene/sceneUtils/TableView"
local GameCell  = require "lua/scene/mainScene/privateRoom/GameCell"

local HelpDialog = class("HelpDialog", DialogDIY)

local _initTableView, _setHelpView

local MJHelpCCB = {
    [1] = "hebeiRule",
    [2] = "zhengzhouRule",
    [3] = "pingdingshanRule",
    [4] = "kaifengRule",
}
function HelpDialog:ctor(...)
    HelpDialog.super.ctor(self, ...)
    self._tableView = nil
    self._mjTypes   = LCAppGlobal:getGameData():getJushuConfig():getAllMjType()
    -- 不支持的麻将类型不显示
    local temp = {}
    for _, t in ipairs(self._mjTypes) do
        if MJHelpCCB[t] then
            table.insert(temp, t)
        end
    end
    self._mjTypes = temp

    self._selectIdx = 1
end

function HelpDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("helpView", proxy, self)
    self:addChild(layer)

    _setHelpView(self)
    _initTableView(self)

    return layer
end

function HelpDialog:ccb_onClose()
    self:dismissSelf()
end

-- private
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
            _setHelpView(self)
        end,
    })
    self._tableView = t
    self.ccb_tableView:addChild(t)
end

_setHelpView = function(self)
    local mjType = self._mjTypes[self._selectIdx]
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(MJHelpCCB[mjType], proxy, self)
    self.ccb_scrollView:setContainer(layer)
    local viewSize = self.ccb_scrollView:getViewSize()
    local contentSize = self.ccb_scrollView:getContentSize()
    self.ccb_scrollView:setContentOffset(ccp(0, viewSize.height - contentSize.height))
end

return HelpDialog
