local LCMJCardImg 			= require("lua/scene/mj/MJCardImg")

local LCMJChiWidget = class( "LCMJChiWidget", function() return CCLayerColor:create(_APPCOLOR_RGB(0, 0, 0, 0)) end )

function LCMJChiWidget:create(...)
	local ret = LCMJChiWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJChiWidget:init(operationItem)
	self.operationItem = operationItem
	self.cellOff = 0
	self.w = 0
	self.h = 0

	local cardsNumList = operationItem.cardsList
	local cellNum =  #cardsNumList/3

	for i=1, cellNum do
		local index = (i-1)*3 + 1
		self:initCell({cardsNumList[index],cardsNumList[index+1],cardsNumList[index+2]})
	end

	local bg = CCScale9Sprite:createWithSpriteFrameName("mj_frame_bg_special.png")
	bg:setAnchorPoint(ccp(0, 0))
	bg:setPreferredSize(CCSizeMake(self.w, self.h))
	self:addChild(bg)
	bg:setZOrder(-100)
	return true
end

function LCMJChiWidget:initCell(numList)
	local cellW = 0
	local cellH = 0
	local offX = 2
	local midX = 0
	local midY = 0

	local function sendMsg()
		LCMJSendMgr.sendChiOperation(numList,self.operationItem.keycard,self.operationItem.srcPos,self.operationItem.desPos)
	end

	for i=1,3 do
		local data = LCMJCardData.new(numList[i])
		local cardImg = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
		self:addChild(cardImg)
		cardImg:setAnchorPoint(0,0)
		cardImg:setPositionX((cardImg:getContentSize().width-offX) * (i - 1) + self.cellOff)
		cellW = (cardImg:getContentSize().width-offX) * 3
		cellH = cardImg:getContentSize().height
		if i == 1 then midX,midY = cardImg:getPosition() end

		if data.num == self.operationItem.keycard then
			cardImg:setColor(_APPCOLOR_HEX("fef8aa"))
		end
	end
	self.cellOff = self.cellOff + 5 + cellW

	self.w = self.w + cellW + 5
	self.h = cellH

	local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
    local spriteSize = CCSizeMake(cellW, cellH)
    local btn = CCControlButton:create(scale9bg)
    btn:setPreferredSize(spriteSize)
    btn:setAnchorPoint(ccp(0, 0))
    btn:addHandleOfControlEvent( function()
     sendMsg() 
     self:removeFromParentAndCleanup(true)
     end , CCControlEventTouchUpInside)
    self:addChild(btn)
    btn:setPosition(midX,midY)
end

return LCMJChiWidget
