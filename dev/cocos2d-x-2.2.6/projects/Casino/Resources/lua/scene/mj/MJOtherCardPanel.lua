local LCMJCard        = require("lua/scene/mj/MJCard")
local LCMJCardData    = require("lua/scene/mj/data/MJCardData")
local LCMJBasePanel   = require("lua/scene/mj/MJBasePanel")
local LCMJCardImg     = require("lua/scene/mj/MJCardImg")

local DRAW_CARD_TOP_OFFX   = -10
local DRAW_CARD_LEFT_OFFX  = 20
local DRAW_CARD_RIGHT_OFFX = 20

local LCMJOtherCardPanel = class( "LCMJOtherCardPanel",LCMJBasePanel)

function LCMJOtherCardPanel:ctor(layer,discardPanel,pairPanel,pType)
	if not LCMJBasePanel.ctor(self,layer) then return end
	CC_SYNTHESIZE(self,		"discardPanel", 	discardPanel)
	CC_SYNTHESIZE(self,		"pairPanel", 		pairPanel)
	CC_SYNTHESIZE(self,		"pType", 			pType)
	CC_SYNTHESIZE(self,		"cardImgList", 		{})
	CC_SYNTHESIZE(self,		"dataList", 		{})
	CC_SYNTHESIZE(self,		"newCard", 			nil)

	math.randomseed(os.time())
end

function LCMJOtherCardPanel:initCards()
	local cardNum = self.deskStatusMgr:getCardsNumByPType(self.pType)
	if not cardNum then return end
	for i=1,cardNum do
		local cardData = LCMJCardData.new(1)
		local cardImg  = LCMJCardImg.new(cardData,self.pType,eMJImgType.stand)
		self.layer:addChild(cardImg)

		local rotation = self.layer:getRotation()
		cardImg:setRotation(-rotation)

		local xPosition,isInvert, isZorder = self:getInitPXAndInvert(cardImg,i)
		cardImg:setPositionX(xPosition)
		cardImg:setPositionY(20)

		if isInvert then
			table.insert(self.cardImgList,1,cardImg)
		else
			table.insert(self.cardImgList,cardImg)
		end

		if isZorder then
			if not self.zOrder then self.zOrder = 1 end
			self.zOrder = self.zOrder - 1
			cardImg:setZOrder(self.zOrder)
		end
	end

end

function LCMJOtherCardPanel:relinkInit()
	local cardsSet = self.deskStatusMgr:getCardsSetByPType(self.pType)

	if not cardsSet then return end

	local cardNum = #cardsSet.leftCards
	for i=1,cardNum do
		local cardData = LCMJCardData.new(1)
		local cardImg  = LCMJCardImg.new(cardData,self.pType,eMJImgType.stand)
		self.layer:addChild(cardImg)

		local rotation = self.layer:getRotation()
		cardImg:setRotation(-rotation)

		local xPosition,isInvert,isZorder = self:getInitPXAndInvert(cardImg,i)
		cardImg:setPositionX(xPosition)
		cardImg:setPositionY(20)

		if isInvert then
			table.insert(self.cardImgList,1,cardImg)
		else
			table.insert(self.cardImgList,cardImg)
		end

		if isZorder then
			if not self.zOrder then self.zOrder = 1 end
			self.zOrder = self.zOrder - 1
			cardImg:setZOrder(self.zOrder)
		end
	end

	local function doChi(numList)
		local dataList = {}
		for i=1,#numList do
			local data = LCMJCardData.new(numList[i])
			table.insert(dataList,data)
		end
		self.pairPanel:chi(dataList)
	end

	local pengList = LCMJUtil.getPengList(cardsSet.pengCards)
	for i=1,#pengList do
		local num = pengList[i]
		local cardData = LCMJCardData.new(num)
		self.pairPanel:peng(cardData)
	end

	local anList = LCMJUtil.getGangList(cardsSet.anGangCards)
	for i=1,#anList do
		local num = anList[i]
		local cardData = LCMJCardData.new(anList[i])
		self.pairPanel:gang(cardData,true)
	end

	local mingList = LCMJUtil.getGangList(cardsSet.mingGangCards)
	for i=1,#mingList do
		local cardData = LCMJCardData.new(mingList[i])
		self.pairPanel:gang(cardData)
	end

	local chiList = cardsSet.chiCards
	local count = math.modf(#chiList/3)
	if count >= 1 then
		for i=1, count do
			local index = (i-1)*3
			doChi({chiList[index+1],chiList[index+2],chiList[index+3]})
		end
	end

	local discardList = cardsSet.discardCards
	for i=1,#discardList do
		self.discardPanel:addCardWithNum(discardList[i])
	end

	local drawCard = cardsSet.drawCard
	if drawCard then
		local data = LCMJCardData.new(drawCard)
		self:panelDrawCard(data)
	end
end

--抓牌
function LCMJOtherCardPanel:panelDrawCard(cardData)
	local cardImg = LCMJCardImg.new(cardData,self.pType,eMJImgType.stand)
	self.layer:addChild(cardImg)
	local rotation = self.layer:getRotation()
	cardImg:setRotation(-rotation)

	local xPosition,zOrder = self:getDrawCardPXAndZOrder(cardImg)
	cardImg:setPositionX(xPosition)
	cardImg:setPositionY(20)
	cardImg:setZOrder(zOrder)

	table.insert(self.cardImgList, cardImg)

	self.newImg = cardImg
end

--出牌
function LCMJOtherCardPanel:panelDiscardWithData(cardData)
	--如果该方法短时间之内执行了两次，那么说明是切到后台过程中两个消息一起过来的。
	--为防止第一张牌动画还未播放完成，第二张牌就开始出牌。进行牌桌的一个刷新。
	if self.running then
		 local scene = LCSceneTransit:getRunningScene()
		 if scene then scene:refreshScene() end
	end

	for i=1,#self.dataList do
		local data = self.dataList[i]
		if data.num == cardData.num then
			table.remove(self.dataList,i)
			break
		end
	end

	local newIndex = #self.cardImgList
	if not self.newImg then
		if self.pType == eMJPType.right then newIndex = 1 end
		if self.pType == eMJPType.top then newIndex = 1 end
		if self.pType == eMJPType.left then newIndex = 1 end

		self.newImg = self.cardImgList[newIndex]
	end

	local p = CE_GET_WORLD_POSITION(self.newImg)
	local sprite = LCMJCardImg.new(cardData,self.pType,eMJImgType.discard, #self.discardPanel.cardsList + 1)

	sprite:setAnchorPoint(ccp(0.5,0.5))
	self.discardPanel:addCardSprite(sprite,p)

	self.newImg:removeFromParentAndCleanup(true)
	table.remove(self.cardImgList,newIndex)
	self.newImg = nil
	self.running = true

	scheduleOnce( LCSceneTransit:getRunningScene(),function() self.running = false end, 0.3)
end

function LCMJOtherCardPanel:panelChi(numList,keyNum)
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])

	local dataList = {}
	for i=1,#numList do
		local data = LCMJCardData.new(numList[i])
		table.insert(dataList,data)
	end

	self:panelDrawCard(dataList[1])
	self.pairPanel:chi(dataList)
