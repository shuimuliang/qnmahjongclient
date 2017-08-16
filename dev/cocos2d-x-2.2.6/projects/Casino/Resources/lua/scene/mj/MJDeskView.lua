local LCMJCard        		= require "lua/scene/mj/MJCard"
local LCMJCardPanel 		= require "lua/scene/mj/MJCardPanel"
local LCMJDiscardPanel 		= require "lua/scene/mj/MJDiscardPanel"
local LCMJCardPairPanel 	= require "lua/scene/mj/MJCardPairPanel"
local LCMJCardData   	 	= require "lua/scene/mj/data/MJCardData"
local LCMJLocalAI 			= require "lua/scene/mj/MJLocalAI"
local LCMJOtherCardPanel 	= require "lua/scene/mj/MJOtherCardPanel"
local LCMJTimerWidget 	    = require "lua/scene/mj/MJTimerWidget"
local LCMJCardImg 			= require "lua/scene/mj/MJCardImg"
local LCMJChoiceWidget 		= require "lua/scene/mj/MJChoiceWidget"
local LCMJDiceWidget 		= require "lua/scene/mj/MJDiceWidget"
local LCPlayerAvatar    	= require "lua/scene/commons/PlayerAvatar"
local LCMJArrow 			= require "lua/scene/mj/MJArrow"
local LCMJResultLayer       = require "lua/scene/mj/MJResultLayer"
local LCMJResultHNLayer     = require "lua/scene/mj/MJResultHNLayer"
local LCMJVoteLayer       	= require "lua/scene/mj/MJVoteLayer"
local LCMJEndLayer          = require "lua/scene/mj/MJEndLayer"
local LCMJChiWidget         = require "lua/scene/mj/MJChiWidget"
local LCMJTopLayer          = require "lua/scene/mj/MJTopLayer"
local LCMJHunWidget  		= require "lua/scene/mj/MJHunWidget"
local LCMJBuyPaoLayer 		= require "lua/scene/mj/MJBuyPaoLayer"
local LCMJConfigLayer 		= require "lua/scene/mj/MJConfigLayer"
local LCMSettingDialog      = require "lua/scene/mainScene/setting/SettingDialog"


local LCGameChatLayer = require("lua/scene/commons/GameChatLayer")

local LCMJDeskView = class( "LCMJDeskView", function() return CCLayer:create() end )

function LCMJDeskView:ctor(deskStatusMgr,scene)
	CC_SYNTHESIZE(self,"scene", 	    scene)
	CC_SYNTHESIZE(self,"myCardsList", 	{})
	CC_SYNTHESIZE(self,"deskStatusMgr", deskStatusMgr)
	self:loadView()
end

