local LCMJCard        = require("lua/scene/mj/MJCard")
local LCMJCardData    = require("lua/scene/mj/data/MJCardData")
local LCMJBasePanel   = require("lua/scene/mj/MJBasePanel")
local LCMJCardImg     = require("lua/scene/mj/MJCardImg")

local LCMJCardPanel = class( "LCMJCardPanel",LCMJBasePanel)

function LCMJCardPanel:ctor(layer,discardPanel,pairPanel)
	if not LCMJBasePanel.ctor(self,layer) then return end
	CC_SYNTHESIZE(self,		"discardPanel", 	discardPanel)
	CC_SYNTHESIZE(self,		"pairPanel", 		pairPanel)
	CC_SYNTHESIZE(self,		"cardsList", 		{})
	CC_SYNTHESIZE(self,		"positionXList", 	{})
	CC_SYNTHESIZE(self,		"newCard", 		    nil)

	math.randomseed(os.time())

end

function LCMJCardPanel:relinkInit()
	local cardsSet = self.deskStatusMgr:getSelfCardsSet()

	local leftCards = cardsSet.leftCards
	for i=1, #leftCards do
		local data = LCMJCardData.new(leftCards[i])
		local card = LCMJCard.new(self,data,i,true)
		self:addChild(card)
		card:setPositionX((i-1) * card:getContentSize().width)
	 	card:setPositionY(0)
		table.insert(self.cardsList, card)
		table.insert(self.positionXList,card:getPositionX())	
		card:updateIndex()
	end

	local function doPeng(num)
		local data = LCMJCardData.new(num)
		self.pairPanel:peng(data)
		if not self.orignalX then self.orignalX = self:getPositionX() end
		self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)
		self:resize()
	end

	local function doGeng(num,isDark)
		local data = LCMJCardData.new(num)		
		self.pairPanel:gang(data,isDark)
		if not self.orignalX then self.orignalX = self:getPositionX() end
		self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)
		self:resize()
	end

	local function doChi(numList)
		local dataList = {}
		for i=1,#numList do
			local data = LCMJCardData.new(numList[i])
			table.insert(dataList,data)
		end
		self.pairPanel:chi(dataList)
		if not self.orignalX then self.orignalX = self:getPositionX() end
		self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)
		self:resize()
	end

	local pengList = LCMJUtil.getPengList(cardsSet.pengCards)
	for i=1,#pengList do
		local num = pengList[i]
		doPeng(num)
	end

	local anList = LCMJUtil.getGangList(cardsSet.anGangCards)
	for i=1,#anList do
		local num = anList[i]
		doGeng(num,true)
	end

	local mingList = LCMJUtil.getGangList(cardsSet.mingGangCards)
	for i=1,#mingList do
		local num = mingList[i]
		doGeng(num,false)
	end

	local discardList = cardsSet.discardCards
	for i=1,#discardList do
		self.discardPanel:addCardWithNum(discardList[i])
	end

	local chiList = cardsSet.chiCards
	local count = math.modf(#chiList/3)
	if count >= 1 then
		for i=1, count do
			local index = (i-1)*3
			doChi({chiList[index+1],chiList[index+2],chiList[index+3]})
		end
	end

	local drawCard = cardsSet.drawCard
	if drawCard then
		local data = LCMJCardData.new(drawCard)
		self:panelDrawCard(data)
	end
end

function LCMJCardPanel:changeHun()
	if self.deskStatusMgr:withHun() then
		local dataList = self.deskStatusMgr:getMyCards()
		local hunNum = self.deskStatusMgr:getHunCard()	
		dataList = LCMJUtil.sortCardDataList(dataList,hunNum)

		for i=1, 13 do
			local card = self.cardsList[i]
			local cardData = dataList[i]
			card:updateData(cardData)
		end

		for i=1,#self.cardsList do
			local card = self.cardsList[i]
			card:setHun()
		end
	end
end

function LCMJCardPanel:initCards()
	local dataList = self.deskStatusMgr:getMyCards()
	local hunNum = self.deskStatusMgr:getHunCard()
		
	if not self.orignalX then self.orignalX = self:getPositionX() end
	if not self.orignalY then self.orignalY = self:getPositionY() end
	self:setPositionX(self.orignalX)
	self:setPositionY(self.orignalY)

	for i=1, 13 do
		local data = dataList[i]
		local card = LCMJCard.new(self,data,i)
	 	self:addChild(card)
	 	card:setPositionX((i-1) * card:getContentSize().width)
	 	card:setPositionY(20)
		table.insert(self.cardsList, card)
		table.insert(self.positionXList,card:getPositionX())
		card:setVisible(false)
		card:updateIndex()
		card:setTouchLock(true)
	end
	dataList = LCMJUtil.sortCardDataList(dataList)

	function showAction(index1,index2)
		local time = 0
		if not index2 then
			local card = self.cardsList[index1]
			time = card:jumpAndShow()
		else
			for i=index1,index2 do
			local card = self.cardsList[i]
				time = card:jumpAndShow()
			end	
		end
		return time
	end

	function rollback(isBack)
		for i=1,13 do
			local card = self.cardsList[i]
			card:doRoll(isBack)
		end

		scheduleOnce(self.layer,function() 
			for i=1, 13 do
				local card = self.cardsList[i]
				local cardData = dataList[i]
				card:updateData(cardData)
				card:setTouchLock(false)
				card:doRoll(false)
			end
		end, 1)
	end

	local time = 0.36
	scheduleOnce(self.layer,function() showAction(1,4) 			end, 0)
	scheduleOnce(self.layer,function() showAction(5,8) 			end, time    + 0.15)
	scheduleOnce(self.layer,function() showAction(9,12) 		end, time*2  + 0.15*2)
	scheduleOnce(self.layer,function() showAction(13) 			end, time*3  + 0.15*3)
	scheduleOnce(self.layer,function() rollback(true) 			end, time*4  + 0.15*6)
end

function LCMJCardPanel:getPositionXByIndex(index)
	return self.positionXList[index]
end

function LCMJCardPanel:removeCard(card)
	if self.newCard then
		if self.newCard.index == card.index then
			self.newCard = nil
		end
	end

	if self.topCard then
		if self.topCard.index == card.index then
			self.topCard = nil
		end
	end
	
	table.remove(self.cardsList,card.index)	

	--在此之后的所有牌的index都-1	
	for i=card.index,#self.cardsList do
		local card = self.cardsList[i]
		card:updateIndex(card.index - 1)
	end

	card:removeFromParentAndCleanup(true)
end


function LCMJCardPanel:addCard(card,index)
	--在此之后的所有牌的index都+1	
	for i=index,#self.cardsList do
		local card = self.cardsList[i]
		card:updateIndex(card.index + 1)
	end

	table.insert(self.cardsList,index,card)
end


function LCMJCardPanel:removeTopCard(topIsNew,card)
	self.topCard = card
	if not self.topCard then GError("error : no top card ") return end
	
	if topIsNew then
		self.newCard = nil
	else
		self:panelUnstretch(self.topCard)
	end
	self:removeCard(self.topCard)
	self.topCard = nil
end

function LCMJCardPanel:resize()
	if not self.orignalS then
		self.orignalS = self:getContentSize() 
	end

	local card = self.cardsList[1]
	self:setContentSize((#self.cardsList + 1) * card:getContentSize().width + 20,card:getContentSize().height )
end

--摸牌
function LCMJCardPanel:panelDrawCard(cardData,notShowHun)
	local index = #self.cardsList + 1

	local card = nil
	if notShowHun then
		card = LCMJCard.new(self,cardData,index)
	else
		card = LCMJCard.new(self,cardData,index,true)
	end

	card:setPositionX(self.layer:getContentSize().width - card:getContentSize().width)
	card:setPositionY(20)
	self.positionXList[card.index] = card:getPositionX()

	card:updateIndex(index)
	
	card:setVisible(false)
	self.layer:addChild(card)
	card:jumpAndShow()
	self.newCard = card

	self:justAdd(index)

	self:setTopCardBottom()
end

function LCMJCardPanel:setTopCardBottom()
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		card:onDownOut()
	end
end

function LCMJCardPanel:setCardsDropEnable(isEable)
	isEable = isEable or false
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		card:setCanDrop(isEable)
	end
end

function LCMJCardPanel:panelChi(numList,keyNum)
	local chiList = {}

	for k=1,#numList do
		for i=1,#self.cardsList do
			local card = self.cardsList[i]
			if card.data.num == numList[k] and card.data.num ~= keyNum then
				table.insert(chiList,card)
				break
			end
		end
	end

	self:removeCard(chiList[1])
	self:removeCard(chiList[2])
	self:resetCardsPosition()

	local dataList = {}
	for i=1,#numList do
		local data = LCMJCardData.new(numList[i])
		table.insert(dataList,data)
	end

	self.pairPanel:chi(dataList)
	if not self.orignalX then self.orignalX = self:getPositionX() end
	self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)

	self:setTopCardBottom()
end


function LCMJCardPanel:setPanelTouchLocked(isLock)
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		card:setTouchLock(isLock)
	end
end

--出牌
function LCMJCardPanel:panelDiscard(card)
	LCMJSendMgr.sendDiscard(card.data.num)

	--出牌的一瞬间禁止用户点击牌
	self:setPanelTouchLocked(true)
	scheduleOnce(self.layer,function() self:setPanelTouchLocked(false) end,0.7)

	self:setCardsDropEnable(false)

	--增加一张图片到发牌库(discard panel)
	local p = CE_GET_WORLD_POSITION(card)
	local sprite = LCMJCardImg.new(card.data,eMJPType.bottom,eMJImgType.discard,#self.discardPanel.cardsList + 1)
	sprite:setAnchorPoint(ccp(0.5,0.5))
	self.discardPanel:addCardSprite(sprite,p)
	

	local topIsNew = (self.newCard and self.newCard.index == card.index) or false

	--移除要出的牌
	self:removeTopCard(topIsNew,card)

	--如果有新起到的牌，则把新起到的牌插入list
	self:justInsert()
	
	--修改展示层，重新设置每张牌的位置
	self:resetCardsPosition(true)
end

function LCMJCardPanel:panelHu(cardNumList)
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		card:doHu()
	end
end


function LCMJCardPanel:panelClear()
	for i=1, #self.cardsList do
		local card = self.cardsList[i]
		card:removeFromParentAndCleanup(true)
	end
	self.cardsList = {}
	self.positionXList = {}
	if self.orignalX then self:setPositionX(self.orignalX) end
	if self.orignalS then self:setContentSize(self.orignalS) end

	self.discardPanel:clearDiscard()
	self.pairPanel:clearPair()

	self.topCard = nil
	self.newCard = nil
end

function LCMJCardPanel:justInsert()
	if not self.newCard then return end
	local index = self:getNewCardIndex()

	table.remove(self.cardsList,self.newCard.index)
	self.newCard:updateIndex(index)

	--在此之后的所有牌的index都+1	
	for i=index,#self.cardsList do
		local card = self.cardsList[i]
		if card then card:updateIndex(card.index + 1) end
	end

	table.insert(self.cardsList,index,self.newCard)
end

function LCMJCardPanel:justAdd(index)
	if not self.newCard then return end
	table.insert(self.cardsList,index,self.newCard)
end

function LCMJCardPanel:panelUnstretch(topCard)
	if self.newCard and self.newCard.index == topCard.index then return end	
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		local onLeft = card:getPositionX() < topCard:getPositionX()
		local onRight = card:getPositionX() > topCard:getPositionX()
		if onLeft then card:setPositionX(card:getPositionX() + 7) end
		if onRight then card:setPositionX(card:getPositionX() - 7) end
	end
end

function LCMJCardPanel:panelStretch(topCard)
	if self.newCard and self.newCard.index == topCard.index then return end
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		local onLeft = card:getPositionX() < topCard:getPositionX()
		local onRight = card:getPositionX() > topCard:getPositionX()
		if onLeft then card:setPositionX(card:getPositionX() - 7) end
		if onRight then card:setPositionX(card:getPositionX() + 7) end
	end
end


function LCMJCardPanel:panelPeng(cardData)
	local pengList = {}
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		if card.data.num == cardData.num then
			table.insert(pengList,card)
		end
	end

	self:removeCard(pengList[1])
	self:removeCard(pengList[2])
	self:resetCardsPosition()

	self.pairPanel:peng(cardData)
	if not self.orignalX then self.orignalX = self:getPositionX() end
	self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)

	self:setTopCardBottom()	
 end

function LCMJCardPanel:panelGang(cardData,isDark)

	local gangList = {}
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		if card.data.num == cardData.num then
			table.insert(gangList,card)
		end
	end

	self:removeCard(gangList[1])
	self:removeCard(gangList[2])
	self:removeCard(gangList[3])
	if isDark then
		self:removeCard(gangList[4])
	end	

	--如果有新起到的牌，则把新起到的牌插入list
	self:justInsert()

	self:resetCardsPosition()

	self.pairPanel:gang(cardData,isDark)
	if not self.orignalX then self.orignalX = self:getPositionX() end
	self:setPositionX(self.pairPanel:getPositionX() + self.pairPanel:getWidth() + 10)

	self:setTopCardBottom()	
end

function LCMJCardPanel:panelPengGang(cardData)
	local gangCard = nil
	for i=1,#self.cardsList do
		local card = self.cardsList[i]
		if card.data.num == cardData.num then
			gangCard = card
			break
		end
	end

	self:removeCard(gangCard)
	--如果有新起到的牌，则把新起到的牌插入list
	self:justInsert()
	
	self:resetCardsPosition()
	self.pairPanel:pengGang(cardData)
	self:setTopCardBottom()	
end

function LCMJCardPanel:resetCard()
	if self.topCard then
		self.topCard:doBottom()
	end
end

function LCMJCardPanel:resetCardsPosition(isAnim)
	if not self.newCard then
		for i=1,#self.cardsList do
			local card = self.cardsList[i]
			if isAnim then
				local action = CCMoveTo:create(0.12, ccp(self.positionXList[i],card:getPositionY()))
				card:runAction(action)
			else
				card:setPositionX(self.positionXList[i])
			end
		end
	else
		if isAnim then
			for i=1,#self.cardsList do
				local card = self.cardsList[i]
				if card.index ~= self.newCard.index then
					local action = CCMoveTo:create(0.12, ccp(self.positionXList[i],card:getPositionY()))
					card:runAction(action)
				end
			end

			local topY = self.newCard:getPositionY() + 100
			local action1 = CCMoveBy:create(0.12, ccp(0, 100))
			local action2 = CCMoveTo:create(0.3,  ccp(self.positionXList[self.newCard.index], topY))
			local action3 = CCMoveBy:create(0.12, ccp(0, -100))
			local action4 = CCCallFunc:create(function() self.newCard = nil end)

			local array = CCArray:create()
			array:addObject(action1)
			array:addObject(action2)
			array:addObject(action3)
			array:addObject(action4)

			local sequence = CCSequence:create(array)
			self.newCard:runAction(sequence)
		else
			for i=1,#self.cardsList do
				local card = self.cardsList[i]
				if card.index ~= self.newCard.index then
					card:setPositionX(self.positionXList[i])
				end
			end

			self.newCard:setPositionX(self.positionXList[self.newCard.index])
			self.newCard = nil
		end
	end
	self:resize()
end


function LCMJCardPanel:panelCheckData(leftNums)
	local hunNum = self.deskStatusMgr:getHunCard()
	local result = LCMJUtil.checkCardsWithNums(self.cardsList,leftNums,hunNum)
	if not result then
    	local scene = LCSceneTransit:getRunningScene()
    	if scene.refreshScene then scene:refreshScene() end
	end
end

function LCMJCardPanel:getNewCardIndex()
	if not self.newCard then GError("error : no self.newCard") return end
	local index = 1

	local hunNum = self.deskStatusMgr:getHunCard()
	if self.newCard.data.num == hunNum then return index end

	for i=1,#self.cardsList-1 do
		local card1 = self.cardsList[i]
		local num1 = LCMJUtil.getChangeNum(card1.data.num,hunNum)

		local card2 = self.cardsList[i+1]
		local num2 =LCMJUtil.getChangeNum(card2.data.num,hunNum)

		local num = LCMJUtil.getChangeNum(self.newCard.data.num,hunNum)

		if i == 1 and num <= num1 then index = 1 return index end
		if num >= num1 and num <= num2 then index = i + 1  return index end
		if i == #self.cardsList-1 and num >= num2 then
			index = #self.cardsList return index 
		end
	end
	return index
end

return LCMJCardPanel
