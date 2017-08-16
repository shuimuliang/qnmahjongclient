local RES_FILE_NAME = require "lua/model/ResFile"
local TEXTURE_NAME = require "lua/model/ResTexture"

local ScriptOverride = {}

local runCount = 0

function ScriptOverride.getPath(file)
	if not file then return end
	local index    = TEXTURE_NAME[file]
	local fileName = RES_FILE_NAME[index]
	local imgName  = RES_FILE_NAME[index+1]
	-- GDebug(fileName ..", ".. imgName)
	if not fileName or not imgName then return end
	local start_idx, end_idx = string.find(fileName, ".plist")
	if not start_idx or not end_idx then return end
	return fileName, imgName
end

function ScriptOverride.start()
    runCount = runCount + 1
    if runCount > 1 then return end -- ensure run only once

    local CCSpriteFrameCachespriteFrameByName = CCSpriteFrameCache.spriteFrameByName
    function CCSpriteFrameCache:spriteFrameByName(file)
        if isset(TEXTURE_NAME, file) then
            local fileName, imgName = ScriptOverride.getPath(file)
			if not fileName or not imgName then return CCSpriteFrameCachespriteFrameByName(self, file) end
            CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(fileName, imgName)
            return CCSpriteFrameCachespriteFrameByName(self, file)
        else
            return CCSpriteFrameCachespriteFrameByName(self, file)
        end
    end

    local CCSpritecreateWithSpriteFrameName = CCSprite.createWithSpriteFrameName
    function CCSprite:createWithSpriteFrameName(file, ...)
        if table.keyof(RES_FILE_NAME, file) then
            return CCSpritecreateWithSpriteFrameName(self, file, ...)
        elseif isset(TEXTURE_NAME, file) then
            local fileName, imgName = ScriptOverride.getPath(file)
			if not fileName or not imgName then return CCSpritecreateWithSpriteFrameName(self, file, ...) end
            CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(fileName, imgName)
            return CCSpritecreateWithSpriteFrameName(self, file, ...)
        else
            return CCSpritecreateWithSpriteFrameName(self, file, ...)
        end
    end

    local CCScale9SpritecreateWithSpriteFrameName = CCScale9Sprite.createWithSpriteFrameName
    function CCScale9Sprite:createWithSpriteFrameName(file, ...)
        if table.keyof(RES_FILE_NAME, file) then
            return CCScale9SpritecreateWithSpriteFrameName(self, file, ...)
        elseif isset(TEXTURE_NAME, file) then
            local fileName, imgName = ScriptOverride.getPath(file)
			if not fileName or not imgName then return CCScale9SpritecreateWithSpriteFrameName(self, file, ...) end
            CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(fileName, imgName)
            return CCScale9SpritecreateWithSpriteFrameName(self, file, ...)
        else
            return CCScale9SpritecreateWithSpriteFrameName(self, file, ...)
        end
    end
end

return ScriptOverride
