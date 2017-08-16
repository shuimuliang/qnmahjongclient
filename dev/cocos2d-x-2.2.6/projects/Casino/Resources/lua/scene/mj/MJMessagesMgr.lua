local LCMatchMessagesMgr = require "lua/scene/managers/MatchMessagesMgr"

local LCMJMessagesMgr = class("LCMJMessagesMgr", LCMatchMessagesMgr)

function LCMJMessagesMgr:ctor(scene,deskStatusMgr)
    LCMJMessagesMgr.super.ctor(self,scene,deskStatusMgr)
    CC_SYNTHESIZE(self,     "scene",             scene)
    CC_SYNTHESIZE(self,     "deskStatusMgr",     deskStatusMgr)
end

--scene执行onEnter时候调用
function LCMJMessagesMgr:setDeskView(deskView)
    self.deskView = deskView
end

function LCMJMessagesMgr:handlePacker(packer, callback)
    LCMJMessagesMgr.super.handlePacker(self, packer, callback)
    packer.callback = callback
    local msgs = {
        [eDefMsg.OtherJoin]       = self.onMsgOtherJoin,
        [eDefMsg.VoteClose]       = self.onMsgVoteClose,
        [eDefMsg.CloseRoom]       = self.onMsgCloseRoom,
        [eDefMsg.PrepareGame]     = self.onMsgPrepareGame,
        [eDefMsg.CancelPrepare]   = self.onMsgCancelPrepare,
        [eDefMsg.StartRound]      = self.onMsgStartRound,
        [eDefMsg.EndRound]        = self.onMsgEndRound,
        [eDefMsg.DrawCard]        = self.onMsgDrawCard,
        [eDefMsg.Discard]         = self.onMsgDiscard,
        [eDefMsg.Operation]       = self.onMsgOperation,
        [eDefMsg.DiscardNotice]   = self.onMsgDiscardNotice,
        [eDefMsg.OperationNotice] = self.onMsgOperationNotice,
        [eDefMsg.ExitRoom]        = self.onMsgExitRoom,
        [eDefMsg.Settlement]      = self.onMsgSettlement,
        [eDefMsg.GameChat]        = self.onMsgGameChat,        
        [eDefMsg.BuyPaoList]      = self.onMsgBuyPaoList,        
        [eDefMsg.BuyPao]          = self.onMsgBuyPao,
    }
    local func = msgs[ packer.seed ]
    if not func then return end
    func(self, packer)
end

function LCMJMessagesMgr:showErrorMsg(msg)
    self.scene:showConfirmMessage(_ID2WORD(msg.status))
end

function LCMJMessagesMgr:onMsgBuyPaoList( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end
    self.deskStatusMgr:onBuyPaoList()
    self.deskView:deskBuyPaoList()
end

function LCMJMessagesMgr:onMsgBuyPao( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then
        self.scene:hideLoading()
        self:showErrorMsg(msg) 
        return 
    end

    self.deskView:deskBuyPao(msg)
end

function LCMJMessagesMgr:onMsgOtherJoin( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:addPlayerByPB(msg.player)
    self.deskView:deskOtherJoin(self.deskStatusMgr:getPlayerByPos(msg.player.pos))
end

function LCMJMessagesMgr:onMsgVoteClose( packer )
    self.scene:hideLoading()
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskVoteClose(msg)
end

function LCMJMessagesMgr:onMsgCloseRoom( packer )
    self.scene:hideLoading()
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskCloseRoom(msg.closeType)
end

function LCMJMessagesMgr:onMsgPrepareGame( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskPrepareGame(msg.pos,true)
end

function LCMJMessagesMgr:onMsgStartRound( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:roundStart(msg)
    self.deskView:deskStartRound()
end

function LCMJMessagesMgr:onMsgEndRound( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:roundEnd(msg.itemList)
    
    local isLiuJu = self.deskStatusMgr:getIsLiuJu()
    if isLiuJu then
         self.deskView:deskShowLiuJu()
    end
    
    local isRelinkEnd = self.deskStatusMgr:getIsRelinkEnd()
    if isRelinkEnd then
        self.deskView:deskEndRound(msg.itemList)
        self.deskStatusMgr:setIsRelinkEnd(false)
    else
        scheduleOnce( LCSceneTransit:getRunningScene(),function( ... )
            self.deskView:deskEndRound(msg.itemList)
        end, 3.8)
    end
end

function LCMJMessagesMgr:onMsgDrawCard( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:updateLeftCardsNum()
    self.deskView:deskDrawCard(msg.pos, msg.card)
end

function LCMJMessagesMgr:onMsgOperationNotice( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskOperationNotice(msg.operationList)
end

function LCMJMessagesMgr:onMsgDiscardNotice( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskDiscardNotice(msg.pos)
end


function LCMJMessagesMgr:onMsgDiscard( packer )
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskDescardRecv(msg.pos, msg.card, msg.leftCards)
end

function LCMJMessagesMgr:onMsgOperation(packer)
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskOperation(msg.operation)
end

function LCMJMessagesMgr:onMsgExitRoom(packer)
    self.scene:hideLoading()    
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:exitRoom(msg.pos)
    self.deskView:deskExitRoom(msg.pos)
end

function LCMJMessagesMgr:onMsgSettlement(packer)
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskStatusMgr:settlement(msg) 
    local isVoteClose = msg.isVoteClose
    if isVoteClose == 1 then
        self.deskView:deskSettlementForVote()
    end
end

function LCMJMessagesMgr:onMsgGameChat(packer)
    local msg = packer.msg
    if msg.status ~= eErrorCode.succ then self:showErrorMsg(msg) return end

    self.deskView:deskGameChat(msg)  
end

return LCMJMessagesMgr
