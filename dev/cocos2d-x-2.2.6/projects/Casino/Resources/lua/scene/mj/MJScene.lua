local LCMJDeskView        = require("lua/scene/mj/MJDeskView")
local LCMJMessagesMgr      = require("lua/scene/mj/MJMessagesMgr")
local LCMJDeskStatusMgr    = require("lua/scene/mj/MJDeskStatusMgr")
local LCGameSceneBase      = require("lua/scene/managers/GameSceneBase")

local LCMJScene = class("LCMJScene", LCGameSceneBase)

function LCMJScene:ctor( key )
	if not key or not key.enterRoomRecv then return false end

    key.deskStatusMgr 	= LCMJDeskStatusMgr.new(self,key.enterRoomRecv)
    key.messagesMgr     = LCMJMessagesMgr.new(self,key.deskStatusMgr)
    if not LCMJScene.super.ctor(self,key) then return end

    CC_SYNTHESIZE(self,"deskStatusMgr",key.deskStatusMgr)
    CC_SYNTHESIZE(self,"messagesMgr",  key.messagesMgr)

    if CIMManager then
        CIMManager:getInstance():enterRoom()
    end
    return true
end

function LCMJScene:onEnter()
    LCMJScene.super.onEnter( self )
    self.deskView = LCMJDeskView.new(self.deskStatusMgr,self)
    self:addChild( self.deskView )
    self.messagesMgr:setDeskView(self.deskView)
end

function LCMJScene:onEnterTransitionDidFinish()
    local musicName = "music/backmusic/mj_bg_music.mp3"
    Audio.playMusic(musicName, true)
end

function LCMJScene:getDiscardPanel()
	return self.deskView.discardPanel
end

function LCMJScene:getDeskView()
	return self.deskView
end

function LCMJScene:refreshScene(withMessage)
    if withMessage then
        local str = "数据有误，请刷新牌桌。"
        LCMessageBox:showConfirm(self,{text = str,
                buttonText = "刷新",
                hideClose = true,
                callback = function( ... )
                local roomID = self.deskStatusMgr:getRoomID()
                self:sendEnterRoom(roomID,true)
        end})
    else
        local roomID = self.deskStatusMgr:getRoomID()
        self:sendEnterRoom(roomID,true)
    end
end

-- 游戏从后台切回来的时候
function LCMJScene:onAppBackEvent()
end

return LCMJScene
