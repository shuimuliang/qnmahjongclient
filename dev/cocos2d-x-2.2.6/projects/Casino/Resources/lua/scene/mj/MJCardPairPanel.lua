local LCMJCard        = require("lua/scene/mj/MJCard")
local LCMJCardData    = require("lua/scene/mj/data/MJCardData")
local LCMJCardImg     = require("lua/scene/mj/MJCardImg")
local LCMJBasePanel   = require("lua/scene/mj/MJBasePanel")


local LCMJCardPairPanel = class( "LCMJCardPairPanel",LCMJBasePanel)

local eMJPairType = {
	peng 	 = 1,
	anGang	 = 2,
	mingGang = 3,
}

local CELL_OFFX = 5


function LCMJCardPairPanel:ctor(layer,pType)
	if not LCMJBasePanel.ctor(self,layer) then return end

	CC_SYNTHESIZE(self,"width", 0)
	CC_SYNTHESIZE(self,"height", 100)
	CC_SYNTHESIZE(self,"pType", pType)
	CC_SYNTHESIZE(self,"imgList", {})

	CC_SYNTHESIZE(self,"scale", 1)
	CC_SYNTHESIZE(self,"scaleRate", 1)
	CC_SYNTHESIZE(self,"offX", 0)
	CC_SYNTHESIZE(self,"offY", 0)

	self:init()
end

function LCMJCardPairPanel:init()
	local function setValue(offX,offY,scale,scaleRate)
		self.offX 		= offX
		self.offY 		= offY
		self.scale 		= scale
		self.scaleRate 	= scaleRate
	end
	local switch = {
	[eMJPType.right] = function() setValue(-35,-12,0.83,0.99) end,
	  [eMJPType.top] = function() setValue(-2 ,0  ,0.51,1   ) end,
	 [eMJPType.left] = function() setValue(-33,8  ,0.71,1.01) end,
   [eMJPType.bottom] = function() setValue(-2 ,0  ,1   ,1   ) end,	}

	local f = switch[self.pType] 
	if f then f() end
end

--胡
function LCMJCardPairPanel:hu(cardNum,index)
	local cellOff = 0
	if index == 1 then cellOff = CELL_OFFX end
	local data = LCMJCardData.new(cardNum)
	self:addImg(data,false,cellOff)
end

--杠
function LCMJCardPairPanel:gang(data,isDark)
	self:peng(data,isDark)

	local midImg = self.imgList[#self.imgList - 1]
	local imgType = eMJImgType.pair

	local cardImg = LCMJCardImg.new(data,self.pType,imgType)

	cardImg:setScale(midImg:getScale())
	cardImg:setRotation(midImg:getRotation())
	cardImg:setAnchorPoint(ccp(0,0))

	self:addChild(cardImg)

	cardImg:setPosition(self:getGangImgPosition(midImg))
end

function LCMJCardPairPanel:chi(dataList)
	for i=1,#dataList do
		local cellOff = 0
		local data = dataList[i]
		if i == 1 then cellOff = CELL_OFFX end
		self:addImg(data,false,cellOff)	
	end

	if self.pType == eMJPType.bottom then
		local cardImg = LCMJCardImg.new(dataList[1],self.pType,eMJImgType.pair)
		local imgW = cardImg:getContentSize().width
		self.width = (imgW - self.offX) * 3 + self.width
	end
end

--碰
function LCMJCardPairPanel:peng(data,isDark)
	for i=1,3 do
		local cellOff = 0
		if i == 1 then cellOff = CELL_OFFX end
		self:addImg(data,isDark,cellOff)
	end

	if self.pType == eMJPType.bottom then
		local cardImg = LCMJCardImg.new(data,self.pType,eMJImgType.pair)
		local imgW = cardImg:getContentSize().width
		self.width = (imgW - self.offX) * 3 + self.width
	end
end

--碰后杠
function LCMJCardPairPanel:pengGang(data)
	local midImg = nil
	for i=1,#self.imgList do
		local img = self.imgList[i]
		if img.data.num == data.num then
			midImg = self.imgList[i + 1]
			break
		end
	end

	local imgType = eMJImgType.pair
	local cardImg = LCMJCardImg.new(data,self.pType,imgType)

	cardImg:setScale(midImg:getScale())
	cardImg:setRotation(midImg:getRotation())
	cardImg:setAnchorPoint(ccp(0,0))

	self:addChild(cardImg)
	cardImg:setPosition(self:getGangImgPosition(midImg))
end

function LCMJCardPairPanel:getWidth()
	return self.width
end

function LCMJCardPairPanel:clearPair()
	self.width = 0
	self.height = 100
	self:init()

	self:removeAllChildrenWithCleanup(true)
	self.imgList = {}
end

function LCMJCardPairPanel:resize()
	self:setContentSize(CCSizeMake(self.width,self.height))
end

function LCMJCardPairPanel:addImg(data,isDark,cellOff)
	local imgType = eMJImgType.pair
	if isDark then imgType = eMJImgType.back end
	local cardImg = LCMJCardImg.new(data,self.pType,imgType)
	cardImg:setAnchorPoint(0,0)
	self:addChild(cardImg)
	table.insert(self.imgList,cardImg)

	self.scale = self.scale * self.scaleRate
	local rotation = self.layer:getRotation()
	cardImg:setRotation(-rotation)
	cardImg:setScale(self.scale)

	local lastImg = self.imgList[#self.imgList - 1]
	if lastImg then
		local lastX = lastImg:getPositionX()
		local lastY = lastImg:getPositionY()
		local lastS = lastImg:getScale()
		local lastW = lastImg:getContentSize().width * lastS
		local lastH = lastImg:getContentSize().height * lastS
		cellOff = cellOff or 0
		cardImg:setPositionX(lastX + lastW + self.offX * lastS + cellOff)
		cardImg:setPositionY(lastY + self.offY * lastS)

		if self.pType == eMJPType.right then
			local lastZ = lastImg:getZOrder()
			cardImg:setZOrder(lastZ - 1)
		end

		if self.pType == eMJPType.top then
			cardImg:setPositionY(0)
		end
	end
end

function LCMJCardPairPanel:getGangImgPosition(midImg)
	local px 	= 0
	local py 	= 0
	local scale = midImg:getScale()
	
	local switch = {
	[eMJPType.right] = function()
	 			  px = midImg:getPositionX() + 10
	  			  py = midImg:getPositionY() - 10
	end,
	  [eMJPType.top] = function()
	 			  px = midImg:getPositionX()
	  			  py = midImg:getPositionY() - 10
	end,
	 [eMJPType.left] = function()
 				  px = midImg:getPositionX() - 13
	  			  py = midImg:getPositionY() - 6
	end,
   [eMJPType.bottom] = function()
 				  px = midImg:getPositionX()
	  			  py = midImg:getPositionY() + 20
	end, 
	}

	local f = switch[self.pType] 
	if f then f() end

	return ccp(px,py)
end

return LCMJCardPairPanel
