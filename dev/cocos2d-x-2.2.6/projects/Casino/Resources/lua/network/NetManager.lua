local protoPack = "mj"
-- ====================================== proto base ====================================== --

LCProtoBase = class("LCProtoBase")
-- LCProtoBase.__index = LCProtoBase

function LCProtoBase:ctor(pack, name, msg, action_type)
	self._package = pack
	self._name = name
	self._msg = msg
	self._action_type = action_type
end

function LCProtoBase:encode()
	local name = self._package .. "." .. self._name
	return protobuf.encode(name,self._msg)
end

function LCProtoBase:decode(msg,len)
	if not len then len = string.len(msg) end
	local name = self._package .. "." .. self._name
	self._msg = protobuf.decode(name,msg,len)
	return self._msg
end

function LCProtoBase:printTitle()
	print("======= " .. self._name .. " =======")
	return self
end
function LCProtoBase:printSection(str) print("============= " .. str .. " =============") end

function LCProtoBase:printItem(key, data)
	if not self._msg then return self end
	-- local val = data -- or self._msg[key]

	-- nil
	if data == nil then GDebug(key .. " : " .. "nil") return self end

	-- table
	if type(data) == "table" then
		local str = ""
		for i=1,table.getn(data) do
			local v = data[i]
			str = str .. v .. " "
		end
		GDebug("%s : %s",tostring(key),tostring(str))
		return self
	end

	-- common
	GDebug("%s : %s",tostring(key),tostring(data))
	return self
end

function LCProtoBase:printByType(varType, var, varName)
	if PB_TABLE[varType] then
		if not var then
			GDebug(varName .. " : nil")
		else
			self:printByDataName(var, varType)
		end
	else
		GError("error var %s, %s", varName, varType)
		self:printItem(varName, var)
	end
end

function LCProtoBase:printByDataName(data, name)
	GDebug("============== %s ============", name)
	local format = PB_TABLE[name]
	if not data then
		GDebug("data is nil")
		return
	end
	if not format then
		GDebug("no pb or no data format, %s", name)
		return
	end
	for _, t in ipairs(format) do
		local rro = t[1] -- r(equired)r(epeated)o(ptional)
		local varType = t[2]
		local varName = t[3]

		if varType == "bool"
		or varType == "string"
		or varType == "int32"
		or varType == "int64"
		or varType == "double"
		or varType == "float"
		or varType == "bytes" then
			self:printItem(varName, data[varName])
		else
			if rro == "required" or rro == "optional" then
				self:printByType(varType, data[varName], varName)
			elseif rro == "repeated" then
				if data[varName] then
					if #data[varName] > 0 then
						self:printSection(varName)
					end
					for _, d in ipairs(data[varName]) do
						self:printByType(varType, d, varName)
					end
				else
					GDebug("no varName %s, varType %s", varName, varType)
				end
			else
				GError("wz error rro"..rro)
			end
		end
	end
end

function LCProtoBase:print()
	self:printByDataName(self._msg, self._name)
end



-- ====================================== net manager ====================================== --
LCNetManager = {}

function LCNetManager:start()

	-- registered messages
	CC_SYNTHESIZE_RO(self,"message",{})

	-- socket info pool
	self._socket = nil

	-- start logic
	self:startLogic()

end

-- =================================> message functions
function LCNetManager:createMessage(id)
	if not id then return nil end
	local seed = ID_SEED(id)
	if not self.message[seed] then GError("id %d can not register",id) return end
	local msg = self.message[seed]
	if IS_RECV_ID(id) then
		return LCProtoBase.new(protoPack, msg.r_pb, {}, SEED_RECV(seed))
	else
		return LCProtoBase.new(protoPack, msg.s_pb, {}, seed)
	end
end

function LCNetManager:registerPB(filename)
	local path = CCFileUtils:sharedFileUtils():fullPathForFilename(filename)
	local buffer = CCFileUtils:sharedFileUtils():getFileData(path, "rb")
	protobuf.register(buffer)
end

function LCNetManager:registerMessage(seed, s_pb, r_pb, r_callBack)
	if not seed then
		GFatal("error register info %d", seed) return
	end

	self.message[seed] = {
		s_pb = s_pb,
		r_pb = r_pb,
		r_callBack = r_callBack,}
end

function LCNetManager:startLogic()
	-- register all pb files
	-- self:registerPB("pb/pub.pb")
	self:registerPB("pb/pf.pb")

	--register all message
	LCPacketManager:start()
end

function LCNetManager:openGameSocket(ip, port)
	self:closeGameSocket()

	local socket = LCSocketKeep.new(ip, port, function(...) self:dispatch(...) end)

	-- save
	self._socket = socket

	-- common config
	-- socket:setConnectTimeout(DEF_NET_CONNECT_TIMEOUT)

	socket:setEncoder(LCNetConfigure.encoder)
	socket:setDecoder(LCNetConfigure.decoder)
	socket:setPrinter(LCNetConfigure.printer)

	-- keep
	socket:setDebugOnOff(DEBUG_NET_KEEP)
	socket:setHeartbeat(DEF_HB_SEND_TIME)
	socket:setAuth(LCNetConfigure.auth_dealer)
	socket:setAutoConnect(true,1,3)

	-- save
	return socket
end

function LCNetManager:getGameSocket() return self._socket end

function LCNetManager:sendToGameServer(...) return self:send(...) end

-- timestamp, {hour, min, wday, day, month, year, sec, yday, isdst, tm}
function LCNetManager:getServerTime()
	local socket = self.socket
    if not socket then return os.time(),{} end
	return socket:getServerTime()
end

function LCNetManager:closeGameSocket()
	if self._socket then
		self._socket:destroy()
	end
	self._socket = nil
end

function LCNetManager:send(proto, option)
	-- check parms
	-- local msglist = (proto.__cname == nil) and proto or {proto}
	-- if not msglist or table.getn(msglist) == 0 then GError("proto is nil") return end
	--
	-- -- check register
	-- local proto = msglist[1]
	if proto._action_type == nil then GFatal("proto can not be registered") return end
	-- if not IS_SEND_ID(proto._action_type) then GFatal("invalid action type, proto %s, %s, %s",proto.__cname, proto._name, proto._action_type) return end
	-- local seed = ID_SEED(proto._action_type)
	local seed = proto._action_type
	local msgInfo = self.message[seed]
	if not msgInfo then GError("can not find message(%d) msgInfo", proto._action_type) return end

	-- find socket
	local socket = self._socket
	if not socket then
		GError("can not find socket")
		return
	end

	-- send
	return socket:send(proto, option)
end

-- return: true: ok   others: error
function LCNetManager:dispatch(protoABS, msglist, banUI)
	-- for i=1,table.getn(msglist) do
		-- decode
		local proto = msglist -- [i]
		local appmsg = proto._msg
		local seed = ID_SEED(proto._action_type)
		local info = self.message[seed]

		-- data update
		local uiUpdater = nil
		if info.r_callBack then uiUpdater = info.r_callBack(seed, appmsg) end

		-- ui update
		if not banUI then
			local scene = LCSceneTransit:getRunningScene()
			if scene and scene.dealPacket then
				scene:dealPacket(seed, appmsg, uiUpdater)
			end
		end
	-- end
	return true
end
