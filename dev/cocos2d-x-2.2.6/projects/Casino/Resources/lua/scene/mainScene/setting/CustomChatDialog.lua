local LCTableView = require("lua/scene/sceneUtils/TableView")
local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local LCCustomLineCell, LCCustomCell

-- constants
local CELL_HEIGHT = 73

local LCCusTomChatDialog = class("LCCusTomChatDialog", function()
    local proxy = CCBProxy:create()
    local layer
    local t = {
        ccb_onBackBtnClicked = function(_, ...)
            layer:ccb_onBackBtnClicked(...)
        end
    }
    local node  = CCBuilderReaderLoad(eResCCBI.customChatLayer, proxy, t)
    layer = tolua.cast(node, "CCLayer")
    table.merge(layer, t)
    return layer
end)

-- events
LCCusTomChatDialog.EVENT_BACK = "EVENT_BACK"

function LCCusTomChatDialog:ctor(scene)
	CC_SYNTHESIZE(self, "customLen", 6)
	EventProtocol.extend(self)

	local nodeMap = {
		{"customLayer","ccb_customLayer","CCLayer"},
		{"oldLayer","ccb_oldLayer","CCLayer"},
		{"customTitle","ccb_customTitle","CCLabelTTF"},
		{"oldTitle","ccb_oldTitle","CCLabelTTF"},
	}
	CE_SYNTHESIZE_CCB_MAP(self, self, nodeMap)

	self.customTitle:setString(ADDCOLON(_("自定义游戏对话")))
	self.oldTitle:setString(ADDCOLON(_("默认游戏对话")))

	self:customInit()

	self:oldInit()
end

function LCCusTomChatDialog:dataInit(inList, len)
	local startIndex=0
	local cells={}
	local outList = {}
	for i=1, #inList do
		local index = math.ceil(i/len)
		if index~=startIndex then
			if startIndex~=0 and #cells>0 then
				table.insert(outList, cells)
			end
			startIndex = index
			cells={}
		end
		table.insert(cells, inList[i])
		if index==startIndex and i==#inList then
		    if #cells>0 then
				table.insert(outList, cells)
			end
		end
	end
	return outList
end

function LCCusTomChatDialog:customInit()
	local size=self.customLayer:getContentSize()

	self.customChatList={}
	local list=LCAppGlobal:getGameData():getGameChatMgr():getCustomList() or {}
	for i=1, self.customLen do
		local text = ""
		local id   = #eResImage.s_chatTalk+i
		if #list>=i and list[i] then
			text=list[i].text
		end
		local data={id=id, text=text, default=_("输入你想说的话")}
		table.insert(self.customChatList, data)
	end
	self.oldCustomList=clone(self.customChatList)
	self.customChatList=self:dataInit(self.customChatList, 2)

	if not self.customScrollView then
		self.customScrollView =  LCTableView:create(size,{
			cellHeight = function(index)
				return CELL_HEIGHT
			end,
			cellAtIndex = function (index, reusedCell)
				local cell = reusedCell
				local i=#self.customChatList-index
				if i>#self.customChatList or i<=0 then return cell end
				local data=self.customChatList[i]
				if not cell then  cell = LCCustomLineCell.new(self, {datas=data, color=ccc3(78, 52, 91), middle = false, callback=function (data)
					if self.dataCallback then self:dataCallback(data) end
				end}) end
				cell:configWithLineData(data)
				return  cell
			end,
			numberOfCells = function() if self.customChatList then return #self.customChatList else return 0 end end,
			cellTouched = function(cell)   end
		})
		self.customLayer:addChild(self.customScrollView)
        self.customScrollView:setTouchEnabled(false)
	else
		if self.customScrollView then self.customScrollView:reloadData(true) end
	end
end

