
local LCRollBase = class("RollNumber", function() return CCNodeRGBA:create() end)

function LCRollBase:init(f_getValue,f_update,time, interval)
	if  not f_getValue or not f_update then
		GError("invalid auguments")
		return
	end
	CC_SYNTHESIZE(self, "f_getValue", f_getValue)
	CC_SYNTHESIZE(self, "f_update", f_update)
	CC_SYNTHESIZE(self, "time", time or 1)
	CC_SYNTHESIZE(self, "interval", interval or 0)

	return true
end

function LCRollBase:update()
	if not self.f_update then return end
	self.f_update(math.ceil( self.f_getValue() ))
end

function LCRollBase:rollStop()
	self:stopAllActions()
end

function LCRollBase:roll(value, callBack)
	if not value then GError("value is nil") return end
	self:rollStop()

	local curValue = self.f_getValue()
	local tickValue = 0
	if self.interval <= 0 then
		tickValue = (value - curValue) * CCDirector:sharedDirector():getAnimationInterval() / self.time
		self.interval = 0
	else
		tickValue = (value - curValue) * self.interval / self.time
	end
	if tickValue < 0 then tickValue = math.floor(tickValue)
	elseif tickValue > 0 then tickValue = math.ceil(tickValue)
	else return end
	schedule(self,function ()
		-- update
		local v = tickValue + self.f_getValue()
		local finished = false
		if tickValue >= 0 and v > value then v = value finished = true end
		if tickValue < 0 and v < value then v = value finished = true end
		self.f_update(math.ceil(v))

		-- call back
		if finished then
			self:rollStop()
			if callBack then callBack() end
		end
	end, self.interval)
end

function LCRollBase:rollWithTime(time, value, callBack)
	if not time then return end
	self.time = time
	return self:roll(value, callBack)
end

function LCRollBase:rollWithSpeed(speed, value, callBack)
	if not speed or speed == 0 then return end
	local curValue = self.f_getValue()
	self.time = math.abs(value - curValue) / speed
	return self:roll(value, callBack)
end



-- ======================== LCRollNumber ======================== --
local LCRollNumber = class("LCRollNumber", LCRollBase)
function LCRollNumber:create(...)
	local ret = LCRollNumber.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCRollNumber:init(node, curValue, cf_setValue, time, interval)
	if not node then GError("node is nil") return end

	CC_SYNTHESIZE(self,"curValue",curValue)
	CC_SYNTHESIZE(self,"node",node)

	if not self.node:getParent() then self:addChild(self.node) end

	if not LCRollNumber.super.init(self,
		function() return self.curValue end,
		function(v)
			self.curValue = v
			cf_setValue( self.node, v )
			self:setContentSize(self.node:getContentSize())
		end,
		time,
		interval
	) then return end

	self:update()
	return true
end


-- ======================== LCRollProgress ======================== --
LCRollProgress = class("LCRollProgress", LCRollBase)
function LCRollProgress:create(...) return _create(LCRollProgress, ...) end
function LCRollProgress:init(curValue, maxValue, node, cf_setValue, time)
	if not node then GError("node is nil") return end
	CC_SYNTHESIZE(self,"curValue",curValue)
	CC_SYNTHESIZE(self,"maxValue",maxValue)
	CC_SYNTHESIZE(self,"node",node)
	if not self.node:getParent() then self:addChild(self.node) end

	if not LCRollBase.init(self,
		function() return self.curValue end,
		function(v)
			self.curValue = v
			cf_setValue(self.node, string.format("%d/%d",v,self.maxValue))
			self:setContentSize(self.node:getContentSize())
		end,
		time
	) then return end

	self:update()
	return true
end

return LCRollNumber