local LCEditLayer = require("lua/scene/commons/EditLayer")
local LCTableView = require("lua/scene/sceneUtils/TableView")
local LCTableViewCell = require("lua/scene/sceneUtils/TableViewCell")
local LCPlayerInfoDialog = require("lua/scene/mainScene/playerInfo/PlayerInfoDialog")
local LCPlayerAvatar = require "lua/scene/commons/PlayerAvatar"
local LCBroadCell = require("lua/scene/commons/BroadCell")

local LCBroadcastLayer190 = class("LCBroadcastLayer190", LCPageBase)

local size = CCDirector:sharedDirector():getWinSize()
local SINGLE_BROADCASR_OFFX = -5
local TABLE_BROADCASR_OFFX = 200
local AVATAR_OFFX = 6


function LCBroadcastLayer190:create(...)
    local ret = LCBroadcastLayer190.new()
    if not ret:init(...) then return nil end
    return ret
end

function LCBroadcastLayer190:init(scene, key)
    if not LCPageBase.init(self, scene, key) then
        return false
    end
    if not key then key = { } end
    CC_SYNTHESIZE(self, "scrollView", nil)
    CC_SYNTHESIZE(self, "contentScrollView", nil)
    CC_SYNTHESIZE(self, "bgLayer", nil)
    CC_SYNTHESIZE(self, "touchIn", false)
    CC_SYNTHESIZE(self, "contentLayer", nil)
    CC_SYNTHESIZE(self, "richLabe", { })
    CC_SYNTHESIZE(self, "textIndex", 1)
    CC_SYNTHESIZE(self, "index", 1)
    CC_SYNTHESIZE(self, "broadcastList", { })
    CC_SYNTHESIZE(self, "newBroList", { })
    CC_SYNTHESIZE(self, "size", key.size or CCSizeMake(674, 62))
    CC_SYNTHESIZE(self, "action", key.action)
    CC_SYNTHESIZE(self, "repeats", key.repeats)
    CC_SYNTHESIZE(self, "clicked", key.clicked)
    CC_SYNTHESIZE(self, "firstIn", key.firstIn)
    CC_SYNTHESIZE(self, "default", { types = eBroadcastType.noTitle, showTime = 3, text = _("欢迎来到街机电玩大师,一大波游戏等你来玩!") })
    CC_SYNTHESIZE(self, "start", false)
    CC_SYNTHESIZE(self, "broadcastLen", eBroadcastLen)
    CC_SYNTHESIZE(self, "posLast", ccp(0, 0))
    CC_SYNTHESIZE(self, "posCurrent", ccp(0, 0))
    CC_SYNTHESIZE(self, "hasMove", false)
    CC_SYNTHESIZE(self, "touchLayer", nil)
    CC_SYNTHESIZE(self, "editLayer", nil)
    CC_SYNTHESIZE(self, "talkCardLabel", nil)
    CC_SYNTHESIZE(self, "scolSize", CCSizeMake(self.size.width, self.size.height))
    CC_SYNTHESIZE(self, "menuPopup", nil)
    CC_SYNTHESIZE(self, "open", nil)
    CC_SYNTHESIZE(self, "curData", nil)
    CC_SYNTHESIZE(self, "_cell", { })
    CC_SYNTHESIZE(self, "broadHeightList", { })
    CC_SYNTHESIZE(self, "gameData", LCAppGlobal:getGameData())
    CC_SYNTHESIZE(self, "offX", key.offX or 274)
    CC_SYNTHESIZE(self, "bgSize", key.bgSize or CCSizeMake(1136, 37))

    self:updateData(self.clicked)

    self:changeWidthAndHeight(self.size.width, self.size.height)

    if self.repeats then
        self.action = false
    else
        self.repeats = false
    end

    if self.action == nil then
        self.action = false
    end

    local bgName = eResImage.s_buttontransparent
    if self.clicked then
        bgName = eResImage.s_buttontransparent
        if #self.broadcastList > self.broadcastLen then
            for i = #self.broadcastList, 1, -1 do
                if i > self.broadcastLen then table.remove(self.broadcastList, i) end
            end
        end
    end
    local rect = CCRectMake(10, 0, 10, 0)
    local bg = CCScale9Sprite:createWithSpriteFrameName(bgName, rect)

    bg:setPreferredSize(self.bgSize)

    -- bg:setAnchorPoint(ccp(0, 0))
    -- self:addChild(bg)
    local menuItemArray = CCArray:create()
    local menuPopupItem = CCMenuItemSprite:create(bg, bg, bg)
    menuPopupItem:ignoreAnchorPointForPosition(false)
    menuPopupItem:setAnchorPoint(ccp(0, 0))
    menuPopupItem:registerScriptTapHandler( function()
        if self.clicked and self.open == nil then
            if (self.contentLayer and not self.contentLayer:isVisible()) or not self.contentLayer then
                self:setTouch()
                self:contentVisible(true)
            else
                self:contentVisible(false)
            end
        else
            if self.contentScrollView then
                self.contentScrollView:setVisible(false)
            end
        end
        _ANLS_EVENT(3009, self.__cname)
    end )
    menuItemArray:addObject(menuPopupItem)
    self.menuPopup = CCMenu:createWithArray(menuItemArray)
    self.menuPopup:setPosition(ccp(0, 0))
    -- self:addChild(self.menuPopup, 1)--

    if self.clicked and self.gameData:getSwitchMgr():isTalkValue() then
        local scale9bg = CCScale9Sprite:createWithSpriteFrameName("lobby_button_horn_01.png")
        -- lobby_button_horn_number
        local spriteSize = CCSizeMake(45, 46)
        local completeBtn = CCControlButton:create(scale9bg)
        completeBtn:setPreferredSize(spriteSize)
        completeBtn:setAnchorPoint(ccp(0.5, 0.5))
        completeBtn:addHandleOfControlEvent( function()
            local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST)
            if not haveCount or haveCount <= 0 then
                LCMessageBox:showConfirm1( {
                    text = _("小喇叭数量不足！"),
                    buttonType = eButtonType.yellow,
                    textColor = _APPCOLOR_HEX("99FFFF"),
                    callback = function()
                        local LCStoreDialog = require("lua/scene/mainScene/shop/StoreDialog")
                        LCStoreDialog.new( { showCurrency = true }):showInScene()
                    end,
                } )
                return
            end
            self.editLayer = LCEditLayer:create(scene, {
                callback = function(...)
                    self.editLayer:removeFromParentAndCleanup(true)
                    self.editLayer = nil
                    self.scene:hideLoading()
                end
            } )
            local mainScene = LCSceneTransit:getRunningScene()
            mainScene:addChild(self.editLayer, 2)
        end , CCControlEventTouchUpInside)

        -- local offX = 274
        completeBtn:setPosition(ccp(self.scolSize.width + self.offX - spriteSize.width / 2 - 5, self.scolSize.height / 2))
        completeBtn:setZoomOnTouchDown(false)
        completeBtn:setScale(0.9)
        -- bg:addChild(completeBtn, 2)

        local talkCardLabel = CCLabelTTF:create("0", DEF_SYSTEM_FONT_NAME, 14)
        talkCardLabel:setAnchorPoint(ccp(1.0, 0.5))
        talkCardLabel:setColor(ccc3(255, 255, 255))
        talkCardLabel:setPosition(ccp(spriteSize.width - 4, 9))
        talkCardLabel:enableStroke(ccc3(255, 255, 255), 1, true)
        completeBtn:addChild(talkCardLabel,100)
        ----------------
        local talkCardLabel_background = CCScale9Sprite:createWithSpriteFrameName("lobby_button_horn_number.png")
        talkCardLabel_background:setAnchorPoint(ccp(1.0, 0.5))
        talkCardLabel_background:setContentSize(CCSizeMake(21, 20))
        talkCardLabel_background:setPosition(ccp(spriteSize.width + 2, 8))
        completeBtn:addChild(talkCardLabel_background)

        self.talkCardLabel = talkCardLabel
        self.talkCardLabel_background = talkCardLabel_background

        self:updateTalkCard()

        self.scolSize.width = self.scolSize.width
    end

    self.size.width = self.size.width - 20
    self.scolSize.width = self.scolSize.width - 20

    local viewLayer = CCLayerColor:create()
    viewLayer:setAnchorPoint(ccp(0, 0))
    viewLayer:changeWidthAndHeight(self.scolSize.width, self.scolSize.height)

    self.richLabe[1] = LCBroadCell:create( {
        boardData = self.default,
        cellSize = CCSizeMake(5000,eBroadCellSize190.height),
        gapLeft = 0,
        fontSize = 18,
        callback = function(data)
            if not data then return end
            self:textCal(data)
        end
    } )

    local content = self.richLabe[1]:getContentSize()
    self.posLast = ccp(SINGLE_BROADCASR_OFFX + 4, -7)
    self.richLabe[1]:setPosition(self.posLast)
    -- viewLayer:addChild(self.richLabe[1], 2)

    self.richLabe[2] = LCBroadCell:create( {
        boardData = self.default,
        cellSize = CCSizeMake(5000,eBroadCellSize190.height),
        gapLeft = 0,
        fontSize = 18,
        callback = function(data)
            if not data then return end
            self:textCal(data)
        end
    } )
    local content1 = self.richLabe[2]:getContentSize()
    self.posCurrent = ccp(SINGLE_BROADCASR_OFFX, self.scolSize.height)
    self.richLabe[2]:setPosition(self.posCurrent)
    -- viewLayer:addChild(self.richLabe[2], 2)
    -- viewLayer:setVisible(false)
    local scrollView = CCScrollView:create(CCSizeMake(self.scolSize.width - 25, self.scolSize.height), viewLayer)
    scrollView:setDirection(kCCScrollViewDirectionVertical)
    scrollView:setAnchorPoint(ccp(0.5, 0.5))
    scrollView:setPosition(ccp(5, 2))
    scrollView:setContentOffset(ccp(0, 0))
    scrollView:setClippingToBounds(true)
    scrollView:setTouchEnabled(false)
    self:addChild(scrollView, 1)
    scrollView:addChild(self.richLabe[1], 2)
    scrollView:addChild(self.richLabe[2], 2)

	self.scrollView = scrollView

    if not self.action then self:startAnimate() end

    return true
