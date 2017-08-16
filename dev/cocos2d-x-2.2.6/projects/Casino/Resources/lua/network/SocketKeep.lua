
LCSocketKeep = class("LCSocketKeep", LCSocketBase)

function LCSocketKeep:ctor(...)
	LCSocketBase.ctor(self, ...)

	self.auth = nil -- {creator, callback}
	self.isAuth = false
end


function LCSocketKeep:hasAuth()
	if not self.auth then return true end
	return self.isAuth == true
end

function LCSocketKeep:setAuthState(isAuth)
	if isAuth == nil then GError("invalid arguments") return end
	local preState = self.isAuth
	self.isAuth = isAuth
end

function LCSocketKeep:setAuth(f_creator, callback)
	self.auth = {creator = f_creator, callback = callback}
end


function LCSocketKeep:send(msglist, option)
	-- save packer
	local packer = self:pack(msglist, option)
	self:packQ_push_back(packer)
	-- try flush
	self:flush(packer)

	self:didSent(packer.messageID, true)
	return packer.messageID
end


-- virtual
function LCSocketKeep:flush(packer)
	-- check connected
	if self:isConnected() then
		-- check auth
		if not self:hasAuth() then return end

		-- flush
		self:flush_packer(packer)

	-- try connect
	else
		self:connect()
	end

end

function LCSocketKeep:didReceive(msg, length)
	if msg == DEF_HB_RECV_MSG then GDebug("======= heart beat pong! =======") return end
	local function deal_auth(header, msglist)
		-- deal auth
		local isAuth = true
		if self.auth.callback then
			isAuth = self.auth.callback(self, header, msglist)
		end

		-- set state
		self:setAuthState(isAuth)
		LCAppGlobal:setReconnect(0) -- reset reconnect flag

		-- flush all message
		if self:hasAuth() then
			self:flush_Q()
			LCSocketBase.didConnect(self)
		end
	end


	local function deal_logic(header, msglist)
		-- check
		-- if #msglist == 0 then
		-- 	GError("recive an empty msglist")
		-- 	return
		-- end

		-- check receipt
		local proto = msglist
		local seed = ID_SEED(proto._action_type)
		self.f_dispatch(header, msglist)
	end

	-- decode
	local header, msglist, isAuth, isError = self:decode(msg, length, true)
	if not header or not msglist then return end

	-- print
	if self.debugOnOff then self:debugPackage(header, msglist, 2) end

	-- choose by phase
	if self:hasAuth() then
		if isAuth then GError("received an repeated auth message") return end
		deal_logic(header, msglist)

	else
		if isAuth == false then
			if isError then
				LCNetManager:closeGameSocket()
				LCAppGlobal:setToken("")
				LCSceneTransit:getRunningScene():hideLoading()
				LCMessageBox:showConfirm(self,{text = _("您的登录已经过期，请重新登录！"),hideClose = true,callback = function ( ... )
					LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
				end})
			end
			GError("can not deal logic message before auth finished") return
		end
		deal_auth(header, msglist)
	end
end

function LCSocketKeep:didConnect()
	if not self:hasAuth() then
		local msglist = LCNetManager:createMessage(eDefMsg.Auth)
		local pack = self:packAuth(msglist)
		self:flush_packer(pack)

		_ANLS_EVENT(8026)
	else
		self:flush_Q()
		LCSocketBase.didConnect(self)
	end
end

function LCSocketKeep:didDisconnect()
	-- clear
	self:setAuthState(false)

	-- discard all packers
	self:discardPackers()

	-- notify disconnect
	LCSocketBase.didDisconnect(self)
end


function LCSocketKeep:flush_Q()
	-- flush all packer
	self:packQ_foreach(function (index, packer)
		self:flush_packer(packer)
	end)
end
