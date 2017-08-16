local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local VersionCheck = class("VersionCheck")

-- events
VersionCheck.EVENT_FORCE_UPDATE   = "EVENT_FORCE_UPDATE"
VersionCheck.EVENT_ERROR          = "EVENT_ERROR"
VersionCheck.EVENT_PASS           = "EVENT_PASS"
VersionCheck.EVENT_HOTFIX_BEGIN   = "EVENT_HOTFIX_BEGIN"
VersionCheck.EVENT_HOTFIX_PERCENT = "EVENT_HOTFIX_PERCENT"
VersionCheck.EVENT_MODULE         = "EVENT_MODULE"
VersionCheck.EVENT_OPTIONAL_UPDATE = "EVENT_OPTIONAL_UPDATE"

local _handleNewVersion
local HOTFIX, OPTIONAL_UPDATE, FORCE_UPDATE = 1, 2, 3

function VersionCheck:ctor()
    self._channel  = CSDKManager:getInstance():getChannelID()
    self._version  = DEF_INTERNAL_VERSION
    self._ip       = DEF_VER_CHECK_IP
    self._list     = {}
    self._deviceID = LCAppGlobal:getDeviceID()

    EventProtocol.extend(self)
    DynamicDownloadExt:getInstance():setScriptEvent(handler(self, self.onStart), handler(self, self.onProcess), handler(self, self.onComplete), handler(self, self.onError))
end

function VersionCheck:request()
    self:dispatchEvent{name = VersionCheck.EVENT_PASS}

  --   local url = string.format("%s?channel=%s&version=%s&deviceID=%s", self._ip, self._channel, self._version, self._deviceID)
  --   GDebug("version check url = " .. url)
  --   local req = CCHttpRequest:open(url, kHttpPost)
  --   req:sendWithHandler(function(res)
  --       local code = res:getResponseCode()
		-- if code ~= eHttpResponseCode.ok or not res:getResponseData() or res:getResponseData() == "" then
  --           self:dispatchEvent{name = VersionCheck.EVENT_ERROR, code = code}
		-- else
		-- 	local resData = cjson.decode(res:getResponseData())
  --           print_lua_table(resData)

  --           if table.isEmpty(resData.versionList) then
  --               self:dispatchEvent{name = VersionCheck.EVENT_PASS}
  --           else
  --               self._list = resData.versionList
  --               _handleNewVersion(self)
  --           end

  --           if resData.module and resData.module ~= "" then
  --               self:dispatchEvent{name = VersionCheck.EVENT_MODULE, module = resData.module}
  --           end
		-- end
  --   end)
end

function VersionCheck:onStart(msg)
    GDebug("start download")
end

function VersionCheck:onComplete(msg)
    table.remove(self._list, 1)
    if table.isEmpty(self._list) then
        CCDirector:sharedDirector():reStart()
        return
    end
    _handleNewVersion(self)
end

function VersionCheck:onProcess(num, msg)
    local size = self._list and self._list[1] and self._list[1].fileSize or 1024
    self:dispatchEvent{name = VersionCheck.EVENT_HOTFIX_PERCENT, percent = num, fileSize = size}
end

function VersionCheck:onError(msg1, msg2)
    CCDirector:sharedDirector():reStart()
end

function VersionCheck:optionalUpdate(url)
    LCMessageBox:showSelect(LCSceneTransit.getRunningScene(), {
        text = "新版本发布，是否前往应用商店下载？",
        okCallback = function()
            CCEPlugin:openURL(url)
            self:optionalUpdate(url)
        end,
        cancelCallback = function()
            if #self._list > 1 then
                self:onComplete()
            end
        end,
    })
end

-- private
_handleNewVersion = function(self)
    local info        = self._list[1]
    local updateType  = info.updateType
    local version     = info.numVersion
    local url         = info.url
    local fileSize    = info.fileSize
    if updateType == FORCE_UPDATE then
        print_lua_table(info)
        self:dispatchEvent{name = VersionCheck.EVENT_FORCE_UPDATE, url = url}
        return
    elseif updateType == HOTFIX then
        if not url or url == "" then
            self:dispatchEvent{name = VersionCheck.EVENT_ERROR, code = ""}
            return
        end
        DynamicDownloadExt:getInstance():startDownload(url, true)
        self:dispatchEvent{name = VersionCheck.EVENT_HOTFIX_BEGIN, fileSize = fileSize}
    elseif updateType == OPTIONAL_UPDATE then
        self:optionalUpdate(url)
        self:dispatchEvent{name = VersionCheck.EVENT_OPTIONAL_UPDATE}
    else
        self:dispatchEvent{name = VersionCheck.EVENT_ERROR, code = ""}
    end
end

return VersionCheck