end

function LCBroadcastLayer190:updateTalkCard()
    if self.talkCardLabel then
        local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST)
        GDebug("-----haveCount="..haveCount)
        -- if haveCount>99 then
        -- 	haveCount="99+"
        -- 	self.talkCardLabel:setScale(0.75)
        -- else

        haveCount=""..haveCount
        self.talkCardLabel:setScale(1)
        -- end
        self.talkCardLabel:setString(haveCount)
        self.talkCardLabel_background:setContentSize(CCSizeMake(21 + 9 *(string.len(haveCount) -1), 20))
    end
end

function LCBroadcastLayer190:setTouch()
    if self.clicked then
        local intouch = false
        local function onTouch(eventType, x, y)

            if not y then y = x[2] x = x[1] end

            local layerPos = self:convertToNodeSpace(ccp(x, y))
            local bBox = nil

            if self.contentLayer then bBox = self.contentLayer:boundingBox() end

            local layerPos1 = nil
            local bBox1 = nil
            if self.scrollView then
                layerPos1 = self.scrollView:convertToNodeSpace(ccp(x, y))
                bBox1 = self.scrollView:boundingBox()
            end

            if eventType == "began" then
                if bBox and bBox:containsPoint(layerPos) then
                    GDebug("tounch in")
                    intouch = true
                else
                    intouch = false
                end
                return true
            elseif eventType == "ended" then
                -- GDebug("ended")
                if bBox and bBox:containsPoint(layerPos) then
                    -- GDebug("tounch in")
                elseif layerPos1 and bBox1 and bBox1:containsPoint(layerPos1) and not intouch then
                    if self.contentScrollView and self.contentScrollView:isVisible() then
                        -- local nameBox = self.richLabe[1].nameMenu:boundingBox()
                        -- local nameBox2 = self.richLabe[2].nameMenu:boundingBox()
                        -- if (nameBox and nameBox:containsPoint(layerPos)) or (nameBox2 and nameBox2:containsPoint(layerPos)) then
                        -- GDebug("ssssssss")
                        -- self:textCal(self.curData)
                        -- else
                        self:contentVisible(false)
                        -- end
                    else
                        self:contentVisible(true)
                    end
                else
                    -- GDebug("tounch out")
                    -- GDebug("----intouch="..tostring(intouch))
                    if not intouch then self:contentVisible(false) end
                end
                intouch = false
            end
            return true
        end
        if not self.touchLayer then
            self.touchLayer = CCLayerColor:create()
            self:addChild(self.touchLayer)
            self.touchLayer:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
            self.touchLayer:setTouchEnabled(true)
        end
    end
