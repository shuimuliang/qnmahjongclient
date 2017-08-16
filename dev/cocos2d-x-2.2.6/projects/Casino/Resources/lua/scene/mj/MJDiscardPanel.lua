local LCMJCard        = require("lua/scene/mj/MJCard")
local LCMJCardData    = require("lua/scene/mj/data/MJCardData")
local LCMJCardImg 	  = require("lua/scene/mj/MJCardImg")


local LCMJDiscardPanel = class( "LCMJDiscardPanel")

local offXListBtm = {0,-5,-6,-3,-5,0,-1,-3,-2,-1,0,-5,-6,-3,-5,0,-1,-3,-2,-1}
local offXListTop = {0,-2,-2,-3,-3,-3,-3,-4,-4,-5,0,-2,-2,-3,-3,-3,-3,-4,-4,-5}

function LCMJDiscardPanel:ctor(layer,pType,arrow)
	if not layer then GError("error : #####  you must use a layer to new it") return end
	CC_SYNTHESIZE(self,"layer", layer)
	CC_SYNTHESIZE(self,"pType", pType)
	CC_SYNTHESIZE(self,"arrow", arrow)
	CC_SYNTHESIZE(self,"cardsList", 	{})
end

function LCMJDiscardPanel:addCardWithNum(num)
	local data = LCMJCardData.new(num)
	local sprite = LCMJCardImg.new(data,self.pType,eMJImgType.discard, #self.cardsList + 1)
	table.insert(self.cardsList,sprite)
	self.layer:addChild(sprite, 0)

	local rotation = self.layer:getRotation()
	sprite:setRotation(-rotation)

	local scale = 1
	local endX	 = 0
	local endY   = 0

	if self.pType == eMJPType.bottom then
		endX,endY,scale = self:getEndPAndSBtm(sprite)
	elseif self.pType == eMJPType.left then
		endX,endY,scale = self:getEndPAndSLeft(sprite)
	elseif self.pType == eMJPType.right then
		endX,endY,scale = self:getEndPAndSRight(sprite)
		sprite:setScale(scale)
	elseif self.pType == eMJPType.top then
		endX,endY,scale = self:getEndPAndSTop(sprite)
	end


	sprite:setScale(scale)
	sprite:setPosition(endX,endY)
	self:setImgZOrder(sprite)
end

function LCMJDiscardPanel:addCardSprite(sprite,p)
	table.insert(self.cardsList,sprite)

	self.layer:addChild(sprite, 0)

	local np = self.layer:convertToNodeSpace(ccp(p.x, p.y))
	sprite:setPosition(np.x,np.y)

	local rotation = self.layer:getRotation()
	sprite:setRotation(-rotation)

	local startX = sprite:getPositionX()
	local startY = sprite:getPositionY()

	local time  = 0.2
	local scale = 1
	local endX	 = 0
	local endY   = 0

	if self.pType == eMJPType.bottom then
		endX,endY,scale = self:getEndPAndSBtm(sprite)
	elseif self.pType == eMJPType.left then
		endX,endY,scale = self:getEndPAndSLeft(sprite)
	elseif self.pType == eMJPType.right then
		endX,endY,scale = self:getEndPAndSRight(sprite)
		sprite:setScale(scale)
	elseif self.pType == eMJPType.top then
		endX,endY,scale = self:getEndPAndSTop(sprite)
	end

	local controlX = startX
	local controlY = endY

    local bezier = ccBezierConfig()
    bezier.controlPoint_1 = ccp(controlX,controlY)
    bezier.controlPoint_2 = ccp(controlX,controlY)
    bezier.endPosition = ccp(endX, endY)

   local action1 = CCBezierTo:create(time, bezier)
   local action2 = CCScaleTo:create(time, scale)
   local spawn = CCSpawn:createWithTwoActions(action1,action2)

   local function callback()
   		self:setImgZOrder(sprite)
   		self.arrow:setPosByImgAndPtype(sprite,self.pType)
   end

   local callfunc = CCCallFunc:create(function() callback() end)
   local sequence = CCSequence:createWithTwoActions(spawn, callfunc)

   sprite:runAction(sequence)
end

function LCMJDiscardPanel:showArrow()
	local sprite = self.cardsList[#self.cardsList]
	if sprite then
		self.arrow:setPosByImgAndPtype(sprite,self.pType)
	end
end

function LCMJDiscardPanel:getEndPAndSLeft(sprite)
	local endX	 = 0
	local endY   = 0
	local scale  = 0.88

	local contentW = sprite:getContentSize().height 
	local contentH = sprite:getContentSize().width
	local rotation = self.layer:getRotation()
	sprite:setRotation(-rotation)

	local offX = (#self.cardsList-1) * (contentW - 1)
	if #self.cardsList == 1 then offX = 0 end

	if #self.cardsList <= 10 then
		local lastX = -contentW/2
		if #self.cardsList >= 2 then
			lastX = self.cardsList[#self.cardsList - 1]:getPositionX()
		end
		scale =0.98^(10 - #self.cardsList) * scale
		endX = lastX + (contentW - 16) * scale
		endY = contentH /2 - ((10 - #self.cardsList) * 5 * scale)

	elseif #self.cardsList > 18 then
		local refer = self.cardsList[#self.cardsList - 7]
		scale = refer:getScale()
		endX  = refer:getPositionX() - 13
		endY  = refer:getPositionY() + contentH * scale - 8
	else
		local refer = self.cardsList[#self.cardsList - 9]
		scale = refer:getScale()
		endX  = refer:getPositionX() - 13
		endY  = refer:getPositionY() + contentH * scale - 8
	end

	return endX,endY,scale
end

function LCMJDiscardPanel:getEndPAndSRight(sprite)
	local endX	 = 0
	local endY   = 0
	local scale  = 0.88

	local contentW = sprite:getContentSize().height 
	local contentH = sprite:getContentSize().width
	local rotation = self.layer:getRotation()
	sprite:setRotation(-rotation)

	local offX = (#self.cardsList-1) * (contentW - 1)
	if #self.cardsList == 1 then offX = 0 end

	if #self.cardsList <= 10 then
		local lastX = -contentW/2
		if #self.cardsList >= 2 then
			lastX = self.cardsList[#self.cardsList - 1]:getPositionX()
		end
		scale =0.98^(#self.cardsList) * scale
		endX = lastX + (contentW - 16) * scale
		endY = contentH /2 + ((10 - #self.cardsList) * 5 * scale)

	elseif #self.cardsList > 18 then
		local refer = self.cardsList[#self.cardsList - 7]
		scale = refer:getScale()
		endX  = refer:getPositionX() + 16
		endY  = refer:getPositionY() + contentH * scale - 9
	else
		local refer = self.cardsList[#self.cardsList - 9]
		scale = refer:getScale()
		endX  = refer:getPositionX() + 16
		endY  = refer:getPositionY() + contentH * scale - 9
	end

	return endX,endY,scale
end

function LCMJDiscardPanel:getEndPAndSBtm(sprite)
	local endX	 = 0
	local endY   = 0
	local scale  = 1

	local contentW = sprite:getContentSize().width 
	local contentH = sprite:getContentSize().height

	local offX = (#self.cardsList-1) * (contentW - 1)
	if #self.cardsList == 1 then offX = 0 end

	if #self.cardsList <= 10 then
		local lastX = -contentW/2
		if #self.cardsList >= 2 then
			lastX = self.cardsList[#self.cardsList - 1]:getPositionX()
		end
		endX = lastX + contentW * scale + offXListBtm[#self.cardsList]* scale
		endY = contentH /2
	elseif #self.cardsList > 18 then
		scale = 0.96
		lastX = self.cardsList[#self.cardsList - 17]:getPositionX()
		endX = lastX * scale + contentW * scale + offXListBtm[#self.cardsList - 16] + 10
		endY = contentH /2 + contentH * 2 - 38
	else
		scale = 0.98
		lastX = self.cardsList[#self.cardsList - 10]:getPositionX()
		endX = lastX * scale + contentW * scale  + offXListBtm[#self.cardsList - 9]  + 5
		endY = contentH /2 + contentH - 19
	end

	return endX,endY,scale
end

function LCMJDiscardPanel:removeCard(data)
	local img = self.cardsList[#self.cardsList]
	table.remove(self.cardsList,#self.cardsList)
	img:removeFromParentAndCleanup(true)
	self.arrow:setVisible(false)
end

function LCMJDiscardPanel:getEndPAndSTop(sprite)
	local endX	 = 0
	local endY   = 0
	local scale  = 0.8

	local contentW = sprite:getContentSize().width * scale
	local contentH = sprite:getContentSize().height * scale

	local offX = (#self.cardsList-1) * (contentW - 1)
	if #self.cardsList == 1 then offX = 0 end

	if #self.cardsList <= 10 then
		local lastX = -contentW/2
		if #self.cardsList >= 2 then
			lastX = self.cardsList[#self.cardsList - 1]:getPositionX()
		end
		endX = lastX + contentW + offXListTop[#self.cardsList] 
		endY = contentH /2
	elseif #self.cardsList > 18 then
		local refer = self.cardsList[#self.cardsList - 7]
		scale = refer:getScale()
		endX  = refer:getPositionX()
		endY  = refer:getPositionY() + contentH * scale - 6
	else
		local refer = self.cardsList[#self.cardsList - 9]
		scale = refer:getScale()
		endX  = refer:getPositionX()
		endY  = refer:getPositionY() + contentH * scale - 6
	end

	return endX,endY,scale
end

function LCMJDiscardPanel:setImgZOrder(img)
	if self.pType == eMJPType.bottom  then
		if #self.cardsList <= 10  then
			img:setZOrder(0)
		elseif #self.cardsList > 18 then
			img:setZOrder(-2)
		else
			img:setZOrder(-1)
		end
	elseif self.pType == eMJPType.right then
		if #self.cardsList <= 10  then
			img:setZOrder(-#self.cardsList)
		elseif #self.cardsList > 18 then
			img:setZOrder(-#self.cardsList + 18)
		else
			img:setZOrder(-#self.cardsList + 10)
		end
	end
end


function LCMJDiscardPanel:clearDiscard()
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		card:removeFromParentAndCleanup(true)
	end

	self.cardsList = {}
end

return LCMJDiscardPanel