function LCMJDeskView:loadView()
	local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.mjtable,proxy,self)
    self:addChild(layer)
	local nodeMap = {
		{"timerLayer",		"ccb_timerLayer",				"CCLayer"},
		{"choiceLayer",	    "ccb_mjChoiceLayer",			"CCLayer"},
		{"diceLayer",	    "ccb_diceLayer",			    "CCLayer"},
		{"prepareLayer",	"ccb_prepareLayer",			    "CCLayer"},
		{"deskNumLab",		"ccb_deskNumLab",			    "CCLabelTTF"},
		{"leftCardLab",		"ccb_leftCardLab",			    "CCLabelTTF"},
		{"roundNumLab",		"ccb_roundNumLab",			    "CCLabelTTF"},
		{"bgSprite",		"ccb_bgSprite",			        "CCSprite"},
		{"inviteBtn",		"ccb_inviteBtn",			    "CCControlButton"},
		{"guideNode",		"ccb_guideNode",			    "CCNode"},
		{"mjHunBg",			"ccb_mjHunBg",			    	"CCSprite"},
		{"titleImg",		"ccb_tableTitleImg",			"CCSprite"},
		{"titleLab",		"ccb_tableTitleLab",			"CCLabelTTF"},
		{"configLab",		"ccb_configLab",				"CCLabelTTF"},
		{"flexBtnImg",		"ccb_flexBtnImg",				"CCSprite"},
		{"flexBG",			"ccb_flexBG",					"CCSprite"},
		{"propertyLayer",	"ccb_propertyLayer",			"CCLayer"},
	}

	for i=1,4 do
		table.insert(nodeMap,{"showNode"..i,		"ccb_showNode"..i,		"CCSprite"})
		table.insert(nodeMap,{"imgPrepare"..i,		"ccb_labPrepare"..i,	"CCSprite"})
		table.insert(nodeMap,{"cardLayer"..i,		"ccb_cardLayer"..i,		"CCLayerColor"})
		table.insert(nodeMap,{"discardLayer"..i,	"ccb_discardLayer"..i,	"CCLayerColor"})
		table.insert(nodeMap,{"pairLayer"..i,		"ccb_pairLayer"..i,		"CCLayerColor"})
		table.insert(nodeMap,{"avatarLayer"..i,		"ccb_avatarLayer"..i,	"CCLayer"})
	end

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self.actionTable                        = {}
	self.actionTable[eMJOperation.peng]     = function(...) self:peng(...)	end
	self.actionTable[eMJOperation.hu]       = function(...) self:hu(...)	end
	self.actionTable[eMJOperation.chi]      = function(...) self:chi(...)	end
	self.actionTable[eMJOperation.ming]     = function(...) self:ming(...)	end
	self.actionTable[eMJOperation.an]       = function(...) self:an(...)	end
	self.actionTable[eMJOperation.guo]      = function(...) self:guo(...)	end
	self.actionTable[eMJOperation.multiHu]  = function(...) self:multiHu(...)	end
	self.actionTable[eMJOperation.pengGang] = function(...) self:pengGang(...)	end
	self.actionTable[eMJOperation.ziHu] 	= function(...) self:ziHu(...)	end
	self.actionTable[eMJOperation.paoHu] 	= function(...) self:paoHu(...)	end


	self:deskInit()

	self.topLayer = LCMJTopLayer:create(self,self.deskStatusMgr)
	self:addChild(self.topLayer)

end

function LCMJDeskView:deskBuyPaoList()
	self:hidePrepares()
	self.inviteBtn:setVisible(false)
	if self.timerWidget then
		self.timerWidget:hideTime()
	end

 	self.buyPaoLayer = LCMJBuyPaoLayer:create()
	self.buyPaoLayer:setAnchorPoint(0.5,0.5)
	self.bgSprite:addChild(self.buyPaoLayer)

	local selfPaoScore = self.deskStatusMgr:getSelfPaoScore()
	if selfPaoScore and selfPaoScore >= 0 then
		self.buyPaoLayer:setPaoScore(selfPaoScore)
	end
end

function LCMJDeskView:deskBuyPao(msg)
	local score = msg.paoScore
	local pos   = msg.pos

	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	if pType == eMJPType.bottom then
		self.scene:hideLoading()
		self.buyPaoLayer:setPaoScore(msg.paoScore)
	end
	self:setPaoScore(msg.pos,msg.paoScore)
end

function LCMJDeskView:deskOtherJoin(player)
	self:setAvatar(player)
end

function LCMJDeskView:deskPrepareGame(pos)
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	self:setPrepare(pType,true)
end

function LCMJDeskView:getGuideNode()
	return self.guideNode
end

function LCMJDeskView:getTimerLayer()
	return self.timerLayer
end

function LCMJDeskView:deskStartRound()
	if self.buyPaoLayer then
		self.buyPaoLayer:removeFromParentAndCleanup(true)
		self.buyPaoLayer=nil
	end
	self:hidePrepares()
	self.inviteBtn:setVisible(false)
	self:updateTopInfo()
	self:updateBankerIcon()
	for i=1,4 do
		self["cardPanel"..i]:initCards()
	end

	--播放牌局开始的动画
	self.startImg = CCSprite:createWithSpriteFrameName("mj_effect_font_start.png")
	self.startImg:setPositionX(self.timerLayer:getContentSize().width/2)
	self.startImg:setPositionY(self.timerLayer:getContentSize().height/2)
	self.timerLayer:addChild(self.startImg)

	local array = CCArray:create()
	local action1 = CCDelayTime:create(3)
	local action2 = CCFadeOut:create( 0.5 )
	local action3 = CCCallFunc:create(function()
		self.startImg:removeFromParentAndCleanup(true)
		self.startImg = nil
	end)
	array:addObject(action1)
	array:addObject(action2)
	array:addObject(action3)
	local sequence = CCSequence:create(array)
	self.startImg:runAction(sequence)

	--混牌动画
	if self.deskStatusMgr:withHun() then
		self.huning = true
		scheduleOnce(self,function()self:initHunWidget(true) end,4)
	end

	scheduleOnce(self,function() self.huning = false self:checkPanelHun()end,7)
