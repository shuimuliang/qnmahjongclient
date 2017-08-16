local LCMJCardImgBase        = require("lua/scene/mj/MJCardBase")

local SUIT_INDEX = {1,2,2,3,3,4,4,5,5,6,2,2,3,3,4,4,5,5,2,3,3,4,4,5,5,6,6,6,6,6}
local SUIT_INDEX_TOP = {6,5,5,4,4,3,3,2,2,1,5,5,4,4,3,3,2,2,5,4,4,3,3,2,2,6,6,6,6,6}

	
local LCMJCardImg = class( "LCMJCardImg", function() return CCSprite:create() end)


--[==========[麻将图片，仅仅用于展示]==========]
--[===[data  	: 麻将数据]===]
--[===[pType 	: 座位类型(eMJPType)]===]
--[===[imgType 	: 图片类型(eMJImgType)]===]
--[===[index 	: 在已出牌库中的位置，用于展示透视]===]
function LCMJCardImg:ctor(data, pType, imgType, index)

	CC_SYNTHESIZE(self,"data",      data)
	CC_SYNTHESIZE(self,"pType",     pType)
	CC_SYNTHESIZE(self,"imgType",   imgType)
	CC_SYNTHESIZE(self,"index",     index or 0)

	self:initBackImg()
	self:initFontImg()
end

function LCMJCardImg:getTopDiscardBG()
	local nameIndex = 0
	local hunName = ""
	if self:isHunCard() then hunName = "_hun" end

	if self.index <= 10 and self.index >= 1 then
		nameIndex = 11 - self.index
	elseif self.index >= 11 and self.index <= 18 then
		nameIndex = 9-(self.index-11)
	elseif self.index >= 19 then
		nameIndex = 8-(self.index-19)
	end
	local bgName = "mj_small_bg_"..nameIndex.."_t"..hunName..".png"
	return bgName
end

function LCMJCardImg:getBottomDiscardBG()
	local bgName = eResImage.s_buttontransparent
	local hunName = ""
	if self:isHunCard() then hunName = "_hun" end

	if self.index <= 10 and self.index >= 1 then
		bgName = "mj_small_bg_"..self.index.."_t"..hunName..".png"
	elseif self.index >= 11 and self.index <= 18 then
		bgName = "mj_small_bg_"..(self.index - 9).."_t"..hunName..".png"	
	elseif self.index >= 19 then
		bgName = "mj_small_bg_"..(self.index - 16).."_t"..hunName..".png"	
	end
	return bgName
end

function LCMJCardImg:getStandBackImg()
	local bgName = eResImage.s_buttontransparent
	if self.pType == eMJPType.left then
		bgName = "mj_l.png"
	elseif self.pType == eMJPType.top then
		bgName = "mj_d.png"
	elseif self.pType == eMJPType.right then
		bgName = "mj_r.png"
	end
	return bgName
end

function LCMJCardImg:getDiscardBackImg()
	local bgName = eResImage.s_buttontransparent
	local hunName = ""
	if self:isHunCard() then hunName = "_hun" end

	if self.pType == eMJPType.bottom then
		bgName = self:getBottomDiscardBG()
	elseif self.pType == eMJPType.top then
		bgName = self:getTopDiscardBG()
	elseif self.pType == eMJPType.left then
		bgName = "mj_small_bg_l"..hunName..".png" 
	elseif self.pType == eMJPType.right then
		bgName = "mj_small_bg_r"..hunName..".png"
	end
	return bgName
end

function LCMJCardImg:getPairBackImg()
	local bgName = eResImage.s_buttontransparent	
	local hunName = ""
	if self:isHunCard() then hunName = "_hun" end

	if self.pType == eMJPType.bottom or self.pType == eMJPType.top then
		bgName = "mj_bg_t"..hunName..".png"
	elseif self.pType == eMJPType.left then
		bgName = "mj_small_bg_l"..hunName..".png"
	elseif self.pType == eMJPType.right then
		bgName = "mj_small_bg_r"..hunName..".png"
	end
	return bgName
end

