
-- ======================= LCPageBase ======================= --
LCPageBase = class("LCPageBase",function() return CCLayerColor:create() end)
function LCPageBase:init(scene, key)
	CC_SYNTHESIZE(self,"scene",scene)
	CC_SYNTHESIZE(self,"key",key)

	if not self.scene then GError("scene is nil") return false end
	
	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()	
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)	
	
	return true
end

-- virtual
function LCPageBase:onEnter() GDebug("onEnter") end
function LCPageBase:onExit() end
function LCPageBase:showMessageBox(...) end
function LCPageBase:dealPacket(...) end
