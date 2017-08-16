local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local LCPlayerAvatar = require "lua/scene/commons/PlayerAvatar"
local EventProtocol = require("lua/scene/sceneUtils/EventProtocol")

local VoteDialog = class("VoteDialog", DialogDIY)

local OPTION_AGREE = 1
local OPTION_DISAGREE = 2

local VOTE_STATE_QUIT = 1
local VOTE_STATE_CONTINUE = 2
local VOTE_STATE_WAIT = 3

local _hideBtns

VoteDialog.EVENT_QUIT = "EVENT_QUIT"

function VoteDialog:ctor(key)
    VoteDialog.super.ctor(self, key)
    self._players = key.players
    self._countdown = key.countdown
    self._countdownTxt = "(%s)"
    self._voteIndex = 1
    EventProtocol.extend(self)
end

function VoteDialog:initContentView()
    if #self._players == 1 then self:setVisible(false) end
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("voteDialog", proxy, self)
    self:addChild(layer)

    local playersCount = #self._players
    local blueBoxWidth = self.ccb_blueBox1:getContentSize().width
    local blueBoxY = self.ccb_blueBox1:getPositionY()
    local avatarY = self.ccb_avatarNode1:getPositionY()
    local nameY = self.ccb_name1:getPositionY()
    local gap = 6

    local optionBgSize = self.ccb_optionBg:getContentSize()
    local optionBgWidth = playersCount * (blueBoxWidth + gap) + gap
    self.ccb_optionBg:setContentSize(CCSize(optionBgWidth, optionBgSize.height))
    local blueBoxesTotalWidth = playersCount * (blueBoxWidth + gap) - gap
    local firstBlueBoxX = self.ccb_optionBg:getPositionX() - blueBoxesTotalWidth / 2 + blueBoxWidth / 2
    for i = 1, 5 do
        local player = self._players[i]
        if player then
            local x = firstBlueBoxX + (i - 1) * (blueBoxWidth + gap)
            self["ccb_blueBox"..i]:setPositionX(x)
            self["ccb_avatarNode"..i]:setPositionX(x)
            self["ccb_name"..i]:setPositionX(x)
            self["ccb_blueBox"..i]:setVisible(player.option == OPTION_AGREE)
            if player.option == OPTION_AGREE then
                self["ccb_name"..i]:setString(player.nickName)
                self["ccb_name"..i]:setVisible(true)
                self["ccb_avatarNode"..i]:removeAllChildrenWithCleanup(true)
                self["ccb_avatarNode"..i]:addChild(LCPlayerAvatar:create(player.avatar, nil, {playerID = player.playerID, isShowBG = true}))
                self._voteDialog = self._voteIndex + 1
            elseif player.option == OPTION_DISAGREE then
                self["ccb_name"..i]:setString(player.nickName)
                self["ccb_name"..i]:setVisible(true)
                self["ccb_avatarNode"..i]:removeAllChildrenWithCleanup(true)
                self["ccb_avatarNode"..i]:addChild(LCPlayerAvatar:create(player.avatar, nil, {playerID = player.playerID, isShowBG = true}))
                self._voteDialog = self._voteIndex + 1
            else
                -- do nothing
            end
        else
            self["ccb_blueBox"..i]:setVisible(false)
            self["ccb_avatarNode"..i]:setVisible(false)
        end
    end

    schedule(self.ccb_countdownTTF, function()
        self._countdown = self._countdown - 1
        if self._countdown <= 0 then self._countdown = 0 end
        self.ccb_countdownTTF:setString(string.format(self._countdownTxt, self._countdown))
        if self._countdown == 0 then self.ccb_countdownTTF:stopAllActions() end
    end, 1)

    return layer
end

