local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local URLSprite = require "lua/scene/commons/URLSprite"

local PlayerInfoDialog = class("PlayerInfoDialog", DialogDIY)

local _getName, _getDistance

function PlayerInfoDialog:ctor(key)
	key = key or {}
	PlayerInfoDialog.super.ctor(self, key)

	self._id       			= key.id or 0
	self._avatar   			= key.avatar or 0
	self._nickname 			= key.nickname or ""
	self._ip       			= key.ip or ""
	self._distanceInfoList 	= key.distanceInfoList
	-- or
	-- {
	-- 	{name = "aaaaaaaabbbb", distance = 1},
	-- 	{name = "aaaaaaaabbbbc", distance = 2000},
	-- 	{name = "呜呜呜呜的的的a", distance = 3},
	-- }
end

function PlayerInfoDialog:initContentView()
	local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("playerInfo_170", proxy, self)
    self:addChild(layer)

	self.ccb_nickname:setString(self._nickname)
	self.ccb_nickname:enableStroke(ccc3(255, 255, 255), 1, true)

	self.ccb_id:setString("ID:" .. self._id)
	self.ccb_ip:setString("IP:" .. self._ip)

	local s = self.ccb_avatarBg:getContentSize()
    local avatar = URLSprite.new(self._avatar, s.width, s.height)
    self.ccb_avatarBg:addChild(avatar)
    avatar:setPosition(s.width/2, s.height/2)

	if self._distanceInfoList then
		self.ccb_line:setVisible(true)
		for i = 1, 3 do
			local info = self._distanceInfoList[i]
			if info then
				self["ccb_playerName"..i]:setVisible(true)
				self["ccb_playerName"..i]:setString("与【" .. _getName(info.name) .. "】")
				self["ccb_playerDistance"..i]:setVisible(true)
				self["ccb_playerDistance"..i]:setString("距离 " .. _getDistance(info.distance) .. " 米")
			else
				self["ccb_playerName"..i]:setVisible(false)
				self["ccb_playerDistance"..i]:setVisible(false)
			end
		end
		self.ccb_avatarNode:setPositionX(196)
		self.ccb_nameBg:setPositionX(196)
		self.ccb_id:setPositionX(196)
		self.ccb_ip:setPositionX(196)
	end

	return layer
end

function PlayerInfoDialog:ccb_onClose()
	self:dismissSelf()
end

-- private
_getName = function(name)
	if CE_GET_STRING_SHOW_LENGTH(name) > 14 then
		return CE_STRING_CUT_OFF(name, 12) .. ".."
	end
	return name
end

_getDistance = function(distance)
	if tonumber(distance) > 999 then
		return ">999"
	end
	return distance
end

return PlayerInfoDialog
