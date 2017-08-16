
coroutineEasy = {}

local listener = {
	stepCallBack = nil,
	finishCallBack = nil,
	errorCallBack = nil,
}

function coroutineEasy.schedule(object,time,func,listener, ...)
	if not time or not func or not object then print("invalid arguments") return end
	local parms = {...}
	local co = coroutine.create(func)
	local function runOnce()
		-- continue
		local ret = nil local err = nil
		ret,err = coroutine.resume(co, unpack(parms))
		if not ret then
			if listener and listener.errorCallBack then listener.errorCallBack(err) end
			return
		end	
		
		-- end of sp head show
		if coroutine.status(co) == "dead" then 
			if listener and listener.finishCallBack then listener.finishCallBack() return end
			return 
		end 
		
		-- continue
		if listener and listener.stepCallBack then listener.stepCallBack() end
		scheduleOnce(object, runOnce, time)
	end	
	runOnce()
end

function coroutineEasy.tick(object,time,func,listener, ...)
	if not time or not func or not object then print("invalid arguments") return end
	local parms = {...}
	local co = coroutine.create(func)
	local function runOnce()
		local preTime = getTickCount()
		while true do
			-- continue
			local ret = nil local err = nil
			ret,err = coroutine.resume(co, unpack(parms))
			if not ret then
				if listener and listener.errorCallBack then listener.errorCallBack(err) end
				return
			end	
			
			-- end running
			if coroutine.status(co) == "dead" then 	
				if listener and listener.finishCallBack then listener.finishCallBack(err) return end
				return 
			end 
			
			-- continue
			if listener and listener.stepCallBack then listener.stepCallBack() end
			if getTickCount() - preTime > time then return end
		end	
	end	
	
	local function monitor()
		runOnce()
		if coroutine.status(co) == "dead" then return end
		scheduleOnce(object,monitor,0)
	end
	
	monitor()
end

function coroutineEasy.frame(object,frame,func,listener, ...)
	if not frame or not func or not object then print("invalid arguments") return end
	local fms = 1
	if frame ~= 0 then fms = (1 / frame - 1 / 60) * 1000  fms = math.max(0, fms) end
	coroutineEasy.tick(object,fms,func,listener, ...)
end

function coroutineEasy.try(func, ...)
	if not func then GError("try function is nil") return end
	local co = coroutine.create(func)
	local ret = nil local err = nil -- err is return or error
	ret,err = coroutine.resume(co, ...)
	if not ret then	GError(err) end
	if coroutine.status(co) ~= "dead" then GError("function contain yield") end
	return err
end		
