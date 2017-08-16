--[[
APP启动入口脚本。
--]]

-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

require "lua/Config"
require "lua/app/AppConfigMgr"

function __G__TRACKBACK__(errorMessage)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(errorMessage) .. "\n")
    print(debug.traceback("", 2))
    print("----------------------------------------")

    if DEBUG_ERROR_MESSAGE then
    	local scene = LCSceneTransit:getRunningScene()
    	if scene then
    		LCMessageBox:showSelect(scene,{text = errorMessage})
    	end
    end

    -- if not DEBUG_ON_OFF then
    	-- report player id
    	local player = LCAppGlobal:getLoginPlayer()
    	if player then
    		local playerID = player.id
    		if playerID and buglySetUserId then buglySetUserId(playerID) end

            local version = DEF_INTERNAL_VERSION
            if version and buglyAddUserValue then buglyAddUserValue("version",version) end
    	end

    	-- report lua error msg
    	local message = errorMessage
    	if buglyReportLuaException then buglyReportLuaException(message, debug.traceback()) end
	-- end
end

LCAppGlobal.start()
