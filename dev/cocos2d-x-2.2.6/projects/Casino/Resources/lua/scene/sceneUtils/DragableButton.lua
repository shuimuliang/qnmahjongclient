

local LCDragableButton = class("LCDragableButton",function(...) return CCLayerColor:create(...) end)

function LCDragableButton:create(...)
	local ret = LCDragableButton.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCDragableButton:init(key)
	 if not key then return false end

	CC_SYNTHESIZE(self,"minValue",key.minValue or 0)
	CC_SYNTHESIZE_RO(self,"maxValue",key.maxValue)
	CC_SYNTHESIZE_RO(self,"curValue",key.curValue or 0)
	CC_SYNTHESIZE(self,"valueChanged",key.valueChanged)
	CC_SYNTHESIZE(self,"deltaMulti",key.deltaMulti or 0)
	CC_SYNTHESIZE_RO(self,"isDrag",false)

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(key.draggable or eResCCBI.draggableButton,proxy,self)
	local  layer = tolua.cast(node,"CCLayer")
    local  size = layer:getContentSize()
	self:changeWidthAndHeight(size.width,size.height)
	self:addChild(layer)

	local nodeMap = {

		{"sliderBgMin","ccb_sliderBgMin","CCSprite"},
		{"sliderBgMax","ccb_sliderBgMax","CCSprite"},
		{"dragSprite","ccb_dragSprite","CCSprite"},
		{"dragArea","ccb_dragArea","CCLayer"},

	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	if key.sliderBgMin then self.sliderBgMin:setDisplayFrame(key.sliderBgMin) end
	if key.sliderBgMax then self.sliderBgMax:setDisplayFrame(key.sliderBgMax) end
	if key.dragSprite then self.dragSprite:setDisplayFrame(key.dragSprite) end

	CC_SYNTHESIZE(self,"minRect",self.sliderBgMin:getTextureRect())
	CC_SYNTHESIZE(self,"maxRect",self.sliderBgMax:getTextureRect())

	self.dragArea:registerScriptTouchHandler(function( ... )
		return self:onTouch(...)
	end,false,eTouchPriority.maskLayer,true)
	self.dragArea:setTouchEnabled(true)
	local drageSpriteSize = self.dragSprite:getContentSize()

	self.deltaWidth = drageSpriteSize.width*self.deltaMulti
	if key.size then
		layer:setContentSize(key.size)
		self.deltaWidth = drageSpriteSize.width/1.05
		local dragSize = CCSizeMake(key.size.width - self.deltaWidth,key.size.height)
		self.dragArea:setContentSize(dragSize)
		self.dragArea:setPosition(ccp(self.deltaWidth/2 ,0))
		local posx,posy = self.sliderBgMin:getPosition()
		self.sliderBgMin:setPosition(ccp(posx,key.size.height/2))
		self.sliderBgMax:setPosition(ccp(posx,key.size.height/2))
		posx,posy = self.dragSprite:getPosition()
		self.dragSprite:setPosition(ccp(posx,key.size.height/2))

	end

	self:updateView()

	return true
end

function LCDragableButton:updateView(layerPos)
	local  dragSize = self.dragArea:getContentSize()

	--GDebug("%s  xxxxx   ",sizestring(dragSize))
	local dragPosX,dragPosY = self.dragSprite:getPosition()

	if self.maxValue < self.minValue then
		GError("maxValue(%s) < minValue(%s)",self.maxValue,self.minValue)
		return
	end

	if (self.maxValue - self.minValue) == 0 then
		dragPosX = 1 * dragSize.width
	else
		if layerPos then
			dragPosX = layerPos.x
			dragPosX = math.max(dragPosX,0)
			dragPosX = math.min(dragPosX,dragSize.width)
		else
			dragPosX = (self.curValue-self.minValue)/(self.maxValue - self.minValue) * dragSize.width
		end
	end
	--GDebug("---------="..dragPosX)
	self.dragSprite:stopAllActions()
	--self.dragSprite:runAction(CCMoveTo:create(0.001, ccp(dragPosX,dragPosY)))
	self.dragSprite:setPosition(ccp(dragPosX,dragPosY))

	-- local newMinX = self.minRect.origin.x + dragPosX
	-- local newMinW = self.minRect.size.width - dragPosX
	local newMinRect = CCRectMake(0, 0, dragPosX + self.deltaWidth/2, self.minRect.size.height)
	-- self.sliderBgMin:setPosition(ccp(dragPosX,dragPosY))

	-- local newMaxSizeW = dragPosX
	-- local newMaxRect = CCRectMake(0, 0,newMaxSizeW, self.maxRect.size.height)

	local posx =  - (self.minRect.size.width - dragPosX - self.deltaWidth/2)/2
	self.sliderBgMin:setPosition(ccp(posx,dragSize.height/2))

	-- _SET_TEXTURE_RECT(self.sliderBgMin, newMinRect)
	_SET_TEXTURE_RECT(self.sliderBgMin, newMinRect)



	--self.sliderBgMin:setTextureRect(newMinRect)
	--self.sliderBgMax:setTextureRect(newMaxRect)
end

function LCDragableButton:setCurValue(curValue)
	if not curValue then return end
	if curValue < self.minValue or curValue > self.maxValue then return end
	self.curValue = curValue
	self:updateView()
end

function LCDragableButton:setMaxValue(maxValue)
	if not maxValue or maxValue<=0 or maxValue<self.minValue then return end
	self.maxValue = maxValue
	self:updateView()
end

function LCDragableButton:onTouch( eventType,x,y )
	local layerPos = self.dragArea:convertToNodeSpace(ccp(x, y))
	local bBox = self.dragSprite:boundingBox()
	local  dragSize = self.dragArea:getContentSize()
	if eventType == "began" then
		if bBox:containsPoint(layerPos) then
			self.isDrag = true
			return true
		end
		return false
	elseif eventType == "moved" then
		if not self.isDrag then GDebug("") return end
	--	if self.isDrag and bBox:containsPoint(layerPos) then
			local posx,posy = self.dragSprite:getPosition()
			posx = layerPos.x
			if posx > dragSize.width then posx = dragSize.width end
			if posx < 0 then posx = 0 end

			local percent = (posx/dragSize.width)
			local len = self.maxValue - self.minValue
			local value = len * percent + self.minValue
			self.curValue = math.floor(value)

--[[			self.curValue = self.minValue + math.ceil((posx/dragSize.width)*(self.maxValue - self.minValue)) - 1
			self.curValue = math.min(self.curValue,self.maxValue)
			self.curValue = math.max(self.curValue,self.minValue)--]]
			if self.valueChanged then self.valueChanged(self.curValue) end
			self:updateView(layerPos)
	--	else
	--		self.isDrag = false
	--	end
	elseif eventType == "ended" then
		if not isDrag then return end
		self.isDrag = false
	elseif eventType == "cancelled" then
		if not isDrag then return end
		self.isDrag = false
	end
end

return LCDragableButton