end

function LCMJDeskView:checkPanelHun()
	self["cardPanel"..eMJPType.bottom]:changeHun()
end

function LCMJDeskView:deskSelfDescard(card)
	self["cardPanel"..eMJPType.bottom]:panelDiscard(card)
	self:showDiscardImg(eMJPType.bottom,card)
	local player =  LCAppGlobal:getLoginPlayer()
	LCMusicBox:mjPlayDiscard(player,card.data.num)

	self.topLayer:hideGuide()
end

function LCMJDeskView:deskDescardRecv(pos,cardNum,leftNums)
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	local data = LCMJCardData.new(cardNum)

	if pType == eMJPType.bottom then 
		self["cardPanel"..pType]:panelCheckData(leftNums)
		return 
	end
	self["cardPanel"..pType]:panelDiscardWithData(data)
	self:showDiscardImg(pType,data)

	local player =  self.deskStatusMgr:getPlayerByPos(pos)
	LCMusicBox:mjPlayDiscard(player,data.num)
end

function LCMJDeskView:deskDiscardNotice(pos)
	local function doDiscardNotice()
		local pType = self.deskStatusMgr:getPTypeByPos(pos)
		if pType == eMJPType.bottom then
			self["cardPanel"..pType]:setCardsDropEnable(true)
			self.topLayer:showGuide()
		end
		self.timerWidget:show(pType)
	end

	if self.huning then
		scheduleOnce(self,function() doDiscardNotice() end,3.5)
	else
		doDiscardNotice()
	end
end

--摸牌
function LCMJDeskView:deskDrawCard(pos,cardNum)
	local mjPType = self.deskStatusMgr:getPTypeByPos(pos)
	local data = LCMJCardData.new(cardNum)

	if self.huning then
		self["cardPanel"..mjPType]:panelDrawCard(data,true)
	else
		self["cardPanel"..mjPType]:panelDrawCard(data)
	end
	self.leftCardLab:setString(self.deskStatusMgr:getLeftCards())
end

--初始化当前房间玩家的准备状态
function LCMJDeskView:initPrepare()
	self:hidePrepares()

	local playerMap = self.deskStatusMgr:getPlayerMap()
	for pos,player in pairs(playerMap) do
		if player then
			local pType = self.deskStatusMgr:getPTypeByPos(player.pos)
			self:setPrepare(pType,true)
		end
	end
end

function LCMJDeskView:hidePrepares()
	for i=1,4 do
		self:setPrepare(i,false)
	end
end

--初始化当前房间玩家的头像
function LCMJDeskView:initAvatars()
	local playerMap = self.deskStatusMgr:getPlayerMap()
	for pos,player in pairs(playerMap) do
		if player then
			self:setAvatar(player)
		end
	end
end

function LCMJDeskView:guo(operationItem)
end

function LCMJDeskView:hu(operationItem)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
	local cardNumList = operationItem.cardsList
	self["cardPanel"..pType]:panelHu(cardNumList)

	local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
	LCMusicBox:mjPlayHu(player)

	self.topLayer:playOpEff(pType,eMJOperation.hu)

	if pType ~= eMJPType.bottom then
		self.topLayer:playLoseEff()
	else
		self:shadow()
	end
end

