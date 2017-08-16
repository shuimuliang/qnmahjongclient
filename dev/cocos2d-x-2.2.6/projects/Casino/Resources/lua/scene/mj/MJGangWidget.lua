local LCMJCardImg 			= require("lua/scene/mj/MJCardImg")

local LCMJGangWidget = class( "LCMJGangWidget", function() return CCLayerColor:create(_APPCOLOR_RGB(0, 0, 0, 0)) end )

function LCMJGangWidget:create(...)
	local ret = LCMJGangWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJGangWidget:init(operationItem)
	self.operationItem = operationItem
	self.cellOff = 0
	self.w = 0
	self.h = 0

	local cardsNumList = operationItem.cardsList

	for i=1, #cardsNumList do
		self:initCell(cardsNumList[i])
	end

	local bg = CCScale9Sprite:createWithSpriteFrameName("mj_frame_bg_special.png")
	bg:setAnchorPoint(ccp(0, 0))
	bg:setPreferredSize(CCSizeMake(self.w, self.h))
	self:addChild(bg)
	bg:setZOrder(-100)
	return true
end

function LCMJGangWidget:initCell(num)
	local cellW = 0
	local cellH = 0
	local offX = 2
	local midX = 0
	local midY = 0

	local function sendMsg()
		LCMJSendMgr.sendAnGangOperation(num,self.operationItem.srcPos,self.operationItem.desPos)
	end

	for i=1,4 do
		local data = LCMJCardData.new(num)
		local cardImg = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
		self:addChild(cardImg)
		cardImg:setAnchorPoint(0,0)
		cardImg:setPositionX((cardImg:getContentSize().width-offX) * (i - 1) + self.cellOff)
		cellW = (cardImg:getContentSize().width-offX) * 4
		cellH = cardImg:getContentSize().height
		if i == 1 then midX,midY = cardImg:getPosition() end
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

return LCMJGangWidget
