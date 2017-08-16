local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"
local ConfigBase    = import(".ConfigBase")

local KaifengConfig = class("KaifengConfig", ConfigBase)

local DIAN_PAO_KE_HU, ZI_MO_HU, MAI_PAO = 1, 2, 3
local ZI_MO_FAN_BEI, AN_GANG_FAN_BEI, GANG_SHANG_HUA_JIA_BEI, QI_DUI_FAN_BEI, GANG_PAO = 4, 5, 6, 7, 8

function KaifengConfig:onEnter()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("kaifengConfig", proxy, self)
    self:addChild(layer)

    local id2View = {
        [DIAN_PAO_KE_HU] = {checkSprite = self.ccb_dianpaokehu, label = self.ccb_dianpaokehuTTF, tip = "可接别人打出的牌胡牌", radioGroup = 1},
        [ZI_MO_HU]       = {checkSprite = self.ccb_zimohu, label = self.ccb_zimohuTTF, tip = "只能自摸胡牌", radioGroup = 1},
        [MAI_PAO]        = {checkSprite = self.ccb_maipao, label = self.ccb_maipaoTTF, tip = "开牌前，自愿设置买跑分数", children = {GANG_PAO}},

        [ZI_MO_FAN_BEI]          = {checkSprite = self.ccb_zimofanbei, label = self.ccb_zimofanbeiTTF, tip = "勾选后自摸胡分翻倍"},
        [AN_GANG_FAN_BEI]        = {checkSprite = self.ccb_angangfanbei, label = self.ccb_angangfanbeiTTF, tip = "勾选后暗杠杠分翻倍"},
        [GANG_SHANG_HUA_JIA_BEI] = {checkSprite = self.ccb_gangshanghuajiabei, label = self.ccb_gangshanghuajiabeiTTF, tip = "勾选后杠上花胡分加倍"},
        [QI_DUI_FAN_BEI]         = {checkSprite = self.ccb_qiduifanbei, label = self.ccb_qiduifanbeiTTF, tip = "勾选后七对胡分翻倍"},
        [GANG_PAO]               = {checkSprite = self.ccb_gangpao, label = self.ccb_gangpaoTTF, tip = "勾选后每个杠分都与双方下跑数相关", root = self.ccb_gangpaoNode},
    }
    for id, t in pairs(id2View) do
        self:_registerBtn(id, t.checkSprite, t.label, t.tip, t.children, t.root, t.radioGroup)
    end
end

function KaifengConfig:ccb_onZimohu(control, controlEvent)
    self:_onBtnClick(ZI_MO_HU, controlEvent)
end

function KaifengConfig:ccb_onMaipao(control, controlEvent)
    self:_onBtnClick(MAI_PAO, controlEvent)
end

function KaifengConfig:ccb_onDianpaokehu(control, controlEvent)
    self:_onBtnClick(DIAN_PAO_KE_HU, controlEvent)
end

function KaifengConfig:ccb_onGangshanghuajiabei(control, controlEvent)
    self:_onBtnClick(GANG_SHANG_HUA_JIA_BEI, controlEvent)
end

function KaifengConfig:ccb_onGangpao(control, controlEvent)
    self:_onBtnClick(GANG_PAO, controlEvent)
end

function KaifengConfig:ccb_onZimofanbei(control, controlEvent)
    self:_onBtnClick(ZI_MO_FAN_BEI, controlEvent)
end

function KaifengConfig:ccb_onAngangfanbei(control, controlEvent)
    self:_onBtnClick(AN_GANG_FAN_BEI, controlEvent)
end

function KaifengConfig:ccb_onQiduifanbei(control, controlEvent)
    self:_onBtnClick(QI_DUI_FAN_BEI, controlEvent)
end

return KaifengConfig
