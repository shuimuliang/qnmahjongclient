-- ======================== BASE ======================== --

LCShopInfoMgr = class("LCShopInfoMgr",LCDataInfoManager)

function LCShopInfoMgr:saveData(info)
	if not info.id or info.id == 0 then return false end
	self:insertData(info.id,info)
	return true
end

LCShopInfo = class("LCShopInfo",LCDataInfo)

function LCShopInfo:loadData(row)
	CC_SYNTHESIZE(self,"id",0)
	CC_SYNTHESIZE(self,"nameID",0)
	CC_SYNTHESIZE(self,"descID",0)
	CC_SYNTHESIZE(self,"iconID",0)
end

function LCShopInfo:getName()
	return _ID2WORD(self.nameID)
end

function LCShopInfo:getDesc()
	return _ID2WORD(self.descID)
end

function LCShopInfo:getIconName()
	return self.iconID
end
function LCShopInfo:getIconSmallFrame()
	local icon = self.iconSmallID
	if icon then
		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
	else
		return nil
	end
	return
end

function LCShopInfo:getIconFrame()
	local icon = self.iconID
	if icon then
		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
	else
		return nil
	end
	return
end

function LCShopInfo:getTreasureFrame(key)
	local icon = eResImage.treasures[key].icon
	if icon then
		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
	else
		return nil
	end
end

-- function LCShopInfo:getBuyPrice() end
-- function LCShopInfo:getSellPrice() return 0 end

-- ======================== GEM INFO ======================== --
LCShopGemInfoMgr = class("LCShopGemInfoMgr",LCShopInfoMgr)

LCShopGemInfo = class("LCShopGemInfo",LCShopInfo)
function LCShopGemInfo:loadData(row)
	self.id = self.tonumber(row,"gem_id",0)
	self.payType = self.tonumber(row,"payType",0)
	self.nameID = self.tonumber(row,"gem_name",0)
	self.descID = self.tostring(row,"gem_des","")
	self.iconID = self.tostring(row,"gem_icon","")

	self.price = self.tonumber(row,"price",0)
	self.count = self.tonumber(row,"gem_count",0)
	self.VIPPoint = self.tonumber(row,"vip_point",0)
	self.productID = self.tostring(row,"product_id","")
end

-- function LCShopGemInfo:getBuyPrice()
-- 	return self.price
-- end
-- function LCShopGemInfo:getName()
-- 	return string.format(_ID2WORD(self.nameID),self.count)
-- end

-- function LCShopGemInfo:getDesc()
-- 	return string.format(_ID2WORD(self.descID),self.count)
-- end
function LCShopGemInfo:getProductID()
	return self.productID
end
-- ======================== GOODS ======================== --
LCShopGoodsInfoMgr = class("LCShopGoodsInfoMgr",LCShopInfoMgr)
LCShopGoodsInfo = class("LCShopGoodsInfo",LCShopInfo)
function LCShopGoodsInfo:loadData(row)
	self.id = self.tonumber(row,"goods_id",0)
	self.nameID = self.tonumber(row,"goods_name",0)
	self.descID = self.tostring(row,"goods_des","")
	self.iconID = self.tostring(row,"goods_icon","")
	self.iconSmallID = self.tostring(row,"goods_icon2","")

	-- self.etype = self.tonumber(row,"goods_type",0)

	-- self.currency = self.tonumber(row,"money_type",0)
	-- self.price = self.tonumber(row,"price",0)
	-- self.pairKey = self.tonumber(row,"treasure_type",0)
	-- self.pairValue = self.tonumber(row,"goods_count",0)

	self.saleLeftTM = self.tonumber(row,"surplus_time",0)
end

function LCShopGoodsInfo:getPairIconFrame()
	local icon = eResImage.treasures[self.pairKey].icon
	if icon then
		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
	else
		return nil
	end
end

-- function LCShopGoodsInfo:getBuyPrice()
-- 	return self.price
-- end

-- function LCShopGoodsInfo:getName()
-- 	if self.etype == eGoodsType.gold then
-- 		return string.format(_ID2WORD(self.nameID),self.pairValue)
-- 	else
-- 		return _ID2WORD(self.nameID)
-- 	end
-- end

-- function LCShopGoodsInfo:getDesc()
-- 	if self.etype == eGoodsType.gold then
-- 		return string.format(_ID2WORD(self.descID),self.pairValue)
-- 	else
-- 		return _ID2WORD(self.descID)
-- 	end
-- end

-- ======================== TREASURE ======================== --
LCShopTreasureInfoMgr = class("LCShopTreasureInfoMgr",LCShopInfoMgr)
LCShopTreasureInfo = class("LCShopTreasureInfo",LCShopInfo)
function LCShopTreasureInfo:loadData(row)
	self.id = self.tonumber(row,"treasure_id",0)
	self.nameID = self.tonumber(row,"treasure_name",0)
	self.descID = "not have"--self.tostring(row,"goods_des","")
	self.iconID = self.tostring(row,"treasure_icon","")

	-- self.price_buy = self.tonumber(row,"buy_price",0)
	-- self.price_sell = self.tonumber(row,"sell_price",0)

end

-- function LCShopTreasureInfo:getBuyPrice()
-- 	return self.price_buy
-- end
-- function LCShopTreasureInfo:getSellPrice()
-- 	return self.price_sell
-- end
