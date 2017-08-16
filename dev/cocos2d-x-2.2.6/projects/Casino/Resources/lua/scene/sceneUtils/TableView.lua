local TableViewCell = require("lua/scene/sceneUtils/TableViewCell")

-- ======================= LCTableView ======================= --
local LCTableView = class("LCTableView",function() return CCLayerColor:create() end)
function LCTableView:create(...)
	local ret = LCTableView.new()
	if not ret:init(...) then return  nil end
	return ret
end
function LCTableView:init(size,delegate,unreuse,swallow)
	if not size then GError("need size , tableView create failed") return end
	CC_SYNTHESIZE(self,"tableView",nil)
	CC_SYNTHESIZE(self,"cellTag",1234)
	CC_SYNTHESIZE(self,"unreuse",unreuse)  -- don't reuse cell

	-- event
	local function onEvent(event)
		if event == "enter" then self:onEnter()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)
    self:changeWidthAndHeight(size.width,size.height)

    if swallow then
    	local maskLayer = CCLayer:create()
    	maskLayer:setContentSize(size)
    	self:addChild(maskLayer)
    	local function onTouch(eventType,x,y)
    		local glPos = self:convertToNodeSpace( ccp( x, y ) )
    		local bBox = maskLayer:boundingBox()
    		if bBox:containsPoint(glPos) then
    	  		return true
    		end
    		return false
  		end
  		maskLayer:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
  		maskLayer:setTouchEnabled(true)  
  	end


-- @param fn string Callback type
-- @param table LuaTableView
-- @param a1 & a2 mixed Difference means for every "fn"
	local h = LuaEventHandler:create(function(fn, table, a1, a2)
		local r
		if fn == "cellSize" then
			-- Return cell size
			-- a1 is cell index
			if not delegate.cellHeight then GError("no cell height delegate") return end
			local width = size.width
			if delegate.cellWidth then width = delegate.cellWidth(a1) end
			r = CCSizeMake(width,delegate.cellHeight(a1))
		elseif fn == "cellAtIndex" then
			-- Return CCTableViewCell, a1 is cell index, a2 is dequeued cell (maybe nil)
			-- Do something to create cell and change the content
			if not delegate.cellAtIndex then GError("no cellAtIndex delegate") return end
			local reusedCell = nil
			if a2 and not unreuse then reusedCell = a2:getChildByTag(self.cellTag) end
			local cell = delegate.cellAtIndex(a1, reusedCell)
			if not a2 or unreuse then
				a2 = CCTableViewCell:create()
				cell:setTag(self.cellTag)
				a2:addChild(cell)
				-- Build cell struct, just like load ccbi or add sprite
			end
  			r = a2
		elseif fn == "numberOfCells" then
    		-- Return number of cells
    		if not delegate.numberOfCells then GError("no numberOfCells delegate") return end
    		r = delegate:numberOfCells()
		elseif fn == "cellTouched" then
    		-- A cell was touched, a1 is cell that be touched. This is not necessary.
    		if not delegate.cellTouched or not self.cellTag or self.cellTag<0 then GError("no cellTouched delegate") return end
    		delegate.cellTouched(a1:getChildByTag(self.cellTag),a2, a1)
		elseif fn == "scroll" then
			if delegate.scrolling then
				delegate.scrolling()
			end
		end
		return r
	end)
	self.tableView = LuaTableView:createWithHandler(h, size)
	self:addChild(self.tableView)
	return true
end

function LCTableView:isWillStop( ... )
	return self.tableView:isWillStop()
end

function LCTableView:setScrollDirection(direction)
	self.tableView:setDirection(direction)
	self:reloadData()
end

function LCTableView:reloadData(notScrollToTop)
	local curOffset = self.tableView:getContentOffset()
	local selfContentSize = self:getContentSize()
	local containerSize = self.tableView:getContainer():getContentSize()
	self.tableView:reloadData()
	if not notScrollToTop then return end

	local direction = self.tableView:getDirection()
	-- scroll to last position
	if curOffset.y <= 0 and direction == kCCScrollViewDirectionVertical then
		local isTop = (containerSize.height + curOffset.y) <= selfContentSize.height
		local newContainerSize = self.tableView:getContainer():getContentSize()
		local isCanScroll = selfContentSize.height <= newContainerSize.height + curOffset.y
		if (not isTop) and isCanScroll then
			self.tableView:setContentOffset(curOffset)
		end
	end
	if curOffset.x <= 0 and direction == kCCScrollViewDirectionHorizontal then
		local isLeft =  curOffset.x >= 0  -- (containerSize.width + curOffset.x) <= selfContentSize.width
		local newcontentOffset = self.tableView:getContentOffset() -- self.tableView:getContainer():getContentSize()
		local isCanScroll = newcontentOffset.x >= 0 -- selfContentSize.width <= newContainerSize.width + curOffset.x
		if (not isLeft) and isCanScroll then
			self.tableView:setContentOffset(curOffset)
		end
	end
end

function LCTableView:scrollToBottom(datalist,cellHeight)
	if not datalist or not cellHeight then return end
	local selfContentSize = self:getContentSize()
	local datacount = #datalist
	local realContentheight = datacount * cellHeight
	if realContentheight <= selfContentSize.height then
		self:setContentOffset(ccp(0,selfContentSize.height - realContentheight))
		return
	end
	self:setContentOffset(ccp(0,0))
end

function LCTableView:setContentOffsetInDuration(offset,animation)
	self.tableView:setContentOffsetInDuration(offset,animation)
end

function LCTableView:setContentOffset(offset)
	self.tableView:setContentOffset(offset)
end

function LCTableView:getContentOffset()
	return self.tableView:getContentOffset()
end

function LCTableView:updateCellAtIndex(index)
	self.tableView:updateCellAtIndex(index)
end

function LCTableView:cellAtIndex(index)
	return self.tableView:cellAtIndex(index)
end


function LCTableView:setEnabled(isEnabled)
	self.tableView:setVisible(isEnabled)
	self.tableView:setTouchEnabled(isEnabled)
end
function LCTableView:setTouchEnabled(isEnabled)
	self.tableView:setTouchEnabled(isEnabled)
end

function LCTableView:getContainer()
	return self.tableView:getContainer()
end

function LCTableView:addTo(parent, zOrder, tag)
    parent:addChild(self, zOrder or 0, tag or 0)
    return self
end

function LCTableView:pos(x, y)
    self:setPosition(x, y)
    return self
end

-- virtual
function LCTableView:onEnter()  end
function LCTableView:onExit() end

-- delegate


-- datasource


return LCTableView