end

function LCBroadcastLayer190:textCal(tables)
    if not tables then return end
    -- GDebug("----tables id="..tables.dic.playerID or "")
    -- GDebug("----tables id="..tables.dic.nickName or "")
    if self.open ~= nil then return end
    if self.contentLayer then
        self.open = self.contentLayer:isVisible()
    else
        self.open = false
    end

    if self.touchLayer then self.touchLayer:setTouchEnabled(false) end
    --[[scheduleOnce( LCSceneTransit:getRunningScene(), function()
		self:contentVisible(false)
	end, 0.1)--]]

    if tables.playerID and tables.nickName then
        local key = {
            title = tables.nickName,
            playerID = tables.playerID,
            infoTypes = eRoleInfoType.rankInfo,
            dismissCallback = function()
                --[[if self.open then
					self:setTouch()
					self:contentVisible(true)
				end--]]
                if self.touchLayer then self.touchLayer:setTouchEnabled(true) end
                scheduleOnce(LCSceneTransit:getRunningScene(), function(...)
                    if self.contentScrollView then
                        self.contentScrollView:setTouchEnabled(false)
                        self.contentScrollView:setTouchEnabled(true)
                    end
                    self:updateContent()
                end , 0.01)
                self.open = nil
            end
        }
        LCPlayerInfoDialog.new(key):showInScene()
    end
end

function LCBroadcastLayer190:contentLayerInit()
    if not self.contentLayer then
        self.contentLayer = CCLayerColor:create(_APPCOLOR_RGB(0, 0, 0, 255 * 0.65))
        local layerSize = CCSizeMake(self.size.width - 22, self.size.height * 6)
        self.contentLayer:changeWidthAndHeight(layerSize.width, layerSize.height)
        self:addChild(self.contentLayer)
        local worldPos = self.contentLayer:convertToWorldSpace(ccp(self:getPosition()))
        self.contentLayer:setPosition(ccp(0, - layerSize.height + 5))
    else
        self.contentLayer:setVisible(true)
    end

    -- self:contentVisible(false)
    self:updateContent()
    self.gameData:getBroadCastMgr():setOpenBroadLayer(true)
end

