
eSceneName = {
	login = "login",
	main = "main",
	test = "test",
	MJ = "MJ"
}

LCSceneTransit = {
	sceneList = {},
	preSceneInfo = {name = nil,key = nil},
	nextSceneInfo = {name = nil,key = nil},
}

function LCSceneTransit:register(name, f_create, res)
	if self.sceneList[name] then GError("%s has registered", name) return end
	self.sceneList[name] = {f_create = f_create, res = res}
end

function LCSceneTransit:start()
	self:register(eSceneName.login, "lua/scene/loginScene/LoginScene", eResLogin)
	self:register(eSceneName.main, "lua/scene/mainScene/HallScene", eResMain)
	self:register(eSceneName.MJ, "lua/scene/mj/MJScene")

	if DEBUG_ON_OFF then
		self:register(eSceneName.test, "lua/scene/testScene/TestScene")
	end
end

function LCSceneTransit:createScene(name,key)
	local info = self.sceneList[name]
	if not info or not info.f_create then GError("invalid scene info %s", name) return end
	local create = info.f_create
	local s = require(create)

	local scene = s.new(key)
	scene.getCoreLayer = function() return scene end
	self.runningScene = scene

	if not scene then GError("create scene %s failed",name) return end
	return scene,info
end


function LCSceneTransit:replaceScene(name, key, withoutLoading, isFade)
	local info = self.sceneList[name]

	withoutLoading = true

	local scene = nil
	if withoutLoading then
		local t1 = getTickCount()
		if name ~= eSceneName.login then
			GDebug("self.nextSceneName : %s",name)
			self:preLoadRes(eResCommon)
			self:preLoadRes(eResGameCommon)
		end
		self:preLoadRes(info.res)
		local t2 = getTickCount()

		GDebug("local resources :    %s",t2-t1)

		scene = self:createScene(name,key)
	else
		local loadingName = eSceneName.loading
		scene = self:createScene(loadingName, {
			nextSceneName = name,
			nextSceneKey = key,
			resources = info.res
		})
	end

	--切换场景把上一场景的音乐关闭（否则android平台会一直播放）
	Audio.stopMusic(true)

	-- first scene without loading
	local runScene = CCDirector:sharedDirector():getRunningScene()
	if not runScene then
		CSceneTransitMgr:getInstance():runWithScene(scene)
		return
	end

	--清楚上一场景的资源
	self:clearRes()

	if isFade then
		local reScene= CCTransitionFade:create(1, scene, ccc3(0, 0, 0));
		CSceneTransitMgr:getInstance():replaceScene(reScene)
	else
		CSceneTransitMgr:getInstance():replaceScene(scene)
	end
end

function LCSceneTransit:clearRes()
	CCSpriteFrameCache:sharedSpriteFrameCache():removeUnusedSpriteFrames()
	CCTextureCache:sharedTextureCache():removeUnusedTextures()
	CCArmatureDataManager:purge()
end

function LCSceneTransit:preLoadRes(res)
	if not res then return end
 	-- pvr
 	local pvrList = res.pvr or {}
 	local musicList = res.music or {}
 	local effectList = res.effect or {}
 	local fontList = res.font or {}
 	for k,v in pairs(musicList) do
 		Audio.preloadMusic(v..eResSuffix.music)
 	end
	local sharedSpriteFrameCache = CCSpriteFrameCache:sharedSpriteFrameCache()
 	for k,v in pairs(pvrList) do
 		sharedSpriteFrameCache:addSpriteFramesWithFile(v..eResSuffix.plist,v..eResSuffix.pvr)
 	end
	local sharedArmatureDataManager = CCArmatureDataManager:sharedArmatureDataManager()
 	for k,v in pairs(effectList) do
 		sharedArmatureDataManager:addArmatureFileInfo(v..eResSuffix.effect)
 	end
 	GDebug("end Load Ress ")
end

function LCSceneTransit:getRunningScene()
	if LCSceneTransit.runningScene then return LCSceneTransit.runningScene end
	local scene = CCDirector:sharedDirector():getRunningScene()
	if not scene or not scene.getCoreLayer then return end
	local layer = scene.getCoreLayer()
	return layer
end