function LCMJDeskView:deskEndRound(itemList)
	if self.frameImg then
		self.frameImg:removeAllChildrenWithCleanup(true)
	end

	for i=1,4 do
		self["cardPanel"..i]:panelClear()
	end

	for pos = 1,4 do
		local player = self.deskStatusMgr:getPlayerByPos(pos)
		if player then
			local pType = self.deskStatusMgr:getPTypeByPos(pos)
			self["avatar"..pType]:setMJScore(player:getScore())
			self["avatar"..pType]:setMJPao(false,0)
		end
	end

	if self.arrow then
		self.arrow:setVisible(false)
	end

	if self.timerWidget then
		self.timerWidget:hideTime()
	end

	self:clearHunWidget()

	self.resultLayer = LCMJResultHNLayer:create(self.deskStatusMgr,itemList)
    self.scene:addChild(self.resultLayer)
	self.leftCardLab:setString(136)
	self:updateBankerIcon(true)
end


function LCMJDeskView:deskShowLiuJu()
	if self.timerWidget then
		self.timerWidget:hideTime()
	end
	self.topLayer:playLiuju()
end

--碰牌
function LCMJDeskView:peng(operationItem)
	local data = LCMJCardData.new(operationItem.keycard)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
    self["cardPanel"..pType]:panelPeng(data)

    local srcPos = operationItem.srcPos
    local srcPtype = self.deskStatusMgr:getPTypeByPos(srcPos)
    self["discardPanel"..srcPtype]:removeCard(data)

    local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
    LCMusicBox:mjPlayPeng(player)

    self.topLayer:playOpEff(pType,eMJOperation.peng)
end

--补杠
function LCMJDeskView:pengGang(operationItem)
	local data = LCMJCardData.new(operationItem.keycard)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
    self["cardPanel"..pType]:panelPengGang(data)

    local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
    LCMusicBox:mjPlayMingGang(player)

    self.topLayer:playOpEff(pType,eMJOperation.ming)
end

--自摸胡
function LCMJDeskView:ziHu(operationItem)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
	local cardNumList = operationItem.cardsList
	self["cardPanel"..pType]:panelHu(cardNumList)

	local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
	LCMusicBox:mjPlayHu(player)

	self.topLayer:playOpEff(pType,eMJOperation.ziHu)

	if pType ~= eMJPType.bottom then
		self.topLayer:playLoseEff()
	else
		self:shadow()
	end
end

--点炮胡
function LCMJDeskView:paoHu(operationItem)
	self:hu(operationItem)
end

function LCMJDeskView:multiHu(operationItem)
	local huList = operationItem.huInfoList
	local isSelf = false
	for _,info in pairs(huList) do
		local pType = self.deskStatusMgr:getPTypeByPos(info.pos)
		local cardNumList = info.cardList
		self["cardPanel"..pType]:panelHu(cardNumList)
		local player =  self.deskStatusMgr:getPlayerByPos(info.pos)
		LCMusicBox:mjPlayHu(player)
		self.topLayer:playOpEff(pType,eMJOperation.hu)
		
		if pType == eMJPType.bottom then
			isSelf = true
		end
	end

	if isSelf then
		self:shadow()
	else
		self.topLayer:playLoseEff()
	end
end

--明杠
function LCMJDeskView:ming(operationItem)
	local data = LCMJCardData.new(operationItem.keycard)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
	self["cardPanel"..pType]:panelGang(data,false)

	local srcPos = operationItem.srcPos
    local srcPtype = self.deskStatusMgr:getPTypeByPos(srcPos)
    self["discardPanel"..srcPtype]:removeCard(data)

    local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
    LCMusicBox:mjPlayMingGang(player)

    self.topLayer:playOpEff(pType,eMJOperation.ming)
end

--暗杠
function LCMJDeskView:an(operationItem)
	local data = LCMJCardData.new(operationItem.keycard)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)

	self["cardPanel"..pType]:panelGang(data,true)

	local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
	LCMusicBox:mjPlayAnGang(player)

	self.topLayer:playOpEff(pType,eMJOperation.an)
end

function LCMJDeskView:chi(operationItem)
	local pType = self.deskStatusMgr:getPTypeByPos(operationItem.desPos)
	self["cardPanel"..pType]:panelChi(operationItem.cardsList,operationItem.keycard)

	local srcPos = operationItem.srcPos
    local srcPtype = self.deskStatusMgr:getPTypeByPos(srcPos)
    self["discardPanel"..srcPtype]:removeCard(data)

    local player =  self.deskStatusMgr:getPlayerByPos(operationItem.desPos)
    LCMusicBox:mjPlayChi(player)
    self.topLayer:playOpEff(pType,eMJOperation.chi)
