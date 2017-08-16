local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"
local ConfigBase    = import(".ConfigBase")

local PingdingshanConfig = class("PingdingshanConfig", ConfigBase)

local DIAN_PAO_KE_HU, ZI_MO_HU, MAI_PAO = 1, 2, 3
local GANG_SHANG_HUA_JIA_BEI, GANG_PAO = 4, 5

function PingdingshanConfig:onEnter()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("pingdingshanConfig", proxy, self)
    self:addChild(layer)

    local id2View = {
        [DIAN_PAO_KE_HU] = {checkSprite = self.ccb_dianpaokehu, label = self.ccb_dianpaokehuTTF, tip = "可接别人打出的牌胡牌", radioGroup = 1},
        [ZI_MO_HU]       = {checkSprite = self.ccb_zimohu, label = self.ccb_zimohuTTF, tip = "只能自摸胡牌", radioGroup = 1},
        [MAI_PAO]        = {checkSprite = self.ccb_maipao, label = self.ccb_maipaoTTF, tip = "开牌前，自愿设置买跑分数", children = {GANG_PAO}},

        [GANG_SHANG_HUA_JIA_BEI] = {checkSprite = self.ccb_gangshanghuajiabei, label = self.ccb_gangshanghuajiabeiTTF, tip = "勾选后杠上花胡分加倍"},
        [GANG_PAO]               = {checkSprite = self.ccb_gangpao, label = self.ccb_gangpaoTTF, tip = "勾选后每个杠分都与双方下跑数相关", root = self.ccb_gangpaoNode},
    }
    for id, t in pairs(id2View) do
        self:_registerBtn(id, t.checkSprite, t.label, t.tip, t.children, t.root, t.radioGroup)
    end
end

function PingdingshanConfig:ccb_onZimohu(control, controlEvent)
    self:_onBtnClick(ZI_MO_HU, controlEvent)
end

function PingdingshanConfig:ccb_onMaipao(control, controlEvent)
    self:_onBtnClick(MAI_PAO, controlEvent)
end

function PingdingshanConfig:ccb_onDianpaokehu(control, controlEvent)
    self:_onBtnClick(DIAN_PAO_KE_HU, controlEvent)
end

function PingdingshanConfig:ccb_onGangshanghuajiabei(control, controlEvent)
    self:_onBtnClick(GANG_SHANG_HUA_JIA_BEI, controlEvent)
end

function PingdingshanConfig:ccb_onGangpao(control, controlEvent)
    self:_onBtnClick(GANG_PAO, controlEvent)
end

return PingdingshanConfig
