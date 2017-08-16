
local LCMJDiceWidget = class( "LCMJDiceWidget", function() return CCSprite:create() end )


function LCMJDiceWidget:create(...)
	local ret = LCMJDiceWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJDiceWidget:init(...)
	self:setAnchorPoint(ccp(0,0))
	self:setNum(3)
	return true
end

function LCMJDiceWidget:showNum(num,startP,endP)

	local frameArr = CCArray:createWithCapacity(24)
	for j=1,24 do
		local framePath = string.format("mj_dice_%d.png",j)
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(framePath) 
		frameArr:addObject(frame)
	end
	local animation = CCAnimation:createWithSpriteFrames(frameArr)
	animation:setDelayPerUnit(1/24)

	self:setPosition(startP)
	local action1 = CCMoveTo:create(0.1, endP)
	local action2 = CCAnimate:create(animation)
	local action3 = CCCallFunc:create(function() self:setNum(num) end)
	local array = CCArray:create()
	array:addObject(action1)
	array:addObject(action2)
	array:addObject(action3)

	local action = CCSequence:create(array)
	self:runAction(action)
end

function LCMJDiceWidget:setNum(num)
	local initNamne = "mj_dice_9.png"
	local switch = {
			 [1] = function() initNamne = "mj_dice_1.png"  end,
			 [2] = function() initNamne = "mj_dice_5.png"  end,
			 [3] = function() initNamne = "mj_dice_9.png"  end,
			 [4] = function() initNamne = "mj_dice_13.png" end,
			 [5] = function() initNamne = "mj_dice_17.png" end,
			 [6] = function() initNamne = "mj_dice_21.png" end,
	}
	local f = switch[num] 
	if f then f() end

	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(initNamne)
	self:setDisplayFrame(frame)
end


return LCMJDiceWidget
