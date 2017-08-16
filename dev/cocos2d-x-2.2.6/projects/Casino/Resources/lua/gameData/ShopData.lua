-- ====================== LCShopManager ====================== --
LCShopManager = class("LCShopManager")

function LCShopManager:ctor()
	CC_SYNTHESIZE(self, "gemList", {} )
	CC_SYNTHESIZE(self, "goodsMap", {} )
	CC_SYNTHESIZE(self, "treasureList", {} )
	CC_SYNTHESIZE(self, "treasureMap", {} )
	CC_SYNTHESIZE(self, "activityList", {} )
	CC_SYNTHESIZE(self, "firstChargeInfo", { goods = nil, status = eFirstChargeStatus.noData } )

	for _,goodsType in pairs(eGoodsType) do
		self.goodsMap[goodsType] = {}
	end
end
function LCShopManager:updateShopData(pbGemList)
	if not pbGemList then GError("error params") return end
	self._dirtyTag:refresh()
	self.gemList = {}
	for _,goodsType in pairs(eGoodsType) do
		self.goodsMap[goodsType] = {}
	end
	for i=1,#pbGemList do
		local gemData = LCGemData:createByPB(pbGemList[i])
		table.insert(self.gemList,gemData)
	end
end

function LCShopManager:isShopActivity()
	local storeActivity=false
	local currentTime=LCNetManager:getServerTime()
	for _,v in pairs(self.activityList) do
		if v.act_percent>1 and currentTime>=v.startActivityTime
			and currentTime<=v.endActivityTime then
			storeActivity=true
		end
	end

	return storeActivity
end

function LCShopManager:removePackData(goodsID)
	if not goodsID then return end
	local packList = self:getGoodsByType(eGoodsType.pack)
	local index = nil
	for i=1,#packList do
		local pack = packList[i]
		if goodsID and goodsID == pack:getGoodsID() then
			index = i
		end
	end
	if index then
		table.remove(packList,index)
		return true
	end

	return false
end

function LCShopManager:setCharged()
end

function LCShopManager:updateTreasure(pbTreasureList)
	self.treasureList = {}
	self.treasureMap = {}
	for i=1,#pbTreasureList do
		local treasureData = LCTreasureData:createByPB(pbTreasureList[i])
		if treasureData and eResImage.treasures[treasureData.treasureID] ~= nil then
			table.insert(self.treasureList,treasureData)
			self.treasureMap[treasureData.treasureID] = treasureData
		end
	end
end

function LCShopManager:getSellTreasureList()
	local treasureList = {}
	for i=1,#self.treasureList do
		local treasure = self.treasureList[i]
		-- local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(treasure.treasureID)
		-- if haveCount > 0 then
			table.insert(treasureList,treasure)
		-- end
	end
	return treasureList
end

function LCShopManager:getTreasureByID(treasureID)
	if not treasureID then GDebug("treasureID should not nil") return end
	local treasureData = self.treasureMap[treasureID]
	return treasureData
end
function LCShopManager:getGemDataByID(gemID)
	if not gemID then GDebug("gemID should not nil") return end
	for i=1,#self.gemList do
		local gemData = self.gemList[i]
		if gemData.gemID == gemID then return gemData end
	end
	return nil
end

function LCShopManager:getGoodsByType(goodsType)
	if not goodsType then return nil end
	return self.goodsMap[goodsType]
end

function LCShopManager:getGemsByType(payType)
	local gemList = {}
	for i=1,#self.gemList do
		local gemData = self.gemList[i]
		if gemData.payType == payType then
			table.insert(gemList,gemData)
		end
	end
	return gemList
end

