
-- ====================== LCMJPlayerData ============================================================ --

LCMJPlayerData = class("LCMJPlayerData")
function LCMJPlayerData:ctor(basePlayer,score,pos,latitude,longitude)
	if not basePlayer then GError("error : no base player to init mj player!") return end

	CC_SYNTHESIZE(self, "basePlayer",   basePlayer)
    CC_SYNTHESIZE(self, "score",       	score)
    CC_SYNTHESIZE(self, "pos",          pos)
    CC_SYNTHESIZE(self, "gender",       basePlayer.gender)
    CC_SYNTHESIZE(self, "id",       	basePlayer.id)
    CC_SYNTHESIZE(self, "nickName",     basePlayer.nickname)
    CC_SYNTHESIZE(self, "avatar",      	basePlayer.avatar)
    CC_SYNTHESIZE(self, "coins",       	basePlayer.coins)
    CC_SYNTHESIZE(self, "cards",       	basePlayer.cards)
    CC_SYNTHESIZE(self, "ip",       	basePlayer.ip)
    CC_SYNTHESIZE(self, "latitude",     latitude)
    CC_SYNTHESIZE(self, "longitude",    longitude)


    if self.gender == ePlayerGender.unknown then
        self.gender = ePlayerGender.male
    end
end

function LCMJPlayerData:createByPB(pb)
	local basePB = pb.player
	local basePlayer = LCPlayerData.new(basePB)
	return LCMJPlayerData.new(basePlayer,pb.score,pb.pos,pb.latitude,pb.longitude)
end

return LCMJPlayerData