end

function LCMJDeskView:deskOperationNotice(operationList)
	if not operationList or #operationList < 1 then return end

	local opType = operationList[1].type
	if opType == eMJOperation.otherOp then
		self.timerWidget:dark()
		return
	end

	local pos = operationList[1].desPos
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	if pType == eMJPType.bottom then
		self.choiceWidget:showChoice(operationList)
	end
	self.timerWidget:show(pType)
end

function LCMJDeskView:deskOperation(operationItem)
	local func = self.actionTable[operationItem.type]
	func(operationItem)
end

function LCMJDeskView:deskVoteClose(msg)
	if self.ownerClose == true then self.ownerClose = false return end

	if not self.voteLayer then
		self.voteLayer = LCMJVoteLayer:create(self.deskStatusMgr)
    	self.scene:addChild(self.voteLayer)
	end

	local result,name = self.voteLayer:updateByMsg(msg)
	if result == eMJVoteType.refuse then
		self.voteLayer:removeFromParentAndCleanup(true)
		self.voteLayer = nil
		LCToastHUD.new(name.."拒绝解散房间"):showInScene()
	end
end

--投票结束游戏时，弹出大结算页面
function LCMJDeskView:deskSettlementForVote()
	local msg = self.deskStatusMgr.settlementMsg
	if msg then
		if self.resultLayer and not tolua.isnull(self.resultLayer) then
			self.resultLayer:removeFromParentAndCleanup(true)
			self.resultLayer = nil
		end

		local endLayer = LCMJEndLayer:create(self.deskStatusMgr,msg.SettleList)
	    self:addChild(endLayer)
	end
end

--听牌
function LCMJDeskView:readyHand()

end

function LCMJDeskView:getAvatarByPos(pos)
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	local te =  self["avatar"..pType]
	return self["avatar"..pType]
end

function LCMJDeskView:test()
	local startP1 = ccp(-80,70)
	local endP1   = ccp(-45,0)
	local startP2 = ccp(40,70)
	local endP2   = ccp(45,0)

	local dice1 = LCMJDiceWidget:create()
	self.diceLayer:addChild(dice1)
	dice1:showNum(5,startP1,endP1)

	local dice2 = LCMJDiceWidget:create()
	self.diceLayer:addChild(dice2)
	dice2:showNum(6,startP2,endP2)
end

function LCMJDeskView:deskCloseRoom(closeType)
	if self.voteLayer then
		self.voteLayer:removeFromParentAndCleanup(true)
		self.voteLayer = nil
	end

 	if closeType == eMJCloseRoom.forceClose then
 		local str = "房主已经解散房间！"
 		local key = {}
 		key.toastString = str
 		LCSceneTransit:replaceScene(eSceneName.main,key)
 	end
end

function LCMJDeskView:shadow()
	local array = CCArray:create()
	array:addObject(CCScaleTo:create(0.2, 1.5))
	local off = 4
	for i=1,8 do
		array:addObject(CCMoveBy:create(0.07, ccp(off, off)))
		array:addObject(CCMoveBy:create(0.07, ccp(-off, -off)))
	end

	array:addObject(CCScaleTo:create(0.2, 1.0))
	self:runAction(CCSequence:create(array))
end

function LCMJDeskView:deskExitRoom(pos)
	local isSelf = (pos == self.deskStatusMgr:getSelfPos())
	if isSelf then
		LCSceneTransit:replaceScene(eSceneName.main)
	else
		self:clearAvatar(pos)
	end
end

function LCMJDeskView:deskGameChat(msg)
	if self.topLayer then
		self.topLayer:chatShow(msg)
	end
end

