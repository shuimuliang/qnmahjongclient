local GameCell = class("GameCell", function() return CCNode:create() end)

local select_color           = ccc3(255, 218, 106)
local unselect_color         = ccc3(91, 44, 4)
local select_img             = "mj_interface_btn_bg_check.png"
local unselect_img           = "mj_interface_btn_bg_uncheck.png"
local sharedSpriteFrameCache = CCSpriteFrameCache:sharedSpriteFrameCache()

function GameCell:ctor()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("GameCell", proxy, self)
    self:addChild(layer)

end

function GameCell.height()
    return 100
end

function GameCell.width()
    return 273
end

function GameCell:setName(name)
    self.ccb_name:setString(name or "")
end

function GameCell:setSelected(select)
    if select then
        self.ccb_name:setColor(select_color)
        self.ccb_bg:setDisplayFrame(sharedSpriteFrameCache:spriteFrameByName(select_img))
    else
        self.ccb_name:setColor(unselect_color)
        self.ccb_bg:setDisplayFrame(sharedSpriteFrameCache:spriteFrameByName(unselect_img))
    end
end

return GameCell