function VoteDialog:reset(voters, countdown)
    self.ccb_countdownTTF:stopAllActions()
    self._countdown = countdown
    schedule(self.ccb_countdownTTF, function()
        self._countdown = self._countdown - 1
        if self._countdown <= 0 then self._countdown = 0 end
        self.ccb_countdownTTF:setString(string.format(self._countdownTxt, self._countdown))
        if self._countdown == 0 then self.ccb_countdownTTF:stopAllActions() end
    end, 1)
    self._players = voters
    for i = 1, 5 do
        local player = self._players[i]
        if player then
            local x = firstBlueBoxX + (i - 1) * (blueBoxWidth + gap)
            self["ccb_blueBox"..i]:setPositionX(x)
            self["ccb_avatarNode"..i]:setPositionX(x)
            self["ccb_name"..i]:setPositionX(x)
            self["ccb_blueBox"..i]:setVisible(player.option == OPTION_AGREE)
            if player.option == OPTION_AGREE then
                self["ccb_name"..i]:setString(player.nickName)
                self["ccb_name"..i]:setVisible(true)
                self["ccb_avatarNode"..i]:removeAllChildrenWithCleanup(true)
                self["ccb_avatarNode"..i]:addChild(LCPlayerAvatar:create(player.avatar, nil, {playerID = player.playerID, isShowBG = true}))
                self._voteDialog = self._voteIndex + 1
            elseif player.option == OPTION_DISAGREE then
                self["ccb_name"..i]:setString(player.nickName)
                self["ccb_name"..i]:setVisible(true)
                self["ccb_avatarNode"..i]:removeAllChildrenWithCleanup(true)
                self["ccb_avatarNode"..i]:addChild(LCPlayerAvatar:create(player.avatar, nil, {playerID = player.playerID, isShowBG = true}))
                self._voteDialog = self._voteIndex + 1
            else
                -- do nothing
            end
        else
            self["ccb_blueBox"..i]:setVisible(false)
            self["ccb_avatarNode"..i]:setVisible(false)
        end
    end
end

function VoteDialog:dealPacket(seed, msg)
    if seed == eDefMsg.CloseRoomVote then
        local status = msg.status
        if status == eErrorCode.succ then
            local option = msg.option
            local playerID = msg.playerID
            local voteState = msg.voteState
            if playerID == LCAppGlobal:getLoginPlayer():getPlayerID() then
                _hideBtns(self)
            end
            for _, p in ipairs(self._players) do
                if p.playerID == playerID then
                    if 1 == self._voteIndex then
                        self.ccb_title:setString(p.nickName .. " 发起解散牌桌")
                    elseif self._voteIndex > #self._players then
                        break
                    end
                    self["ccb_blueBox"..self._voteIndex]:setVisible(option == OPTION_AGREE)
                    self["ccb_name"..self._voteIndex]:setString(p.nickName)
                    self["ccb_name"..self._voteIndex]:setVisible(true)
                    self["ccb_avatarNode"..self._voteIndex]:addChild(LCPlayerAvatar:create(p.avatar, nil, {playerID = playerID, isShowBG = true}))
                    self._voteIndex = self._voteIndex + 1
                    break
                end
            end
            if voteState == VOTE_STATE_QUIT then
                LCToastHUD.new("投票同意退出房间"):showInScene()
                performWithDelay(self, function()
                    self:dispatchEvent{name = VoteDialog.EVENT_QUIT}
                    self:dismissSelf()
                end, 2)
            elseif voteState == VOTE_STATE_CONTINUE then
                LCToastHUD.new("投票不同意退出房间,游戏继续"):showInScene()
                performWithDelay(self, function()
                    self:dismissSelf()
                end, 2)
            elseif voteState == VOTE_STATE_WAIT then
                -- do nothing
            end
        end
    end
end

function VoteDialog:ccb_onAgree(control, controlEvent)
    -- self:dealPacket(eDefMsg.CloseRoomVote, {status = eErrorCode.succ, playerID = LCAppGlobal:getLoginPlayer():getPlayerID()})
    local pb = LCNetManager:createMessage(eDefMsg.CloseRoomVote)
    pb._msg.option = OPTION_AGREE
    LCNetManager:send(pb)
end

function VoteDialog:ccb_onDisagree(control, controlEvent)
    local pb = LCNetManager:createMessage(eDefMsg.CloseRoomVote)
    pb._msg.option = OPTION_DISAGREE
    LCNetManager:send(pb)
end

function VoteDialog.test(scene)
    local dialog = VoteDialog.new({
        players = {
            {avatar = 1, playerID = LCAppGlobal:getLoginPlayer():getPlayerID(), nickName = "asdf"},
            {avatar = 2, playerID = 2, nickName = "asssdf"},
            {avatar = 3, playerID = 3, nickName = "asdsdfsf"},
        },
        countdown = 60
    }):showInScene()
    -- scene:addChild(dialog)
end

-- private

_hideBtns = function(self)
    self.ccb_agreeBtn:setVisible(false)
    self.ccb_disagreeBtn:setVisible(false)
    self._countdownTxt = "您已投票，请等待其他玩家投票(%s)"
    self.ccb_countdownTTF:setString(string.format(self._countdownTxt, self._countdown))
end

return VoteDialog
