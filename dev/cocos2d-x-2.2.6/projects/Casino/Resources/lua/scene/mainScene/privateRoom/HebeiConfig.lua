local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"
local ConfigBase    = import(".ConfigBase")

local HebeiConfig = class("HebeiConfig", ConfigBase)

local DAI_FENG_PAI, KE_CHI_PAI, DIAN_PAO_KE_HU, HUANG_ZHUANG_HUANG_GANG = 1, 2, 3, 4
local DAI_ZHUANG_XIAN, MEN_QING, BIAN_KA_DIAO, ZHUO_WU_KUI, DAI_HUN_ER, SU_HU, HUN_ER_DIAO = 5, 6, 7, 8, 9, 10, 11
local DIAN_PAO_YI_JIA_CHU, DIAN_PAO_SAN_JIA_CHU, DIAN_PAO_DA_BAO = 12, 13, 14

function HebeiConfig:onEnter()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("hebeiConfig", proxy, self)
    self:addChild(layer)
    local id2View = {
        [DAI_FENG_PAI]            = {checkSprite = self.ccb_daifengpai, label = self.ccb_daifengpaiTTF, tip = "勾选后牌组中包含东南西北风和中发白牌"},
        [KE_CHI_PAI]              = {checkSprite = self.ccb_kechipai, label = self.ccb_kechipaiTTF, tip = "上家打出牌，与自己手中的牌组成顺子时可选择吃此张牌"},
        [DIAN_PAO_KE_HU]          = {checkSprite = self.ccb_dianpaokehu, label = self.ccb_dianpaokehuTTF, tip = "可接别人打出的牌胡牌", children = {DIAN_PAO_YI_JIA_CHU, DIAN_PAO_SAN_JIA_CHU, DIAN_PAO_DA_BAO}},
        [HUANG_ZHUANG_HUANG_GANG] = {checkSprite = self.ccb_huangzhuanghuanggang, label = self.ccb_huangzhuanghuanggangTTF, tip = "流局不计算杠分"},

        [DAI_ZHUANG_XIAN] = {checkSprite = self.ccb_daizhuangxian, label = self.ccb_daizhuangxianTTF, tip = "庄家输赢结算时分数皆x2"},
        [MEN_QING]        = {checkSprite = self.ccb_menqing, label = self.ccb_menqingTTF, tip = "胡牌时没有吃，碰，明杠过，得分x2"},
        [BIAN_KA_DIAO]    = {checkSprite = self.ccb_biankadiao, label = self.ccb_biankadiaoTTF, tip = "胡边，卡，吊分数x2。边：手里有12或89牌胡3/7牌；卡：胡顺子中间的那张牌；吊：手里有一张牌，单吊另一张牌做将"},
        [ZHUO_WU_KUI]     = {checkSprite = self.ccb_zhuowukui, label = self.ccb_zhuowukuiTTF, tip = "手中有4万和6万胡卡五万，得分x4"},
        [DAI_HUN_ER]      = {checkSprite = self.ccb_daihuner, label = self.ccb_daihunerTTF, tip = "混儿牌可以代替任何牌。抓完牌后，庄家翻出一张牌，此张牌的数字加一，为混儿牌", children = {SU_HU, HUN_ER_DIAO}},
        [SU_HU]           = {checkSprite = self.ccb_suhu, label = self.ccb_suhuTTF, tip = "胡牌时牌中没有混儿牌，得分x2", root = self.ccb_suhuNode},
        [HUN_ER_DIAO]     = {checkSprite = self.ccb_hunerdiao, label = self.ccb_hunerdiaoTTF, tip = "混儿做将牌，单吊任意一张牌胡牌，只能自摸胡，得分x2", root = self.ccb_hunerdiaoNode},

        [DIAN_PAO_YI_JIA_CHU]  = {checkSprite = self.ccb_dianpaoyijiachu, label = self.ccb_dianpaoyijiachuTTF, tip = "只有点炮者输分，其他两家不输分", radioGroup = 1, root = self.ccb_suanfenNode},
        [DIAN_PAO_SAN_JIA_CHU] = {checkSprite = self.ccb_dianpaosanjiachu, label = self.ccb_dianpaosanjiachuTTF, tip = "有人点炮后，三家皆输分，点炮者多输一倍的分数", radioGroup = 1, root = self.ccb_suanfenNode},
        [DIAN_PAO_DA_BAO]      = {checkSprite = self.ccb_dianpaodabao, label = self.ccb_dianpaodabaoTTF, tip = "点炮者输三倍的分数，其他两家不输分", radioGroup = 1, root = self.ccb_suanfenNode},
    }
    for id, t in pairs(id2View) do
        self:_registerBtn(id, t.checkSprite, t.label, t.tip, t.children, t.root, t.radioGroup)
    end
end

function HebeiConfig:ccb_onDaifengpai(control, controlEvent)
    self:_onBtnClick(DAI_FENG_PAI, controlEvent)
end

function HebeiConfig:ccb_onKechipai(control, controlEvent)
    self:_onBtnClick(KE_CHI_PAI, controlEvent)
end

function HebeiConfig:ccb_onDianpaokehu(control, controlEvent)
    self:_onBtnClick(DIAN_PAO_KE_HU, controlEvent)
end

function HebeiConfig:ccb_onHuangzhuanghuanggang(control, controlEvent)
    self:_onBtnClick(HUANG_ZHUANG_HUANG_GANG, controlEvent)
end

function HebeiConfig:ccb_onDaizhuangxian(control, controlEvent)
    self:_onBtnClick(DAI_ZHUANG_XIAN, controlEvent)
end

function HebeiConfig:ccb_onMenqing(control, controlEvent)
    self:_onBtnClick(MEN_QING, controlEvent)
end

function HebeiConfig:ccb_onBiankadiao(control, controlEvent)
    self:_onBtnClick(BIAN_KA_DIAO, controlEvent)
end

function HebeiConfig:ccb_onZhuowukui(control, controlEvent)
    self:_onBtnClick(ZHUO_WU_KUI, controlEvent)
end

function HebeiConfig:ccb_onDaihuner(control, controlEvent)
    self:_onBtnClick(DAI_HUN_ER, controlEvent)
end

function HebeiConfig:ccb_onSuhu(control, controlEvent)
    self:_onBtnClick(SU_HU, controlEvent)
end

function HebeiConfig:ccb_onHunerdiao(control, controlEvent)
    self:_onBtnClick(HUN_ER_DIAO, controlEvent)
end

function HebeiConfig:ccb_onSuanfen(control, controlEvent)
    if control == self.ccb_dianpaoyijiachuBtn then
        self:_onBtnClick(DIAN_PAO_YI_JIA_CHU, controlEvent)
    elseif control == self.ccb_dianpaosanjiachuBtn then
        self:_onBtnClick(DIAN_PAO_SAN_JIA_CHU, controlEvent)
    else
        self:_onBtnClick(DIAN_PAO_DA_BAO, controlEvent)
    end
end

return HebeiConfig
