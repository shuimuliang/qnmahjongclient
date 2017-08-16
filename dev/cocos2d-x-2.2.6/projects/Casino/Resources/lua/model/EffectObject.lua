local debug_effect_map = false

_EFFECT_MAP = {}

while debug_effect_map do
	local function _PRINT_EFFECT_MAP()
		print(" ============================ _PRINT_EFFECT_MAP START ============================ ")
		for k,count in pairs(_EFFECT_MAP) do
			if count > 0 then
				local str = string.format("%s ( %s )",k,count)
				print(str)
			end
		end
		print(" ============================ _PRINT_EFFECT_MAP END ============================ ")
	end
	local scheduler = CCDirector:sharedDirector():getScheduler()
	scheduler:scheduleScriptFunc(_PRINT_EFFECT_MAP,1,false)
	break
end

LCEffectObject = class("LCEffectObject", function() return CCNodeRGBA:create() end)

function LCEffectObject:create(modelFile,...)
	local ret
	if not DEF_LOW_DEVICE then
		ret = LCEffectObject.new()
	else
		local effectName = modelFile .. eResSuffix.effect
		ret = LCEffectEmptyObject.new()
	end

	if not ret:init(modelFile,...) then return nil end
	return ret
end

function LCEffectObject:init(modelFile,callback)
	local modelName = nil

	if not modelFile then
		GError("model file is nil")
		if callback then callback() end
		return
	end

	local pos = string.find(modelFile,".ExportJson")
	if not pos then
		modelName = modelFile
		modelFile = modelName .. ".ExportJson"
	else
		modelName = string.sub(modelFile,1,pos-1)
	end

	-- effect name
	self.effectName = "play"
	self.callback = callback
	self.direct = eDirect2D.right
	self.autoPlay = true

	-- init armature
	CC_SYNTHESIZE(self,"armature",CArmatureExt:create(modelFile,modelName)	)
	if not self.armature then
		GError("[LCEffectObject:init] armature init failed")
		return
	end
	self:addChild(self.armature)

	CC_SYNTHESIZE(self,"autoDelete", not self:isLoop())

	-- enter exit event
	local function onEvent(event)
		if event == "enter" then
			self:onEnter()
		elseif event == "exit" then
			while debug_effect_map do
				_EFFECT_MAP[modelFile] = (_EFFECT_MAP[modelFile] or 0) - 1
				break
			end
			self:onExit()
		end
	end
	self:registerScriptHandler(onEvent)


	while debug_effect_map do
		_EFFECT_MAP[modelFile] = (_EFFECT_MAP[modelFile] or 0) + 1
		break
	end

	return true
end

function LCEffectObject:onEnter() if self.autoPlay then self:play() end end
function LCEffectObject:onExit() end
function LCEffectObject:setPlayName(name) self.effectName = name end
function LCEffectObject:setAutoPlay(onOff) self.autoPlay = onOff end



function LCEffectObject:play(name, callBack)
	local animName = name or self.effectName
	self.armature:stop()
	self.armature:play(animName)
	if self:isLoop(animName) then
		if callBack or self.callback then
            GError("invalid call back, effect is loop :  %s",animName or "nil")
			return
		end
	end

	-- self delete
	if not callBack and not self.callback and not self.autoDelete then return end
	if self.armature:getAnimationEventOnceData(animName) then
		GError("animation %s has bound before", animName)
		return
	end
	self.armature:setScriptCurAnimationComplete(function()
		if callBack then callBack(self)
		elseif self.callback then self.callback(self) end
		if not self.autoDelete then return end
		self:removeFromParentAndCleanup(true)
	end)
end

function LCEffectObject:isLoop(name)
	local animName = name or self.effectName
	return self.armature:isLoop(animName)
end

function LCEffectObject:pause(frame) self.armature:pause(frame) end
function LCEffectObject:resume(frame) self.armature:resume(frame) end

function LCEffectObject:getDirect() return self.direct end
function LCEffectObject:setDirect(dir)
	local typeDir = type(dir)
	if typeDir == "number" then
		self.direct = dir
	elseif typeDir == "userdata" then
		local rotate = angle2Rotate(dir:getAngle());
		if dir.x > 0 then
			self.direct = eDirect2D.right
		else
			self.direct = eDirect2D.left
		end
	else
		return
	end

	-- set rotate
	if self.direct == eDirect2D.left then
		self.armature:setRotationY(180);
	else
		self.armature:setRotationY(0);
	end
end


function LCEffectObject:rotate(angle)	-- angle like 90,180,45
	if not angle then GError("angle is nil") end
	if type(angle) ~= "number" then
		GError("angle type not number is %s",type(angle))
		return
	end

	self.armature:setRotationX( angle )
	self.armature:setRotationY( angle )
end

function LCEffectObject:getBone(name)
	return self.armature:getBone(name)
end


LCEffectEmptyObject = class("LCEffectEmptyObject", LCEffectObject )

function LCEffectEmptyObject:init(modelFile,callback)
	local modelName = nil

	if not modelFile then
		GError("model file is nil")
		if callback then callback() end
		return
	end

	local pos = string.find(modelFile,".ExportJson")
	if not pos then
		modelName = modelFile
		modelFile = modelName .. ".ExportJson"
	else
		modelName = string.sub(modelFile,1,pos-1)
	end

	-- effect name
	self.effectName = "play"
	self.callback = callback
	self.direct = eDirect2D.right
	self.autoPlay = true

	CC_SYNTHESIZE(self,"autoDelete", not self:isLoop())

	-- enter exit event
	local function onEvent(event)
		if event == "enter" then
			self:onEnter()
		elseif event == "exit" then
			self:onExit()
		end
	end
	self:registerScriptHandler(onEvent)
	return true
end

function LCEffectEmptyObject:onEnter()
	if self.autoPlay then self:play() end
end

function LCEffectEmptyObject:onExit()

end

function LCEffectObject:setPlayName(name) self.effectName = name end
function LCEffectObject:setAutoPlay(onOff) self.autoPlay = onOff end

function LCEffectEmptyObject:play(name, callBack)
	if callBack then
		callBack()
	elseif self.callback then
		self.callback()
	end
end

function LCEffectEmptyObject:isLoop(name)
	return false
end

function LCEffectEmptyObject:pause(frame)

end
function LCEffectEmptyObject:resume(frame)

end

function LCEffectEmptyObject:getDirect()

end

function LCEffectEmptyObject:setDirect(dir)

end

function LCEffectEmptyObject:rotate(angle)

end

function LCEffectEmptyObject:getBone(name)

end
