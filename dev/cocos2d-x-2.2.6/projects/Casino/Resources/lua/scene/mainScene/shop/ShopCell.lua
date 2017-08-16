local URLSprite = require "lua/scene/commons/URLSprite"

local ShopCell = class("ShopCell", function() return CCNode:create() end)

function ShopCell:ctor()
    -- 这个不能放在 onEnter 里，因为 Dialog 的 tableview 先调用 update 再 addChild 会找不到 ccb_xxx 变量
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("shopCell", proxy, self)
    self:addChild(layer)

    self._urlSprite = URLSprite.new()
    self.ccb_item:addChild(self._urlSprite)

    local s = layer:getContentSize()
    layer:setPositionX((ShopCell.width() - s.width)/2)
end

function ShopCell.height()
    return 319
end

function ShopCell.width()
    return 214
end

function ShopCell:update(data)
    local id      = data.gemID
    local count   = data.gemCount
    local extra   = data.extra
    local price   = data.money
    local iconUrl = data.info.iconID

    if extra > 0 then
        self.ccb_extra:setString("送" .. extra .. "枚")
        self.ccb_sale:setVisible(true)
    else
        self.ccb_sale:setVisible(false)
    end
    self.ccb_amount:setString("x" .. count)
    self.ccb_price:setString("￥" .. price)

    self._urlSprite:update(iconUrl)
end

return ShopCell