function LCMJDeskView:ccb_backBtnClicked()
	local isStart = self.deskStatusMgr:getIsStartGame()
	local isOwner = self.deskStatusMgr:getIsOwner()

	if isStart then
		LCMJSendMgr.sendVoteClose(eMJVoteType.agree)
	else
		if isOwner then
			LCMessageBox:showConfirm(self,{text = _("是否解散房间？"),
				buttonText = "确定",
			    hideClose = false,
			    callback = function()
			    	self.ownerClose = true
			    	LCMJSendMgr.sendCloseRoom()
					self.scene:showLoading(_("请稍等..."))
			    end})
		else
			LCMessageBox:showConfirm(self,{text = _("是否退出房间？"),
				buttonText = "确定",
			    hideClose = false,
			    callback = function()
					LCMJSendMgr.sendExitRoom()
					self.scene:showLoading(_("请稍等..."))
			    end})
		end
	end
end

function LCMJDeskView:initPanels()
	for i=1,4 do
		self["discardPanel"..i] 	= LCMJDiscardPanel.new(self["discardLayer"..i],	i , self.arrow)
		self["pairPanel"..i] 		= LCMJCardPairPanel.new(self["pairLayer"..i],	i )
		if i == eMJPType.bottom then
			self["cardPanel"..i] 	= LCMJCardPanel.new(self["cardLayer"..i],		self["discardPanel"..i],	self["pairPanel"..i] )
		else
			self["cardPanel"..i] 	= LCMJOtherCardPanel.new(self["cardLayer"..i],	self["discardPanel"..i],	self["pairPanel"..i],	i )
		end
	end
end

function LCMJDeskView:initConfig()
	local configs = self.deskStatusMgr:getConfigs()
	local mjType = self.deskStatusMgr:getMJType()
	local configStr = ""
	for i=1,#configs do
		local config = configs[i]
		local itemStr = eMJRuleItem[mjType][config]
		if not itemStr then return end
		if i == #configs then 
			configStr = configStr .. itemStr
		else
			configStr = configStr .. itemStr .. "  "
		end
	end
	self.configLab:setString(configStr)

	if string.len(configStr) <= 57 then
		self.notFlex = true
		self:ccb_flexBtnClicked()
		self.flexBtnImg:setVisible(false)
		self.propertyLayer:setPositionX(30)
		self.configLab:setPositionX(30)
	end
end

function LCMJDeskView:deskInit()
	self:initArrow()
	self:initAvatars()
	self:initPrepare()
	self:initChoiceWidget()
	self:initTimerWidget()
	self:initPanels()
	self:initConfig()
	self:updateTopInfo()

	local switch = {
	   [eMJGameStatus.playingRound] = function() self:doPlayingRound() end,
		[eMJGameStatus.oneRoundEnd] = function() self:doOneRoundEnd()  end,
		[eMJGameStatus.allRoundEnd] = function() self:doAllRoundEnd()  end,
		[eMJGameStatus.beforeRound] = function() self:doBeforeRound()  end,
	}
	local f = switch[self.deskStatusMgr.gameStatus]
	if f then f() end

	self.ai = LCMJLocalAI.new(self)
end

function LCMJDeskView:doBeforeRound()
	if CSDKManager:getInstance():isWXAppInstalled() then
		self.inviteBtn:setVisible(true)
	end
	LCMJSendMgr.sendPrepare()
end

function LCMJDeskView:doPlayingRound()
	self.inviteBtn:setVisible(false)
	self:updateBankerIcon()

	--隐藏准备提示
	self:hidePrepares()

	--panel更新
	for i=1,4 do self["cardPanel"..i]:relinkInit() end

	--计时器更新
	local curP = self.deskStatusMgr:getCurPos()
	local pType = self.deskStatusMgr:getPTypeByPos(curP)
	self.timerWidget:show(pType)

	--箭头显示
	local lastP = self.deskStatusMgr:getLastPos()
	local pType = self.deskStatusMgr:getPTypeByPos(lastP)
	if pType then
		self["discardPanel"..pType]:showArrow()
	end

	--如果还存在牌局开始图片，则删除
	if self.startImg then
		self.startImg:removeFromParentAndCleanup(true)
		self.startImg = nil
	end

	if self.deskStatusMgr:withHun() then
		self:initHunWidget(false)
	end

	local paoScoreMap = self.deskStatusMgr:getPaoScoreMap()
	for pos,score in pairs(paoScoreMap) do
		self:setPaoScore(pos,score)
	end
