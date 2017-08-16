local LCMJChiWidget         = require "lua/scene/mj/MJChiWidget"
local LCMJGangWidget         = require "lua/scene/mj/MJGangWidget"


local LCMJChoiceWidget = class( "LCMJChoiceWidget", function() return CCLayer:create() end )

function LCMJChoiceWidget:create(...)
	local ret = LCMJChoiceWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJChoiceWidget:init(deskView)
	self.deskView = deskView
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjChoice,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	local nodeMap = {
		{"btn1","ccb_btn1","CCControlButton"},
		{"btn2","ccb_btn2","CCControlButton"},
		{"btn3","ccb_btn3","CCControlButton"},
		{"btn4","ccb_btn4","CCControlButton"},
	}

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self.choiceTable                        = {}
	self.choiceTable[eMJOperation.guo]      = {res = "mj_choose_guo.png"  , func = function(...) self:doGuo(...)  end}
	self.choiceTable[eMJOperation.peng]     = {res = "mj_choose_peng.png" , func = function(...) self:doPeng(...) end}
	self.choiceTable[eMJOperation.ming]     = {res = "mj_choose_gang.png" , func = function(...) self:doGang(...) end}
	self.choiceTable[eMJOperation.an]       = {res = "mj_choose_gang.png" , func = function(...) self:doGang(...) end}
	self.choiceTable[eMJOperation.hu]       = {res = "mj_choose_hu.png"   , func = function(...) self:doHu(...)   end}
	self.choiceTable[eMJOperation.chi]      = {res = "mj_choose_chi.png"  , func = function(...) self:doChi(...)  end}
	self.choiceTable[eMJOperation.multiHu]  = {res = "mj_choose_hu.png"   , func = function(...) self:doHu(...)   end}
	self.choiceTable[eMJOperation.pengGang] = {res = "mj_choose_gang.png" , func = function(...) self:doPeng(...) end}
	self.choiceTable[eMJOperation.ziHu] 	= {res = "mj_choose_hu.png"   , func = function(...) self:doHu(...)   end}
	self.choiceTable[eMJOperation.paoHu] 	= {res = "mj_choose_hu.png"   , func = function(...) self:doHu(...)   end}

	self.orignalX1 = self.btn1:getPositionX()
	self.orignalX2 = self.btn2:getPositionX()
	self.orignalX3 = self.btn3:getPositionX()
	self.orignalX4 = self.btn4:getPositionX()

	return true
end


function LCMJChoiceWidget:showChoice(operationList)
	if not operationList or #operationList < 1 then return end

	local chiList = {}
	local gangList = {}

	for i= #operationList,1,-1 do
		local operation = operationList[i]
		if operation.type == eMJOperation.chi then
			table.insert(chiList,operation)
			table.remove(operationList,i)
		elseif operation.type == eMJOperation.an then
			table.insert(gangList,operation)
			table.remove(operationList,i)
		end
	end

	local chiOp = {cardsList = {}}
	for i=1,#chiList do
		local op = chiList[i]
		chiOp.type = op.type
		chiOp.keycard = op.keycard
		chiOp.srcPos = op.srcPos
		chiOp.desPos = op.desPos
		table.insert(chiOp.cardsList,op.cardsList[1])
		table.insert(chiOp.cardsList,op.cardsList[2])
		table.insert(chiOp.cardsList,op.cardsList[3])
	end

	if #chiList >=1 then
		table.insert(operationList,chiOp)
	end

	local gangOp = {cardsList = {}}
	for i=1,#gangList do
		local op = gangList[i]
		gangOp.type = op.type
		gangOp.keycard = op.keycard
		gangOp.srcPos = op.srcPos
		gangOp.desPos = op.desPos
		table.insert(gangOp.cardsList,op.keycard)
	end

	if #gangList >=1 then
		if #gangList == 1 then
			table.insert(operationList,gangList[1])
		else
			table.insert(operationList,gangOp)
		end
	end

	--把过的操场拿出来单独处理
	self.opGuo = nil
	for i= #operationList,1,-1 do
		local op = operationList[i]
		if op.type == eMJOperation.guo then
			self.opGuo = op
			table.remove(operationList,i)
			break
		end
	end

	self:setVisible(true)
	self:setBtnEnable(self.btn1,true)
	self:setBtnEnable(self.btn2,true)
	self:setBtnEnable(self.btn3,true)
	self:setBtnEnable(self.btn4,true)

	self.item1 = operationList[1]
	self.item2 = operationList[2]
	self.item3 = operationList[3]

	_CBTN_CFRAME(self.btn1,self.choiceTable[eMJOperation.guo].res,  true)
	local num = #operationList

	if num == 1 then
		_CBTN_CFRAME(self.btn2,self.choiceTable[self.item1.type].res,        true)
		self:setBtnEnable(self.btn3,false)
		self:setBtnEnable(self.btn4,false)
		self.btn1:setPositionX(self.orignalX1 + 105)
		self.btn2:setPositionX(self.orignalX2 + 160)
	elseif num == 2 then
		_CBTN_CFRAME(self.btn2,self.choiceTable[self.item1.type].res,        true)
		_CBTN_CFRAME(self.btn3,self.choiceTable[self.item2.type].res,        true)
		self:setBtnEnable(self.btn4,false)
		self.btn1:setPositionX(self.orignalX1 + 60)
		self.btn2:setPositionX(self.orignalX2 + 70)
		self.btn3:setPositionX(self.orignalX3 + 90)
	elseif num == 3 then
		_CBTN_CFRAME(self.btn2,self.choiceTable[self.item1.type].res,        true)
		_CBTN_CFRAME(self.btn3,self.choiceTable[self.item2.type].res,        true)
		_CBTN_CFRAME(self.btn4,self.choiceTable[self.item3.type].res,        true)
	end
