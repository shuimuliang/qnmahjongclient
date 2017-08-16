
DEF_TARGET_PLATFORM = getOSPlatform()
DEF_PLATFORM_IOS           		= 1
DEF_PLATFORM_ANDROID            = 2
DEF_PLATFORM_WIN32              = 3
DEF_PLATFORM_MARMALADE          = 4
DEF_PLATFORM_LINUX              = 5
DEF_PLATFORM_BADA               = 6
DEF_PLATFORM_BLACKBERRY         = 7
DEF_PLATFORM_MAC                = 8
DEF_PLATFORM_NACL               = 9
DEF_PLATFORM_EMSCRIPTEN         = 10
DEF_PLATFORM_TIZEN              = 11
DEF_PLATFORM_WINRT              = 12
DEF_PLATFORM_WP8                = 13

function cpf_setDefaultFont(name,file)
	if not name or not file then GFatal("invalid arguments") end
	if  DEF_TARGET_PLATFORM == DEF_PLATFORM_IOS or
		DEF_TARGET_PLATFORM == DEF_PLATFORM_WIN32 or
		DEF_TARGET_PLATFORM == DEF_PLATFORM_WP8 then
		CCLabelTTF:setDefaultFontName(name)
	else
		local path = CCFileUtils:sharedFileUtils():fullPathForFilename(DEF_PATH.font .. file)
		CCLabelTTF:setDefaultFontName(path)
	end
end	

function getShopTableName()
	local channel = CSDKManager:getInstance():getChannelID()
	if DEF_TARGET_PLATFORM == DEF_PLATFORM_ANDROID then
		if channel == DEF_SDK_CHANNEL_GOOGLE_PLAY then
			return "android_shop"
		else
			return "duandai_shop"
		end
	elseif DEF_TARGET_PLATFORM == DEF_PLATFORM_IOS then
		return "ios_shop"
	else
		return "duandai_shop"
	end
end