end

function LCMJDeskView:doOneRoundEnd()
	self.inviteBtn:setVisible(false)
	self:hidePrepares()
end

function LCMJDeskView:doAllRoundEnd()
	self.inviteBtn:setVisible(false)
	self:hidePrepares()
end

function LCMJDeskView:initTimerWidget()
	local selfPos = self.deskStatusMgr:getSelfPos()
	self.timerWidget = LCMJTimerWidget:create(selfPos)
	self.timerLayer:addChild(self.timerWidget)
	self.timerWidget:hideTime()
end

function LCMJDeskView:initArrow()
	self.arrow = LCMJArrow.new(self)
	self.bgSprite:addChild(self.arrow)
	self.arrow:setVisible(false)
end

function LCMJDeskView:initChoiceWidget()
	self.choiceWidget = LCMJChoiceWidget:create(self)
	self.choiceLayer:addChild(self.choiceWidget)
	self.choiceWidget:setVisible(false)
end

function LCMJDeskView:updateTopInfo()
	self.leftCardLab:setString(self.deskStatusMgr:getLeftCards())
	self.deskNumLab:setString(self.deskStatusMgr:getRoomID())
	local curRound = self.deskStatusMgr:getCurRound()
	local totalRound = self.deskStatusMgr:getTotalRound()
	self.roundNumLab:setString(curRound.."/"..totalRound)

	local mjType = self.deskStatusMgr:getMJType()
	if mjType == 3 then
		local frame=CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_table_bg_tuidaohu.png")
		if frame and self.titleImg then self.titleImg:setDisplayFrame(frame) end
		if self.titleLab then self.titleLab:setString("推倒胡") end
	elseif mjType == 4 then
		local frame=CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_table_bg_font_kaifeng.png")
		if frame and self.titleImg then self.titleImg:setDisplayFrame(frame) end
		if self.titleLab then self.titleLab:setString("开封麻将") end 
	end
end

function LCMJDeskView:updateBankerIcon(hide)
	local pos = self.deskStatusMgr:getBankerPos()
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	for i=1,4 do
		if self["avatar"..i] then self["avatar"..i]:setMJBanker(not hide and tonumber(pType) == i) end
	end
end

function LCMJDeskView:setPaoScore(pos,paoScore)
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	if not pType then return end
	if paoScore < 0 then return end
	if self["avatar"..pType] then
		self["avatar"..pType]:setMJPao(true,paoScore)
	end
end

function LCMJDeskView:setAvatar(mjPlayer)
	local pType = self.deskStatusMgr:getPTypeByPos(mjPlayer.pos)
	if self["avatar"..pType] then
		self["avatar"..pType]:removeFromParentAndCleanup(true)
		self["avatar"..pType] = nil
	end

	local key = {}
	key.clipMode = "mj_head_bg.png"
	key.isMJ = true
	self["avatar"..pType] = LCPlayerAvatar:create(mjPlayer.avatar,function()
    	local distanceInfoList = self.deskStatusMgr:getDistanceInfosByPlayer(mjPlayer)
    	require "lua/scene/mainScene/playerInfo/PlayerInfoDialog".new({id = mjPlayer:getId(), avatar = mjPlayer:getAvatar(), nickname = mjPlayer:getNickName(), ip = mjPlayer:getIp(), distanceInfoList = distanceInfoList}):showInScene()
		end,key)

	self["avatar"..pType]:setMJScore(mjPlayer.score)
	self["avatarLayer"..pType]:addChild(self["avatar"..pType])
end

function LCMJDeskView:clearAvatar(pos)
	local pType = self.deskStatusMgr:getPTypeByPos(pos)
	if self["avatar"..pType] then
		self["avatar"..pType]:removeFromParentAndCleanup(true)
		self["avatar"..pType] = nil
	end
	self:setPrepare(pType,false)
end

function LCMJDeskView:setPrepare(pType,isShow)
	local img = self["imgPrepare"..pType]
	isShow = isShow or false
	img:setVisible(isShow)
end

