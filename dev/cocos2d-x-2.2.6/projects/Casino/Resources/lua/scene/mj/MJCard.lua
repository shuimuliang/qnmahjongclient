local LCMJCardBase        = require("lua/scene/mj/MJCardBase")


local LCMJCard = class( "LCMJCard", LCMJCardBase)

function LCMJCard:ctor(panel,data,index,showHun)
	if not LCMJCard.super.ctor(self) then return false end

	CC_SYNTHESIZE(self,	"panel",  	panel)
	CC_SYNTHESIZE(self,	"data",   	data)
	CC_SYNTHESIZE(self,	"index",  	index)
	CC_SYNTHESIZE(self,	"showHun",  showHun)
	CC_SYNTHESIZE(self,	"canDrop",	false)


	local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.mjcard,proxy,self)
    self:addChild( layer )
    self.size = layer:getContentSize()
    self:setContentSize(self.size)

	local nodeMap = {
		{ "img", 			"ccb_mjSprite", 		"CCSprite" },
		{ "rollImg", 		"ccb_rollImg", 			"CCSprite" },
		{ "backImg", 		"ccb_backImg", 			"CCSprite" },
		{ "testLab", 		"ccb_testLab", 			"CCLabelTTF" },
	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	local function onEvent(event)
		if event == "enter" then self:onEnter() end			
	end
	self:registerScriptHandler(onEvent)

	self.rollImg:setVisible(false)
end

--=================================public function=================================--
function LCMJCard:updateIndex(index)
	self.index = index or self.index
	self.testLab:setString(self.index)

	self.originalY = 0
	self.originalX = self.panel:getPositionXByIndex(self.index)
end

function LCMJCard:updateData(data)
	self.data = data
	self:setImg()
end

function LCMJCard:doBottom()
	self:setPosition(self.originalX,self.originalY)
	self.isTop = false
	self.p = nil
	self:setScale(1)
	self.panel:panelUnstretch(self)
	self.panel.topCard = nil
end

function LCMJCard:doTop()
	self:setPositionY(self.originalY + 50)
	self.panel.topCard = self
	self.isTop = true

	self.topX = self:getPositionX()
	self.topY = self:getPositionY()

	self:setScale(1.2)
	self.panel:panelStretch(self)
end

function LCMJCard:doDrop()
	local deskView = self.scene:getDeskView()
	deskView:deskSelfDescard(self)
end

function LCMJCard:doRoll(isBack)
	isBack = isBack or false
	self.rollImg:setVisible(isBack)
end

function LCMJCard:doHu()
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_hu.png")
	if self:getHunNum() == self.data.num then
		frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_hu_hun.png")
	end
	
	self.backImg:setDisplayFrame(frame)

	self.img:setPositionY(self.img:getPositionY() + 23)
end
--=================================private function=================================--

function LCMJCard:getHunNum()
	local deskStatusMgr = self.scene:getDeskStatusMgr()
	local hunNum = deskStatusMgr:getHunCard()
	return hunNum
end

function LCMJCard:setImg()
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.data.img)
	self.img:setDisplayFrame(frame)

	local backFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_u.png")
	if self.showHun and self:getHunNum() == self.data.num then
		backFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_u_hun.png")
	end
	self.backImg:setDisplayFrame(backFrame)
end

function LCMJCard:setHun()
	if self:getHunNum() == self.data.num then
	local backFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_u_hun.png")
		self.backImg:setDisplayFrame(backFrame)
	end
end

function LCMJCard:followFinger(x,y)
	if self.isTop and self.canDrop then
	-- if self.isTop then
		if not self.p then 
			self.p = ccp(x,y)
		else
			local offX = x - self.p.x
			local offY = y - self.p.y

			if offX > 25 or offY > 25 then
				self.isDrop = true
			else
				self.isDrop = false
			end

			self:setPositionX(self.topX + offX)
			self:setPositionY(self.topY + offY)
		end
	else
		self.p = nil
		self.isDrop = false
	end
end

function LCMJCard:onEnter()
	self:setImg()
	-- self:updateIndex(self.index)
	self.originalX = self:getPositionX()
	self.originalY = 0
end

function LCMJCard:onDownOut()
	if not self.isTop then return end
	self:doBottom()
end

function LCMJCard:onUpIn()
	if self.isDrop then
		self:doDrop()
	else
		if self.topX and self.topY then
			-- self:setPositionX(self.topX)
			-- self:setPositionY(self.topY)
			self.panel:resetCard()
			self:doTop()
		end
	end
end

function LCMJCard:onUpOut()
	if self.isDrop then
		self:doDrop()
	else
		if self.isTop and self.topX and self.topY then
			self:setPositionX(self.topX)
			self:setPositionY(self.topY)
		end
	end	
end

function LCMJCard:onDownIn()
	if self.isTop then return end
	self.panel:resetCard()
	self:doTop()
end

function LCMJCard:onMoveIn(x,y)
	self:onDownIn()
	self:followFinger(x,y)
end
	
function LCMJCard:onMoveOut(x,y)
	self:followFinger(x,y)
end

return LCMJCard