function LCBroadcastLayer190:updateData(first)
    self.broadcastList = self.gameData:getBroadCastMgr():getBroadCastMap() or { }
    self.newBroList = self.gameData:getBroadCastMgr():getNewBroList() or { }
    if #self.newBroList <= 0 and #self.broadcastList > 0 and not self.action then
        if first and self.firstIn then
            for i = 1, #self.broadcastList do
                local v = self.broadcastList[#self.broadcastList - i + 1]
                if v and #self.newBroList < self.broadcastLen then
                    v.firstIn = true
                    table.insert(self.newBroList, v)
                end
            end
        else
            local data = self.broadcastList[1]
            table.insert(self.newBroList, data)
        end
    else
        if not self.firstIn then
            for i = #self.newBroList, 1, -1 do
                local data = self.newBroList[i]
                if data and data.firstIn then table.remove(self.newBroList, i) end
            end
        end
    end

end

function LCBroadcastLayer190:update()
    self:updateData()
    if self.start == false then
        self:startAnimate()
    end
    self:updateContent()
end

function LCBroadcastLayer190:updateContent()
    if not self.contentLayer then return end

    if not self.contentScrollView then
        local size = self.contentLayer:getContentSize()
        local cellSize = CCSizeMake(size.width-5, size.height)
        local sizeTableView = CCSizeMake(size.width, size.height)

        self.contentScrollView = LCTableView:create(sizeTableView, {
            cellHeight = function(index)
                -- local data=self.broadcastList[#self.broadcastList-index]
                -- local height=data.height
                -- if not height then
                -- 	local cell = LCBroadCell:create({boardData=data})
                -- 	data.height=cell:getHeight()
                -- 	height=data.height
                -- end
                return eBroadCellSize190.height
            end,
            cellAtIndex = function(index, reusedCell)
                local cell = reusedCell
                local data = self.broadcastList[#self.broadcastList - index]
                if not cell then
                    -- LCBroadCell
                    cell = LCBroadCell:create( {
                        boardData = data,
                        cellSize = cellSize,
                        fontSize = 18,
                        callback = function(data)
                            self:textCal(data)
                        end
                    } )
                    -- data.height=cell:getHeight()
                else
                    cell:configWithData(data)
                end
                cell:setPositionX(0)
                return cell
            end,
            numberOfCells = function() return #self.broadcastList end,
            cellTouched = function(cell) end
        } )
        self.contentLayer:addChild(self.contentScrollView)
    else
        if self.contentScrollView and not self.open then self.contentScrollView:reloadData(true) end
    end
end

function LCBroadcastLayer190:contentVisible(isVisible)
    if self.contentLayer then
        self.contentLayer:setVisible(isVisible)
        -- self.contentLayer:setTouchEnabled(isVisible)
        if self.clicked and isVisible then
            if self.touchLayer then self.touchLayer:setTouchEnabled(true) end

            if self.contentScrollView then
                self.contentScrollView:setTouchEnabled(false)
                self.contentScrollView:setTouchEnabled(true)
            end
        else
            -- if self.touchLayer and self.contentLayer and self.contentLayer:isVisible() then self.touchLayer:setTouchEnabled(false) end
            -- if self.contentScrollView then self.contentScrollView:setTouchEnabled(false) end
            if self.touchLayer then
                self.touchLayer:removeFromParentAndCleanup(true)
                self.touchLayer = nil
            end
            if self.contentLayer then
                self.contentLayer:removeFromParentAndCleanup(true)
                self.contentLayer = nil
                self.contentScrollView = nil
            end
        end

    end

    local tick1 = getTickCount()
    if self.clicked then
        if isVisible then
            if not self.contentLayer then
                local t1 = getTickCount()
                self:contentLayerInit()
                local t2 = getTickCount()
            else
                local t3 = getTickCount()
                self:updateContent()
                local t4 = getTickCount()
            end
        else
            if self.contentLayer then
                self.contentLayer:setVisible(false)
                -- self.contentLayer:setTouchEnabled(false)
            end
            if self.touchLayer then
                self.touchLayer:setTouchEnabled(false)
            end
            if self.contentScrollView then self.contentScrollView:setTouchEnabled(false) end
        end
    else
        self:setTouchEnabled(false)
    end
   -- local tick2 = getTickCount()
    if not isVisible then
        self.gameData:getBroadCastMgr():setOpenBroadLayer(false)
        self.gameData:getBroadCastMgr():reMaxBroad()
    end
    -- GDebug("xxxxxxxxxxxxxxxxxxxx :%s",tick2 - tick1)
end

function LCBroadcastLayer190:startAnimate()
    self.start = true
    local lastIndex = self.textIndex
	local scolSize  = self.scolSize
	local scolWidth = scolSize.width - 20
	local scolHeight= scolSize.height

    self.textIndex =(self.textIndex + 1) % 3
    if self.textIndex == 0 then self.textIndex = 1 end
    local lastMove = CCMoveBy:create(0.25, ccp(0, - scolHeight))
    self.richLabe[lastIndex]:runAction(CCSequence:createWithTwoActions(lastMove, CCCallFunc:create( function()
        self.richLabe[lastIndex]:setPosition(self.posCurrent)
    end )))

    local data = self.default
    if #self.newBroList > 0 then
        data = self.newBroList[1]
        table.remove(self.newBroList, 1)
        if self.contentScrollView then self:updateContent() end
    elseif #self.broadcastList > 0 then
        data = self.broadcastList[#self.broadcastList]
    end

    self.richLabe[self.textIndex]:configWithData(data)
    local content = self.richLabe[self.textIndex]:getCellContentSize()
    self.curData = data

    local array = CCArray:create()
    array:addObject(CCCallFunc:create( function()
        if self.action and self.hasMove == false then
            self.hasMove = true
            self:runAction(CCMoveBy:create(0.25, ccp(0, - scolHeight * 1.5)))
        end
    end ))
    array:addObject(CCMoveTo:create(0.25, self.posLast))
    array:addObject(CCDelayTime:create(data.showTime / 2))
    local length = content.width
    if length > scolWidth then
        local showTime =(length - scolWidth) / scolWidth * 5
        local move = CCMoveBy:create(showTime, ccp(- length + scolWidth - 20, 0))
        array:addObject(move)
    end
    array:addObject(CCDelayTime:create(data.showTime / 2))
    array:addObject(CCCallFunc:create( function()
        if self.action then
            if #self.newBroList <= 0 then
                self.start = false
                self:runAction(CCSequence:createWithTwoActions(CCMoveBy:create(0.25, ccp(0, scolHeight * 1.5)), CCCallFunc:create( function()
                    self.hasMove = false
                end )))
            else
                self:startAnimate()
            end
        else
            if self.repeats == false and #self.newBroList <= 0 then
                self.start = false
                if length > scolWidth then
                    self.richLabe[self.textIndex]:setPositionX(self.posLast.x)
                end
            else
                if #self.broadcastList > 1 or #self.newBroList > 0 then
                    self:startAnimate()
                else
                    self.start = false
                    if length > scolWidth then
                        self.richLabe[self.textIndex]:setPositionX(self.posLast.x)
                    end
                end
            end
        end

    end ))
    self.richLabe[self.textIndex]:runAction(CCSequence:create(array))
end

function LCBroadcastLayer190:contentTouch(eventType, x, y)
    local layerPos = self.scrollView:convertToNodeSpace(ccp(x, y))
    local bBox = self.scrollView:boundingBox()
    if eventType == "began" then
        if bBox:containsPoint(layerPos) then
            GDebug("tounch in")
            self.touchIn = true
            if self.contentLayer and self.contentLayer:isVisible() then
                self:contentVisible(false)
                return false
            else
                self:contentVisible(true)
            end
            --[[else
			GDebug("tounch out")
			self.touchIn = false
			local bBox2 = self.contentLayer:boundingBox()
			if bBox2:containsPoint(layerPos)==false then
				self:contentVisible(false)
			end--]]
        end
        return true
    elseif eventType == "ended" then
        -- GDebug("ended")
        if self.touchIn then
            if bBox:containsPoint(layerPos) then
                -- 	GDebug("up in side")
                -- self:contentVisible(true)
            end
        end
    end
end

--[[
	msg={
		msg.playerID
		msg.nickName
		msg.avatar
		msg.types
		msg.content
		msg.vipValue
	}
--]]
function LCBroadcastLayer190:addBroadCast(msg, noTopShow)
    -- msg need
    if not msg then return end
    self:parseData(msg)
end

function LCBroadcastLayer190:dealPacket(seed, msg, uiUpdater)
    if seed == eDefMsg.Broadcast then
        self.scene:hideLoading()
        if msg.status and msg.status ~= eErrorCode.succ then
            local playerData = LCAppGlobal:getLoginPlayer()
            if msg.playerID == playerData.playerID then
                self.scene:showConfirmMessage(_ID2WORD(msg.status))
            end
            return
        end

        local mainScene = LCSceneTransit:getRunningScene()
        if mainScene and mainScene.__cname ~= "LCMainScene" and LCAppGlobal:getLoginPlayer():isEqualSelfPlayer(msg.playerID)
            and(msg.game == eRoomType.slotRoom or msg.game == eRoomType.RouletRoom or msg.game == eRoomType.VarySlot) then
            scheduleOnce(mainScene, function(...)
                self:parseData(msg)
            end , 5)
        else
            self:parseData(msg)
        end
    elseif seed == eDefMsg.BuyGoods then
        self:updateTalkCard()
    end
end

function LCBroadcastLayer190:parseData(msg)
    if not msg then return end
    if msg.types == eBroadcastType.player then
        -- self:updateTalkCard()
        -- if self.gameData:getSwitchMgr():isTalkValue() == false then return end
        -- if not msg.content or #msg.content <= 0 then return end
        return
    end
    -- GDebug("---start="..tostring(self.start))
    --[[if self.start == false then
		self:startAnimate()
	end--]]

    self:update()
end



---------------------------

LCBroadcastCell190 = class("LCBroadcastCell190", LCTableViewCell)

function LCBroadcastCell190:create(...)
    local ret = LCBroadcastCell190.new()
    if not ret:init(...) then return nil end
    return ret
end
function LCBroadcastCell190:init(key)
    if not LCTableViewCell.init(self) then return false end
    if not key then key = { } end
    CC_SYNTHESIZE(self, "callback", key.callback)
    CC_SYNTHESIZE(self, "boardData", key.boardData)
    CC_SYNTHESIZE(self, "cellSize", key.cellSize or eBroadCellSize190)
    CC_SYNTHESIZE(self, "gapLeft", key.gapLeft or 5)

    -- const
    CC_SYNTHESIZE(self, "fontSize", 18)
    CC_SYNTHESIZE(self, "avatarScale", 0.15)
    -- ui releated
    CC_SYNTHESIZE(self, "titleLabel", nil)
    CC_SYNTHESIZE(self, "avatar", nil)
    CC_SYNTHESIZE(self, "nameMenu", nil)
    CC_SYNTHESIZE(self, "name2Menu", nil)
    CC_SYNTHESIZE(self, "menu", nil)
    CC_SYNTHESIZE(self, "contentLabel", nil)
    CC_SYNTHESIZE(self, "contentLabel2", nil)
    CC_SYNTHESIZE(self, "cellContentSize", self.cellSize)


    self:changeWidthAndHeight(self.cellSize.width, self.cellSize.height)
    -- ui create
    self.titleLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, self.fontSize)
    self.titleLabel:setAnchorPoint(ccp(0, 0.5))

    self.contentLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, self.fontSize)
    self.contentLabel:setAnchorPoint(ccp(0, 0.5))

    self.contentLabel2 = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, self.fontSize)
    self.contentLabel2:setAnchorPoint(ccp(0, 0.5))
    self.avatar = LCPlayerAvatar:create(eDefaultAvatar.min, function(avatar, url)
        self:infoCallback()
    end )
    self.avatar:setAvatarScale(self.avatarScale)

    self.menu = CCMenu:create()
    local nameLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, self.fontSize)
    self.nameMenu = CCMenuItemLabel:create(nameLabel)
    self.nameMenu:registerScriptTapHandler( function(...)
        self:infoCallback()
    end )
    self.menu:addChild(self.nameMenu)
    self.menu:setPosition(ccp(2, 0))
    self.nameMenu:setAnchorPoint(ccp(0, 0.5))

    self:addChild(self.titleLabel)
    self:addChild(self.contentLabel)
    self:addChild(self.contentLabel2)
    self:addChild(self.avatar)
    self:addChild(self.menu)

    -- update with data
    self:configWithData(self.boardData)
    self:setPositionX(TABLE_BROADCASR_OFFX)

    return true