end

function LCMJChoiceWidget:setBtnEnable(btn,isEnable)
	btn:setVisible(isEnable)
	btn:setTouchEnabled(isEnable)
end

function LCMJChoiceWidget:ccb_btn1Clicked()
	if self.opGuo then
		self:doGuo(self.opGuo)
	end
end

function LCMJChoiceWidget:ccb_btn2Clicked()
	if self.item1 then
		self.choiceTable[self.item1.type].func(self.item1)
	end
end

function LCMJChoiceWidget:ccb_btn3Clicked()
	if self.item2 then
		self.choiceTable[self.item2.type].func(self.item2)
	end
end

function LCMJChoiceWidget:ccb_btn4Clicked()
	if self.item3 then
		self.choiceTable[self.item3.type].func(self.item3)
	end
end

--杠
function LCMJChoiceWidget:doGang(operationItem)
	local cardList = operationItem.cardsList
	if #cardList > 1 and operationItem.type == eMJOperation.an then
		local gangWidget = LCMJGangWidget:create(operationItem)
		self.deskView:addChild(gangWidget)
		local wordP = self:getParent():convertToWorldSpace(ccp(self:getPositionX(),self:getPositionY()))
		local nodeP = self.deskView:convertToNodeSpace(wordP)
		gangWidget:setAnchorPoint(0,0)
		gangWidget:setPosition(nodeP.x - 150,nodeP.y + 20)
		self:setVisible(false)
	else
		LCMJSendMgr.sendOperation(operationItem)
		self:setVisible(false)
	end
end

--过
function LCMJChoiceWidget:doGuo(operationItem)
	LCMJSendMgr.sendOperation(operationItem)
	self:setVisible(false)
end

--碰
function LCMJChoiceWidget:doPeng(operationItem)
	LCMJSendMgr.sendOperation(operationItem)
	self:setVisible(false)	
end

--胡
function LCMJChoiceWidget:doHu(operationItem)
	LCMJSendMgr.sendOperation(operationItem)	
	self:setVisible(false)
end

--吃
function LCMJChoiceWidget:doChi(operationItem)
	local cardList = operationItem.cardsList
	if #cardList > 3 then
		local chiWidget = LCMJChiWidget:create(operationItem)
		self.deskView:addChild(chiWidget)
		local wordP = self:getParent():convertToWorldSpace(ccp(self:getPositionX(),self:getPositionY()))
		local nodeP = self.deskView:convertToNodeSpace(wordP)
		chiWidget:setAnchorPoint(0,0)
		chiWidget:setPosition(nodeP.x + 50,nodeP.y + 50)
		self:setVisible(false)
	else
		LCMJSendMgr.sendOperation(operationItem)	
		self:setVisible(false)
	end
end	

return LCMJChoiceWidget
