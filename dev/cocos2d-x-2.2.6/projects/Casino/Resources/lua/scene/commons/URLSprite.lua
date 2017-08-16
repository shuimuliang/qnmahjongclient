local URLSprite = class("URLSprite", function() return CCSprite:createWithSpriteFrameName("load.png") end)

local _setTexture

function URLSprite:ctor(url, w, h)
    self._width = w or 0
    self._height = h or 0
    self:update(url, w, h)
end

function URLSprite:update(url, w, h)
    if not url then return end
    local fileName = string.gsub(url, "/", "")
	fileName = string.gsub(fileName, ":", "")

    self._width = w or self._width
    self._height = h or self._height

	local avatarPath = CCFileUtils:sharedFileUtils():getWritablePath() .. fileName ..".png"
	local file,err = io.open(avatarPath, "r+")
	if file ~= nil then
		_setTexture(self, CCTextureCache:sharedTextureCache():addImage(avatarPath))
		file:close()
	else
		local httpRequest = HttpRequestExt:create()
		httpRequest:sendGet(url, function(code, tag, strError, spr)
			if spr == nil then
				GDebug(strError)
			else
                if tolua.isnull(self) then return end
				local sprite = tolua.cast(spr, "CCSprite")
				_setTexture(self, sprite:getTexture())
			end
		end)
	end
end

-- private
_setTexture = function(self, texture)
    self:setTexture(texture)
	local textureSize = texture:getContentSize()
    self:setTextureRect(CCRect(0, 0, textureSize.width, textureSize.height))
    if self._width ~= 0 and self._height ~= 0 then
        local ws = self._width / textureSize.width
        local hs = self._height / textureSize.height
        self:setScale(math.min(ws, hs))
    end
end

return URLSprite
