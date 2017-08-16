
local selfEast               = {}
selfEast[eMJDirect.east]     = eMJPType.bottom
selfEast[eMJDirect.south]    = eMJPType.right
selfEast[eMJDirect.west]     = eMJPType.top
selfEast[eMJDirect.north]    = eMJPType.left

local selfSouth              = {}
selfSouth[eMJDirect.east]    = eMJPType.left
selfSouth[eMJDirect.south]   = eMJPType.bottom
selfSouth[eMJDirect.west]    = eMJPType.right
selfSouth[eMJDirect.north]   = eMJPType.top

local selfWest               = {}
selfWest[eMJDirect.east]     = eMJPType.top
selfWest[eMJDirect.south]    = eMJPType.left
selfWest[eMJDirect.west]     = eMJPType.bottom
selfWest[eMJDirect.north]    = eMJPType.right

local selfNorth              = {}
selfNorth[eMJDirect.east]    = eMJPType.right
selfNorth[eMJDirect.south]   = eMJPType.top
selfNorth[eMJDirect.west]    = eMJPType.left
selfNorth[eMJDirect.north]   = eMJPType.bottom

local POS_2_PTYPE            = {}
POS_2_PTYPE[eMJDirect.east]  = selfEast
POS_2_PTYPE[eMJDirect.south] = selfSouth
POS_2_PTYPE[eMJDirect.west]  = selfWest
POS_2_PTYPE[eMJDirect.north] = selfNorth


local LCMJDeskStatusMgr = class("LCMJDeskStatusMgr")

function LCMJDeskStatusMgr:ctor(scene, enterRoomRecv )
    CC_SYNTHESIZE(self, "roomID",       enterRoomRecv.roomID)
    CC_SYNTHESIZE(self, "leftCards",    enterRoomRecv.leftCards)
    CC_SYNTHESIZE(self, "curRound",     enterRoomRecv.curRound)
    CC_SYNTHESIZE(self, "gameStatus",   enterRoomRecv.gameStatus)
    CC_SYNTHESIZE(self, "totalRound",   enterRoomRecv.totalRound)
    CC_SYNTHESIZE(self, "bankerPos",    enterRoomRecv.bankerPos)
    CC_SYNTHESIZE(self, "curPos",       enterRoomRecv.curPos)
    CC_SYNTHESIZE(self, "lastPos",      enterRoomRecv.lastPos)
    CC_SYNTHESIZE(self, "hunCard",      enterRoomRecv.hunCard)
    CC_SYNTHESIZE(self, "configs",      enterRoomRecv.configs)
    CC_SYNTHESIZE(self, "MJType",       enterRoomRecv.mjType)
    CC_SYNTHESIZE(self, "cheat",        enterRoomRecv.cheat)     
    CC_SYNTHESIZE(self, "preHunCard",   0)
    CC_SYNTHESIZE(self, "playerMap",    {}) -- 东南西北
    CC_SYNTHESIZE(self, "cardsNumMap",  {}) -- 玩家牌数map 东南西北
    CC_SYNTHESIZE(self, "selfPos",      1)
    CC_SYNTHESIZE(self, "myCards",      {})
    CC_SYNTHESIZE(self, "paoScoreMap",  {})
    CC_SYNTHESIZE(self, "isStartGame",  enterRoomRecv.gameStatus ~= eMJGameStatus.beforeRound)
    CC_SYNTHESIZE(self, "isRelinkEnd",  enterRoomRecv.gameStatus == eMJGameStatus.oneRoundEnd)


    CC_SYNTHESIZE(self, "cardsSetMap",  {})


    local player = LCAppGlobal:getLoginPlayer()
    local pbPlayerList = enterRoomRecv.playerList
    for i=1,#pbPlayerList do
        local pb = pbPlayerList[i]
        local mjPlayer = LCMJPlayerData:createByPB(pb)
        self.playerMap[mjPlayer.pos] = mjPlayer

        if player.id == mjPlayer.id then self.selfPos = mjPlayer.pos end
    end

    self:initCards(enterRoomRecv.cardsInfoList)
end

function LCMJDeskStatusMgr:getIsOwner()
    --房主是在东部的位置
    local owner = self.playerMap[eMJDirect.east]
    local player = LCAppGlobal:getLoginPlayer()
    if owner and player then
        if owner.id == player.id then
            return true
        end
    end
    return false
end

function LCMJDeskStatusMgr:initCards(cardsInfoList)
    if not cardsInfoList or #cardsInfoList < 2 then return end

    for i=1, #cardsInfoList do
        local info = cardsInfoList[i]
        local cardsSet = LCMJCardsSet.new(info,self.hunCard)
        self.cardsSetMap[info.pos] = cardsSet
        self.paoScoreMap[info.pos] = info.paoScore
    end
end

function LCMJDeskStatusMgr:exitRoom(pos)
    self.playerMap[pos] = nil
end

function LCMJDeskStatusMgr:getSelfCardsSet()
    return self.cardsSetMap[self.selfPos]
end

function LCMJDeskStatusMgr:getCardsSetByPos(pos)
    return self.cardsSetMap[pos]
end

function LCMJDeskStatusMgr:getCardsSetByPType(pType)
    local pos = self:getPosByPType(pType)
    return self.cardsSetMap[pos]
