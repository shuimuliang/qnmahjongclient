local DialogFullScreen  = require "lua/scene/commons/dialog/DialogDIY"

local SettingDialog = class("SettingDialog", DialogFullScreen)

local _initSlider

function SettingDialog:ctor(key)
    SettingDialog.super.ctor(self, key)
end

function SettingDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad(eResCCBI.SettingView, proxy, self)
	self:addChild(layer)

    local musicSlider = _initSlider(self.ccb_musicBg, self.ccb_musicBar, self.ccb_musicBtn)
    musicSlider:setValue(Audio.getMusicVolume())
    musicSlider:addHandleOfControlEvent(function(control, controlEvent) Audio.setMusicVolume(musicSlider:getValue()) end, CCControlEventValueChanged)

    local soundSlider = _initSlider(self.ccb_soundBg, self.ccb_soundBar, self.ccb_soundBtn)
    soundSlider:setValue(Audio.getSoundVolume())
    soundSlider:addHandleOfControlEvent(function(control, controlEvent) Audio.setSoundVolume(soundSlider:getValue()) end, CCControlEventValueChanged)
    
    return layer
end

function SettingDialog:ccb_onLogout(control, controlEvent)
    LCNetManager:send(LCNetManager:createMessage(eDefMsg.Logout))
    LCAppGlobal:playerLogout()
    LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
end

function SettingDialog:ccb_onExit(control, controlEvent)
    CCDirector:sharedDirector():endToLua()
    os.exit()
end

function SettingDialog:ccb_onClose(control, controlEvent)
    self:dismissSelf()
end

function SettingDialog:hideTwoBtns()
    self.ccb_logoutNode:setVisible(false)
    self.ccb_quitNode:setVisible(false)
end

-- private
_initSlider = function(bg, bar, btn)
    local parent = bg:getParent()
    local x, y = bg:getPosition()
    bg:retain(); bg:removeFromParentAndCleanup(true)
    bar:retain(); bar:removeFromParentAndCleanup(true)
    btn:retain(); btn:removeFromParentAndCleanup(true)
    local slider = CCControlSlider:create(bg, bar, btn)
    slider:setMinimumValue(0)
    slider:setMaximumValue(1)
    bg:release()
    bar:release()
    btn:release()
    slider:setPosition(x, y)
    parent:addChild(slider)
    return slider
end

return SettingDialog
