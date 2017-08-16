-- ======================== BASE ======================== --

LCTalkingGameInfoMgr = class("LCTalkingGameInfoMgr",LCDataInfoManager)

function LCTalkingGameInfoMgr:saveData(info)
	if not info.talkID or info.talkID == 0 then return false end
	self:insertData(info.talkID,info)
	return true
end

LCTalkingGameInfo = class("LCTalkingGameInfo",LCDataInfo)

function LCTalkingGameInfo:loadData(row)
	self.talkID = self.tonumber(row,"talkID",0)
	self.talkName = self.tostring(row,"talkName","")
end
