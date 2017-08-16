local LCHTMLLayer = require "lua/scene/sceneUtils/HTMLLayer"
local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local AboutView = class("AboutView", function()
    local proxy = CCBProxy:create()
    local layer
    local t = {
        ccb_onBackBtnClicked = function(_, ...)
            layer:ccb_onBackBtnClicked(...)
        end
    }
    local node  = CCBuilderReaderLoad(eResCCBI.AboutView, proxy, t)
    layer = tolua.cast(node, "CCLayer")
    table.merge(layer, t)
    return layer
end)

-- events
AboutView.EVENT_BACK = "EVENT_BACK"

function AboutView:ctor(key)
    local activityName  = key.activityName or "aboutActivity"
    local docName       = key.docName or eResDocs.about
    local title         = key.title or _("关于")

    EventProtocol.extend(self)
    local nodeMap = {
        {"headerLayer", "ccb_headerLayer", "CCLayer"},
        {"htmlLayer", "ccb_htmlLayer", "CCLayer"},
        {"titleTTF", "ccb_titleTTF", "CCLabelTTF"},
	}
	CE_SYNTHESIZE_CCB_MAP(self, self, nodeMap)

    -- local selfHeight = self:getContentSize().height
    -- local headerHeight = self.headerLayer:getContentSize().height
    -- local htmlWidth = self.htmlLayer:getContentSize().width
    local htmlSize = self.htmlLayer:getContentSize()
    local html = LCHTMLLayer:create({
        activityName = activityName,
        url = _APPHTML(docName),
        -- dimensions = CCSizeMake(htmlWidth, selfHeight - headerHeight)
        dimensions = htmlSize,
    })
    GDebug(htmlSize.width .. ".."..htmlSize.height)
    self.htmlLayer:addChild(html)
    -- self.htmlLayer:setContentSize(CCSizeMake(htmlWidth, selfHeight - headerHeight))

    self.titleTTF:setString(title)
end

function AboutView:ccb_onBackBtnClicked(control, controlEvent)
    if controlEvent == CCControlEventTouchUpInside then
        self:dispatchEvent{name = AboutView.EVENT_BACK}
    end
end

return AboutView
