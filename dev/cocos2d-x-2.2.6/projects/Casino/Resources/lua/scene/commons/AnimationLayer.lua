local LCRichLabel = require("lua/scene/sceneUtils/RichLabel")
local LCRollNumber  = require("lua/scene/sceneUtils/RollNumber")

local animateManger = class("animateManger", function(...) return CCLayerColor:create() end)--ccc4(255, 0, 0, 255)

function animateManger:create(...)
	local ret = animateManger.new()
	if not ret:init(...) then return nil end
	return ret
end

function animateManger:init(key)
	if not key then key={} end
	CC_SYNTHESIZE(self,"key", key)
	CC_SYNTHESIZE(self,"textLayer", {})
	--local winSize =  CCDirector:sharedDirector():getWinSize()
	self:changeWidthAndHeight(1136, 640)

	return true
end

function animateManger:getTextLayerByID(playerID)
	if not playerID then return end
	for k,v in pairs(self.textLayer) do
		if tostring(playerID)==tostring(v:getTag()) then
			return v
		end
	end
end

function animateManger:playGiftEffect(node, effName, callback, scale, res)
	if not node or not effName then return end
	local content=node:getContentSize()
	if not res then res=eResCommon.effect.gift end
	local effect = LCEffectObject:create(res)
	effect:setAutoPlay(false)
	effect:setAutoDelete(false)
	local nodeScale = node:getScale()
	if scale then effect:setScale(scale) end
	if effName == "play7" then
		effect:setPosition((content.width/2 + 20)*nodeScale,(content.height/2 - 29)*nodeScale)
	else
		effect:setPosition(content.width/2*nodeScale,content.height/2*nodeScale)
	end
	node:addChild(effect, 2)

	effect:play(effName, function (...)
		effect:removeFromParentAndCleanup(true)
		if callback then callback() end
	end)
end

function animateManger:spriteFly(imageFile, fromPos, toPos, callback, scale, notScaleEff,arriveFunc)
	local sprite=CCSprite:createWithSpriteFrameName(imageFile)
	sprite:setPosition(fromPos)
	self:addChild(sprite)

	local array = CCArray:create()
	array:addObject(CCMoveTo:create(0.5, toPos))
	array:addObject(CCCallFunc:create(function ()
		if arriveFunc then arriveFunc() end
	end))

	if notScaleEff then
		--sprite:setScale(scale)
	else
		array:addObject(CCDelayTime:create(0.2))
		array:addObject(CCMoveBy:create(0.01, ccp(4, 4)))
		array:addObject(CCScaleTo:create(0.2, scale or 1.0))
		array:addObject(CCDelayTime:create(1))
	end
	array:addObject(CCCallFunc:create(function ()
		sprite:removeFromParentAndCleanup(true)
		if callback then callback() end
	end))
	sprite:runAction(CCSequence:create(array))
end

function animateManger:talkIconShow(pos, msg, callback)
	local sprite=CCSprite:createWithSpriteFrameName(msg.text)
	sprite:setPosition(pos)
	sprite:setScale(0)
	self:addChild(sprite)

	local array = CCArray:create()
	array:addObject(CCCallFunc:create(function ()
		local list=eResImage.s_chatIcon[msg.types]
		--LCMusicBox:playSound(list[msg.messID].voice, false)
	end))
	array:addObject(CCScaleTo:create(0.5, 1.2))
	array:addObject(CCScaleTo:create(0.2, 1.0))
	array:addObject(CCDelayTime:create(1.3))
	array:addObject(CCCallFunc:create(function ()
		sprite:removeFromParentAndCleanup(true)
		if callback then callback() end
	end))
	sprite:runAction(CCSequence:create(array))
end

function animateManger:talkTextShow(pos, msg, direct, sex ,callback)
	local layer = self:getTextLayerByID(playerID)
	if not layer then
		layer=CCLayerColor:create()
		layer:changeWidthAndHeight(1136, 640)
		layer:setTag(msg.playerID)
		self:addChild(layer)
		table.insert(self.textLayer, layer)
	else
		layer:removeAllChildrenWithCleanup(true)
	end
	
	local len = string.len(msg.text)
	local dimensions = CCSizeMake(177, 0)
	if direct==1 then 
		pos.x=pos.x-dimensions.width
		if len <= 12 then pos.x = pos.x + 60 end
	end

	local sayText = CCLabelTTF:create(msg.text, DEF_SYSTEM_FONT_NAME, 19)
	sayText:setAnchorPoint(ccp(0, 1))
	sayText:setDimensions(dimensions)
	sayText:setPosition(pos)
	sayText:setColor(_APPCOLOR_HEX("3a1e49"))
	layer:addChild(sayText, 2)

	if len <= 12 then
		sayText:setDimensions(CCSizeMake(177/2,0))
	end
	local content = sayText:getContentSize()

	local bgName = eResImage.s_message_whitebox
	local bgContent=CCSizeMake(content.width+20, content.height+20)
	if content.width<dimensions.width/2 then bgContent.width=dimensions.width/2+20 end
	local pos_x = -10

	local sprite = CCSprite:createWithSpriteFrameName(bgName)
	if content.height<sprite:getContentSize().height then
		local dis=bgContent.height*0.1
		bgContent.height = bgContent.height-dis
		pos.y = pos.y-dis/2
	end
	local bg = CCScale9Sprite:createWithSpriteFrameName(bgName)
	bg:setAnchorPoint(0, 1)
	bg:setPreferredSize(bgContent)
	local bgPos = ccp(pos.x+pos_x, pos.y+10)
	bg:setPosition(bgPos)
	layer:addChild(bg, 1)

	local arrow = CCSprite:createWithSpriteFrameName(eResImage.s_message_whitebox_2)
	dis=bgContent.width*0.2
	if direct and direct==1 then
		arrow:setFlipX(true)
	else
		dis=-dis
	end
	arrow:setPosition(ccp(bgContent.width/2+dis, -9))
	bg:addChild(arrow, 1)

	if content.width<dimensions.width then
		sayText:setPositionX(pos.x+(bgContent.width-content.width)/2-10)
	end

	local array = CCArray:create()
	array:addObject(CCCallFunc:create(function ()
		if msg.types==eChatType.talk and msg.messID<=(#eResImage.s_chatTalk2) then
			if sex==ePlayerGender.male then
				LCMusicBox:playPeopleChatSound(eResImage.s_chatTalk2[msg.messID].voice, false)
			else
				LCMusicBox:playPeopleChatSound(eResImage.s_chatTalk[msg.messID].voice, false)
			end
		else
			LCMusicBox:playMessSounds()
		end
	end))
	array:addObject(CCDelayTime:create(5))
	array:addObject(CCCallFunc:create(function ()
		layer:removeAllChildrenWithCleanup(true)
		if callback then callback() end
	end))
	self:runAction(CCSequence:create(array))
end

return animateManger