function LCCusTomChatDialog:dataCallback(data)
	if not data then return end
	local list={}
	for _,v in pairs(self.oldCustomList) do
		if v.id==data.id then
			v.text=data.text
		end
		if v.text and #v.text>0 then
			table.insert(list, clone(v))
		end
	end
	table.sort(list, function(a, b) return a.id<b.id end)
	LCAppGlobal:getGameData():getGameChatMgr():updateCustomList(list)
end

function LCCusTomChatDialog:oldInit()
	local size=self.oldLayer:getContentSize()

	self.oldChatList=clone(eResImage.s_chatTalk)
	self.oldChatList=self:dataInit(self.oldChatList, 2)

	if not self.oldScrollView then
		self.oldScrollView =  LCTableView:create(size,{
			cellHeight = function(index)
				return CELL_HEIGHT
			end,
			cellAtIndex = function (index, reusedCell)
				local cell = reusedCell
				local i=#self.oldChatList-index
				if i>#self.oldChatList or i<=0 then return cell end
				local data=self.oldChatList[i]
				if not cell then  cell = LCCustomLineCell.new(self, {datas=data, middle=true, callback=function (data)
					--if self.dataCallback then self:dataCallback(data) end
				end}) end
				cell:configWithLineData(data)
				return  cell
			end,
			numberOfCells = function() if self.oldChatList then return #self.oldChatList else return 0 end end,
			cellTouched = function(cell)   end
		})
		self.oldLayer:addChild(self.oldScrollView)
	else
		if self.oldScrollView then self.oldScrollView:reloadData(true) end
	end
end

function LCCusTomChatDialog:ccb_onBackBtnClicked(control, controlEvent)
    if controlEvent == CCControlEventTouchUpInside then
        self:dispatchEvent{name = LCCusTomChatDialog.EVENT_BACK}
    end
end

----------------------- LCCustomLineCell -----------------------

LCCustomLineCell = class("LCCustomLineCell", function() return CCLayer:create() end)

function LCCustomLineCell:ctor(scene, key)
	local key=key or {}
	CC_SYNTHESIZE(self,"datas", key.datas) -- register when communication on
	CC_SYNTHESIZE(self,"viewScene", scene)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"winWidth", 1136)
	CC_SYNTHESIZE(self,"len", key.len or 2)
	CC_SYNTHESIZE(self,"cells", {})
	CC_SYNTHESIZE(self,"color", key.color or ccc3(255, 255, 255))
	CC_SYNTHESIZE(self,"middle", key.middle)

    local bg = CCSprite:createWithSpriteFrameName(eResImage.ranking_background_01)
    self:addChild(bg)
    bg:setAnchorPoint(0, 0)

	for i=1, self.len do

		local cell = LCCustomCell.new(self, {data=self.datas[i], middle=self.middle,
			color=self.color, callback=function (data)
			if self.callback then self.callback(data) end
		end})
        local cellSize = cell:getSize()
        local cellWidth, cellHeight = cellSize.width + 3, cellSize.height
        local sizeSpace = (self.winWidth - cellWidth * 2) / 2
        cell:setPosition(sizeSpace + cellWidth * (i - 1), (CELL_HEIGHT - cellHeight) / 2)
		self:addChild(cell)

		self.cells[i]=cell
	end

	-- return true
end

function LCCustomLineCell:configWithLineData(datas)
	if not datas then return end
	self.datas = datas

	for i=1, self.len do
		self.cells[i]:configWithData(self.datas[i])
	end
end


----------------------- LCCustomCell -----------------------

LCCustomCell = class("LCCustomCell", function() return CCLayer:create() end)