end
function LCBroadcastCell190:infoCallback(broadData)
    GDebug("info clicked")
    broadData = broadData or self.boardData
    if self.callback then self.callback(broadData) end

end

function LCBroadcastCell190:configWithData(boardData)
    if not boardData then return end
    self.boardData = boardData

    self.titleLabel:setVisible(false)
    self.contentLabel:setVisible(false)
    self.contentLabel2:setVisible(false)
    self.avatar:setVisible(false)
    self.avatar:setTouchEnabled(false)

    self.menu:setVisible(false)
    self.menu:setTouchEnabled(false)

    if self.name2Menu then
        self.name2Menu:removeFromParentAndCleanup(true)
        self.name2Menu = nil
    end

    self.cellContentSize = self.cellSize

    local cellContentWidth = self.cellContentSize.width
    local broadType = boardData.broadTypes
    local titleColor = eDefColor.boardCastBlue
    if broadType == eBroadcastType.active then
        titleColor = eDefColor.boardCastGreen
    elseif broadType == eBroadcastType.game or
        broadType == eBroadcastType.common or
        broadType == eBroadcastType.marry then
        titleColor = eDefColor.boardCastYellow
    end
    self.titleLabel:setColor(titleColor)
    -- relayout
    if broadType == eBroadcastType.system or broadType == eBroadcastType.active then
        local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
        self.titleLabel:setVisible(true)
        self.contentLabel:setVisible(true)

        self.titleLabel:setString(title)
        self.contentLabel:setString(boardData.text or "")
        self.contentLabel:setColor(eDefColor.boardCastNomal)

        local titleSize = self.titleLabel:getContentSize()
        local contentSize = self.contentLabel:getContentSize()
        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        local contentPosX, contentPosY = titlePosX + titleSize.width, titlePosY

        self.titleLabel:setPosition(ccp(titlePosX, titlePosY))
        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        cellContentWidth = contentPosX + contentSize.width
    elseif broadType == eBroadcastType.game then
        self.titleLabel:setVisible(true)
        self.contentLabel:setVisible(true)
        self.menu:setVisible(true)
        self.menu:setTouchEnabled(true)

        self.contentLabel2:setVisible(true)

        local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
        self.titleLabel:setString(title)
        self.contentLabel:setString(_("恭喜"))
        self.nameMenu:setString(boardData.nickName or "")

        local roomType = boardData.game
        local gameName = eGameName[roomType] and eGameName[roomType].name or _("神秘游戏")
        local text2 = string.format(_("在%s中赢取%s筹码！"), gameName or "", _NICE_MONEY3(boardData.winGold or 0))
        if DEF_DESGIN_LANGUAGE == kLanguageEnglish then
            text2 = string.format(_("在%s中赢取%s筹码！"), _NICE_MONEY3(boardData.winGold or 0), gameName or "")
        end
        self.contentLabel2:setString(text2)

        self.contentLabel:setColor(eDefColor.boardCastNomal)
        self.nameMenu:setColor(eDefColor.boardCastBlue)
        self.contentLabel2:setColor(eDefColor.boardCastNomal)

        local titleSize = self.titleLabel:getContentSize()
        local contentSize = self.contentLabel:getContentSize()
        local content2Size = self.contentLabel2:getContentSize()
        local menuSize = self.nameMenu:getContentSize()
        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        local contentPosX, contentPosY = titlePosX + titleSize.width, titlePosY
        local menuPosX, menuPosY = contentPosX + contentSize.width, titlePosY
        local content2PosX, content2PosY = menuPosX + menuSize.width, titlePosY

        self.titleLabel:setPosition(ccp(titlePosX, titlePosY))
        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        self.nameMenu:setPosition(ccp(menuPosX, menuPosY))
        self.contentLabel2:setPosition(ccp(content2PosX, content2PosY))
        cellContentWidth = content2PosX + content2Size.width

    elseif broadType == eBroadcastType.player then
        self.avatar:setVisible(true)
        self.avatar:setTouchEnabled(true)
        self.menu:setVisible(true)
        self.menu:setTouchEnabled(true)
        self.contentLabel:setVisible(true)


        self.avatar:updateURL(boardData.avatar)

        self.nameMenu:setString((boardData.nickName or "") .. ":")
        self.contentLabel:setString(boardData.text or "")
        self.contentLabel:setColor(eDefColor.boardCastNomal)
        self.nameMenu:setColor(eDefColor.boardCastBlue)

        local originAvatarSize = self.avatar:getContentSize()

        -- GDebug("liulihua   : %s",sizestring(originAvatarSize))

        local avatarSize = CCSizeMake(originAvatarSize.width * self.avatarScale, originAvatarSize.height * self.avatarScale)
        local contentSize = self.contentLabel:getContentSize()
        local menuSize = self.nameMenu:getContentSize()

        local avatarPosX, avatarPosY = self.gapLeft - originAvatarSize.width *(1 - self.avatarScale) / 2,(self.cellSize.height - originAvatarSize.height) / 2
        -- -
        avatarPosX = avatarPosX + AVATAR_OFFX
        local menuPosX, menuPosY = avatarPosX + avatarSize.width + originAvatarSize.width *(1 - self.avatarScale) / 2 + 3, self.cellSize.height / 2
        local contentPosX, contentPosY = menuPosX + menuSize.width, menuPosY

        self.avatar:setPosition(ccp(avatarPosX, avatarPosY))
        self.nameMenu:setPosition(ccp(menuPosX, menuPosY))
        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        cellContentWidth = contentPosX + contentSize.width
    elseif broadType == eBroadcastType.common then
        local text = boardData.text or ""
        local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
        local name = boardData.nickName or ""
        self.menu:setVisible(true)
        self.menu:setTouchEnabled(true)
        self.contentLabel:setVisible(true)
        self.contentLabel2:setVisible(true)
        self.titleLabel:setVisible(true)
        self.titleLabel:setString(title)

        local startIndex, endIndex = string.find(text, "{{{")
        local startIndex1, endIndex1 = string.find(text, "}}}")
        if startIndex then
            local content1 = string.sub(text, 1, startIndex - 1)
            self.contentLabel:setString(content1 or "")
            self.contentLabel:setColor(eDefColor.boardCastYellow)

            self.nameMenu:setString(name or "")
            self.nameMenu:setColor(_APPCOLOR_HEX("77f4ff"))
        end

        if endIndex1 then
            local content2 = string.sub(text, endIndex1 + 1, #text)
            self.contentLabel2:setString(content2)
            self.contentLabel2:setColor(eDefColor.boardCastYellow)
        end

        if not startIndex and not endIndex1 then
            self.contentLabel:setString(text or "")
            self.contentLabel:setColor(eDefColor.boardCastYellow)
        end

        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        self.titleLabel:setPosition(ccp(titlePosX, titlePosY))
        local titleSize = self.titleLabel:getContentSize()
        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        local contentPosX, contentPosY = titlePosX + titleSize.width, titlePosY
        local contentSize = self.contentLabel:getContentSize()
        local menuPosX, menuPosY = contentPosX + contentSize.width, titlePosY
        local menuSize = self.nameMenu:getContentSize()
        local content2PosX, content2PosY = menuPosX + menuSize.width, titlePosY
        local content2Size = self.contentLabel2:getContentSize()

        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        self.nameMenu:setPosition(ccp(menuPosX, menuPosY))
        self.contentLabel2:setPosition(ccp(content2PosX, content2PosY))

        cellContentWidth = content2PosX + content2Size.width
    elseif broadType == eBroadcastType.marry then
        local text = boardData.text or ""
        local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
        local name = boardData.nickName or ""
        self.menu:setVisible(true)
        self.menu:setTouchEnabled(true)
        self.contentLabel:setVisible(true)
        self.contentLabel2:setVisible(false)
        self.titleLabel:setVisible(true)
        self.titleLabel:setString(title)

        local nameLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, self.fontSize)
        self.name2Menu = CCMenuItemLabel:create(nameLabel)
        self.name2Menu:registerScriptTapHandler( function(...)
            local t = { }
            for k, v in pairs(boardData) do t[k] = v end
            t.nickName = boardData.player2_name
            t.playerID = boardData.player2_id
            self:infoCallback(t)
        end )
        self.menu:addChild(self.name2Menu)
        self.name2Menu:setAnchorPoint(ccp(0, 0.5))

        local start, middle, ends = xstr.partition(text, "{{{")
        -- start 恭喜
        local start2, middle2, ends2 = xstr.partition(ends, "}}}")
        local start3, middle3, ends3 = xstr.partition(ends2, "{{{")
        -- start3  和
        local start4, middle4, ends4 = xstr.partition(ends3, "}}}")
        -- ends4  最后一点

        local name1 = boardData.nickName or ""
        local playerID = boardData.playerID or ""

        local name2 = boardData.player2_name or ""
        local playerID2 = boardData.player2_id or ""
        start = start or ""
        start3 = start3 or ""
        ends4 = ends4 or ""

        self.nameMenu:setString(name1 .. " ")
        self.nameMenu:setColor(_APPCOLOR_HEX("77f4ff"))

        self.name2Menu:setString(name2 .. " ")
        self.name2Menu:setColor(_APPCOLOR_HEX("77f4ff"))


        local name1Size = self.nameMenu:getContentSize()
        local name2Size = self.name2Menu:getContentSize()
        local space1Num = math.ceil(name1Size.width / __ceSpaceWidth25)
        local space2Num = math.ceil(name2Size.width / __ceSpaceWidth25)
        local space1Text, space2Text = "", ""
        for i = 1, space1Num do space1Text = space1Text .. " " end
        for i = 1, space2Num do space2Text = space2Text .. " " end
        local contentText = start .. space1Text .. start3 .. space2Text .. ends4
        self.contentLabel:setString(contentText or "")
        self.contentLabel:setColor(eDefColor.boardCastYellow)

        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        self.titleLabel:setPosition(ccp(titlePosX, titlePosY))
        local titleSize = self.titleLabel:getContentSize()
        local titlePosX, titlePosY = self.gapLeft, self.cellSize.height / 2
        local contentPosX, contentPosY = titlePosX + titleSize.width, titlePosY

        self.contentLabel2:setString(start)
        local content2Size = self.contentLabel2:getContentSize()
        local tGap1X =(space1Num * __ceSpaceWidth25 - name1Size.width) / 2
        local tGap2X =(space2Num * __ceSpaceWidth25 - name2Size.width) / 2
        local menuPosX, menuPosY = contentPosX + content2Size.width + tGap1X, titlePosY
        local menuSize = self.nameMenu:getContentSize()

        self.contentLabel2:setString(start3)
        local content3Size = self.contentLabel2:getContentSize()
        local menu2PosX, menu2PosY = menuPosX + menuSize.width + content3Size.width + tGap1X + tGap2X, titlePosY
        -- local content2Size = self.contentLabel2:getContentSize()

        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        self.nameMenu:setPosition(ccp(menuPosX, menuPosY))
        self.name2Menu:setPosition(ccp(menu2PosX, menu2PosY))
        -- self.contentLabel2:setPosition(ccp(content2PosX, content2PosY))

        cellContentWidth = self.contentLabel:getContentSize().width
        -- content2PosX + content2Size.width
    elseif broadType == eBroadcastType.jackpot then

        self.titleLabel:setVisible(true)
        self.contentLabel:setVisible(true)
        self.menu:setVisible(true)
        self.menu:setTouchEnabled(true)

        self.contentLabel2:setVisible(true)

        local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
        self.titleLabel:setString(title)
        self.contentLabel:setString(_("恭喜"))
        self.nameMenu:setString(boardData.nickName or "")

        local roomType=boardData.game
        local text2 = string.format(_("在拉彩中赢取了%s的大奖！"), _NICE_MONEY3(boardData.winGold or 0))
        self.contentLabel2:setString(text2)

        self.contentLabel:setColor(_APPCOLOR_HEX("ffcb67"))
        self.nameMenu:setColor(_APPCOLOR_HEX("77f4ff"))
        self.contentLabel2:setColor(_APPCOLOR_HEX("ffcb67"))

        local titleSize = self.titleLabel:getContentSize()
        local contentSize = self.contentLabel:getContentSize()
        local content2Size = self.contentLabel2:getContentSize()
        local menuSize = self.nameMenu:getContentSize()
        local titlePosX, titlePosY = self.gapLeft,self.cellSize.height/2
        local contentPosX, contentPosY = titlePosX + titleSize.width , titlePosY
        local menuPosX, menuPosY = contentPosX + contentSize.width, titlePosY
        local content2PosX, content2PosY = menuPosX + menuSize.width , titlePosY

        self.titleLabel:setPosition(ccp(titlePosX, titlePosY))
        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        self.nameMenu:setPosition(ccp(menuPosX, menuPosY))
        self.contentLabel2:setPosition(ccp(content2PosX, content2PosY))
        cellContentWidth = content2PosX + content2Size.width
    elseif broadType == eBroadcastType.noTitle then
        self.contentLabel:setVisible(true)
        self.contentLabel:setString(boardData.text or "")
        self.contentLabel:setColor(eDefColor.boardCastYellow)
        local contentSize = self.contentLabel:getContentSize()
        local contentPosX, contentPosY = self.gapLeft, self.cellSize.height / 2
        self.contentLabel:setPosition(ccp(contentPosX, contentPosY))
        cellContentWidth = contentPosX + contentSize.width
    end
    self.cellContentSize = CCSizeMake(cellContentWidth, self.cellContentSize.height)

end

return LCBroadcastLayer190
