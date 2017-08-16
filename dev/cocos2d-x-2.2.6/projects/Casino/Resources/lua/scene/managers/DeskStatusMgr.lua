local LCDeskStatusMgr = class("LCDeskStatusMgr")

--[[
	key = {
		room = room,
	}
--]]
function LCDeskStatusMgr:ctor(scene,key)
	if not key or not key.room then
		GError("LCDeskStatusMgr init failed")
		return
	end
	CC_SYNTHESIZE(self,"scene",scene)
	CC_SYNTHESIZE(self, "room", key.room)
end


function LCDeskStatusMgr:updateWithEnterRoom(enterRoomRecv)
	GError("should not here ,subclass should implement it")
end
function LCDeskStatusMgr:getPlayerByID(playerID)
	GError("should not here ,subclass should implement it")
end
function LCDeskStatusMgr:clearDesk()
	GError("should not here ,subclass should implement it")
end
function LCDeskStatusMgr:pos2uiPos(pos)
	return pos
end
function LCDeskStatusMgr:uiPos2Pos(pos)
	return pos
end


return LCDeskStatusMgr