function LCMJCardImg:getReverseBackImg()
	local bgName = eResImage.s_buttontransparent	
	if self.pType == eMJPType.bottom or self.pType == eMJPType.top then
		bgName = "mj_bg_t_k.png"
	elseif self.pType == eMJPType.left then
		bgName = "mj_small_bg_l_b.png"
	elseif self.pType == eMJPType.right then
		bgName = "mj_small_bg_r_b.png"
	end
	return bgName
end

function LCMJCardImg:isHunCard()
    local scene = LCSceneTransit:getRunningScene()
	local deskStatusMgr = scene:getDeskStatusMgr()
	if not deskStatusMgr then return false end

	local hunNum = deskStatusMgr:getHunCard()
	return hunNum == self.data.num
end

function LCMJCardImg:initBackImg()
	local bgName = eResImage.s_buttontransparent

	local switch = {
	[eMJImgType.stand] = function()
				bgName = self:getStandBackImg()
	end,
  [eMJImgType.discard] = function()
				bgName = self:getDiscardBackImg()
	end,
	 [eMJImgType.pair] = function()
				bgName = self:getPairBackImg()
	end,
     [eMJImgType.back] = function()
			    bgName = self:getReverseBackImg()
	end, 
	}

	local f = switch[self.imgType] 
	if f then f() end
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(bgName)
	self:setDisplayFrame(frame)	
end

function LCMJCardImg:getBottomDiscardFontSuit()
	local index = SUIT_INDEX[self.index]
	local prefix,num = LCMJUtil.getImgPrefixAndNumByNum(self.data.num)

	local suitName = eResImage.s_buttontransparent	
	suitName = "mj_small_"..prefix..num.."_t_"..index..".png"
	return suitName
end

function LCMJCardImg:getTopDiscardFontSuit()
	local index = SUIT_INDEX_TOP[self.index]
	local prefix,num = LCMJUtil.getImgPrefixAndNumByNum(self.data.num)

	local suitName = eResImage.s_buttontransparent	
	suitName = "mj_small_"..prefix..num.."_t_"..index..".png"
	return suitName
end

function LCMJCardImg:getDiscardFontInfo()
	local suitName = eResImage.s_buttontransparent
	local xOffRate = 1
	local yOffRate = 1

		if self.pType == eMJPType.bottom then
			suitName = self:getBottomDiscardFontSuit()
			xOffRate = 0.5
			yOffRate = 0.65
		elseif self.pType == eMJPType.left then
			suitName = self.data.imgL
			xOffRate = 0.5
			yOffRate = 0.7		
		elseif self.pType == eMJPType.right then
			suitName = self.data.imgR
			xOffRate = 0.5
			yOffRate = 0.75
		elseif self.pType == eMJPType.top then
			suitName = self:getTopDiscardFontSuit()
			xOffRate = 0.5
			yOffRate = 0.65	
		end

	return suitName,xOffRate,yOffRate
end

function LCMJCardImg:initFontImg()
	local suitName = eResImage.s_buttontransparent
	local xOffRate = 1
	local yOffRate = 1

	if self.imgType == eMJImgType.discard then
		suitName,xOffRate,yOffRate = self:getDiscardFontInfo()
			
	elseif self.imgType == eMJImgType.pair then
		if self.pType == eMJPType.bottom or self.pType == eMJPType.top then
			suitName = self.data.imgT
			xOffRate = 0.5
			yOffRate = 0.65
		elseif self.pType == eMJPType.left then
			suitName = self.data.imgL
			xOffRate = 0.5
			yOffRate = 0.7		
		elseif self.pType == eMJPType.right then
			suitName = self.data.imgR
			xOffRate = 0.5
			yOffRate = 0.75
		end
	end

	local suit = CCSprite:createWithSpriteFrameName(suitName)

	self:addChild(suit)
	self.fontImg = suit
	self:setCascadeOpacityEnabled(true)
	suit:setPositionX(self:getContentSize().width * xOffRate)
	suit:setPositionY(self:getContentSize().height * yOffRate)
end

return LCMJCardImg
