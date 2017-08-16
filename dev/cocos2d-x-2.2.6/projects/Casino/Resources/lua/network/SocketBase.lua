
LCPackOption = class("LCPackOption")

function LCPackOption:ctor(timeout, failed_cb)
	self.timeoutTick = timeout and timeout * 1000 or nil
	self.failedSentCallback = failed_cb
end

function LCPackOption:destory()
	self.failedSentCallback = nil
end

local LCPackage = class("LCPackage")

function LCPackage:ctor(messageID, bytes, header, msglist, option)
	self.messageID = messageID
	self.bytes = bytes
	self.header = header
	self.msglist = msglist
	self.option = option or LCPackOption.new()
	self.createTime = getTickCount()
	self.sentID = 0
end

function LCPackage:notifyFailedSent(socket) if self.option.failedSentCallback then self.option.failedSentCallback(self.messageID, socket) end end

function LCPackage:isTimeout(time)
	local timeoutTick = self.option.timeoutTick
	if not timeoutTick or not time then return false end  	-- guyierror should not time
	return (time - self.createTime) >= timeoutTick
end

LCSocketBase = class("LCSocketBase")

function LCSocketBase:ctor(ip, port, f_dispatch)
	if not ip then GError("socket 没有 IP") return end
	self.socket = WebSocket:create("ws://" .. ip)
	if self.socket then
        self.socket:registerScriptHandler(handler(self, self.didConnect), kWebSocketScriptHandlerOpen)
        self.socket:registerScriptHandler(handler(self, self.didReceive), kWebSocketScriptHandlerMessage)
        self.socket:registerScriptHandler(handler(self, self.didDisconnect), kWebSocketScriptHandlerClose)
        self.socket:registerScriptHandler(handler(self, self.didDisconnect), kWebSocketScriptHandlerError)
    end
	self.timer = CSystemTimerData:new_local()
	self.ip = ip
	self.port = port
	self.messageID = 1
	self.f_dispatch = f_dispatch
	self.timestamp = { tm_server = os.time(), tm_local = os.time() }
	self.packQueue = stllist.new()
	self.packRecycleBin = nil

	-- extend property
	CC_SYNTHESIZE(self, "debugOnOff", false)
	CC_SYNTHESIZE(self, "connectTimeout", 0xffff)
	CC_SYNTHESIZE_RO(self, "autoConnect",  {onOff = false, intelligent = false, minTime = 0, maxTime = 0, curTime = 0})

	-- extern
	CC_SYNTHESIZE(self, "decoder", nil)
	CC_SYNTHESIZE(self, "encoder", nil)
	CC_SYNTHESIZE(self, "printer", nil)

	-- listener
	self.disconnectListeners = {}
	self.failedConnectListeners = {}
	self.onConnectListeners = {}

	-- check timeout
	CC_SYNTHESIZE(self, "scheduler", CCDirector:sharedDirector():getScheduler())
end

function LCSocketBase:__tostring()
	return string.format("[IP:%s PORT:%d]",self.ip or "0", self.port or "0")
end


function LCSocketBase:destroy()
	self.socket:close()
	if self.timer then self.timer:clearTimer() end
	self.socket = nil
	self.timer = nil
end


-- virtual
function LCSocketBase:didConnect(...)
	self:resetAutoConnectTime()
	self:notifyEventOnConnect(self)
end

function LCSocketBase:didSent(id, isDel)
	-- delete sent packer
	local sentPacker = nil
	self:packQ_foreach(function (index, packer)
		if packer.messageID == id then
			sentPacker = packer
			if isDel == true then self:packQ_erase(index) end
			return true
		end
	end)
end

function LCSocketBase:didReceive(header, msglist) end

function LCSocketBase:didFailedConnect()
	self:discardPackers()
	self:notifyEventFailedConnect(self)
end

function LCSocketBase:didDisconnect(...)
	if self._heartBeat then
		self.scheduler:unscheduleScriptEntry(self._heartBeat)
		self._heartBeat = nil
	end
	-- auto connect
	self:startAutoConnect()

	-- event notify
	self:notifyEventDisconnect(self)
end


function LCSocketBase:discardPackers()
	-- save queue
	local tmpQ = self.packQueue

	-- put to recycle bin
	if self:packQ_size() > 0 then
		self.packRecycleBin = self.packQueue
	end

	-- new queue
	self:packQ_new()

	-- notify
	tmpQ:foreach(function (index, packer)
		packer:notifyFailedSent(self)
	end)

	return tmpQ
