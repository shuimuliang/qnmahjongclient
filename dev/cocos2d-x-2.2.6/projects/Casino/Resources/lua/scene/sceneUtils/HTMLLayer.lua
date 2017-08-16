
local LCWebView = require("lua/scene/sceneUtils/WebView")

local LCHTMLLayer = class("LCHTMLLayer", function(...) return CCLayerColor:create(...) end)

LCHTMLLayer.__index = LCHTMLLayer

--[[

key = {
	url = "http://baidu.com",
	dimensions = CCSizeMake(100,100),
	activityName = "aboutActivity",
	notInitVisibel = true,
}
url, x,y,w,h,parentNode
--]]

function LCHTMLLayer:create(...)
	local ret = LCHTMLLayer.new()
	if not ret:init(...) then return nil end
	return ret
end	

function LCHTMLLayer:init(key)
	if not key then GError("key should not nil") return end

	CC_SYNTHESIZE(self,"url", key.url or "http://weibo.com")
	CC_SYNTHESIZE(self,"dimensions", key.dimensions or CCSizeMake(300,300))
	CC_SYNTHESIZE(self,"webView", nil)
	CC_SYNTHESIZE(self,"activityName", key.activityName or "LCHTMLLayer")
	CC_SYNTHESIZE(self,"notInitVisibel", key.notInitVisibel )

	self:changeWidthAndHeight(self.dimensions.width,self.dimensions.height)

	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()	
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)	

	return true
end

function LCHTMLLayer:updateURL(url)
	if not url then return end
	self.url = url
	self:showWebView()
end
function LCHTMLLayer:setVisible(visible)
	if visible then
		GDebug("xxxxxvvvvvvvvvvvvvvvvvvvvvv1")
		self:showWebView()
	else
		GDebug("xxxxxvvvvvvvvvvvvvvvvvvvvvv2")
		self:removeWebView()
	end
end

function LCHTMLLayer:showWebView()
	if not self.webView then
		self.webView = LCWebView.new({activityName = self.activityName})
	end
	self.webView:showWebView(self.url,0,0,self.dimensions.width,self.dimensions.height,self:getParent())
end

function LCHTMLLayer:removeWebView()
	if self.webView then self.webView:removeWebView() end
	self.webView = nil
end

function LCHTMLLayer:onEnter()
	-- LCHTMLLayer.super.onEnter(self)
	-- todo
	if not self.notInitVisibel then self:showWebView() end
end

function LCHTMLLayer:onExit()
	-- todo
	self:removeWebView()
	-- LCHTMLLayer.super.onExit(self)
end

return LCHTMLLayer