local LCDragableButton = require "lua/scene/sceneUtils/DragableButton"
local LCButton = require "lua/scene/sceneUtils/CButton"

local LCCountControlLayer = class("LCCountControlLayer",function(...) return CCLayerColor:create() end)

function LCCountControlLayer:create(...)
	local ret = LCCountControlLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end
--[[
	key={
		max=1000,
		min=0,
		text=,
		callback=,
		changeCal=,
		btnText = ,
	}

--]]
function LCCountControlLayer:init(scene, key)
	CC_SYNTHESIZE(self,"runScene", LCSceneTransit:getRunningScene())
	CC_SYNTHESIZE(self,"max", key.max or 1000)
	CC_SYNTHESIZE(self,"min", key.min or 0)
	CC_SYNTHESIZE(self,"text", key.text or "")
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"changeCal", key.changeCal)
	CC_SYNTHESIZE(self,"btnText", key.btnText or _("完成"))
	CC_SYNTHESIZE(self,"showGem", key.showGem or false)
	CC_SYNTHESIZE(self,"ccb", key.ccb or eResCCBI.countControlLayer)

	CC_SYNTHESIZE(self,"slider_bg_01", key.slider_bg_01)
	CC_SYNTHESIZE(self,"slider_bg_02", key.slider_bg_02)
	CC_SYNTHESIZE(self,"slider_bg_03", key.slider_bg_03)


	CC_SYNTHESIZE(self,"count", self.min)
	CC_SYNTHESIZE(self,"startChange", false)
	CC_SYNTHESIZE(self,"subClicked", nil)
	CC_SYNTHESIZE(self,"addClicked", nil)

	CC_SYNTHESIZE(self,"slider", nil)

	CC_SYNTHESIZE(self, "debugOnOff", true)

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(self.ccb,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
	local size = layer:getContentSize()
 --  	self:changeWidthAndHeight(size.width,size.height)
	self:setContentSize(size)
	self:addChild(layer)

	local nodeMap = {
		{"giftControlLayer","ccb_controlLayer","CCLayer"},
		{"showLayer","ccb_showLayer","CCLayer"},
		{"showCountTxt","ccb_showCountTxt","CCLabelTTF"},
		{"showCountValue","ccb_showCountValue","CCLabelTTF"},

		{"ccontrolLayer","ccb_controlLayer","CCLayer"},
		{"ccontrolLayerAdd","ccb_controlLayerAdd","CCLayer"},
		{"ccontrolLayerSub","ccb_controlLayerSub","CCLayer"},
		{"gemLayer","ccb_gemLayer","CCLayer"},
		{"gemLab","ccb_gemLab","CCLabelTTF"},

		{"draggableLayer","ccb_draggableLayer","CCLayer"},
		{"completeButton","ccb_completeBtn","CCControlButton"},
	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)
	self:initControlLayer()
	self:updateBtnState()
	if self.gemLayer then
		self.gemLayer:setVisible(self.showGem)
	end
	self.showCountValue:setString(self.count)

	self:setCompleteText(self.btnText)
	return true
end

function LCCountControlLayer:setCompleteText(btnText, fontName)
	if not btnText then return end
	_CBTN_CTEXTF(self.completeButton, btnText , fontName or eUIFont.fonts_greenbutton)
end

function LCCountControlLayer:setLimitMax(value)
	self.max = value
	if self.slider then
		self.slider:setMaxValue(self.max)
	end
end

function LCCountControlLayer:onlyShowButton(bool)
	self.showLayer:setVisible(not bool)
end

function LCCountControlLayer:initControlLayer()
	-- draggableLayer
	local sharedSpriteFrameCache = CCSpriteFrameCache:sharedSpriteFrameCache()
	local sliderBgMin = sharedSpriteFrameCache:spriteFrameByName(self.slider_bg_02 or eResImage.slider_bg_02)
	local sliderBgMax = sharedSpriteFrameCache:spriteFrameByName(self.slider_bg_03 or eResImage.slider_bg_03)
	local dragSprite = sharedSpriteFrameCache:spriteFrameByName(self.slider_bg_01 or eResImage.slider_bg_01)

	self.slider = LCDragableButton:create({
		size = self.draggableLayer:getContentSize(),
		minValue = self.min,
		maxValue = self.max,
		sliderBgMin = sliderBgMin,
		sliderBgMax = sliderBgMax,
		dragSprite = dragSprite,
		valueChanged = function (curValue)
			self.count = curValue
			self:updateMess()
			self:updateBtnState()
	end})
	self.draggableLayer:addChild(self.slider)

	-- button

	self.subClicked = LCButton:create({name=eResImage.s_peacockbluebutton_reduction})
	self.ccontrolLayerSub:addChild(self.subClicked)
	self.subClicked:setTouchBenganCal(function (x, y)
		self:countChange(2)
		self:updateBtnState()
	end)
	self.subClicked:setTouchOutMoveCal(function (x, y)
		self.startChange = false
		self:updateBtnState()
	end)
	self.subClicked:setTouchEndCal(function (x, y)
		self.startChange = false
		self:updateBtnState()
	end)
	-- local subSprite=CCSprite:createWithSpriteFrameName(eResImage.s_peacockbluebutton_reduction)
	-- local content = self.subClicked:getContentSize()
	-- subSprite:setPosition(ccp(content.width/2, content.height/2))
	-- self.subClicked:addChild(subSprite)

	self.addClicked = LCButton:create({name=eResImage.s_peacockbluebutton_plus})
	self.ccontrolLayerAdd:addChild(self.addClicked)
	self.addClicked:setTouchBenganCal(function (x, y)
		self:countChange(1)
		self:updateBtnState()
	end)
	self.addClicked:setTouchOutMoveCal(function (x, y)
		self.startChange = false
		self:updateBtnState()
	end)
	self.addClicked:setTouchEndCal(function (x, y)
		self.startChange = false
		self:updateBtnState()
	end)
	-- local addSprite=CCSprite:createWithSpriteFrameName(eResImage.s_peacockbluebutton_plus)
	-- local content = self.addClicked:getContentSize()
	-- addSprite:setPosition(ccp(content.width/2, content.height/2))
	-- self.addClicked:addChild(addSprite)

	if self.text then
		self.showCountTxt:setString(self.text)
	end
end

function LCCountControlLayer:setGem(gemCount)
	self.gemLab:setString(gemCount)
end

function LCCountControlLayer:countChange(types)--add 1, sub 2
	local step=1
	if types==2 then step=-1 end

	if self:isStopChange(types) then return end

	self.startChange = true
	self.count = self.count+step
	self:updateMess()
	function startCal()
		if self:isStopChange(types) then return end
		self.count = self.count+step
		self:updateMess()
		if self.startChange then
			scheduleOnce(self,function( ... )
				startCal()
			end, 0.2)
		end
	end

	scheduleOnce(self,function( ... )
		if self.startChange then
			startCal()
		end
	end, 1)
end

function LCCountControlLayer:isStopChange(types)
	if types==2 then
		if self.count<=self.min then
			self.startChange=false
			self:updateBtnState()
			return true
		end
	else
		if self.count>=self.max then return true end
	end
	return false
end


function LCCountControlLayer:updateMess()
	self.showCountValue:setString(self.count)
	if self.changeCal then self.changeCal(self.count) end
	if self.slider then
		self.slider:setCurValue(self.count)
	end
end

function LCCountControlLayer:updateBtnState()
	if self.count<=self.min then
		self.subClicked:enableGrey()
	else
		self.subClicked:defaultShader()
	end

	if self.count>=self.max then
		self.addClicked:enableGrey()
	else
		self.addClicked:defaultShader()
	end
end


function LCCountControlLayer:ccb_completeClicked(control,controlEvent)
	GDebug("ccb_completeClicked")
	if self.callback then self.callback(self.count) end
end

function LCCountControlLayer:restart()
	self.count = self.min
	self:updateMess()
	self:updateBtnState()

	if self.text then
		self.showCountTxt:setString(self.text)
	end
end

function LCCountControlLayer:setVisibleExpOK(isVisible)
	if isVisible==nil then return end
	self.showLayer:setVisible(isVisible)
end

return LCCountControlLayer
