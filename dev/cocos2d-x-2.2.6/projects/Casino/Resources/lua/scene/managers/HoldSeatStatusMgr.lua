LCHoldSeatStatusMgr = class("LCHoldSeatStatusMgr")
function LCHoldSeatStatusMgr:ctor(scene,key)
	CC_SYNTHESIZE(self,"seatMap",{})	--<seat_pos,LCHoldSeatStatusData>
end

function LCHoldSeatStatusMgr:updateWithEnterRoom(enterRoomRecv)
	GError("should not here ,subclass should implement it")
end
function LCHoldSeatStatusMgr:getPlayerByID(playerID)
	GError("should not here ,subclass should implement it")
end
function LCHoldSeatStatusMgr:clearDesk()
	GError("should not here ,subclass should implement it")
end

function LCHoldSeatStatusMgr:onPlayerGoldChange(playerID, gold)
	local deskPlayer = self:getPlayerByID(playerID)
	if not deskPlayer then return end
	deskPlayer.player.goldCount = gold
end

function LCHoldSeatStatusMgr:getValidPlayerList()
	return self.playerList
end












LCHoldSeatStatusData = class("LCHoldSeatStatusData")
function LCHoldSeatStatusData:ctor(seat_pos,playerID,name)
	CC_SYNTHESIZE(self,"seat_pos",seat_pos)
	CC_SYNTHESIZE(self,"playerID",playerID)
	CC_SYNTHESIZE(self,"name",name or "")
end
function LCHoldSeatStatusData:createByPB(pb)
	return LCHoldSeatStatusData.new(
		pb.seat_pos,
		pb.playerID,
		pb.name
	)
end
