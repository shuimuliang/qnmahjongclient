
local LeaveLayer = class("LeaveLayer", function() return CCLayer:create() end)

function LeaveLayer:create(...)
	local ret = LeaveLayer.new()
	if not ret:init(...) then return nil end
	return ret
end

function LeaveLayer:init(key)
	local key = key or {}
	CC_SYNTHESIZE(self,"callback", key.callback)

	local proxy = CCBProxy:create()
	local layer = CCBuilderReaderLoad(eResCCBI.leaveLayer, proxy, self)
	self:addChild(layer)
	self:setContentSize(layer:getContentSize())

	self:registerScriptTouchHandler(function( ... )
		return self:onBTNTouch(...)
	end, false, 1, true)
	self:setTouchEnabled(true)

	return true
end

function LeaveLayer:ccb_leClicked1()
	if self.callback then self.callback(1) end
end

function LeaveLayer:ccb_leClicked2()
	if self.callback then self.callback(2) end
end

function LeaveLayer:ccb_leClicked3()
	if self.callback then self.callback(3) end
end

function LeaveLayer:setBtnTexture(texture, idx)
	if idx == 2 then
		_CBTN_CFRAME(self.ccb_btn2, texture, true)
	end
end

function LeaveLayer:onBTNTouch(eventType,x,y)
	local glPos = ccp(x, y)

	if eventType == "began" then
		return true
	elseif eventType == "moved" then

	elseif eventType == "ended" then
	end
end

function LeaveLayer:ccb_closeClicked(control,controlEvent)
	if self.callback then self.callback(0) end
end


return LeaveLayer
