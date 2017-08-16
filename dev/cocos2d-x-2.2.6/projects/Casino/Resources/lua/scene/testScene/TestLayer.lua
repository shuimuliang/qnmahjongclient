LCTestLayer = class("LCTestLayer",function() return CCLayer:create() end)

function LCTestLayer:create(...)
	local ret = LCTestLayer.new()
	if not ret:init(...) then return nil
	end
	return ret
end

function LCTestLayer:init()
	
	CC_SYNTHESIZE(self, "testSprite", CCSprite:create("dialog_bg.png"))
	
	self:addChild(self.testSprite)


	local function onTouchBegin( ... )
		return true
	end 



	-- touch event
	local function onTouch(eventType,x,y)
		-- local glPos = ccp(x,y) 
		 GDebug("mask pos %s   type: %s",posstring(glPos),eventType)
		if eventType == "began" then return onTouchBegin(eventType,x,y) end
		-- 	if not self.tapToDissmiss then return end
		-- 	self:dismissSelf()
		-- end
		-- return true
		--self:test()
	end

	self:registerScriptTouchHandler( --function( eventType,x,y )
		--if eventType == "began" then GDebug("touch begin") return true end
		--self:test()
	--end
	onTouch
	, false, 0, true)
	self:setTouchEnabled(true)
	
	return true
end

function LCTestLayer:test()
	-- self.testSprite:setTouchEnabled(true)
	self.testSprite:setVisible(false)		
	GDebug("%s",self.testSprite)

end

function LCTestLayer:onEnter()
	
end
function LCTestLayer:onExit()
	
end