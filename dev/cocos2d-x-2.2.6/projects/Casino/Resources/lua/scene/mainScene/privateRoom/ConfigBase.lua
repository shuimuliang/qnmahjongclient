local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local ConfigBase = class("ConfigBase", function() return CCNode:create() end)

-- events
ConfigBase.EVENT_UPDATE_LOCAL_CONFIG = "EVENT_UPDATE_LOCAL_CONFIG"
ConfigBase.EVENT_JUSHU               = "EVENT_JUSHU"

local COLOR_ON  = ccc3(169, 90, 16)
local COLOR_OFF = ccc3(87, 40, 0)
local JU1, JU2, JU3, JU4 = -1, -2, -3, -4
local _jushu, _isDefaultRadioBox, _initRuleBtn, _resetChildren, _initChildren

function ConfigBase:ctor(config, mjType)
    local function onEvent(event)
		if event == "enter" then self:onEnter()
        elseif event == "enterTransitionFinish" then self:onEnterTransitionDidFinish()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)

    EventProtocol.extend(self)

    self._config       = config
    self._mjType       = mjType
    self._id2View      = {}
    self._radioGroup   = {}
    self._showTipsOnly = false
end

function ConfigBase:onEnterTransitionDidFinish()
    if not self.ccb_jushuDot1 then GError("没有局数选择界面") return end
    _initRuleBtn(self)
    local id2View = {
        [JU1] = {checkSprite = self.ccb_jushuDot1, label = self.ccb_jushuTxt1},
        [JU2] = {checkSprite = self.ccb_jushuDot2, label = self.ccb_jushuTxt2},
        [JU3] = {checkSprite = self.ccb_jushuDot3, label = self.ccb_jushuTxt3},
        [JU4] = {checkSprite = self.ccb_jushuDot4, label = self.ccb_jushuTxt4},
    }
    for id, t in pairs(id2View) do
        self:_registerBtn(id, t.checkSprite, t.label, t.tip)
    end
    _jushu(self)

    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("configTip", proxy, self)
    self:addChild(layer)
    layer:setVisible(false)
end

function ConfigBase:onExit()
    self:dispatchEvent{name = ConfigBase.EVENT_UPDATE_LOCAL_CONFIG, type = self._mjType, config = self._config}
end

function ConfigBase:getJushu()
    return self._config.round
end

function ConfigBase:getLocalConfig()
    return self._config
end

function ConfigBase:getServerConfig()
    local ret = {}
    for idx, bool in pairs(self._config) do
        if bool == true and type(idx) == "number" and idx > 0 then
            table.insert(ret, idx)
        end
    end
    table.sort(ret)
    GDebug("wz : configs -> " .. table.concat(ret, ", "))
    return ret
end

function ConfigBase:showTipsOnly(bool)
    self._showTipsOnly = checkbool(bool)
end

function ConfigBase:ccb_onJushu(control, controlEvent)
    if self._showTipsOnly then return end
    local jushuList = LCAppGlobal:getGameData():getJushuConfig().getAllJushuByMjType(self._mjType)
    if control == self.ccb_jushuBtn1 then
        self:_btnSelected(JU1, true)
        self:_btnSelected(JU2, false)
        self:_btnSelected(JU3, false)
        self:_btnSelected(JU4, false)
        self:dispatchEvent{name = ConfigBase.EVENT_JUSHU, count = jushuList[1]}
        self._config.round = jushuList[1]
    elseif control == self.ccb_jushuBtn2 then
        self:_btnSelected(JU1, false)
        self:_btnSelected(JU2, true)
        self:_btnSelected(JU3, false)
        self:_btnSelected(JU4, false)
        self:dispatchEvent{name = ConfigBase.EVENT_JUSHU, count = jushuList[2]}
        self._config.round = jushuList[2]
    elseif control == self.ccb_jushuBtn3 then
        self:_btnSelected(JU1, false)
        self:_btnSelected(JU2, false)
        self:_btnSelected(JU3, true)
        self:_btnSelected(JU4, false)
        self:dispatchEvent{name = ConfigBase.EVENT_JUSHU, count = jushuList[3]}
        self._config.round = jushuList[3]
    elseif control == self.ccb_jushuBtn4 then
        self:_btnSelected(JU1, false)
        self:_btnSelected(JU2, false)
        self:_btnSelected(JU3, false)
        self:_btnSelected(JU4, true)
        self:dispatchEvent{name = ConfigBase.EVENT_JUSHU, count = jushuList[4]}
        self._config.round = jushuList[4]
    end
end

