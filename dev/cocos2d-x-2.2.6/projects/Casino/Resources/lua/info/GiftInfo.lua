-- ======================== BASE ======================== --

LCGiftInfoMgr = class("LCGiftInfoMgr",LCDataInfoManager)

function LCGiftInfoMgr:saveData(info)
	if not info.id or info.id == 0 then return false end
	self:insertData(info.id,info)
	return true
end


LCGiftInfo = class("LCGiftInfo",LCDataInfo)
-- LCGiftInfo.__index = LCGiftInfo

function LCGiftInfo:loadData(row)
	self.id = self.tonumber(row,"res_id", 0)
	self.name = _ID2WORD(self.tonumber(row,"res_name", 0))
	self.iconName = self.tostring(row,"res_icon", "")
	self.startTime = self.tonumber(row,"start_time", 0)
	self.endTime = self.tonumber(row,"end_time", 0)
	self.price = self.tonumber(row,"cost_gold", 0)
end

-- function LCGiftInfo:getName()
-- 	return _ID2WORD( self.name )
-- end
--
-- function LCGiftInfo:getStartTime()
-- 	return self.startTime
-- end
--
-- function LCGiftInfo:getEndTime()
-- 	return self.endTime
-- end
--
-- function LCGiftInfo:getIconName()
-- 	return self.iconName
-- end
--
-- function LCGiftInfo:getNeedGold()
-- 	return self.price
-- end
--
-- function LCGiftInfo:setNeedGold(gold)
-- 	if not gold then return end
-- 	self.price=gold
-- end
--
-- function LCGiftInfo:getIconFrame()
-- 	if self.iconName then
-- 		return CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.iconName)
-- 	else
-- 		return nil
-- 	end
-- 	return
-- end
