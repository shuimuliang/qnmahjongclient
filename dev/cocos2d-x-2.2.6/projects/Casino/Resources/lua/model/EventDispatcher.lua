
eEvent = {
	-- ui
	page_replace_end = {id = 0x101, interrupt = false}, -- (pageObj)
	view_set_end = {id = 0x102, interrupt = false}, -- (viewObj)
	msgbox_start = {id = 0x104, interrupt = false}, -- (msgboxObj)
	
	-- net work
	packet_pay_deal = {id = 0x201, interrupt = false}, -- (seed msg)
	
	-- battle
	battle_state_change = {id = 0x302, interrupt = true}, --(state)
	
	-- new player
	newplayer_step_end = {id = 0x401, interrupt = true}, --(new player object)
}


LCEventDispatcher = class("LCEventDispatcher")

function LCEventDispatcher:register(event,manager)
	-- check lock 
	if self.lock then
		table.insert(self.addList,{event,manager})
		return
	end
	
	if not manager or not event then GError("invalid argument") return end
	if not self.eventMap[event] then self.eventMap[event] = {} end
	local list = self.eventMap[event]
	
	-- 	check
	for i=1,table.getn(list) do if list[i].manager == manager then return end end
	if not manager.dealEvent then 
		GError("%s can not implement dealEvent function", manager.__cname)
		return
	end
		
	-- save
	local item = {manager = manager,}
	table.insert(list, item)
end	


function LCEventDispatcher:start() 
	CC_SYNTHESIZE(self,"eventMap",{}) -- <event, item list>

	-- register system
	self:registerSystemEvent()
end

function LCEventDispatcher:registerSystemEvent()
	local function bind(event, callBack)
		CEventManager:getInstance():registerScriptEvent(event,callBack)
	end 
	
	-- event home
	bind(CE_EVENT_APP_HOME, function () 
		-- music		
		Audio.pauseMusic() 
		Audio.pauseAllEffects()
		-- LCLocalDataCenter:saveData()
	end)
	
	-- event back
	bind(CE_EVENT_APP_BACK, function () 
		-- music
		
		local runScene = LCSceneTransit:getRunningScene()
		local isMJ = (runScene and runScene.__cname == "LCMJScene")
		if runScene and runScene.onAppBackEvent then runScene:onAppBackEvent() end
		
		local setting = LCUserContext:getGameSettings()	
		if setting.bgMusicOnOff and isMJ then Audio.resumeMusic() end
		if setting.soundEffectOnOff then Audio.resumeAllEffects() end
	end)
	
end

function LCEventDispatcher:send(event, ...)
	-- check event
	if not event or type(event) ~= "table" then GError("event is nil") return end
	
	-- check event or interrupt
	if event.interrupt then
		return self:_sendInterrupt(event, ...)
	else
		return self:_sendEvent(event, ...)
	end
end

function LCEventDispatcher:notifySource(source)
	if not source then GError("invalid source") return end
	if not source.callBack then return end
	if source.disabled then GError("repeated notify, event %d", source.event.id) return end
	source.disabled = true
	source.callBack()
end


function LCEventDispatcher:_sendEvent(event, object, ...)
	-- check
	if not event then GError("event is nil") return end
	if not object or not object.__cname then GError("invalid event source") return end
	local source = {event = event, object = object}
	return self:_send(event, source, ...)
end

function LCEventDispatcher:_sendInterrupt(event, callBack, object, ...)
	if not callBack or type(callBack) ~= "function" then GError("invalid call back") return end
	if not object or not object.__cname then GError("invalid event source") return end
	local source = {event = event, object = object, callBack = callBack,}
	return self:_send(event, source, ...)
end


function LCEventDispatcher:_send(event, source, ...)
	-- find manager for deal
	local list = self.eventMap[event]
	if not list then return false end
	
	-- dispatch
	local parms = {...}
	local isDeal = false
	for i=1,table.getn(list) do
		local item = list[i]
		isDeal = coroutineEasy.try(function() return item.manager:dealEvent(event, source, unpack(parms)) end)
		if isDeal then break end
	end	
	
	return isDeal
end