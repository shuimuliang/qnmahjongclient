local LCMJArrowBase        = require("lua/scene/mj/MJCardBase")



local LCMJArrow = class( "LCMJArrow", function() return CCSprite:create() end)


function LCMJArrow:ctor(deskView)
	self.deskView = deskView

	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("mj_arrow.png")
	self:setDisplayFrame(frame)

	local array = CCArray:create()
	array:addObject(CCMoveBy:create(0.5, ccp(0, 10)))
	array:addObject(CCMoveBy:create(0.5, ccp(0, -10)))

    local sequence = CCSequence:create(array)
    local action = CCRepeatForever:create(sequence)
    self:runAction(action)
end


function LCMJArrow:setPosByImgAndPtype(img,pType)
	self:setVisible(true)
	local wordP = img:getParent():convertToWorldSpace(ccp(img:getPositionX(),img:getPositionY()))
	local nodeP = self.deskView.bgSprite:convertToNodeSpace(wordP)
	local offY = 50
	if pType == eMJPType.top then
		offY = 60
	elseif pType == eMJPType.bottom then
		offY = 65
	end
	self:setPosition(nodeP.x,nodeP.y + offY)
end

function LCMJArrow:onEnter()

end

return LCMJArrow
