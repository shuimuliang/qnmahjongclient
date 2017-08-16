local LCMatchMessagesMgr = class("LCMatchMessagesMgr")

function LCMatchMessagesMgr:ctor(scene)
	CC_SYNTHESIZE(self,"scene",scene)
	CC_SYNTHESIZE(self, "curHandleMsg", nil)
	CC_SYNTHESIZE(self, "isPaused", false)
	CC_SYNTHESIZE(self, "packerQueue", stllist.new())
end

-- public methods
function LCMatchMessagesMgr:pushMessageToQueue(seed, msg, uiUpdater)
	if not msg then return end
	local packer = {
					seed = seed,
					msg = msg,
					uiUpdater = uiUpdater,
					time = getTickCount(),
					callback = nil,
					waitFinish = false,
					}
	self.packerQueue:push_back(packer)
	if not self.isPaused then self:updateMatchUI() end
end
function LCMatchMessagesMgr:clearAllMessagesInQueue()
	self.packerQueue:clear()
end

function LCMatchMessagesMgr:pauseHandleMessage()
	self.isPaused = true
end
function LCMatchMessagesMgr:startHandleMessage()
	self.isPaused = false
	self:updateMatchUI()
end

function LCMatchMessagesMgr:forceHandleNextMessage()
	if self.packerQueue:size() < 1 then return end
	local curPacker = self.packerQueue:front()
	if curPacker.waitFinish then
		self:messageHandleFinished(curPacker)
	end
end

-- private methods
function LCMatchMessagesMgr:messageHandleFinished(packer)
	GDebug("end handling message type: %d",packer.seed)
	self.packerQueue:foreach_reverse(function (index, aPacker)
		if packer == aPacker then
			self.packerQueue:erase(index)
		end
	end)
	if not self.isPaused then self:updateMatchUI() end
end

function LCMatchMessagesMgr:updateMatchUI()
	if self.packerQueue:size() < 1 then return end
	local packerToHandle = self.packerQueue:front()

	if packerToHandle.waitFinish then
		if (getTickCount() - packerToHandle.time)/1000 > DEF_MSG_PROCESS_MAX_TIME then
			GError("process should not exceed %d s",DEF_MSG_PROCESS_MAX_TIME)
			self:messageHandleFinished(packerToHandle)
		end
		return
	end

	-- check process time
	if (getTickCount() - packerToHandle.time)/1000 > DEF_MSG_PROCESS_MAX_TIME then
		GError("should not wait %ds to handle xxxxxxxxxxxxxxxxxx",DEF_MSG_PROCESS_MAX_TIME)
	end
	GDebug("start handling message type: %d",packerToHandle.seed)

	-- handle message
	local function handleEnd()
		self:messageHandleFinished(packerToHandle)
	end


	local function handleBegin()
		self:handlePacker(packerToHandle,handleEnd)
	end

	local err = coroutineEasy.try(handleBegin)
	if err or (not packerToHandle.waitFinish) then
		handleEnd()
	end
end

function LCMatchMessagesMgr:handlePacker(packer,callback)

end

return LCMatchMessagesMgr