-- protect
-- id : 注册按钮的id，唯一确定一个按钮
-- checkSprite : 选中与否的图片（勾或者圆点）
-- label : 按钮的文字
-- tip : 按钮的说明
-- children : 不勾选的情况下，children 按钮都需要隐藏
-- root : 按钮的根节点，root:setVisible(false) 则隐藏按钮
-- radioGroup : 如果是单选按钮，则标记在一个组里
function ConfigBase:_registerBtn(id, checkSprite, label, tip, children, root, radioGroup)
    self._id2View[id] = {checkSprite = checkSprite, label = label, tip = tip, children = children or {}, root = root, radioGroup = radioGroup}
    if radioGroup then
        self._radioGroup[radioGroup] = self._radioGroup[radioGroup] or {}
        table.insert(self._radioGroup[radioGroup], id)
    end
end

function ConfigBase:_btnSelected(id, state)
    state = checkbool(state)
    if not isset(self._id2View, id) then GError("错误的 id " .. (id or -1)) return end
    local checkSprite = self._id2View[id].checkSprite
    checkSprite:setVisible(state)
    local label = self._id2View[id].label
    label:setColor(state and COLOR_ON or COLOR_OFF)
end

function ConfigBase:_onBtnClick(id, controlEvent)
    if controlEvent == CCControlEventTouchUpInside then
        self.ccb_tipBg:setVisible(false)
        if self._showTipsOnly then return end
        local group = self._id2View[id].radioGroup
        if group then -- 如果是单选按钮，则要去掉同组其他按钮的选中状态
            for _, btnId in ipairs(self._radioGroup[group]) do
                self._config[btnId] = btnId == id
                self:_btnSelected(btnId, self._config[btnId])
            end
        else -- 如果是复选按钮则要反转当前的选中状态
            self._config[id] = not self._config[id]
            self:_btnSelected(id, self._config[id])
        end
        _resetChildren(self, id)
    elseif controlEvent == CCControlEventTouchDown then
        if not self._id2View[id] or not self._id2View[id].tip then return end
        local p = self._id2View[id].label:convertToWorldSpace(ccp(0, 30))
        self.ccb_tipBg:setVisible(true)
        self.ccb_tipBg:setPosition(self:convertToNodeSpace(p))
        self.ccb_tipTxt:setString(self._id2View[id].tip)
    elseif controlEvent == CCControlEventTouchUpOutside then
        self.ccb_tipBg:setVisible(false)
    end
end

-- private
 _jushu = function(self)
    local jushuList = LCAppGlobal:getGameData():getJushuConfig().getAllJushuByMjType(self._mjType)
    local idx = table.indexof(jushuList, self._config.round)
    if not idx then  -- 服务器局数列表更改，本地存储的局数对不上，则默认选第一个
        idx = 1
        self._config.round = jushuList[idx]
    end
    for i, jushu in ipairs(jushuList) do
        self["ccb_jushuBg"..i]:setVisible(true)
        self["ccb_jushuTxt"..i]:setString(jushu.."局")
        self:_btnSelected(-i, idx == i)
    end
    for i = #jushuList+1, 4 do
        self["ccb_jushuBg"..i]:setVisible(false)
    end
end

-- 单选按钮在激活时，其中第一个默认选中
_isDefaultRadioBox = function(self, btnId)
    local key = self._id2View[btnId].radioGroup
    if key then
        local group = self._radioGroup[key]
        if table.isEmpty(group) then GError("单选组为空") return false end
        table.sort(group)
        return btnId == group[1]
    end
    return false
end

_resetChildren = function(self, parentId)
    local t = self._id2View[parentId]
    if not table.isEmpty(t.children) then -- 如果有孩子节点，则自己的选中状态关系到孩子节点的显示
        for _, child in ipairs(t.children) do
            if self._id2View[child].root then
                self._id2View[child].root:setVisible(self._config[parentId])
            else
                GError("没有配置 root 节点")
            end
            self._config[child] = false
            if self._config[parentId] and _isDefaultRadioBox(self, child) then
                self._config[child] = true
            end
            self:_btnSelected(child, self._config[child])
        end
    end
end

_initChildren = function(self, parentId)
    local t = self._id2View[parentId]
    if not table.isEmpty(t.children) then -- 如果有孩子节点，则自己的选中状态关系到孩子节点的显示
        for _, child in ipairs(t.children) do
            if self._id2View[child].root then
                self._id2View[child].root:setVisible(self._config[parentId])
            else
                GError("没有配置 root 节点")
            end
            self:_btnSelected(child, self._config[child])
        end
    end
end

_initRuleBtn = function(self)
    local c = self._config
    for btnId, t in pairs(self._id2View) do
        c[btnId] = checkbool(c[btnId])
        self:_btnSelected(btnId, c[btnId])

        _initChildren(self, btnId)
    end
end

return ConfigBase