--打出牌的显示
function LCMJDeskView:showDiscardImg(mjPType,cardOrData)
	if not self.frameImg then
		self.frameImg = CCSprite:createWithSpriteFrameName("mj_frame_chupai.png")
		self:addChild(self.frameImg)
	else
		self.frameImg:removeAllChildrenWithCleanup(true)
	end

	self.frameImg:setVisible(true)
	self.frameImg:setOpacity(255)

	local wordP = self["showNode"..mjPType]:getParent():convertToWorldSpace(ccp(self["showNode"..mjPType]:getPositionX(),self["showNode"..mjPType]:getPositionY()))
	local nodeP = self:convertToNodeSpace(wordP)
	self.frameImg:setPosition(nodeP.x,nodeP.y)

	if cardOrData.__cname == "LCMJCard" then cardOrData = cardOrData.data end
	local cardImg =  LCMJCardImg.new(cardOrData,eMJPType.bottom,eMJImgType.pair)
	self.frameImg:addChild(cardImg)
	self.frameImg:setCascadeOpacityEnabled(true)
	cardImg:setPositionX(self.frameImg:getContentSize().width/2)
	cardImg:setPositionY(self.frameImg:getContentSize().height/2)

	function action()
		local action1 = CCFadeOut:create( 0.3 )
		local action2 = CCCallFunc:create(function() self.frameImg:setVisible(false) end)
		local sequence = CCSequence:createWithTwoActions(action1, action2)
		self.frameImg:runAction(sequence)
	end

	scheduleOnce(self,function() action() end,1.2)
end


function LCMJDeskView:initHunWidget(isAnim)
	local function callback( )
		self.deskStatusMgr:updateLeftCardsNum()
		self.leftCardLab:setString(self.deskStatusMgr:getLeftCards())
	end
	if self.hunWidget then self.hunWidget:removeFromParentAndCleanup(true) self.hunWidget = nil end

	local preHunCard = self.deskStatusMgr:getPreHunCard()
	local hunCard = self.deskStatusMgr:getHunCard()
	self.hunWidget = LCMJHunWidget:create(preHunCard,hunCard,self.mjHunBg,isAnim,callback)
	self.bgSprite:addChild(self.hunWidget)
	self.hunWidget:setAnchorPoint(0.5,0.5)
	self.hunWidget:setPosition(self.timerLayer:getPositionX(),self.timerLayer:getPositionY())
end

function LCMJDeskView:clearHunWidget()
	if self.hunWidget then self.hunWidget:removeFromParentAndCleanup(true) self.hunWidget = nil end
	self.mjHunBg:setVisible(false)
end

function LCMJDeskView:ccb_chatBtnClicked(control,controlEvent)
	if self.topLayer then self.topLayer:onChatBtnClicked(control,controlEvent) end
end

function LCMJDeskView:ccb_inviteBtnClicked(control,controlEvent)
	if self.topLayer then  self.topLayer:onInviteBtnClicked(control,controlEvent) end
end

function LCMJDeskView:ccb_voiceBtnClicked(control,controlEvent)
	if self.topLayer then self.topLayer:onVoiceBtnClicked(control,controlEvent) end
end

function LCMJDeskView:ccb_configBtnClicked(control,controlEvent)
	local configLayer = LCMJConfigLayer:create(self.deskStatusMgr)
	self:addChild(configLayer)
end

function LCMJDeskView:ccb_settingBtnClicked(control,controlEvent)
	local settingDialog =	LCMSettingDialog.new():showInScene()
	settingDialog:hideTwoBtns()
end

function LCMJDeskView:ccb_flexBtnClicked(control,controlEvent)
	if not self.isTopFiex then
		self.flexBG:setPositionY(162)
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_table_attribute_configured_arrow_down.png")
		self.flexBtnImg:setDisplayFrame(frame)
		self.isTopFiex = true
		self.configLab:setDimensions(CCSizeMake(425.0,30))
	else
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_table_attribute_configured_arrow_up.png")
		self.flexBtnImg:setDisplayFrame(frame)
		self.isTopFiex = false
		
		if self.notFlex then return end

		self.flexBG:setPositionY(130.0)
		self.configLab:setDimensions(CCSizeMake(425.0,60))
	end
end

return LCMJDeskView
