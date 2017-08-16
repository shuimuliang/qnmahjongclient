local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

local JoinRoomPanel = class("JoinRoomPanel", DialogDIY)

local ID_COUNT = 5
local _setNum, _reset

function JoinRoomPanel:ctor(...)
    JoinRoomPanel.super.ctor(self, ...)
    self._id = {}
    self._inputMap = {}
end

function JoinRoomPanel:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.joinPrivateRoom, proxy, self)
    self:addChild(layer)

    self._inputMap = {
        [self.ccb_num0] = "0",
        [self.ccb_num1] = "1",
        [self.ccb_num2] = "2",
        [self.ccb_num3] = "3",
        [self.ccb_num4] = "4",
        [self.ccb_num5] = "5",
        [self.ccb_num6] = "6",
        [self.ccb_num7] = "7",
        [self.ccb_num8] = "8",
        [self.ccb_num9] = "9",
    }
    return layer
end

function JoinRoomPanel:ccb_onReset(control, controlEvent)
    _reset(self)
end

function JoinRoomPanel:ccb_onDelete(control, controlEvent)
    table.remove(self._id)
    local delIdx = #self._id + 1
    _setNum(self, delIdx, "")
end

function JoinRoomPanel:ccb_onInput(control, controlEvent)
    if #self._id >= ID_COUNT then
        return
    end
    if isset(self._inputMap, control) then
        local num = self._inputMap[control]
        table.insert(self._id, num)
        _setNum(self, #self._id, num)
    end
    if #self._id == ID_COUNT then
        local roomId = tonumber(table.concat(self._id))
        if roomId == 0 then
            _reset(self)
            LCToastHUD.new("请输入正确的房间号"):showInScene()
        else
            -- LCSceneTransit.getRunningScene():sendEnterRoom(roomId)
            local pb = LCNetManager:createMessage(eDefMsg.EnterRoom)
            pb._msg.roomID = roomId
            pb._msg.latitude = CCEPlugin:getLatitude()
            pb._msg.longitude = CCEPlugin:getLongitude()
            LCNetManager:send(pb)
            LCSceneTransit.getRunningScene():showLoading(_("正在进入房间..."))
        end
    end
end

function JoinRoomPanel:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function JoinRoomPanel:dealPacket(seed, msg)
    if seed == eDefMsg.EnterRoom then
        local status = msg.status
        if status ~= eErrorCode.succ then
            -- LCToastHUD.new(_ID2WORD(status)):showInScene()
            _reset(self)
        end
    end
end

-- private
_setNum = function(self, idx, num)
    if self["ccb_id" .. idx] then
        self["ccb_id" .. idx]:setString(tostring(num))
    else
        GError("no idx " .. (idx or ""))
    end
end

_reset = function(self)
    self._id = {}
    for i = 1, ID_COUNT do
        _setNum(self, i, "")
    end
end

return JoinRoomPanel
