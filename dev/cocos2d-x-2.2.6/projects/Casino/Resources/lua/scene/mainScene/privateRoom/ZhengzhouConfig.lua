local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"
local ConfigBase    = import(".ConfigBase")

local ZhengzhouConfig = class("ZhengzhouConfig", ConfigBase)

local DAI_FENG_PAI, DAI_HUN_ER, MAI_PAO, DIAN_PAO_KE_HU = 1, 2, 3, 4
local QI_DUI_JIA_BEI, GANG_SHANG_HUA_JIA_BEI, ZHUANG_JIA_JIA_DI, GANG_PAO= 5, 6, 7, 8

function ZhengzhouConfig:onEnter()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("zhengzhouConfig", proxy, self)
    self:addChild(layer)

    local id2View = {
        [DAI_FENG_PAI]   = {checkSprite = self.ccb_daifengpai, label = self.ccb_daifengpaiTTF, tip = "勾选后牌组中包含东南西北风和中发白牌"},
        [DAI_HUN_ER]     = {checkSprite = self.ccb_daihuner, label = self.ccb_daihunerTTF, tip = "混牌只能在胡牌时充当任何牌，在碰、杠时不能充当其他的牌"},
        [MAI_PAO]        = {checkSprite = self.ccb_maipao, label = self.ccb_maipaoTTF, tip = "开牌前，自愿设置买跑分数", children = {GANG_PAO}},
        [DIAN_PAO_KE_HU] = {checkSprite = self.ccb_dianpaokehu, label = self.ccb_dianpaokehuTTF, tip = "可接别人打出的牌胡牌"},

        [QI_DUI_JIA_BEI]         = {checkSprite = self.ccb_qiduijiabei, label = self.ccb_qiduijiabeiTTF, tip = "勾选后七对牌型胡牌时，胡分加倍"},
        [GANG_SHANG_HUA_JIA_BEI] = {checkSprite = self.ccb_gangshanghuajiabei, label = self.ccb_gangshanghuajiabeiTTF, tip = "勾选后杠上花胡分加倍"},
        [ZHUANG_JIA_JIA_DI]      = {checkSprite = self.ccb_zhuangjiajiadi, label = self.ccb_zhuangjiajiadiTTF, tip = "勾选后若结算双方有任何一方为庄家，则胡分和杠分再加一个底分"},
        [GANG_PAO]               = {checkSprite = self.ccb_gangpao, label = self.ccb_gangpaoTTF, tip = "勾选后每个杠分都与双方下跑数相关", root = self.ccb_gangpaoNode},
    }
    for id, t in pairs(id2View) do
        self:_registerBtn(id, t.checkSprite, t.label, t.tip, t.children, t.root)
    end
end

function ZhengzhouConfig:ccb_onDaifengpai(control, controlEvent)
    self:_onBtnClick(DAI_FENG_PAI, controlEvent)
end

function ZhengzhouConfig:ccb_onDaihuner(control, controlEvent)
    self:_onBtnClick(DAI_HUN_ER, controlEvent)
end

function ZhengzhouConfig:ccb_onMaipao(control, controlEvent)
    self:_onBtnClick(MAI_PAO, controlEvent)
end

function ZhengzhouConfig:ccb_onDianpaokehu(control, controlEvent)
    self:_onBtnClick(DIAN_PAO_KE_HU, controlEvent)
end

function ZhengzhouConfig:ccb_onQiduijiabei(control, controlEvent)
    self:_onBtnClick(QI_DUI_JIA_BEI, controlEvent)
end

function ZhengzhouConfig:ccb_onGangshanghuajiabei(control, controlEvent)
    self:_onBtnClick(GANG_SHANG_HUA_JIA_BEI, controlEvent)
end

function ZhengzhouConfig:ccb_onGangpao(control, controlEvent)
    self:_onBtnClick(GANG_PAO, controlEvent)
end

function ZhengzhouConfig:ccb_onZhuangjiajiadi(control, controlEvent)
    self:_onBtnClick(ZHUANG_JIA_JIA_DI, controlEvent)
end

return ZhengzhouConfig
