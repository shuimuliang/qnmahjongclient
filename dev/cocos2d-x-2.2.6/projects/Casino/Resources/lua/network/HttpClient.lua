LCHttpClient = {}

--=======  private  ===============

function LCHttpClient:genRequest( method, baseURL, path, params )
	if( method ~= kHttpGet and
		method ~= kHttpPost and
		method ~= kHttpPut and
		method ~= kHttpDelete) then
		GDebug("error request type : %s",method)
		return nil
	end
	local paramsStr = ""
	if params then
		for k,v in pairs(params) do
			if #paramsStr > 0 then paramsStr = paramsStr.."&"..tostring(k).."="..tostring(v)
		 	else paramsStr = tostring(k).."="..tostring(v) end
		end
	end
	local url = (baseURL or "")..(path or "")

	if DEBUG_ON_OFF then
		GDebug("==== http requst start  =====")
		GDebug("url : "..url.."\n method : "..method.."\n params : "..paramsStr)
	end

	local request = CCHttpRequest:open(url, method,paramsStr)
	if not request then GDebug("request create failed") end
	return request
end

function LCHttpClient:genRequestWithPb( method, baseURL, path, proto )
	if( method ~= kHttpGet and
		method ~= kHttpPost and
		method ~= kHttpPut and
		method ~= kHttpDelete) then
		GDebug("error request type : %s",method)
		return nil
	end
	local msglist = proto -- (proto.__cname == nil) and proto or {proto}
	-- if not msglist or table.getn(msglist) == 0 then GError("proto is nil") return end
	local paramsStr,protoABS = LCNetConfigure.encoder(msglist)

	self:debugPackage(protoABS,msglist,1)

	-- paramsStr = base64Encode(paramsStr,string.len(paramsStr))
	local url = (baseURL or "")..(path or "")

	if DEBUG_ON_OFF then
		GDebug("==== http requst start  =====")
		GDebug("url : "..(url or ""))
		-- GDebug("url : "..(url or "").."\n method : "..(method or "").."\n params : "..(paramsStr or ""))
	end
	local request = CCHttpRequest:open(url, method,paramsStr)
	if not request then GDebug("request create failed") end
	return request
end

--=======  public  ===============
function LCHttpClient:sendWithHandler(request,succCallback,failedCallback)
	if not request then GError("request should not nil") return end

	local function httpHandler(res, hnd)
		local code = res:getResponseCode()
		if code ~= eHttpResponseCode.ok then
			GError("response error :%s",code)
			if failedCallback then failedCallback(code) end
		else
			if not res:getResponseData() or res:getResponseData()=="" then
				GError("LCHttpClient lines 47 res:getResponseData() nil")
				return
			end
			local resData = cjson.decode(res:getResponseData())
			if succCallback then succCallback(resData.response) end
		end
	end

	request:sendWithHandler(httpHandler)
end
function LCHttpClient:sendPbWithHandler(request,succCallback,failedCallback)
	if not request then GError("request should not nil") return end

	local function httpHandler(res, hnd)
		local code = res:getResponseCode()
		if code ~= eHttpResponseCode.ok then
			GError("response error :%s",code)
			if failedCallback then failedCallback(code) end
		else
			if not res:getResponseData() or res:getResponseData()=="" then
				GError("LCHttpClient lines 47 res:getResponseData() nil")
				return
			end
			local resData = res:getResponseData()
			resData = base64Decode(resData,string.len(resData))

			GDebug("recive httpResponse :  %s   len: %s",resData,string.len(resData))
			local protoABS, protolist = LCNetConfigure.decoder(resData,string.len(resData), true)

			self:debugPackage(protoABS,protolist,2)


			if not protolist then failedCallback(cod) return end
			local proto = protolist
			local appmsg = proto._msg
			local seed = ID_SEED(proto._action_type)
			if succCallback then succCallback(seed,appmsg) end
		end
	end

	request:sendWithHandler(httpHandler)
end


function LCHttpClient:debugPackage(header, msglist, tag) -- tag : 1:encode 2:decode
	if DEBUG_ON_OFF then
		LCNetConfigure.printer(header, msglist, tag)
	end
end


--[[
parmas: channel
		innerVersion
return:  CCHttpRequest
--]]


function LCHttpClient:feedback(playerID, version, channel, time, url, info)
	local params = {player_id=playerID, version=version, channel=channel, version=version,
	 time=time, url=url, info=info}
	return self:genRequest(kHttpPost, DEF_FEEDBACK_PATH, "", params)
end

function LCHttpClient:playerLogin(pb)
	GDebug("%s",DEF_LOGIN_SERVER_IP)
	return self:genRequestWithPb(kHttpPost,DEF_LOGIN_SERVER_IP, "", pb)
end