function LCShopManager:getStoreItemByType(itemType)
	local switcherMgr = LCAppGlobal:getGameData():getSwitchMgr()
	if not itemType then return end
	local itemList = {}
	if itemType == eStoreItemType.goods then
		local goodsList = self:getGoodsByType(eGoodsType.gold)
		for i=1,#goodsList do
			local item = goodsList[i]
			table.insert(itemList,item)
		end
	elseif itemType == eStoreItemType.gems then
		for i=1,#self.gemList do
			local item = self.gemList[i]
			table.insert(itemList,item)
		end
	elseif itemType == eStoreItemType.assets then
		local switcherMgr = LCAppGlobal:getGameData():getSwitchMgr()
		local isKick = switcherMgr:isKickValue()
		local isSeat = switcherMgr:isSeatValue()
		local isTalk = switcherMgr:isTalkValue()
		local isRoomCard = switcherMgr:isCreateRoomEnable()
		local isEditCard = switcherMgr:isEditCardValue()
		local goodsList = self:getGoodsByType(eGoodsType.property)
		for i=1,#goodsList do
			local item = goodsList[i]
			if item.treasureType==ePairKey.KICK_CARD then
				if isKick then table.insert(itemList,item) end
			elseif item.treasureType==ePairKey.CHAIR_CARD then
				if isSeat then table.insert(itemList,item) end
			elseif item.treasureType==ePairKey.BROADCAST then
				if isTalk then table.insert(itemList,item) end
			elseif item.treasureType==ePairKey.EDIT_CARD then
				if isEditCard then table.insert(itemList,item) end
			elseif item.treasureType==ePairKey.ROOM_CARD then
				if isRoomCard then table.insert(itemList,item) end
			else
				table.insert(itemList,item)
			end
		end

	end
	return itemList
end

-- ======================== BASE ======================== --

LCShopData = class("LCShopData")
function LCShopData:ctor()
	CC_SYNTHESIZE(self, "info", nil)
	CC_SYNTHESIZE(self, "sort", nil)
end

function LCShopData:getIconFrame()
	return self.info:getIconFrame()
end

function LCShopData:getName()
	return self.info:getName()
end
function LCShopData:getDesc()
	return self.info:getDesc()
end
function LCShopData:getTreasureFrame()
	local key = self.moneyType or ePairKey.GOLD
	local icon = eResImage.treasures[key].icon
	if icon then
		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
	else
		return nil
	end
end

-- ======================== LCGemData ======================== --
LCGemData = class("LCGemData",LCShopData)
function LCGemData:ctor()
	LCShopData.ctor(self)
	CC_SYNTHESIZE(self, "gemID", nil)
	CC_SYNTHESIZE(self, "payType", nil)
	CC_SYNTHESIZE(self, "gemCount", nil)
	CC_SYNTHESIZE(self, "money", nil)
	CC_SYNTHESIZE(self, "productID", nil)
	CC_SYNTHESIZE(self, "extra", nil)
end

function LCGemData:niceMoney(money)
	local unkown = 0
	if not money then return unkown end

	local format = "%.2f"
	local moneyStr = string.format(format,money)
	local len = #moneyStr
	if len <= 2 then return money end
	local last1 = string.sub(moneyStr,len,len)
	local last2 = string.sub(moneyStr,len-1,len-1)
	if last1 == "0" then
		moneyStr = string.sub(moneyStr,1,len-1)
		if last2 == "0" then moneyStr = string.sub(moneyStr,1,len-3) end
	end
	if not moneyStr or #moneyStr <= 0 then return unkown end

	return tonumber(moneyStr)
end

function LCGemData:createByPB(pb)
	local data = LCGemData.new()
	data.gemID = pb.id
	data.payType = pb.payType
	data.gemCount = pb.count
	data.money = self:niceMoney( pb.price )
	data.productID = pb.productID
	data.sort = pb.sort
	data.extra = pb.extra

	local info = LCDataManager:getShopGemInfoMgr():getInfo(data.gemID)
	if not info then
		info = LCShopGemInfo.new()
		info.id = pb.id
		info.payType = pb.payType
		info.nameID = "千胜玉"
		info.descID = "千胜玉"
		info.iconID = pb.iconUrl
		info.VIPPoint = 0
		info.productID = pb.productID
	end
	data.info = info

	return data
end

function LCGemData:getName()
	return string.format(self.info:getName(),self.gemCount)
end
function LCGemData:getDesc()
	return string.format(self.info:getDesc() ,self.gemCount)
end



