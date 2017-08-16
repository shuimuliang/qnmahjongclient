require "lua/common/extern"
require "lua/mainModule/MainConfig"

LCLoadingLayer = class("LCLoadingLayer",function() return CCLayer:create() end)

--constants
local kVersionUserDefualtKey = "LCLoadingLayer.localVersion"
local kInnerVersion = 100100000
local kVersionServerURL = "http://www.baidu.com"

function LCLoadingLayer:create(...)
	local ret = LCLoadingLayer.new()
	if not ret:init(...) then return nil 
	end
	return ret
end	


function LCLoadingLayer:init(playerNum)
	print("player : %d",playerNum)	

	-- 1、去服务器取最新的版本号(若需更新 2  否则  3)
	-- 2、下载最新资源、解压、配置资源路径
	-- 3、开始进入游戏主模块

	-- skip version check
	self:enterMainModule()

	return true
end


function LCLoadingLayer:showLoading(isShow)
	print("isLoading : %d",isShow)
end
function LCLoadingLayer:reportError(errorInfo)
	print(errorInfo)
end


function LCLoadingLayer:getSeverVersion()
	local channel = CSDKManager:getChannelID() or 0
	local curVersion = CCUserDefault:sharedUserDefault():getIntegerForKey(kVersionUserDefualtKey,kInnerVersion)  
	local params = "channel="..channel.."&clientVersion="..kInnerVersion
	local request = CCHttpRequest:open(kVersionServerURL, kHttpPost,params)
	request:sendWithHandler(function(res, hnd)
		--成功 
		--需要更新 开始更新
		--否则 进入游戏

		--失败 
		--提示错误、重试
	end)
end

function LCLoadingLayer:downloadUpdate()
	--1、下载资源
	--2、解压、拷贝、配置路径
	--3、成功后 ，设置本地最新版本号

	-- CCUserDefault:sharedUserDefault():setIntegerForKey(kVersionUserDefualtKey,kInnerVersion)
end
function LCLoadingLayer:enterMainModule()
	LCMainConfig:start()
	LCSceneTransit:replaceScene(LCMainScene)
end

