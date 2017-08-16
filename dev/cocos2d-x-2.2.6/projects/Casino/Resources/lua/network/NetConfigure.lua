LCNetConfigure = {}

local protoPack = "mj"

function LCNetConfigure.encoder(protolist, isAuth)
	print("wz : ", protolist._action_type)

	local protoABS = LCProtoBase.new(protoPack, "AbsMessage", {}, 0)
	protoABS._msg.token = LCAppGlobal:getToken()
	protoABS._msg.msgID = protolist._action_type
	protoABS._msg.msgBody = protolist:encode()
	return protoABS:encode(), protoABS
end

function LCNetConfigure.decoder(msg, length, isRecv)
	local protoABS = LCProtoBase.new(protoPack, "AbsMessage", {}, 0)
	local pbABS = protoABS:decode(msg,length)
	if not pbABS then GError("can not decode message, length %d", length) return end

	if pbABS.token ~= "" then
		LCAppGlobal:setToken(pbABS.token)
	end
	local msgBody = pbABS.msgBody
	local proto
	if isRecv then
		proto = LCNetManager:createMessage(SEED_RECV(pbABS.msgID))
	else
		proto = LCNetManager:createMessage(pbABS.msgID)
	end
	if not proto then GFatal("message(%d) can not be analyzed", pbABS.msgID) return end
	proto:decode(msgBody)

	return protoABS, proto, 0, (pbABS.msgID == eDefMsg.Auth), (pbABS.msgID == eDefMsg.Error)
end

function LCNetConfigure.printer(header, msglist, tag)
	-- print tag
	if tag == 1 then GDebug(">>>>>>>>>>>>>>>>>> SEND >>>>>>>>>>>>>>>>>>")
	elseif tag == 2 then GDebug("<<<<<<<<<<<<<<<<<< RECEIVE <<<<<<<<<<<<<<<<<<") end

	-- print abs
	if DEBUG_NET_ABS then
		header:printTitle()
		header:print()
		print("")
	end

	local proto = msglist
	proto:printTitle()
	proto:print()
	print("")

	GDebug("============================================")
	print("")
end

function LCNetConfigure.auth_dealer(socket, header, msglist)
	local proto = msglist[1]
	if not proto then return false end
	if proto._msg.status ~= 0 then
		LCAppGlobal:setNeedReconnect(false)
		local callback = function( ... )
			-- logout game
			LCAppGlobal:setNeedReconnect(true)
			LCAppGlobal:playerLogout()
			LCSceneTransit:replaceScene(eSceneName.login,{disableAutoLogin = true})
		end
		local runScene = LCSceneTransit:getRunningScene()
		LCMessageBox:showConfirm(runScene,{text = _("您已离线，请重新登录！"),hideClose = true,callback = callback})
		LCNetManager:closeGameSocket()
		GError("auth failed")
		return false

	else
		return true
	end
end