end

function LCSocketBase:restorePackers()
	if not self.packRecycleBin then return end
	self.packQueue = self.packRecycleBin
	self.packRecycleBin = nil
end

function LCSocketBase:recycleBinSize()
	if self.packRecycleBin == nil then return 0 end
	return self.packRecycleBin:size()
end

function LCSocketBase:updateIP(address)
	if not address then return end
	self.ip = address
end

function LCSocketBase:send()  end

-- base function
function LCSocketBase:flush_packer(packer)
	self.socket:sendBinaryStringMsg(packer.bytes)
	if self.debugOnOff then
		self:debugPackage(packer.header, packer.msglist, 1)
	end
end

function LCSocketBase:isConnected() return self.socket and self.socket:getReadyState() == kStateOpen end

function LCSocketBase:setHeartbeat(stime)
	if self._heartBeat then GDebug("heart beating...") return end
	self._heartBeat = self.scheduler:scheduleScriptFunc(function()
		GDebug("send heart beat")
		self.socket:sendBinaryStringMsg("ping")
	end, stime, false)
end

function LCSocketBase:setAutoConnect(onOff, minTime, maxTime, intelligent)
	if onOff == nil or not minTime or not maxTime then GError("invalid arguments") return end
	self.autoConnect.onOff = onOff
	self.autoConnect.minTime = minTime
	self.autoConnect.maxTime = maxTime
	self.autoConnect.curTime = minTime
	self.autoConnect.intelligent = intelligent
end

-- listeners
local function _registerListener(list, k, f)
	if not list or not k or not f then GError("invalid arguments") return end
	if list[k] then GError("key %s is repeated", tostring(k) or "userdata") return end
	list[k] = f
end
local function _unregisterListener(list, k)
	if not list or not k then GError("invalid arguments") return end
	list[k] = nil
end
local function _notifyListeners(list, ...)
	for k,f in pairs(list) do
		f(...)
	end
end

function LCSocketBase:registerEventDisconnect(...) _registerListener(self.disconnectListeners, ...) end
function LCSocketBase:registerEventFailedConnect(...) _registerListener(self.failedConnectListeners, ...) end
function LCSocketBase:registerEventOnConnect(...) _registerListener(self.onConnectListeners, ...) end

function LCSocketBase:unregisterEventDisconnect(...) _unregisterListener(self.disconnectListeners, ...) end
function LCSocketBase:unregisterEventFailedConnect(...) _unregisterListener(self.failedConnectListeners, ...) end
function LCSocketBase:unregisterEventOnConnect(...) _unregisterListener(self.onConnectListeners, ...) end


function LCSocketBase:notifyEventDisconnect(...) _notifyListeners(self.disconnectListeners, ...) end
function LCSocketBase:notifyEventFailedConnect(...) _notifyListeners(self.failedConnectListeners, ...) end
function LCSocketBase:notifyEventOnConnect(...) _notifyListeners(self.onConnectListeners, ...) end


-- private
function LCSocketBase:connect()
	-- check connectting or work
	if self:isConnected() then return true end

	self:disconnect()
	self.socket = WebSocket:create("ws://" .. self.ip)
	if self.socket then
        self.socket:registerScriptHandler(handler(self, self.didConnect), kWebSocketScriptHandlerOpen)
        self.socket:registerScriptHandler(handler(self, self.didReceive), kWebSocketScriptHandlerMessage)
        self.socket:registerScriptHandler(handler(self, self.didDisconnect), kWebSocketScriptHandlerClose)
        self.socket:registerScriptHandler(handler(self, self.didDisconnect), kWebSocketScriptHandlerError)
    end
	self:setHeartbeat(DEF_HB_SEND_TIME)
	performWithDelay(LCSceneTransit.getRunningScene(), function()
		GDebug("socket ready state = " .. self.socket:getReadyState())
		if not self:isConnected() then
			self:disconnect()
			LCSceneTransit.getRunningScene():hideLoading()
			LCMessageBox:showConfirm(LCSceneTransit.getRunningScene(),{text = _("连接服务器失败,请检查您的网络..."),hideClose = true,callback = function()
				-- logout game
				local curScene = LCSceneTransit:getRunningScene()
				if curScene and curScene.__cname == "LCLoginScene" then return end

				LCAppGlobal:setNeedReconnect(true)
				LCAppGlobal:playerLogout()
				LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
			end})
		end
	end, 10)
	-- try connect
	-- local ret = self.socket:connect(self.ip, self.port, self.connectTimeout)
	-- if ret == false then
	--
	--
	-- 	local errorEnum = self.socket:lastErrorEnum()
	-- 	_ANLS_EVENT(8027,errorEnum)
	--
	-- 	GFatal("%s connect failed, error:%s", tostring(self), self.socket:lastError())
	--
	-- 	-- delay to notify
	-- 	self.timer:addTimer(0, false, function ()
	-- 		self:didFailedConnect()
	-- 	end)
	--
	-- 	-- start auto connect
	-- 	if self:isAutoConnect() then
	-- 		self:startAutoConnect()
	-- 	end
	-- end

	return true
