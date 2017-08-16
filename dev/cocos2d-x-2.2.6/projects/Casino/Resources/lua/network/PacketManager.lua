local LCPaymentMgr        = require("lua/model/Payment")

eDefMsg = {
    Login           = 100,
    Logout          = 200,
    Auth            = 300,
    Dirty           = 400,
    Error           = 500,
    Feedback        = 600,
    ResourceChange  = 700,
    Broadcast       = 800,

    InviteCode      = 1100,
    InviteList      = 1200,
    InviteAward     = 1300,
    GoodsList       = 1400,
    OrderApply      = 1500,
    OrderApplyNew   = 1600,

    EnterRoom       = 2100, -- 自己进入房间
    OtherJoin       = 2200, -- 其他人进入房间
    ExitRoom        = 2300, -- 退出房间
    CloseRoom       = 2400, -- 解散房间
    VoteClose       = 2500, -- 投票解散
    GameChat        = 2600, -- 游戏内聊天
    GameRecord      = 3600, -- 历史战绩

    PrepareGame     = 3100, -- 准备游戏
    CancelPrepare   = 3200, -- 取消准备
    StartRound      = 3300, -- 开始一局
    EndRound        = 3400, -- 结束一局(小结算)
    Settlement      = 3500, -- 全部结束(大结算)
    BuyPaoList      = 3700, -- 买跑列表
    BuyPao          = 3800, -- 买跑

    DrawCard        = 4100, -- 抓牌
    Discard         = 4200, -- 出牌
    Operation       = 4300, -- 操作
    DiscardNotice   = 4400, -- 出牌提示
    OperationNotice = 4500, -- 操作提示
}

function SEED_SEND(id) return id + 10000 end
function SEED_RECV(id) return id + 20000 end
function ID_SEED(id) return math.mod(id, 10000) end
function IS_RECV_ID(id) return (id > 20000) end
function IS_SEND_ID(id) return (id > 10000 and id < 20000) end

LCPacketManager = {}

function LCPacketManager:start()
	for name, id in pairs( eDefMsg ) do
		LCNetManager:registerMessage( id, name.."Send", name.."Recv", self["NCB_"..name])
	end
end

function LCPacketManager.NCB_OrderApply(seed, msg)
    local status = msg.status
    local coins  = msg.coins
    local cards  = msg.cards

    if status ~= eErrorCode.succ then return end
    local p = LCAppGlobal:getLoginPlayer()
    if coins > 0 then
        p:setCoins(coins)
    end
    if cards > 0 then
        p:setCards(cards)
    end

    local orderID   = msg.orderID
    local productID = msg.productID
    local price     = msg.price
    local priceType = "CNY"
    local count     = msg.count
    local payType   = msg.payType

    LCAnalysisMgr:logChargeReq(orderID,
                                   productID,
                                   price,
                                   priceType,
                                   count,
                                   payType)
    LCAnalysisMgr:logChargeSucc(orderID)
end

function LCPacketManager.NCB_Dirty(seed, msg)
    if msg.status ~= eErrorCode.succ then return end
	LCAppGlobal:getLoginPlayer():updateAll(msg.player)
    LCAppGlobal:setHighID(msg.highID)
    LCAppGlobal:setCSWChatID(msg.wchatID)
    -- LCAppGlobal:setCSWChatID("dibariiiii")
    LCAppGlobal:getGameData():getJushuConfig().update(msg.jushus)
    LCUserContext:saveLogin({playerID = msg.player.id, refreshToken = msg.refreshToken})
    LCAnalysisMgr:logLogin()
    if msg.module ~= "" then
        local module = cjson.decode(msg.module)
        local switchMgr = LCAppGlobal:getGameData():getSwitchMgr()
        switchMgr.update(module)
    end

    --用户登录之后bugly赋予参数
    if msg.player and msg.player.id then
        local playerID = msg.player.id
        if playerID and buglySetUserId then buglySetUserId(playerID) end
        local version = DEF_INTERNAL_VERSION
        if version and buglyAddUserValue then buglyAddUserValue("version",version) end
    end
end

function LCPacketManager.NCB_GoodsList(seed, msg)
    local shopMgr = LCAppGlobal:getGameData():getShopMgr()
    shopMgr:updateShopData(msg.goods)
end

function LCPacketManager.NCB_OrderApplyNew(seed, msg)
    local status = msg.status
    local coins  = msg.coins
    local cards  = msg.cards

    if status ~= eErrorCode.succ then return end
    local p = LCAppGlobal:getLoginPlayer()
    if coins > 0 then
        p:setCoins(coins)
    end
    if cards > 0 then
        p:setCards(cards)
    end

    LCPaymentMgr:serverCallback(msg)
end

function LCPacketManager.NCB_GameChat(seed, msg)
	GDebug("NCB_GameChat in data layer")
	LCAppGlobal:getGameData():getGameChatMgr():updateChatByPB(msg)
end

function LCPacketManager.NCB_ResourceChange(seed, msg)
    local status = msg.status
    local coins  = msg.coins
    local cards  = msg.cards
    if status == eErrorCode.succ then
        local p = LCAppGlobal:getLoginPlayer()
        if coins >= 0 then
            p:setCoins(coins)
        end
        if cards >= 0 then
            p:setCards(cards)
        end
    end
end

function LCPacketManager.NCB_Broadcast(seed, msg)
    local status = msg.status
    local text   = msg.text
    if status == eErrorCode.succ then
        LCAppGlobal:getGameData():getBroadCastMgr().update(text)
    end
end
