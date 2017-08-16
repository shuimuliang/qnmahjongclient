
LCUserInfoData = class("LCUserInfoData")

function LCUserInfoData:ctor()
	CC_SYNTHESIZE(self, "player", LCAppGlobal:getLoginPlayer())
	CC_SYNTHESIZE(self, "recordList", {})
	CC_SYNTHESIZE(self, "treasureMap", {})
	CC_SYNTHESIZE(self, "shareLink", "http://cs.mobcolor.com")
end

function LCUserInfoData:updateShareLink(link)
	self.shareLink = link
end

function LCUserInfoData:getSelfPlayer()
	return self.player
end

function LCUserInfoData:getSelfRecordList()
	return self.recordList
end

function LCUserInfoData:getSelfTreasureMap()
	return self.treasureMap
end

function LCUserInfoData:getTreasureVlaue(key)
	local value = self.treasureMap[key]
	if value == nil then return 0 end
	return value
end

function LCUserInfoData:updateTreasureVlaue(key, value)
end

function LCUserInfoData:updateTreasures(pairList)
	if not pairList then return end
	for i=1,#pairList do
		self:updateTreasure(pairList[i])
	end
end
function LCUserInfoData:updateTreasure(pair)
	if not pair then return end
	self:updateTreasureVlaue(pair.key,pair.value)
end

function LCUserInfoData:updateInfo(msg)
	if not msg then return end

	if msg.player ~= nil then self.player = msg.player end

	if msg.recordList ~= nil then
		self.recordList = {}
		for i=1, #msg.recordList do
			local item = LCRecordList:createByRecord(msg.recordList[i])
			table.insert(self.recordList,item)
		end
	end

	if msg.treasureList ~= nil then
		for i=1, #msg.treasureList do
			local item = msg.treasureList[i]
			self:updateTreasureVlaue(item.key,item.value)
		end
	end

	--if msg.praiseCount>=0 then self.praiseCount = msg.praiseCount end
end


LCRecordList = class("LCRecordList")
function LCRecordList:ctor(recordType, totalWin, biggestWin, winGames, totalGames, cardList)
	CC_SYNTHESIZE(self, "recordType", recordType)
	CC_SYNTHESIZE(self, "totalWin", totalWin)
	CC_SYNTHESIZE(self, "biggestWin", biggestWin)
	CC_SYNTHESIZE(self, "winGames", winGames)
	CC_SYNTHESIZE(self, "totalGames", totalGames)
	CC_SYNTHESIZE(self, "cardList", {})
	for i,card in pairs(cardList) do
		local card = {num=card.num,classify=card.classify}
		table.insert(self.cardList, card)
	end
end

function LCRecordList:createByRecord(recordList)
	return LCRecordList.new(
		recordList.recordType,
		recordList.totalWin,
		recordList.biggestWin,
		recordList.winGames,
		recordList.totalGames,
		recordList.cardList
	)
end
