---============================ LCSystemConfig ===========================
local LCSystemConfig = {}
LCSystemConfig.logIndex = 1


---============================ LCGameSettings ===========================
local LCGameSettings = {}
LCGameSettings.bgMusicOnOff = true
LCGameSettings.soundEffectOnOff = true
LCGameSettings.bgMusicVolume = .5
LCGameSettings.soundEffectVolume = .5
LCGameSettings.vibrationOnOff = 1

---============================ Payment Order ===========================
local LCOrderRecorder = {}
--[[
	-- recorder
		-- [player uuid:string]
			-- [orderid:string]
				-- [
					orderID:string,
					status:int,
					jsonStr:string,
					]
]]

---============================ LCIpAndPort ===========================
local LCIpAndPort = {ip = DEF_LOGIN_SERVER_IP, idx = 1}

---============================ heibei majiang ===========================
local heibeiConfig = {[-1] = true, [1] = true, [3] = true, [5] = true, [6] = true, [7] = true, [12] = true}

local mjConfigDefault = {
	[1] = {round = 8, [1] = true, [3] = true, [5] = true, [6] = true, [7] = true, [12] = true},
	[2] = {round = 4, [1] = true, [2] = true, [3] = true},
	[3] = {round = 8, [1] = true, },
	[4] = {round = 8, [1] = true, },
	selectedType = 1,
	cheat = false,
}

local default_login = {playerID = 0, refreshToken = ""}

---============================ LCUserContext ===========================

LCUserContext = {playerTag = nil,}

local LCUserContextKeys = {}
LCUserContextKeys.systemConfig = "LCUserContextKeys.systemConfig"
LCUserContextKeys.gameSettings = "LCUserContextKeys.gameSettings"
LCUserContextKeys.orderRecorder = "LCUserContextKeys.orderRecorder"
LCUserContextKeys.ipAndPort = "LCUserContextKeys.ipAndPort"
LCUserContextKeys.heibeiConfig = "LCUserContextKeys.HeibeiConfig"
LCUserContextKeys.login = "LCUserContextKeys.login"

function LCUserContext:start()
	self.gameSettings = nil
	self.systemConfig = self:getSystemConfig()
end

function LCUserContext:setPlayerTag(playerTag)
	self.playerTag = playerTag
end
-- logic related
function LCUserContext:getSystemConfig()
	if self.systemConfig then return self.systemConfig end
	local key = LCUserContextKeys.systemConfig
	self.systemConfig = self:_load(key,LCSystemConfig)
	return self.systemConfig

end

function LCUserContext:saveSystemConfig(systemConfig)
	self.systemConfig = systemConfig or self.systemConfig
	local key = LCUserContextKeys.systemConfig
	self:_save(key,self.systemConfig)
end

function LCUserContext:saveLogin(config)
	if type(config) ~= "table" then return end
	self:_save(LCUserContextKeys.login, config)
end

function LCUserContext:getLogin()
	return self:_load(LCUserContextKeys.login, default_login)
end

-- ip port
function LCUserContext:saveIpAndPort(ipAndPort)
	self.ipAndPort = ipAndPort or self.ipAndPort
	self:_save(LCUserContextKeys.ipAndPort, self.ipAndPort)
end

function LCUserContext:getIpAndPort()
	self.ipAndPort = self:_load(LCUserContextKeys.ipAndPort, LCIpAndPort)
	return self.ipAndPort or {}
end

-- heibei majiang
function LCUserContext:saveHeibeiConfig(config)
	self.heibeiConfig = config or self.heibeiConfig
	self:_save(LCUserContextKeys.heibeiConfig, self.heibeiConfig)
end

function LCUserContext:getHeibeiConfig()
	self.heibeiConfig = self:_load(LCUserContextKeys.heibeiConfig, heibeiConfig)
	return self.heibeiConfig or {}
end

-- mj config
function LCUserContext:saveMJConfig(config)
	self:_save("mjConfig", config)
end

function LCUserContext:getMJConfig()
	local c = self:_load("mjConfig", mjConfigDefault)
	for k, v in pairs(mjConfigDefault) do -- 新添加玩法的默认配置时，要同步到已存在的本地配置中
		if not c[k] then
			c[k] = v
		end
	end
	return c
end

-- game settings
function LCUserContext:getGameSettings()
	-- if not self.playerTag then GError("player tag is nil") return end
	if self.gameSettings == nil then
		local key = LCUserContextKeys.gameSettings
		self.gameSettings = self:_load(key,LCGameSettings)
	end
 	return self.gameSettings
end

function LCUserContext:saveGameSettings(gameSettings)
	-- if not self.playerTag then GError("player tag is nil") return end
	self.gameSettings = gameSettings or self.gameSettings
	local key = LCUserContextKeys.gameSettings
	self:_save(key,self.gameSettings)
end

-- public
function LCUserContext:getObjectForKey(key,defaultValue)
	if not key then GError("key should not nil") return false end
	local object = self:_load(key,defaultValue)
 	return object or defaultValue
end

function LCUserContext:setObjectForKey(key,object)
	if not key then GError("key should not nil") return false end
	if not object then GError("object for save is nil") return false end
	self:_save(key,object)
	return true
end



-- private method
function LCUserContext:_save(key,value)
	if not key or not value then GError("invalid arguments : %s",key) return end
	local str = serialize(value)
	CCUserDefault:sharedUserDefault():setStringForKey(key, str)
	return str
end

function LCUserContext:_load(key,default)
	if not key then GError("key is nil") return end
	local str = CCUserDefault:sharedUserDefault():getStringForKey(key)
	if not str or string.len(str) == 0 then str = self:_save(key,default) end
	return unserialize(str)
end

-- order recorder
function LCUserContext:getOrderRecorder()
	if self.orderRecorder == nil then
		local key = LCUserContextKeys.orderRecorder
		self.orderRecorder = self:_load(key,LCOrderRecorder)
	end
 	return self.orderRecorder
end

function LCUserContext:saveOrderRecorder(orderRecorder)
	self.orderRecorder = orderRecorder or self.orderRecorder
	local key = LCUserContextKeys.orderRecorder
	self:_save(key,self.orderRecorder)
end
