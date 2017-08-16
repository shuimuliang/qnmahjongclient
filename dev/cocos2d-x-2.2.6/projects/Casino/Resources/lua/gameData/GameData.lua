local JushuConfig    = import(".JushuConfig")
local SystemSwitcher = import(".SystemSwitcher")
local BroadcastData  = import(".BroadcastData")

local LCDirtyTag = class("LCDirtyTag")

function LCDirtyTag:ctor(dataMgr, dirtyTime)
	CC_SYNTHESIZE(self,"dataMgr",dataMgr)
	CC_SYNTHESIZE(self,"dirtyTime",dirtyTime)
	CC_SYNTHESIZE(self,"refreshTime",0)
end

function LCDirtyTag:isDirty()
	if not self.dirtyTime then return false end
	local curTime = getTickCount()
	if self.refreshTime > 0 and (curTime - self.refreshTime) / 1000 < self.dirtyTime then return false end
	return true
end

function LCDirtyTag:refresh() self.refreshTime = getTickCount() end


LCGameData = class("LCGameData")

function LCGameData:ctor()
	self:registerDataMgr( "shopMgr",	LCShopManager.new()	,		60*1)
	self:registerDataMgr( "switchMgr", 	SystemSwitcher)
	self:registerDataMgr( "broadCastMgr", BroadcastData)
	self:registerDataMgr( "gameChatMgr",  LCGameChatData.new())
	self:registerDataMgr( "jushuConfig",  JushuConfig)
end

-- dirty time is second
function LCGameData:registerDataMgr(name,dataMgr,dirtyTime)
	CC_SYNTHESIZE(self, name, dataMgr )
	dataMgr._dirtyTag =  LCDirtyTag.new(dataMgr,dirtyTime)
	dataMgr._isDirty = function() return dataMgr._dirtyTag:isDirty() end
	dataMgr._clearDirty = function() return dataMgr._dirtyTag:refresh() end
end
