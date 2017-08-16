LCBattleField = class("LCBattleField",function() return CCLayer:create() end)
function LCBattleField:create(...)
	local ret = LCBattleField.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCBattleField:init()
	CC_SYNTHESIZE(self,"debugDraw",CDebugDraw:create())
	self:addChild(self.debugDraw)
	return true
end

	
function LCBattleField:debugDrawData()
	local touchRectInfo = st_DrawInfo:new_local()
	touchRectInfo.rgba4b = ccc4(255,0,0,255)
	touchRectInfo.size = 2
	touchRectInfo.isSolid = true
	
	
	local function drawFriendFiled()
		self.debugDraw:drawRectangle(CCRectMake(100,100,200,200),touchRectInfo)
	end	
	local function draw()
		drawFriendFiled()
	end
	self.dbgAct = schedule(self,draw,0)
end
