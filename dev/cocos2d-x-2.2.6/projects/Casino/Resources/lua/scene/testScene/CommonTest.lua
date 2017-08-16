LCCommonTest = {}
LCCommonTest.__index = LCCommonTest

function LCCommonTest:testUserdefault()

	local player = LCLocalDataCenter:player()


	GDebug("playerName : %s",player.nickName)
	LCUserContext:setObjectForKey(eUserDefaultKey.kLocalServerPlayer,player)

	local savedPlayer = LCUserContext:getObjectForKey(eUserDefaultKey.kLocalServerPlayer)
	GDebug("playerName : %s",savedPlayer.nickName)


end

function LCCommonTest:testHttpClient()

	-- local request = LCHttpClient:checkAppVersion(5,176)
	-- LCHttpClient:sendWithHandler(request,function(res, hnd)
	-- 	local code = res:getResponseCode()
	-- 	local resData = cjson.decode(res:getResponseData())
	-- 	local response = resData.response
	-- 	GDebug(response.share_sinaweibo) 
 --    end)

	local httpRequest = HttpRequestExt:create()

	local url = "http://hkcardavatar.qiniudn.com/919f05fb6f4d34a4aeb2fd142c7be601_1412762414_131924?imageView/1/w/100/h/100/sharpen/1"
	httpRequest:sendGet(url,function(code,tag,strError,spr)
		if spr == nil then	
			GDebug(strError)
		else
			local sprite = tolua.cast(spr,"CCSprite")
			
			-- GDebug("%s",path)
			-- local cachePath = path.."/cacheImage"


			local texture = sprite:getTexture()

			GDebug("%s",sprite)
		end
	end)
end

