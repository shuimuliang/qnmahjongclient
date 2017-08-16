
local LCMJCardBase = class( "LCMJCardBase", function() return CCLayer:create() end )

function LCMJCardBase:ctor()
	self.scene = LCSceneTransit:getRunningScene()
	self.isTop = false
	CC_SYNTHESIZE(self,	"touchLock",  	false)

	local function ontouch(eventType,x,y) return self:ontouch(eventType,x,y) end
	self:registerScriptTouchHandler(ontouch, false, eTouchPriority.maskLayer, false)
	self:setTouchEnabled(true)

	return true
end

function LCMJCardBase:ontouch( eventType, x, y )
	if self.touchLock then return end
	local bBox = self:boundingBox()

	if self.isTop then
		y = y + 50 
		bBox.size.height = bBox.size.height + 50
	end
	local parent = self:getParent()
	if not parent then return true end
	local layerPos = parent:convertToNodeSpace(ccp(x, y))


	if eventType == "began" then
		if bBox:containsPoint(layerPos) then
			self:onDownIn()
		else
			self:onDownOut()
		end

	elseif eventType == "moved" then
		if self.isTop then y = y - 50 end
		if bBox:containsPoint(layerPos) then
			self:onMoveIn(x,y)
		else
			self:onMoveOut(x,y)
		end

	elseif eventType == "ended" then
		if bBox:containsPoint(layerPos) then
			self:onUpIn()
		else
			self:onUpOut()
		end
	end

	return true
end

function LCMJCardBase:jumpAndShow()
	self:setVisible(true)

	local time1 = 0.12
	local time2 = 0.06
	local time3 = 0.06

	local action1 = CCMoveBy:create(time1, ccp(0, -20))
	local action2 = CCMoveBy:create(time2, ccp(0, 10))
	local action3 = CCMoveBy:create(time2, ccp(0, -10))
	local action4 = CCMoveBy:create(time3, ccp(0, 5))
	local action5 = CCMoveBy:create(time3, ccp(0, -5))

	local array = CCArray:create()
	array:addObject(action1)
	array:addObject(action2)
	array:addObject(action3)
	array:addObject(action4)
	array:addObject(action5)


	local sequence = CCSequence:create(array)
	self:runAction(sequence)

	return time1 + time2 * 2 + time3 * 2
end

function LCMJCardBase:changeTopStatus(isTop)
	self.isTop = isTop
end


function LCMJCardBase:onMoveIn(x,y)

end

function LCMJCardBase:onMoveOut(x,y)

end

function LCMJCardBase:onDownIn()

end

function LCMJCardBase:onDownOut()

end

function LCMJCardBase:onUpIn()

end

function LCMJCardBase:onUpOut()

end

return LCMJCardBase
