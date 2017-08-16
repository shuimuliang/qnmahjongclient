local ZhengzhouConfig    = require "lua/scene/mainScene/privateRoom/ZhengzhouConfig"
local PingdingshanConfig = require "lua/scene/mainScene/privateRoom/PingdingshanConfig"
local KaifengConfig = require "lua/scene/mainScene/privateRoom/KaifengConfig"


local LCMJConfigLayer = class( "LCMJConfigLayer", function() return CCLayer:create() end )

local CHEAT_COLOR_ON  = ccc3(169, 90, 16)
local CHEAT_COLOR_OFF = ccc3(87, 40, 0)

function LCMJConfigLayer:create(...)
	local ret = LCMJConfigLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJConfigLayer:init(deskStatusMgr)
    self.deskStatusMgr = deskStatusMgr

    local function onTouch(eventType,x,y)
		if eventType == "ended" then
            GDebug("wz onTouch")
		end
		return true
	end
	self:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
	self:setTouchEnabled(true)

	local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.mjConfig,proxy,self)
    self:addChild(layer)
	local nodeMap = {
		{"configPanel",		"ccb_configPanel",				"CCLayer"},
	}

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	local configs = self.deskStatusMgr:getConfigs()
	local data = {}
	data.round = self.deskStatusMgr:getTotalRound()

	for i=1, #configs do
		local config = configs[i]
		data[config] = true
	end

	local mjType = self.deskStatusMgr:getMJType()
	if mjType == 3 then
		self._configView = PingdingshanConfig.new(data, 2)
	elseif mjType == 4 then
		self._configView = KaifengConfig.new(data, 2)
	else
		self._configView = ZhengzhouConfig.new(data, 2)
	end

    self._configView:showTipsOnly(true)
    self.configPanel:addChild(self._configView)

	if self.deskStatusMgr:getCheat() then
		self.ccb_cheatTTF:setColor(CHEAT_COLOR_ON)
		self.ccb_cheat:setVisible(true)
	else
		self.ccb_cheatTTF:setColor(CHEAT_COLOR_OFF)
		self.ccb_cheat:setVisible(false)
	end

	local proxy = CCBProxy:create()
    local tip = CCBuilderReaderLoad("configTip", proxy, self)
    self:addChild(tip)
    tip:setVisible(false)

	return true
end

function LCMJConfigLayer:ccb_onCheat(control, controlEvent)
	if controlEvent == CCControlEventTouchUpInside then
        self.ccb_tipBg:setVisible(false)
    elseif controlEvent == CCControlEventTouchDown then
        local p = self.ccb_cheatTTF:convertToWorldSpace(ccp(0, 30))
        self.ccb_tipBg:setVisible(true)
        self.ccb_tipBg:setPosition(self:convertToNodeSpace(p))
        self.ccb_tipTxt:setString("地理位置太近或者IP相同的玩家无法进入同一个牌桌")
    elseif controlEvent == CCControlEventTouchUpOutside then
        self.ccb_tipBg:setVisible(false)
    end
end


function LCMJConfigLayer:ccb_onClose()
	self:removeFromParentAndCleanup(true)
end

return LCMJConfigLayer