end

--获取与其他用户的位置信息 infoList 
-- info = {name,distance}
function LCMJDeskStatusMgr:getDistanceInfosByPlayer(showPlayer)
    if not showPlayer then return nil end

    local list = {}
    local sLat = showPlayer.latitude
    local sLon = showPlayer.longitude

    for _,player in pairs(self.playerMap) do
        if player.id ~= showPlayer.id then
            local pLat = player.latitude
            local pLon = player.longitude
            local distance = POINTS_2_DISTANCE(sLat,sLon,pLat,pLon)

            local info = {}
            info.name = player.nickName
            info.distance = math.floor(distance)
            if info.distance <=1 then info.distance = 1 end

            table.insert(list,info)
        end
    end

    return list
end

function LCMJDeskStatusMgr:isConfigBuyPao()
    local isBuyPao = false
    if self.MJType == 2 or self.MJType == 3 or self.MJType == 4 then
        for k,config in pairs(self.configs) do
            if config == 3 then isBuyPao = true end
        end   
    end
    return isBuyPao
end

function LCMJDeskStatusMgr:settlement(msg)
    self.settlementMsg = msg
end

function LCMJDeskStatusMgr:getSelfPlayer()
    return self.playerMap[self.selfPos]
end

function LCMJDeskStatusMgr:updateLeftCardsNum()
    self.leftCards = self.leftCards - 1
end

function LCMJDeskStatusMgr:getMyCardsTest()
    -- local temp = {1,2,4,6,2,23,14,25,17,20,30,32,37}
    local temp = {1,2,4,6,2,23,14,25,17,21,22,32,32,6}

    local t = {}
    for i=1,#temp do
        local data = LCMJCardData.new(temp[i])
        table.insert(t,data)
    end
    return t
end

function LCMJDeskStatusMgr:roundEnd(itemList)
    self.isLiuJu = true

    for i=1,#itemList do
        local item = itemList[i]
        local pos = item.pos
        local player = self.playerMap[pos]
        player:setScore(item.curScore)
        if item.isHu == 1 then self.isLiuJu = false end
    end

    self.paoScoreMap = {}
end

function LCMJDeskStatusMgr:getIsLiuJu()
    return self.isLiuJu
end

function LCMJDeskStatusMgr:addPlayerByPB(pbPlayer)
    local mjPlayer = LCMJPlayerData:createByPB(pbPlayer)
    self.playerMap[mjPlayer.pos] = mjPlayer
end

function LCMJDeskStatusMgr:getPlayerByPos(pos)
    return self.playerMap[pos]
end

function LCMJDeskStatusMgr:getPlayerByID(playerID)
    for pos, player in pairs(self.playerMap) do
        if player.id == playerID then
            return player
        end
    end
end

function LCMJDeskStatusMgr:getPTypeByPos(pos)
    local dirTable = POS_2_PTYPE[self.selfPos]
    if not dirTable then return nil end
    return dirTable[pos]
end

function LCMJDeskStatusMgr:getPosByPType(pType)
    local dirTable = POS_2_PTYPE[self.selfPos]
    for pos,dir in pairs(dirTable) do
        if dir == pType then return pos end
    end
end

function LCMJDeskStatusMgr:getPosByPlayerID(playerID)
    for pos, player in pairs(self.playerMap) do
        if player.id == playerID then
            return pos
        end
    end
end

function LCMJDeskStatusMgr:getCardsNumByPType(pType)
    local pos = self:getPosByPType(pType)
    return self.cardsNumMap[pos]
end

function LCMJDeskStatusMgr:isLastRound()
    return self.curRound == self.totalRound
end

function LCMJDeskStatusMgr:onBuyPaoList()
    self.isStartGame = true
end

function LCMJDeskStatusMgr:onBuyPao(pos,score)
    self.paoScoreMap[pos] = score
end

function LCMJDeskStatusMgr:getSelfPaoScore()
    return self.paoScoreMap[self.selfPos]
end

function LCMJDeskStatusMgr:getPaoScoreByPos(pos)
    return self.paoScoreMap[pos]
end

function LCMJDeskStatusMgr:withHun()
    return self.hunCard > 0
end

function LCMJDeskStatusMgr:roundStart(pb)
    self.isStartGame = true
    self.curRound = pb.curRound
    self.leftCards = pb.leftCards
    self.totalRound = pb.totalRound
    self.bankerPos = pb.bankerPos
    self.hunCard = pb.hunCard
    self.preHunCard = pb.preHunCard
    self.myCards = {}

    for i=1,#pb.cardsCount do
        self.cardsNumMap[i] = pb.cardsCount[i]
    end

    for i=1,#pb.myCards do
        local num = pb.myCards[i]
        local cardData = LCMJCardData.new(num)
        table.insert(self.myCards,cardData)
    end

    local scoreList = pb.scoreList
    if not scoreList or #scoreList < 1 then return end
    for pos,player in pairs(self.playerMap) do
        for i=1,#scoreList do
            if pos == i then
                player:setScore(scoreList[i])
            end
        end
    end
end

function LCMJDeskStatusMgr:updateWithEnterRoom( data )

end

return LCMJDeskStatusMgr
