local LCStencilTexture = class("LCStencilTexture",function() return CCLayerColor:create() end)

function LCStencilTexture:create(...)
	local ret = LCStencilTexture.new()
	if not ret:init(...) then return nil end
	return ret
end	

function LCStencilTexture:createWithImage(image, ...)
	return LCStencilTexture:createWithButton(image, ...)
end

function LCStencilTexture:createWithButton(button, ...)
	if not button then GError("button is nil") return end
	if button:isScale9Enabled() then GError("can not supoort scale9") return end
	
	local clone = button:clone()		
	local render = clone:getVirtualRenderer()	
	local model =  tolua.cast(render,"CCSprite")	
	model:setScale(button:getScale())
	model:removeFromParentAndCleanup(true)
	model:setPosition(button:getPosition())
	
	return LCStencilTexture:create(model,...)
end

function LCStencilTexture:init(model, pos, r,g,b,a)
	if not model then GError("model is nil") return end
	
	CC_SYNTHESIZE(self,"model",model)
	CC_SYNTHESIZE(self,"renderTexture",nil)

	if pos then model:setPosition(pos) end
	
	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()	
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)	
	
	local size = CCDirector:sharedDirector():getWinSize()
	-- render testure
	local renderTexture = CCRenderTexture:create(size.width, size.height, 0)
	self:addChild(renderTexture)

	renderTexture:setPosition(ccp(size.width/2,size.height/2))
	renderTexture:clear(r or 0, g or 0, b or 0, a or 0.6)

	-- blend
	local blend = ccBlendFunc()
	blend.src = GL_ZERO
	blend.dst = GL_ONE_MINUS_SRC_ALPHA	
	model:setBlendFunc(blend)
	
	-- render
	renderTexture:begin()
	model:visit()
	renderTexture:endToLua()
	
	self.renderTexture = renderTexture
	return true	
end

function LCStencilTexture:onEnter() end
function LCStencilTexture:onExit()  end

function LCStencilTexture:getSprite() 
	if not self.renderTexture then return end
	return self.renderTexture:getSprite()
end
	
return LCStencilTexture