LCGoodsData = class("LCGoodsData",LCShopData)
function LCGoodsData:ctor()
	LCShopData.ctor(self)
	CC_SYNTHESIZE(self, "goodsID", goodsID)
	CC_SYNTHESIZE(self, "goodsType", goodsType)
	CC_SYNTHESIZE(self, "treasureType", treasureType)
	CC_SYNTHESIZE(self, "goodsCount", goodsCount)
	CC_SYNTHESIZE(self, "moneyType", moneyType)
	CC_SYNTHESIZE(self, "price", price)
	CC_SYNTHESIZE(self, "surplusTime", surplusTime)
	CC_SYNTHESIZE(self, "act_percent", 1)

end

function LCGoodsData:createByPB(pb)
	local data = LCGoodsData.new()
	if pb.goodsType == eGoodsType.property then
		data = LCPropData.new()
	elseif pb.goodsType == eGoodsType.pack then
		data = LCPackData.new(pb.packPairList)
	end
	data.goodsID 			= pb.goodsID
	data.goodsType 			= pb.goodsType
	data.treasureType 		= pb.treasureType
	data.goodsCount 		= pb.goodsCount
	data.moneyType 			= pb.moneyType
	data.price 				= pb.price
	data.surplusTime 		= pb.surplusTime
	data.sort 				= pb.sort
	data.normalCount       	= pb.normalCount
	data.startActivityTime 	= pb.startActivityTime
	data.endActivityTime 	= pb.startActivityTime+pb.surplusTime
	data.firstGold			= pb.firstGold
	data.days				= pb.days
	data.buildNum			= pb.buildNum

	if data.normalCount then data.act_percent=data.goodsCount/data.normalCount end

	local info = LCDataManager:getShopGoodsInfoMgr():getInfo(data.goodsID)
	if not info then
		if pb.goodsType ~= eGoodsType.gold then return end

		info = LCShopGoodsInfo.new()
		info.id = pb.goodsID
		info.nameID 	= 21101
		info.descID 	= 21501
		info.iconID 	= eResImage.goodsIcon[2106]
		info.iconSmallID = eResImage.goodsIcon[2106]
		-- info.etype = pb.goodsType
		-- info.currency = pb.moneyType
		-- info.price = pb.price
		-- info.pairKey = pb.treasureType
		-- info.pairValue = pb.goodsCount
		info.saleLeftTM = 0

	end
	data.info = info

	return data
end

function LCGoodsData:getName()
	if self.goodsType == eGoodsType.gold then
		local gold=self.goodsCount
		local currentTime=LCNetManager:getServerTime()
		if (self.act_percent>1 and self.normalCount) or (currentTime<self.startActivityTime
			or currentTime>self.endActivityTime) then gold=self.normalCount end
 		return string.format(self.info:getName(), gold)
 	else
 		return self.info:getName()
 	end
 end

function LCGoodsData:getCurrentCount()
	return self.goodsCount or 0
end

function LCGoodsData:getDesc()
	if self.goodsType == eGoodsType.gold then
 		return string.format(self.info:getDesc(),self.goodsCount)
 	else
 		return self.info:getDesc()
 	end
end


LCPropData = class("LCPropData",LCGoodsData)

LCPackData = class("LCPackData",LCGoodsData)
function LCPackData:ctor(packPairList)
	LCGoodsData.ctor(self)
	CC_SYNTHESIZE(self, "packPairList", packPairList or {} )
end


LCTreasureData = class("LCTreasureData",LCShopData)
function LCTreasureData:ctor()
	LCShopData.ctor(self)
	CC_SYNTHESIZE(self, "treasureID", treasureID)
	CC_SYNTHESIZE(self, "treasureValue", treasureValue)
	CC_SYNTHESIZE(self, "sellValue", sellValue)
end

function LCTreasureData:createByPB(pb)
	local data = LCTreasureData.new()
	data.treasureID = pb.treasureID
	data.treasureValue = pb.treasureValue
	data.sellValue = pb.sellValue
	data.sort = pb.sort

	local info = LCDataManager:getShopTreasureInfoMgr():getInfo(data.treasureID)
	if not info then return end
	data.info = info

	return data
end

function LCTreasureData:genGiftData(isSell)
	local data = LCGiftData.new()
	data:setGiftID(self.treasureID)
	data:setName(eResImage.treasures[self.treasureID].name)
	data:setIconName(eResImage.treasures[self.treasureID].icon)
	if isSell then
		data:setPrice(self.sellValue)
	else
		data:setPrice(self.treasureValue)
	end
	return data
end
