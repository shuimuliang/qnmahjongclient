local LCMJChiWidget         = require "lua/scene/mj/MJChiWidget"
local LCMJGangWidget         = require "lua/scene/mj/MJGangWidget"


local LCMJBuyPaoLayer = class( "LCMJBuyPaoLayer", function() return CCLayer:create() end )

function LCMJBuyPaoLayer:create(...)
	local ret = LCMJBuyPaoLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJBuyPaoLayer:init(deskView)
	self.deskView = deskView
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjBuyPao,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	local nodeMap = {
		{"buyPaoImg","ccb_buyPaoImg","CCSprite"},
		{"tipImg","ccb_tipImg","CCSprite"},
	}

	for i=0,3 do
		table.insert(nodeMap,{"btn"..i,		"ccb_buyPaoBtn"..i,		"CCControlButton"})
	end

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	return true
end

function LCMJBuyPaoLayer:showInScene()
    local scene = LCSceneTransit:getRunningScene()
    scene:addChild(self)
    self:setPosition(eUIConst.screenWidth/2,eUIConst.screenHeight/2)
end

function LCMJBuyPaoLayer:sendMsg(paoScore)
    LCSceneTransit.getRunningScene():showLoading("发送中...")

	local pb = LCNetManager:createMessage(eDefMsg.BuyPao)
	pb._msg.paoScore = paoScore
	LCNetManager:send(pb)
end

function LCMJBuyPaoLayer:setPaoScore(paoScore)
	self:setBtnEnable(paoScore)
	self.buyPaoImg:setVisible(true)
	self.tipImg:setVisible(true)
end

function LCMJBuyPaoLayer:setBtnEnable(index)
	for i=0,3 do
		self["btn"..i]:setEnabled(false)
	end
	self["btn"..index]:setEnabled(true)
	self["btn"..index]:setTouchEnabled(false)
end

function LCMJBuyPaoLayer:ccb_btnBuy0Clicked( ... )
	self:sendMsg(0)
end

function LCMJBuyPaoLayer:ccb_btnBuy1Clicked( ... )
	self:sendMsg(1)
end

function LCMJBuyPaoLayer:ccb_btnBuy2Clicked( ... )
	self:sendMsg(2)
end

function LCMJBuyPaoLayer:ccb_btnBuy3Clicked( ... )
	self:sendMsg(3)
end

return LCMJBuyPaoLayer