end

function LCSocketBase:disconnect()
	if self.socket then
        self.socket:close()
        self.socket = nil
    end
end

function LCSocketBase:isAutoConnect() return self.autoConnect.onOff end

function LCSocketBase:startAutoConnect()
	-- check on off
	if not self.autoConnect.onOff then return end

	-- check intellegent
	if self.autoConnect.intelligent == true and self:packQ_size() == 0 then return end

	-- delay to connect
	if self.autoConnectTimerID then return end
	self.autoConnectTimerID = self.timer:addTimer(self.autoConnect.curTime, false, function ()
		if self.debugOnOff then
			GDebug("%s start auto connect, queueSize:%d autoConnectTime:%d",
				tostring(self), self:packQ_size(), self.autoConnect.curTime)
		end
		self.autoConnectTimerID = nil
		self:connect()
	end)

	-- reset auto connect time
	self:resetAutoConnectTime(self.autoConnect.curTime + 1)
end

function LCSocketBase:resetAutoConnectTime(time)
	if not self.autoConnect.onOff then return end
	self.autoConnect.curTime = time or self.autoConnect.minTime
	-- if self.autoConnect.curTime > self.autoConnect.maxTime then
	-- 	self.autoConnect.curTime = self.autoConnect.maxTime
	-- end
end


function LCSocketBase:pack(msglist, option, messageID)
	if not messageID then messageID = self:applyMessageID() end
	local bytes, header = self:encode(msglist)
	if option == nil then option = LCPackOption.new() end
	return LCPackage.new(messageID, bytes, header, msglist, option)
end

function LCSocketBase:packAuth(msglist)
	local messageID = 0
	local bytes, header = self:encode(msglist, true)
	return LCPackage.new(messageID, bytes, header, msglist, nil)
end

function LCSocketBase:packQ_push_front(packer) self.packQueue:push_front(packer) end
function LCSocketBase:packQ_push_back(packer) self.packQueue:push_back(packer) end
function LCSocketBase:packQ_erase(index) return self.packQueue:erase(index) end
function LCSocketBase:packQ_pop_front() return self.packQueue:pop_front() end
function LCSocketBase:packQ_front() return self.packQueue:front() end
function LCSocketBase:packQ_size() return self.packQueue:size() end
function LCSocketBase:packQ_clear() return self.packQueue:clear() end
function LCSocketBase:packQ_foreach(...) self.packQueue:foreach(...) end
function LCSocketBase:packQ_new() self.packQueue = stllist.new() end


-- common functions
function LCSocketBase:debugPackage(header, msglist, tag) -- tag : 1:encode 2:decode
	if not self.debugOnOff then return end
	self.printer(header, msglist, tag)
end

-- return pb abs
function LCSocketBase:encode(msglist, isAuth)
	return self.encoder(msglist, isAuth)
end

-- return header, {appProto ...}
function LCSocketBase:decode(msg, length, isRecv)
	local header, msglist, time, isAuth, isError = self.decoder(msg, length, isRecv)
	self:updateServerTime(time)
	return header, msglist, isAuth, isError
end

function LCSocketBase:updateServerTime(time)
	if not time then
		GError("invalid server time")
		return
	end
	self.timestamp.tm_server = time
	self.timestamp.tm_local = os.time()
end

function LCSocketBase:getServerTime()
	local tm_inc = os.time() - self.timestamp.tm_local
	local tm_cur = self.timestamp.tm_server + tm_inc

	return tm_cur , os.date("*t", tm_cur)
end

function LCSocketBase:applyMessageID()
	local messageID = self.messageID
	self.messageID = self.messageID + 1
	return messageID
end
