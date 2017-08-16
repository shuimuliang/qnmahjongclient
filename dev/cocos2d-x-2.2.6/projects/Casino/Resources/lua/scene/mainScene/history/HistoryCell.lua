local HistoryCell = class("HistoryCell", function() return CCNode:create() end)

function HistoryCell:ctor()
    -- 这个不能放在 onEnter 里，因为 Dialog 的 tableview 先调用 update 再 addChild 会找不到 ccb_xxx 变量
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("historyCell", proxy, self)
    self:addChild(layer)
end

function HistoryCell.height()
    return 100
end

function HistoryCell:update(titles, names, score, ownerIdx)
    for i, name in ipairs(names) do
        local len = string.len(name)
        if len > 42 then   --如果是汉字，大于14个字时，字号减少一些
            self["ccb_name" .. i]:setFontSize(16)
        else
            self["ccb_name" .. i]:setFontSize(18)
        end
        self["ccb_name" .. i]:enableStroke(ccc3(255, 255, 255), 1, true)
        self["ccb_name" .. i]:setString(name)
        self["ccb_name" .. i]:setVisible(true)
    end
    for i = #names + 1, 4 do
        self["ccb_name" .. i]:setVisible(false)
    end
    for i, s in ipairs(score) do
        self["ccb_score" .. i]:setVisible(true)
        if tonumber(s) >= 0 then
            self["ccb_score" .. i]:setColor(ccc3(67, 24, 0))
            self["ccb_score" .. i]:setString("+" .. s)
        else
            self["ccb_score" .. i]:setColor(ccc3(151, 27, 32))
            self["ccb_score" .. i]:setString(s)
        end
    end
    for i = #score + 1, 4 do
        self["ccb_score" .. i]:setVisible(false)
    end
    self.ccb_ownerFlag:setPositionX(self["ccb_name"..ownerIdx]:getPositionX() - 20)
    for i, title in ipairs(titles) do
        self["ccb_title" .. i]:setString(title)
    end
end

return HistoryCell
