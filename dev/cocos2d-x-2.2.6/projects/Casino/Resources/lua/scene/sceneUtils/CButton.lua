
local LCButton = class("LCButton", function(...) return CCLayerColor:create() end)--ccc4(255, 0, 0, 255)

function LCButton:create(...)
	local ret = LCButton.new()
	if not ret:init(...) then return nil end
	return ret
end

--[[key={
	必须
	name=图片名,

	可选字段
	plies=层数,
	text=,
	font=,
	fontSize=,
	follow=跟随,
	swallow=是否吞没事件,
	只剩began事件,
	AnchorPoint
	scale
	sound
}--]]

function LCButton:init(key)
	if not key then GError("key should not nil") return end
	CC_SYNTHESIZE(self,"key", key)
	CC_SYNTHESIZE(self,"name", key.name)
	CC_SYNTHESIZE(self,"swallow", key.swallow or false)
	CC_SYNTHESIZE(self,"plies", key.plies or 1)
	CC_SYNTHESIZE(self,"title", key.text)
	CC_SYNTHESIZE(self,"fontSize", key.fontSize or 25)
	CC_SYNTHESIZE(self,"font", key.font or DEF_SYSTEM_FONT_NAME)
	CC_SYNTHESIZE(self,"follow", key.follow or false)
	CC_SYNTHESIZE(self,"sound", key.sound)
	CC_SYNTHESIZE(self,"sprite", nil)
	CC_SYNTHESIZE(self,"textLable", nil)
	CC_SYNTHESIZE(self,"touchIn", false)
	CC_SYNTHESIZE(self,"BeganCallback", nil)
	CC_SYNTHESIZE(self,"moveCallback", nil)
	CC_SYNTHESIZE(self,"moveOutCallback", nil)
	CC_SYNTHESIZE(self,"endCallback", nil)
	CC_SYNTHESIZE(self,"endNotMoveCall", nil)
	CC_SYNTHESIZE(self,"endMoveCallback", nil)
	CC_SYNTHESIZE(self,"size", nil)
	CC_SYNTHESIZE(self,"beganpPos", nil)
	CC_SYNTHESIZE(self,"selfbeganpPos", nil)
	CC_SYNTHESIZE(self,"anchor", key.AnchorPoint or ccp(0, 0))
	CC_SYNTHESIZE(self,"scal", key.scale or 1)
	CC_SYNTHESIZE(self,"isGray", false)
	if self.sound==nil then self.sound=true end
--	GDebug("----self.sound="..tostring(self.sound))
	self.sprite = CCSprite:createWithSpriteFrameName(self.name)
	self.sprite:setAnchorPoint(self.anchor)
	self.sprite:setScale(self.scal)
	self.size = self.sprite:getContentSize()
	self:changeWidthAndHeight(self.size.width, self.size.height)
	self:addChild(self.sprite)

	if self.title then
		self.textLable = CCLabelTTF:create(self.title, self.font, self.fontSize)
		self.textLable:setPosition(ccp(self.size.width/2, self.size.height/2))
		self.sprite:addChild(self.textLable)
	end

	self:registerScriptTouchHandler(function( ... )
		return self:onBTNTouch(...)
	end, false, self.plies, false)
	self:setTouchEnabled(true)

	return true
end

function LCButton:setText(text)
	self.title=text or ""
	self.textLable:setString(text)
end

function LCButton:onBTNTouch(eventType,x,y)
	local glPos = ccp(x, y)
	local layerPos = self:convertToNodeSpace(ccp(x, y))
	local bBox = self.sprite:boundingBox()
	local visible = self:getVisible(self)
	--GDebug("visible="..tostring(visible))
	if eventType == "began" then
		self.touchIn = false
		self.beganpPos = glPos
		local posX, posY = self:getPosition()
		self.selfbeganpPos = ccp(posX, posY)
		if bBox:containsPoint(layerPos) then
			self.touchIn = true
			if self.BeganCallback and visible then
				self.BeganCallback(x, y)
			end
			if self.swallow then return false end
			self.sprite:setColor(ccc3(125, 125, 125))
		end
		return true
	elseif eventType == "moved" then
		if not bBox:containsPoint(layerPos) then
			if self.moveOutCallback and visible and self.touchIn then
				self.moveOutCallback(x, y)
			else
				self.touchIn = false
			end
			if self.isGray==false then self.sprite:setColor(ccc3(255, 255, 255)) end
		else
			if self.moveCallback and visible then self.moveCallback(x, y) end
			if self.follow then
				self:setPosition(ccp(self.selfbeganpPos.x+(glPos.x-self.beganpPos.x), self.selfbeganpPos.y+(glPos.y-self.beganpPos.y)))
				local posX, posY = self:getPosition()
				GDebug("posX="..posX.."posY="..posY)
			end
		end
	elseif eventType == "ended" then
		if self.touchIn and visible then
			if (math.abs(self.beganpPos.x-x)<10 and math.abs(self.beganpPos.y-y)<10) then
				if self.sound then LCMusicBox:playControlSounds() end
				if self.endNotMoveCall then self.endNotMoveCall(x, y) end
			else
				if self.sound then LCMusicBox:playControlSounds() end
				if self.endMoveCallback then self.endMoveCallback(x, y) end
			end

			if self.endCallback then
				if self.sound then LCMusicBox:playControlSounds() end
				self.endCallback(x, y)
			end
		end
		if self.isGray==false then self.sprite:setColor(ccc3(255, 255, 255)) end
		self.touchIn = false
	end
end

function LCButton:getVisible(node)
	if not node then return false end
	local isVisible = node:isVisible()
	function visibleCal(gNode)
		local parent = gNode:getParent()
		if parent then
			local visible = parent:isVisible()
			if not visible then
				isVisible = false
				return isVisible
			else
				return visibleCal(parent)
			end
		else
			return isVisible
		end
	end

	return visibleCal(node)
end

-- public
function LCButton:setTextColor(color)
	if not color then return end
	self.textLable:setColor(color)
end

function LCButton:setTouchBenganCal(callback)
	if not callback then return end
	self.BeganCallback = callback
end

function LCButton:setTouchMoveCal(callback)
	if not callback then return end
	self.moveCallback = callback
end

function LCButton:setTouchOutMoveCal(callback)
	if not callback then return end
	self.moveOutCallback = callback
end

function LCButton:setTouchEndCal(callback)
	if not callback then return end
	self.endCallback = callback
end

function LCButton:setTouchEndNotMoveCal(callback)
	if not callback then return end
	self.endNotMoveCall = callback
end

function LCButton:setTouchEndMoveCal(callback)
	if not callback then return end
	self.endMoveCallback = callback
end

function LCButton:setDisplayFrame(frame)
	if not frame then return end
	self.sprite:setDisplayFrame(frame)
end
function LCButton:setSpritePosition(pos)
	if not pos then return end
	self.sprite:setPosition(pos)
end

function LCButton:getSpritePosition()
	return self.sprite:getPosition()
end

function LCButton:getSpriteChildByTag(tag)
	if not tag then return end
	return self.sprite:getChildByTag(tag)
end

function LCButton:getSpriteContentSize()
	return self.sprite:getContentSize()
end

function LCButton:setGray(isGray)
	self.isGray = isGray
	if self.isGray==true then
		self.sprite:setColor(ccc3(125, 125, 125))
	else
		self.sprite:setColor(ccc3(255, 255, 255))
	end
end

function LCButton:enableGrey()
	self.sprite:enableGrey()
end

function LCButton:defaultShader()
	self.sprite:defaultShader()
end

return LCButton
