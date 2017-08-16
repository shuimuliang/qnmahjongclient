
LCSharedBrowser = CCWebView:create("sharedWebView")
LCSharedBrowser:retain()

local LCWebView = class("LCWebView")

function LCWebView:ctor(key)
	-- object
	key = key or {}
	CC_SYNTHESIZE(self,"_browser",nil)
	CC_SYNTHESIZE(self,"_activityName",key.activityName)
	
	-- pos and size
	CC_SYNTHESIZE(self,"_x",0)
	CC_SYNTHESIZE(self,"_y",0)
	CC_SYNTHESIZE(self,"_w",100)
	CC_SYNTHESIZE(self,"_h",100)
end

function LCWebView:createBrowser()
	assert(self._activityName, "ActivityName is necessary!")
	if not self._browser then
		self._browser = LCSharedBrowser
	end
end

function LCWebView:setActivityName(name)
	self._activityName = name
end

function LCWebView:getActivityName()
	return self._activityName
end

function LCWebView:setPos(x,y)
	self._x = x
	self._y = y
end

function LCWebView:getPos()
	return self._x, self._y
end

function LCWebView:setSize(w,h)
	self._w = w
	self._h = h
end

function LCWebView:getSize()
	return self._w, self._h
end

function LCWebView:showWebView(url, x,y,w,h,parentNode)
	local scale = GET_ADAPTER_SCALE()
	local parentNodePos = ccp(0,0)
	local parentNodeSize = CCSizeMake(0,0)
	local anchorPoint = parentNode:getAnchorPoint()
	local visibleSize = CCDirector:sharedDirector():getWinSize()
	if parentNode then
		parentNodePos = CE_GET_WORLD_POSITION(parentNode,anchorPoint)
		parentNodeSize = parentNode:getContentSize()

		if x then x = parentNodePos.x - parentNodeSize.width*anchorPoint.x end
		if y then y = visibleSize.height - (parentNodePos.y + parentNodeSize.height* (1-anchorPoint.y)) end
	end

	GDebug("%s    ",sizestring(visibleSize))
	GDebug("%s   %s   %s  %s  ",parentNodePos.x,parentNodePos.y ,parentNodeSize.width,parentNodeSize.height)
	GDebug("%s   %s   %s  %s   %s  %s    %s",x,y ,w,h,anchorPoint.x,anchorPoint.y,scale)

	if x and y then 
		x = x*scale   
		y = y *scale
		self:setPos(x,y) 
	end
	if w and h then 
		w = w*scale   
		h = h *scale
		self:setSize(w,h) 
	end
	if self._browser then
		self._browser:updateURL(url)
	else
		self:createBrowser()
		self._browser:showWebView(url, self._x, self._y, self._w, self._h)
	end
end

function LCWebView:removeWebView()
	if self._browser then
		-- self._browser:release()
		self._browser:removeWebView()
		-- self._browser = nil
	end
end

return LCWebView