function LCCustomCell:ctor(scene, key)
	if not key then return false end
	CC_SYNTHESIZE(self,"data", key.data)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"size", nil)
    self.color = key.color or ccc3(255, 255, 255)
    self.middle = key.middle
    self.textMaxLength = 30

    local bgFrameName = self.middle and eResImage.setting_zidingyi2 or eResImage.setting_zidingyi1
    local bg = CCSprite:createWithSpriteFrameName(bgFrameName)
    self.size = bg:getContentSize()
	self.menuPopupItem = CCMenuItemSprite:create(bg, bg, bg)
	self.menuPopupItem:ignoreAnchorPointForPosition(false)
	self.menuPopupItem:setAnchorPoint(0, 0)
	self.menuPopupItem:registerScriptTapHandler(function ()
		if self.callback then self.callback(self.data) end
	end)
	local menuPopup = CCMenu:createWithItem(self.menuPopupItem)
	menuPopup:setPosition(ccp(0, 0))
	self:addChild(menuPopup)

	if self.middle then
		self.textLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 22)
        self.textLabel:setAnchorPoint(0, 0.5)
        self.textLabel:setPosition(10, self.size.height/2)
		self.textLabel:setColor(self.color)
        self.textLabel:setDimensions(CCSizeMake(self.size.width - 20, 0))
        self.textLabel:setHorizontalAlignment(kCCTextAlignmentLeft)
		self:addChild(self.textLabel)

		self:updateString()
	else
		self:editUpdate()

		self.sprite = CCSprite:createWithSpriteFrameName(eResImage.setting_addbutton)
		local content=self.sprite:getContentSize()
		self.sprite:setPosition(self.size.width-content.width/2-5, self.size.height/2)
		self:addChild(self.sprite)
	end

    self:setContentSize(self.size)
end

function LCCustomCell:editUpdate()
	if self.edit == nil then
		local function editBoxTextEventHandle(strEventName,pSender)
			local edit = tolua.cast(pSender,"CCEditBox")
			local text = edit:getText() or ""
			if strEventName == "began" then
				self.cancle=false
				-- changeEditBG(false)
			elseif strEventName == "return" then
				self.data.text=text
				if self.callback then self.callback(self.data) end
				GDebug("------ended")
			elseif strEventName == "ended" then
				self.data.text=text
				if self.data.text and #self.data.text>0 then
					-- changeEditBG(true)
				else
					-- changeEditBG(false)
				end
				if self.callback then self.callback(self.data) end
			elseif strEventName == "changed" then
				if text and CE_GET_STRING_SHOW_LENGTH(text) > self.textMaxLength then
					edit:setText(CE_STRING_CUT_OFF(text,self.textMaxLength))
				end
				if not text or #text<=0 then
					self.sprite:setVisible(true)
				else
					self.sprite:setVisible(false)
				end
			end
		end
		local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
		local layerSize = self.size
		scale9bg:setContentSize(layerSize)
		self.edit = CCEditBox:create(layerSize, scale9bg)
        local e = self.edit
		e:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, 22)
		e:setFont(DEF_SYSTEM_FONT_NAME, 22)
		e:setFontColor(ccc3(255, 255, 255))
	    e:setMaxLength(self.textMaxLength)
		e:setReturnType(kKeyboardReturnTypeDone)
		e:setAnchorPoint(0, 0.5)
		e:setContentSize(layerSize)
		e:setPosition(0, layerSize.height*0.5)
		e:registerScriptEditBoxHandler(editBoxTextEventHandle)
		self:addChild(e)
	end

	self.edit:setPlaceHolder(self.data.default)
	if self.data.text and #self.data.text>0 then
		text=self.data.text
		self.edit:setText(text)
		-- changeEditBG(true)
		if self.sprite then self.sprite:setVisible(false) end
	end

end


function LCCustomCell:configWithData( data )

	self.data=data

	if self.middle then
		self:updateString()
	else
		if not data then return end
		self:editUpdate()
	end

end

function LCCustomCell:updateString()
	local text
	local data=self.data
	if data and data.text and #data.text>0 then
		text=data.text
	end
	if self.textLabel then
		if text then
			self.textLabel:setString(text)
			self.textLabel:setVisible(true)
		else
			self.textLabel:setVisible(false)
		end
	end
end

return LCCusTomChatDialog
