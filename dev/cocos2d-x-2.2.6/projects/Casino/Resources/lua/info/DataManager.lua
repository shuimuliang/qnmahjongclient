-- ============= DB MAP function ============= --
local CUR_DEVICE_RES_LEVEL = DEBUG_RESOURCE_LEVEL

function _GET_DEVICE_RES_LEVEL()
	if CUR_DEVICE_RES_LEVEL then return CUR_DEVICE_RES_LEVEL end
	local device = CCEPlugin:getDeviceModel()
	local info = DB_DEVICE[string.lower(device)]
	if not info then CUR_DEVICE_RES_LEVEL = 1
	else CUR_DEVICE_RES_LEVEL = info.level or 1 end
	return CUR_DEVICE_RES_LEVEL
end

local function _GET_RES_MAPPED_ID(id)
	if not id then return nil end
	local level = _GET_DEVICE_RES_LEVEL()
	if level == 1 then return id end
	local info =  DB_RES_MAPPED[id]
	if not info then return id end
	return info["resource"..level] or id
end

local function _ADD_ROOT_PATH(info)
	-- assemble root dir
	if info.type < 100 then
		-- art
		return DEF_ART_DIRECTORY_ROOT .. info.path .. info.resource

	elseif info.type > 100 and info.type < 200 then
		-- music
		return DEF_MUSIC_DIRECTORY_ROOT .. info.path .. info.resource
	end
	return info.path .. info.resource
end

local function _ID2RES(id, ignoreMapped)
	-- check
	if not id then return end
	if type(id) ~= "number" then GError("invalid id type %s",type(id)) return end

	-- get mapped id
	local mID = ignoreMapped and id or _GET_RES_MAPPED_ID(id)
	local info = DB_RESOURCE[mID]

	-- get resource info
	if not info then
		GError("can not find resource info %d", mID) return nil
	end
	if not info.path or not info.resource then return end

	-- assemble root dir
	return _ADD_ROOT_PATH(info)
end

------------------------------- LCDataInfoManager -----------------------------------
LCDataInfoManager = class("LCDataInfoManager")

function LCDataInfoManager:ctor()
	self._infolist = {}
end
function LCDataInfoManager:getInfoList() return self._infolist end
function LCDataInfoManager:saveData(info) end
function LCDataInfoManager:insertData(key,data)
	if not key or self._infolist[key] then return end
	self._infolist[key] = data
end
function LCDataInfoManager:getInfo(key)
	if not key then return nil end
	return self._infolist[key]
end

---------------------------------- LCDataInfo -----------------------------------
LCDataInfo = class("LCDataInfo")
function LCDataInfo:loadData(row) end
function LCDataInfo.tostring(row,key,def)
	if not row or not key then return def end
	local lowKey = string.lower(key)
	local val = row[lowKey]
	if not val then return def end
	if string.len(val) == 0 then return def end
	return val or def
end

function LCDataInfo.toresource(row,key)
	if not row or not key then return nil end
	local lowKey = string.lower(key)
	local id = row[lowKey]
	if not id then return nil end
	return _ID2RES(id)
end

function LCDataInfo.tonumber(row,key,def)
	if not row or not key then return def end
	local lowKey = string.lower(key)
	local val = row[lowKey]
	if val then return tonumber(val) end
	return def
end

function LCDataInfo.toboolean(row,key,def)
	if not row or not key then return def end
	local lowKey = string.lower(key)
	local val = row[lowKey]
	if val then return val == 1 end
	return def
end

function LCDataInfo:print()
	CCLuaLog("=============== data info ===============")
	for k,v in pairs(self) do
		if k ~= __index then
			print(tostring(k) .. " : " .. tostring(v))
		end
	end
	CCLuaLog("=========================================")
end

---------------------------------------- LCDataManager --------------------------------------
LCDataManager = {_dataMap = {}, debugOnOff = false, _mode = "lua"}

function LCDataManager:register(name,cmgr,cinfo,dbname,dbtable)
	if not name or not cmgr or not cinfo or not dbname or not dbtable then
		GError("parameters check error")
		if dbtable then print("table %s error", dbtable) end
		return
	end
	if self._dataMap[name] then
		GError("model " .. name .. " exsist")
		return
	end
	local infoMgr = cmgr.new()
	local data = {_mgr = infoMgr, _cinfo = cinfo, _dbname = dbname, _dbtable = dbtable, _loaded = false}
	self._dataMap[name] = data
	self["get" .. name] = function()
		if not self._dataMap[name]._loaded then
			if self:loadDB(name) then
				self._dataMap[name]._loaded = true
			else
				-- return mgr without data
				GDebug("load db error. ".. name)
			end
		end
		return self._dataMap[name]._mgr
	end
end

function LCDataManager:start()
	self:register("GiftInfoMgr", LCGiftInfoMgr, LCGiftInfo, "zero.db", "bgift")
	self:register("ShopGemInfoMgr",LCShopGemInfoMgr,LCShopGemInfo,"zero.db","bgems")
	self:register("ShopGoodsInfoMgr",LCShopGoodsInfoMgr,LCShopGoodsInfo,"zero.db","bgoods")
	self:register("ShopTreasureInfoMgr",LCShopTreasureInfoMgr,LCShopTreasureInfo,"zero.db","btreasure")
	self:register("TalkingGameInfoMgr",LCTalkingGameInfoMgr,LCTalkingGameInfo,"zero.db","btalking")
end

function LCDataManager:loadDB(name)
	local data = self._dataMap[name]
	if not data then GError("no DB "..name) return false end
	local tbl = require(DEF_PATH.db .. data._dbtable)
	for i, t in ipairs(tbl) do
		local info = data._cinfo.new()
		info:loadData(t)
		if not data._mgr:saveData(info) then
			GError("info load failed " .. data._dbtable .. " row : " .. i)
			return false
		end
	end
	return true
end
