

local LCMJBasePanel = class( "LCMJBasePanel")

function LCMJBasePanel:ctor(layer)
	if not layer then GError("you must use a layer to new it") return end
	CC_SYNTHESIZE(self,		"layer", 			layer)
	CC_SYNTHESIZE(self,		"scene", 			LCSceneTransit:getRunningScene())
	CC_SYNTHESIZE(self,		"deskStatusMgr", 	self.scene.deskStatusMgr)
	return true
end

function LCMJBasePanel:setPositionX(x)
	self.layer:setPositionX(x)
end

function LCMJBasePanel:getPositionX()
	return self.layer:getPositionX()
end

function LCMJBasePanel:setPositionY(y)
	self.layer:setPositionY(y)
end

function LCMJBasePanel:getPositionY()
	return self.layer:getPositionY()
end

function LCMJBasePanel:setPosition(...)
	self.layer:setPosition(...)
end

function LCMJBasePanel:getPosition()
	return self.layer:getPosition()
end

function LCMJBasePanel:getContentSize()
	return self.layer:getContentSize()
end

function LCMJBasePanel:getWidth()
	return self.layer:getContentSize().width
end

function LCMJBasePanel:getHeight()
	return self.layer:getContentSize().height
end

function LCMJBasePanel:setContentSize( ... )
	self.layer:setContentSize(...)
end

function LCMJBasePanel:addChild(child)
	self.layer:addChild(child)
end

function LCMJBasePanel:removeAllChildrenWithCleanup(...)
	self.layer:removeAllChildrenWithCleanup(...)
end

return LCMJBasePanel
