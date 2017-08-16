
local LCMJTimerWidget = class( "LCMJTimerWidget", function() return CCLayer:create() end )

local LabBottom = {}
LabBottom[eMJDirect.south]	= {[1] = "mj_lab_r_nan.png",	[2] = "mj_lab_wr_nan.png"}
LabBottom[eMJDirect.east] 	= {[1] = "mj_lab_r_dong.png", 	[2] = "mj_lab_wr_dong.png"}
LabBottom[eMJDirect.north] 	= {[1] = "mj_lab_r_bei.png", 	[2] = "mj_lab_wr_bei.png"}
LabBottom[eMJDirect.west] 	= {[1] = "mj_lab_r_xi.png", 	[2] = "mj_lab_wr_xi.png"}

local LabRight = {}
LabRight[eMJDirect.south]	= {[1] = "mj_lab_g_nan.png",	[2] = "mj_lab_wg_nan.png"}
LabRight[eMJDirect.east] 	= {[1] = "mj_lab_g_dong.png", 	[2] = "mj_lab_wg_dong.png"}
LabRight[eMJDirect.north] 	= {[1] = "mj_lab_g_bei.png", 	[2] = "mj_lab_wg_bei.png"}
LabRight[eMJDirect.west] 	= {[1] = "mj_lab_g_xi.png", 	[2] = "mj_lab_wg_xi.png"}

local LabTop = {}
LabTop[eMJDirect.south]		= {[1] = "mj_lab_b_nan.png",	[2] = "mj_lab_wb_nan.png"}
LabTop[eMJDirect.east] 		= {[1] = "mj_lab_b_dong.png", 	[2] = "mj_lab_wb_dong.png"}
LabTop[eMJDirect.north] 	= {[1] = "mj_lab_b_bei.png", 	[2] = "mj_lab_wb_bei.png"}
LabTop[eMJDirect.west] 		= {[1] = "mj_lab_b_xi.png", 	[2] = "mj_lab_wb_xi.png"}

local LabLeft = {}
LabLeft[eMJDirect.south]	= {[1] = "mj_lab_y_nan.png",	[2] = "mj_lab_wy_nan.png"}
LabLeft[eMJDirect.east] 	= {[1] = "mj_lab_y_dong.png", 	[2] = "mj_lab_wy_dong.png"}
LabLeft[eMJDirect.north] 	= {[1] = "mj_lab_y_bei.png", 	[2] = "mj_lab_wy_bei.png"}
LabLeft[eMJDirect.west] 	= {[1] = "mj_lab_y_xi.png", 	[2] = "mj_lab_wy_xi.png"}

local res = {
	[eMJPType.bottom] = LabBottom,
	[eMJPType.right]  = LabRight,
	[eMJPType.top]    = LabTop,
	[eMJPType.left]   = LabLeft,
}

local DEFULT_TIME = 15

function LCMJTimerWidget:create(...)
	local ret = LCMJTimerWidget.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJTimerWidget:init(selfPos)
	if selfPos == 1 then
		self.dirTable = {1,2,3,4}
	elseif selfPos == 2 then
		self.dirTable = {2,3,4,1}
	elseif selfPos == 3 then
		self.dirTable = {3,4,1,2}
	elseif selfPos == 4 then
		self.dirTable = {4,1,2,3}
	end
	self.time     = DEFULT_TIME

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjTimer,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local  size  = layer:getContentSize()
	self:addChild(layer)

	local nodeMap = {
		{ "bg",  			"ccb_bg",  			"CCSprite" },
		{ "timeBMLable",  	"ccb_timeBMLable",  "CCLabelBMFont" }
	}

	for i=1,4 do
		table.insert( nodeMap, { "img"..i,  "ccb_img"..i,  "CCSprite" } )
		table.insert( nodeMap, { "node"..i, "ccb_node"..i, "CCNode" } )
	end
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self.timeBMLable:setString(0)

	for i=1,4 do
		local labTable = res[i]
		local dir = self.dirTable[i]
		local name = labTable[dir][2]
		local sprite1 = CCSprite:createWithSpriteFrameName(name)
		self["img"..i]:addChild(sprite1)
		self["img"..i]:setCascadeOpacityEnabled(true)

		sprite1:setPosition(self["node"..i]:getPosition())

		local name = labTable[dir][1]
		local sprite2 =  CCSprite:createWithSpriteFrameName(name)
		self.bg:addChild(sprite2)
		local wordP = self["node"..i]:getParent():convertToWorldSpace(ccp(self["node"..i]:getPositionX(),self["node"..i]:getPositionY()))
		local nodeP = self.bg:convertToNodeSpace(wordP)
		sprite2:setPosition(nodeP)
		self["bgLab"..i] = sprite2

		self["img"..i]:setVisible(false)
	end

	return true
end

function LCMJTimerWidget:dark()
	for i=1,4 do
		self["img"..i]:setVisible(false)
		self["bgLab"..i]:setVisible(true)
		self.pType = nil
	end
end

function LCMJTimerWidget:show(pType)
	self:setVisible(true)
	
	if self.pType == pType then return end
	self.pType = pType
	for i=1,4 do
		if i ~= pType then
			self["img"..i]:setVisible(false)
			self["bgLab"..i]:setVisible(true)
		else
			self["img"..i]:setVisible(true)
			self["bgLab"..i]:setVisible(false)
		end
	end
	self:twinkle(pType)
	self:timeStartRun()
end

function LCMJTimerWidget:resetTime()
	self.time = 0
	if self.schedule then 
		unschedule(self,self.schedule) 
		self.schedule = nil 
	end
	self.pType = nil
	self.timeBMLable:setString(0)
end

function LCMJTimerWidget:hideTime()
	self:setVisible(false)
	self.time = 0
	if self.schedule then 
		unschedule(self,self.schedule) 
		self.schedule = nil 
	end
end

function LCMJTimerWidget:timeStartRun()
	if self.schedule then 
		unschedule(self,self.schedule) 
		self.schedule = nil 
	end

	local function changeTime()
		if self.time < 0 then return end
		self.timeBMLable:setString(self.time)
		self.time = self.time - 1
	end

	self.time = DEFULT_TIME
	changeTime()
	self.schedule = schedule(self, function() changeTime() end, 1)
end

function LCMJTimerWidget:twinkle(pType)
	local img = self["img"..pType]
	if not img then GError("error : you must add pType to call") return end

	local action1 = CCFadeTo:create(0.5,255*0.7)
	local action2 = CCFadeTo:create(0.5,255*1)

	local action = CCRepeatForever:create(CCSequence:createWithTwoActions(action1,action2))
	img:runAction(action)
end

return LCMJTimerWidget
