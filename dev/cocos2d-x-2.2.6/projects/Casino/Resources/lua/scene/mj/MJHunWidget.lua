
local LCMJHunWidget = class( "LCMJHunWidget", function() return CCSprite:create() end )


function LCMJHunWidget:create(...)
	local ret = LCMJHunWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJHunWidget:init(num1,num2,node,isAnim,callback)
	self.num1 = num1
	self.num2 = num2
	self.node = node
	self.isAnim = isAnim
	self.callback = callback

	-- enter exit event
	local function onEvent(event)
		if event == "enter" then
			self:onEnter()
		end
	end
	self:registerScriptHandler(onEvent)

	return true
end

function LCMJHunWidget:onEnter()
	if not self.isAnim then
		self:initWithNoAnim()
		return 
	end

	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_hu.png")
	self:setDisplayFrame(frame)
	
	local name1 = LCMJUtil.getHunImgByNum(self.num1)
	self.sprite = CCSprite:createWithSpriteFrameName(name1)
	self:addChild(self.sprite)
	self.sprite:setPositionX(self:getContentSize().width * 0.5)
	self.sprite:setPositionY(self:getContentSize().height * 0.6)

	
	self.node:setOpacity(0)
	self.node:setVisible(true)
	self:rotate()
end

function LCMJHunWidget:doFront()
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_k.png")
	self:setDisplayFrame(frame)

	local frame2 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(eResImage.s_buttontransparent)
	self.sprite:setDisplayFrame(frame2)
end

function LCMJHunWidget:doBack()
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_hu.png")
	self:setDisplayFrame(frame)

	local name2 = LCMJUtil.getHunImgByNum(self.num2)
	local frame2 = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(name2)
	self.sprite:setDisplayFrame(frame2)
end

function LCMJHunWidget:rotate()
	local orbit_time = 0.4
	local array = CCArray:create()
	local orbit1 = CCOrbitCamera:create(orbit_time, 1, 0, 0, -90, 0, 0)
	local orbit2 = CCOrbitCamera:create(orbit_time, 1, 0, 90, -90, 0, 0)
	local orbit3 = CCOrbitCamera:create(orbit_time, 1, 0, 0, -90, 0, 0)
	local orbit4 = CCOrbitCamera:create(orbit_time, 1, 0, 90, -90, 0, 0)

	array:addObject(orbit1)
	array:addObject(CCCallFunc:create(function() self:doFront() end))
	array:addObject(orbit2)
	array:addObject(orbit3)
	array:addObject(CCCallFunc:create(function() self:doBack() end))
	array:addObject(orbit4)
	array:addObject(CCDelayTime:create(0.4))
	array:addObject(CCCallFunc:create(function() self:doFly() end))

	local action = CCSequence:create(array)
	self:runAction(action)
end

function LCMJHunWidget:doFly()
	self.node:runAction(CCFadeIn:create(0.5))

	local time= 1
	local wordP = self.node:getParent():convertToWorldSpace(ccp(self.node:getPositionX(),self.node:getPositionY()))
	local nodeP = self:getParent():convertToNodeSpace(wordP)


	local action1 = CCMoveTo:create(time,ccp(nodeP.x,nodeP.y - 14))
	local action2 = CCScaleTo:create(time, 0.65)

	local array = CCArray:create()
	array:addObject(action1)
	array:addObject(action2)

	local action = CCSpawn:create(array)
	self:runAction(action)

	--仅仅在混牌动画播放结束之后，更新剩余牌数。断线重连的情况下，服务器会把剩余牌数算好直接发过来。
	if self.callback then
		scheduleOnce(self.node,function() self.callback() end, 1)
	end
end

function LCMJHunWidget:initWithNoAnim()
	self.node:setVisible(true)

	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_hu.png")
	self:setDisplayFrame(frame)

	local name2 = LCMJUtil.getHunImgByNum(self.num2)
	self.sprite = CCSprite:createWithSpriteFrameName(name2)
	self:addChild(self.sprite)
	self.sprite:setPositionX(self:getContentSize().width * 0.5)
	self.sprite:setPositionY(self:getContentSize().height * 0.6)

	local wordP = self.node:getParent():convertToWorldSpace(ccp(self.node:getPositionX(),self.node:getPositionY()))
	local nodeP = self:getParent():convertToNodeSpace(wordP)
	self:setScale(0.65)
	self:setPosition(nodeP.x,nodeP.y - 14)
end

return LCMJHunWidget