end

--初始化十三张牌时，得到每张牌的X坐标和插入table时是否逆序
function LCMJOtherCardPanel:getInitPXAndInvert(cardImg, index)
	local xPosition = 0
	local isInvert  = false
	local isZorder  = false

	local switch = {
		[eMJPType.right] = function()
				isInvert = true
			   xPosition = (index-1) * ((cardImg:getContentSize().width-10))
		end,
		  [eMJPType.top] = function()
				isInvert = true
			   xPosition = (index-1) * ((cardImg:getContentSize().width-4))
		end,
		 [eMJPType.left] = function()
				isInvert = true
				isZorder = true
			   xPosition = (index-1) * ((cardImg:getContentSize().width-10))
		end 
	}

	local f = switch[self.pType] 
	if f then f() end

	return xPosition,isInvert,isZorder
end

--摸牌时候，最新抓到的牌的x坐标和Zorder
function LCMJOtherCardPanel:getDrawCardPXAndZOrder(cardImg)
	local xPosition = 0
	local zOrder    = 0

	local switch = {
		[eMJPType.right] = function()
			      zOrder = -1
			 local refer = self.cardImgList[#self.cardImgList]
			   xPosition = refer:getPositionX() - (cardImg:getContentSize().width - 10) - DRAW_CARD_RIGHT_OFFX
		end,
		  [eMJPType.top] = function()
				isInvert = true
			   xPosition = -cardImg:getContentSize().width + DRAW_CARD_TOP_OFFX
		end,
		 [eMJPType.left] = function()
			 local refer = self.cardImgList[#self.cardImgList]
			   xPosition = refer:getPositionX() - (cardImg:getContentSize().width - 10) - DRAW_CARD_RIGHT_OFFX
		end 
	}

	local f = switch[self.pType] 
	if f then f() end

	return xPosition,zOrder
end

function LCMJOtherCardPanel:removeCard(cardImg,index)
	index = index or 1
	for i=1,#self.dataList do
		local data = self.dataList[i]
		if data.num == cardImg.data.num then
			table.remove(self.dataList,i)
			break
		end
	end

	table.remove(self.cardImgList,index)
	cardImg:removeFromParentAndCleanup(true)
end

function LCMJOtherCardPanel:panelPeng(cardData)
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])

	self:panelDrawCard(cardData)
	self.pairPanel:peng(cardData)
end

function LCMJOtherCardPanel:panelPengGang(cardData)
	self:removeCard(self.newImg,#self.cardImgList)
	self.newImg = nil
	self.pairPanel:pengGang(cardData)
end

function LCMJOtherCardPanel:panelHu(cardNumList)
	for i=1, #cardNumList do
		local num = cardNumList[i]
		self.pairPanel:hu(num,i)
	end

	for i=1, #self.cardImgList do
		local card = self.cardImgList[i]
		card:removeFromParentAndCleanup(true)
	end
	self.cardImgList = {}
end

function LCMJOtherCardPanel:panelClear()
	for i=1,#self.cardImgList do
		local img = self.cardImgList[i]
		img:removeFromParentAndCleanup(true)
	end
	self.cardImgList = {}

	self.pairPanel:clearPair()
	self.discardPanel:clearDiscard()
	self.newImg = nil
end

function LCMJOtherCardPanel:panelGang(cardData,isDark)
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])
	self:removeCard(self.cardImgList[1])
	if isDark then
		self:removeCard(self.cardImgList[#self.cardImgList],#self.cardImgList)
		self.newImg = nil
	end

	self.pairPanel:gang(cardData,isDark)
end

return LCMJOtherCardPanel
