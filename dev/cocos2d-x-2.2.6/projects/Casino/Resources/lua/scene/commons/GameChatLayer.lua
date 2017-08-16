local LCTableViewCell = require("lua/scene/sceneUtils/TableViewCell")
local LCTableView = require("lua/scene/sceneUtils/TableView")
local LCRichLabel = require("lua/scene/sceneUtils/RichLabel")
local LCBroadCell = require("lua/scene/commons/BroadCell")
local LCPlayerInfoDialog = require("lua/scene/mainScene/playerInfo/PlayerInfoDialog")

local LCGameChatLayer = class("LCGameChatLayer",function(...) return CCLayerColor:create(...) end)
function LCGameChatLayer:create(...)
	local ret = LCGameChatLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end
function LCGameChatLayer:init(scene,key)
	key = key or {}
	CC_SYNTHESIZE(self,"scene",scene)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"records", key.recordList or {})
	CC_SYNTHESIZE(self,"onlyWorld", key.onlyWorld or false)
	-- CC_SYNTHESIZE(self,"switcherMgr",LCAppGlobal:getGameData():getSwitchMgr())
	CC_SYNTHESIZE(self,"iconTabel",{})
	CC_SYNTHESIZE(self,"iconBtnPos",{})
	CC_SYNTHESIZE(self,"iconTableView",{})
	CC_SYNTHESIZE(self,"iconList",{})
	CC_SYNTHESIZE(self,"iconLen", 4)
	CC_SYNTHESIZE(self,"curIconIndex", 0)
	CC_SYNTHESIZE(self,"selectIndex", self:getLastClicked())
	CC_SYNTHESIZE(self,"maskLayer",CCLayerColor:create())
	CC_SYNTHESIZE(self,"textMaxLength", 44)

	if #self.records >= 1 then
		for i=#self.records,1,-1 do
			local record = self.records[i]
			if record.types == 4 then
				table.remove(self.records,i)
			end
		end
	end

	self:addChild(self.maskLayer)
	self.maskLayer:registerScriptTouchHandler(function( ... )
		if self.onBTNTouch then return self:onBTNTouch(...) end
	end, false, eTouchPriority.maskLayer, true)
	self.maskLayer:setTouchEnabled(true)

	local function onEvent(event)
		if event == "enter" then self:onEnter()
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onEvent)

	local  proxy = CCBProxy:create()
	local  node  = CCBuilderReaderLoad(eResCCBI.gameChat,proxy,self)
	local  layer = tolua.cast(node,"CCLayer")
	local size = layer:getContentSize()
	self:changeWidthAndHeight(size.width,size.height)
	self:addChild(layer)

	local nodeMap = {
		{"baseLayer","ccb_baseLayer","CCLayer"},
		{"inputBg","ccb_inputBg","CCScale9Sprite"},

		{"iconLayer","ccb_iconLayer","CCLayer"},
		{"sendBtnBg","ccb_sendBtnBg","CCControlButton"},
	}

	for i=1,4 do
		table.insert(nodeMap,{table.concat({"rightBtn",i}), table.concat({"ccb_rightBtn",i}), "CCControlButton"})
		table.insert(nodeMap,{table.concat({"icon",i}), table.concat({"ccb_icon",i}), "CCSprite"})
		table.insert(nodeMap,{table.concat({"contentLayer",i}), table.concat({"ccb_layer",i}), "CCLayer"})
		if i<=3 then
			table.insert(nodeMap,{table.concat({"iconTitle",i}), table.concat({"ccb_iconTitle",i}), "CCControlButton"})
		end
	end

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self._oldPosX=self.contentLayer1:getPositionX()

	_CBTN_CTEXTF(self.iconTitle1, _("最近"))
	_CBTN_CTEXTF(self.iconTitle2, _("基本"))
	_CBTN_CTEXTF(self.iconTitle3, _("专属"))

	return true
end

eChatLayer={
	world = 1,
	icon  = 2,
	said  = 3,
	old   = 4,
}

local echatIconLayer={
	current = 1,
	common  = 2,
	own     = 3,
}

local eTitleIndex={
	one = 1,
	two = 2,
	three = 3,
}

-- virtual
function LCGameChatLayer:onEnter()
	if self.onlyWorld then self.selectIndex=eChatLayer.world end
	self:visibleLayer()
end

function LCGameChatLayer:setVisible(visible)
	if visible then
	--	self:setPositionX(0)
	--	self.baseLayer:setVisible(true)
	--	self.maskLayer:setTouchEnabled(true)
	else
		if self.callback then self.callback() end
	end
end

function LCGameChatLayer:onBTNTouch(eventType,x,y)
	local glPos = ccp(x, y)
	if eventType == "began" then
		local layerPos = self:convertToNodeSpace(ccp(x, y))
		local bBox = self.baseLayer:boundingBox()
		if bBox:containsPoint(layerPos) then
			self.touchIn=true
		else
			self.touchIn=false
		end
		return true
	elseif eventType == "moved" then

	elseif eventType == "ended" then
		local layerPos = self:convertToNodeSpace(ccp(x, y))
		local bBox = self.baseLayer:boundingBox()
		if (not bBox:containsPoint(layerPos) and not self.touchIn) or not self.touchIn then
			self:setVisible(false)
		end
		self.touchIn=false
	end
end

function LCGameChatLayer:visibleLayer()
	local  index=self.selectIndex

	if index==eChatLayer.world then
		self:worldLayerUpdate()
	elseif index==eChatLayer.icon then
		self:iconLayerUpdate(echatIconLayer.common)
	elseif index==eChatLayer.said then
		self:textLayerUpdate()
	else
		self:recordLayerUpdate()
	end

	self:rightBtnUpdate(index)

	self:saveLastClicked(index)

	self:editUpdate()
end

function LCGameChatLayer:rightBtnUpdate(index)
	for i=1, 4 do
		local layer   =self[table.concat({"contentLayer", i})]
		local rightBtn=self[table.concat({"rightBtn", i})]
		local icon    =self[table.concat({"icon", i})]
		local frameTable   = eResImage.s_chatIconTitle[i] or {}
		local frame
		if i==index then
			layer:setPositionX(self._oldPosX)
			layer:setVisible(true)
			layer:setTouchEnabled(true)
			_CBTN_CFRAME(rightBtn, eResImage.s_chat_button1play)
			frame=CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(frameTable.on)
		else
			layer:setVisible(false)
			layer:setTouchEnabled(false)
			layer:setPositionX(1136)
			_CBTN_CFRAME(rightBtn, eResImage.s_chat_button1nomal)
			frame=CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(frameTable.off)
		end
		if frame then icon:setDisplayFrame(frame) end

		if self.onlyWorld then
			rightBtn:setTouchEnabled(false)
			rightBtn:setVisible(false)
			self["icon"..i]:setVisible(false)
		end
	end
end

function LCGameChatLayer:iconLayerUpdate(index)
	if not index or index==self.curIconIndex then return end
	self.curIconIndex=index
	if #self.iconTabel<=0 then
		local size=self.iconLayer:getContentSize()
		for i=1, 3 do
			local layer=CCLayerColor:create()
			layer:changeWidthAndHeight(size.width, size.height)
			layer:setTag(i)
			self.iconLayer:addChild(layer)
			self.iconTabel[i]=layer

			self.iconBtnPos[i]=ccp(self[table.concat({"iconTitle", i})]:getPosition())
			self:commonIconInit(layer, i)
		end
	end
	local _lastIndex=self[table.concat({"iconTitle", index})]:getTag()
	for i=1, 3 do
		local layer    = self.iconTabel[i]
		local titleBtn = self[table.concat({"iconTitle", i})]
		if layer then
			if index==i then
				layer:setVisible(true)
				layer:setTouchEnabled(true)
				_CBTN_CFRAME(titleBtn, eResImage.s_chat_button2play)
			--	titleBtn:setZOrder(eTitleIndex.two)
			--	titleBtn:runAction(CCEaseSineInOut:create(CCMoveTo:create(0.2, self.iconBtnPos[2])))
			--	titleBtn:setTag(eTitleIndex.two)
			else
				layer:setVisible(false)
				layer:setTouchEnabled(false)
			--	local tag=titleBtn:getTag()
				_CBTN_CFRAME(titleBtn, eResImage.s_buttontransparent)
				--[[local newTag
				if tag==eTitleIndex.one then
					newTag=eTitleIndex.three
				elseif tag==eTitleIndex.three then
					newTag=eTitleIndex.one
				elseif tag==eTitleIndex.two then
					if _lastIndex==eTitleIndex.one then
						newTag=eTitleIndex.three
					else
						newTag=eTitleIndex.one
					end
				end
				if newTag then
					titleBtn:runAction(CCEaseSineInOut:create(CCMoveTo:create(0.2, self.iconBtnPos[newTag])))
					titleBtn:setTag(newTag)
				end--]]
			end
		end
	end


end

function LCGameChatLayer:worldLayerUpdate()
	-- if not self.switcherMgr:isBroadcastValue() then return end

	local worldSize=self.contentLayer1:getContentSize()
	self.broadcastList=LCAppGlobal:getGameData():getBroadCastMgr():getBroadCastMap() or {}
	if not self.worldScrollView then
		if self.onlyWorld then worldSize.width=worldSize.width+self.rightBtn1:getContentSize().width end
		local cellSize=CCSizeMake(worldSize.width, eGameCellSize.height)
		self.worldScrollView =  LCTableView:create(worldSize,{
			cellHeight = function(index)
				--local data=self.broadcastList[index+1]
				--local cell = LCBroadCell:create({boardData=data, cellSize=cellSize, fontSize=22})
				return eGameCellSize.height
			end,
			cellAtIndex = function (index, reusedCell)
				local cell = reusedCell
				local data=self.broadcastList[index+1]
				if not cell then  cell = LCBroadCell:create({boardData=data, cellSize=cellSize, fontSize=22, callback=function (data)
					self:textCal(data)
				end})
				else
					cell:configWithData(data)
				end
				return  cell
			end,
			numberOfCells = function() if self.broadcastList then return #self.broadcastList else return 0 end end,
			cellTouched = function(cell)   end
		})
		self.contentLayer1:addChild(self.worldScrollView)

		self.worldScrollView:scrollToBottom(self.broadcastList, eGameCellSize.height)
	else
		if self.selectIndex==eChatLayer.world then
			if self.worldScrollView then self.worldScrollView:reloadData(true) end
			self:editUpdate()
		end
	end
end

function LCGameChatLayer:textLayerUpdate()

	local size=self.contentLayer3:getContentSize()
	local lineHeight=55
	if not self.talkScrollView then
		self.talkList = clone(eResImage.s_chatTalk)
		table.sort(self.talkList, function(a, b) return #a>#b end)

		local list=LCAppGlobal:getGameData():getGameChatMgr():getCustomList() or {}
		table.sort(list, function(a, b) return a.id>b.id end)
		for _,v in pairs(list) do
			table.insert(self.talkList, 1, v)
		end

		self.talkScrollView =  LCTableView:create(size,{
			cellHeight = function(index)
				return lineHeight
			end,
			cellAtIndex = function (index, reusedCell)
				local cell = reusedCell
				local i=#self.talkList-index
				if i>#self.talkList or i<=0 then return cell end
				local data=self.talkList[i]
				if not cell then
					cell = LCTalkCell:create(self.viewScene, {data=data, size=CCSizeMake(size.width, lineHeight), callback=function (data)
						data.types=eChatType.talk
						if self.dataCallback then self:dataCallback(data) end
					end})
				end
				cell:configWithData(data)
				return  cell
			end,
			numberOfCells = function() if self.talkList then return #self.talkList else return 0 end end,
			cellTouched = function(cell)   end
		})
		self.contentLayer3:addChild(self.talkScrollView)
	else
		if self.talkScrollView then self.talkScrollView:reloadData(true) end
	end
end

function LCGameChatLayer:recordLayerUpdate()
	local size=self.contentLayer4:getContentSize()
	if #self.records<=0 then
		if not self._noRecord then
			self._noRecord=CCLabelTTF:create(_("房间里好冷清啊,赶快互动一下吧!"), DEF_SYSTEM_FONT_NAME, 22)
			self._noRecord:setAnchorPoint(ccp(0.5, 0))
			self._noRecord:setDimensions(CCSizeMake(size.width-10, 0))
			self._noRecord:setColor(_APPCOLOR_HEX("FFEDAF"))
			self._noRecord:setPosition(ccp(size.width/2, size.height/2))
			self.contentLayer4:addChild(self._noRecord)
		end
	else
		if self._noRecord then
			self._noRecord:removeFromParentAndCleanup(true)
			self._noRecord=nil
		end

		if not self.recordScrollView then
			local lineHeight=60
			self.recordScrollView =  LCTableView:create(CCSizeMake(size.width, size.height-5),{
				cellHeight = function(index)
					return lineHeight
				end,
				cellAtIndex = function (index, reusedCell)
					local cell = reusedCell
					local data=self.records[#self.records-index]
					if not cell then
						--[[cell = LCQuickTalkCell:create({boardData=data, cellSize=CCSizeMake(size.width, lineHeight), callback=function (data)
							self:textCal(data)
						end}) --]]
						cell=LCBroadCell:create({boardData=data, fontSize=22, cellSize=CCSizeMake(size.width, lineHeight), gapLeft=5, callback=function (data)
							if not data then return end
							self:textCal(data)
						end})
					else
						cell:configWithData(data)
					end
					return  cell
				end,
				numberOfCells = function() if self.records then return #self.records else return 0 end end,
				cellTouched = function(cell)   end
			})
			self.contentLayer4:addChild(self.recordScrollView)
			self.recordScrollView:scrollToBottom(self.records, lineHeight)
		else
			if self.selectIndex==eChatLayer.old then
				if self.recordScrollView then self.recordScrollView:reloadData(true) end
			end
		end

	end
end

function LCGameChatLayer:textCal(tables)
	if not tables then return end
	if tables.playerID and tables.nickName then
		local key = {
			nickname = tables.nickName,
			id = tables.playerID,
			avatar = tables.avatar,
			-- infoTypes = eRoleInfoType.friendInfo,
			-- callback = function ()
			-- end
		}
		LCPlayerInfoDialog.new(key):showInScene()
	end
end

function LCGameChatLayer:dataInit(inList, types, len)
	local startIndex=0
	local cells={}
	local outList = {}
	for i=1, #inList do
		inList[i].types=types
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

function LCGameChatLayer:commonIconInit(layer, index)
	if not layer then return end
	local types=eChatType.icon1
	local size =layer:getContentSize()
	local tables={}
	if index==echatIconLayer.common then
		self.iconList[index]=self:dataInit(eResImage.s_chatIcon[types], types, self.iconLen)
		tables = self.iconList[index]

	elseif index==echatIconLayer.own then
		local textLable = CCLabelTTF:create(_("您目前没有专属表情!"), DEF_SYSTEM_FONT_NAME, 22)
		textLable:setAnchorPoint(ccp(0.5, 0))
		textLable:setColor(_APPCOLOR_HEX("FFEDAF"))
		textLable:setDimensions(CCSizeMake(size.width-5, 0))
		textLable:setPosition(ccp(size.width/2, size.height/2))
		layer:addChild(textLable)
	elseif index==echatIconLayer.current then
		local list=LCAppGlobal:getGameData():getGameChatMgr():getCurrentUse()
		self.iconList[index]=self:dataInit(list, types, self.iconLen)
		tables = self.iconList[index]
	end

	if index~=echatIconLayer.own then
		self.iconTableView[index] =  LCTableView:create(size,{
			cellHeight = function() return 110 end,
			cellAtIndex = function (index, reusedCell)
				local cell = reusedCell
				if not cell then
					cell = LCChatLineCell:create(self.scene, {len=self.iconLen, width=size.width, callback=function (data)
						if self.dataCallback then self:dataCallback(data) end
					end})
				end
				cell:configWithFriendData(tables[#tables-index])
				return cell
			end,
			numberOfCells = function() return #tables end,
			cellTouched = function(cell) GDebug("") end
		})
		layer:addChild(self.iconTableView[index])
	end
end

function LCGameChatLayer:editUpdate()
	if self.edit == nil then
		local function editBoxTextEventHandle(strEventName,pSender)
			local edit = tolua.cast(pSender,"CCEditBox")
			local text = edit:getText()
			if strEventName == "began" then
				self.cancle=false
			elseif strEventName == "return" then
				if text and #text>0 and not self.cancle then
					-- if self.selectIndex==eChatLayer.world and self.switcherMgr:isBroadcastValue() then
					-- 	self:sendBroadMess(text)
					-- else
						self:sendMess(eChatType.edit, 0, text, 0)
					-- end
				else
					--self.scene:showConfirmMessage(_("亲，您还什么都没写呢哦!"))
				end
				--GDebug("------ended")
			elseif strEventName == "ended" then

			elseif strEventName == "changed" then
				if text and CE_GET_STRING_SHOW_LENGTH(text) > self.textMaxLength then
					edit:setText(CE_STRING_CUT_OFF(text,self.textMaxLength))
				end
				self:saveLastInput(text or "")
			end
		end
		local scale9bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
		local layerSize = self.inputBg:getContentSize()
		scale9bg:setContentSize(layerSize)
		self.edit = CCEditBox:create(scale9bg:getContentSize(), scale9bg)
		self.edit:setPlaceholderFont(DEF_SYSTEM_FONT_NAME, 25)
		self.edit:setFont(DEF_SYSTEM_FONT_NAME, 25)
		self.edit:setFontColor(ccc3(0, 0, 0))
	    self.edit:setMaxLength(40)
		self.edit:setReturnType(kKeyboardReturnTypeDone)
		self.edit:setAnchorPoint(ccp(0, 0.5))
		self.edit:setContentSize(scale9bg:getContentSize())
		self.edit:setPosition(ccp(0, layerSize.height*0.5))
		self.edit:registerScriptEditBoxHandler(editBoxTextEventHandle)
		self.inputBg:addChild(self.edit)
	end

	local defaultText=_("说点什么呢？")
	-- if self.selectIndex==eChatLayer.world and self.switcherMgr:isBroadcastValue() then
	-- 	local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST) or 0
	-- 	defaultText=string.format(_("您拥有%s个世界广播小喇叭"), haveCount)
	-- 	_CBTN_CFRAME(self.sendBtnBg, eResImage.s_chat_orangebutton)
	-- else
		_CBTN_CFRAME(self.sendBtnBg, eResImage.s_greenbutton_biger)
	-- end
	self.edit:setPlaceHolder(defaultText)

	local last=self:getLastInput()
	if last and #last>0 then
		self.edit:setText(last)
	end
end

function LCGameChatLayer:dataCallback(data)
	if not data then return end
	local dText=""
	if data.types==eChatType.talk then
		dText=data.text
	else
		dText=data.name
	end
	local text=dText
	self:sendMess(data.types, data.id, text)
end

function LCGameChatLayer:sendMess(types, id, text, textType)
	if not text then return end
	local textType=textType or 1
	local pb = LCNetManager:createMessage(eDefMsg.GameChat)
	pb._msg.types 	 = types
	pb._msg.messID 	 = id
	pb._msg.text 	 = text
	LCNetManager:send(pb)
	local scene = LCSceneTransit:getRunningScene()
	self:saveLastInput("")
	if self.callback then self.callback() end
end

function LCGameChatLayer:setRecordList(recordList)
	if not recordList then return end
	self.records=recordList
		for i=#self.records,1,-1 do
			local record = self.records[i]
			if record.types == 4 then
				table.remove(self.records,i)
			end
		end
	self:recordLayerUpdate()
end

function LCGameChatLayer:onExit()
	GDebug("onExit")
	self.worldScrollView = nil
	self.recordScrollView = nil
	self.talkScrollView = nil

	for i=1, #echatIconLayer do
		self.iconTableView[i]=nil
	end

end

function LCGameChatLayer:saveLastClicked(types)
	if not types then return end
	LCUserContext:setObjectForKey(eUserDefaultKey.kLocalGameChatLastType, types)
end

function LCGameChatLayer:getLastClicked()
	local last=LCUserContext:getObjectForKey(eUserDefaultKey.kLocalGameChatLastType, eChatLayer.said)
	return last
end

function LCGameChatLayer:saveLastInput(text)
	if not text then return end
	LCUserContext:setObjectForKey(eUserDefaultKey.kLocalLastInputKey, text)
end

function LCGameChatLayer:getLastInput()
	local last=LCUserContext:getObjectForKey(eUserDefaultKey.kLocalLastInputKey, "")
	return last or ""
end


function LCGameChatLayer:ccb_sendClicked(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_sendClicked")

	local editText=self.edit:getText()
	if not editText or editText=="" then return end

	-- if self.selectIndex==eChatLayer.world and self.switcherMgr:isBroadcastValue() then
	-- 	self:sendBroadMess(editText)
	-- 	return
	-- end

	self:sendMess(eChatType.edit, 0,editText, 0)
end

function LCGameChatLayer:sendBroadMess(editText)
	if not editText then return end
	local haveCount = LCAppGlobal:getUserInfoData():getTreasureVlaue(ePairKey.BROADCAST) or 0
	if not haveCount or haveCount<=0 then
		local scene=LCSceneTransit:getRunningScene()
		local key = {
			text = _ID2WORD(4111),
			okText = _("确定"),
			cancelText = _("取消"),
			okCallback = function ()
				local LCStoreDialog = require("lua/scene/mainScene/shop/StoreDialog")
				--LCStoreDialog:showInScene(scene, {selectedIndex=2})
				LCStoreDialog.new({showCurrency = true}):showInScene()
			end,
		}
		LCMessageBox:showBox(LCMessageBoxSelect,key)
	elseif #editText>70 then
		LCTextHUD.new(_("输入内容过长")):showInScene()
	else
		local playerData = LCAppGlobal:getLoginPlayer()
		local pb = LCNetManager:createMessage(eDefMsg.Broadcast)
		pb._msg.playerID = playerData.playerID
		pb._msg.content = editText
		LCNetManager:send(pb)
		self.edit:setText("")
	end
	self:saveLastInput("")
end

function LCGameChatLayer:ccb_rightClicked1(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_rightClicked1")
	self.selectIndex=eChatLayer.world
	self:visibleLayer()
end

function LCGameChatLayer:ccb_rightClicked2(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_rightClicked2")
	self.selectIndex=eChatLayer.icon
	self:visibleLayer()
end

function LCGameChatLayer:ccb_rightClicked3(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_rightClicked3")
	self.selectIndex=eChatLayer.said
	self:visibleLayer()
end

function LCGameChatLayer:ccb_rightClicked4(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_rightClicked4")
	self.selectIndex=eChatLayer.old
	self:visibleLayer()
end

function LCGameChatLayer:ccb_iconClicked1(control,controlEvent)
--	GDebug("-------LCGameChatLayer:ccb_iconCurrentClicked")
	self:iconLayerUpdate(echatIconLayer.current)
end
function LCGameChatLayer:ccb_iconClicked3(control,controlEvent)
--	GDebug("-------LCGameChatLayer:ccb_iconOwnClicked")
	self:iconLayerUpdate(echatIconLayer.own)
end
function LCGameChatLayer:ccb_iconClicked2(control,controlEvent)
	--GDebug("-------LCGameChatLayer:ccb_iconCommonClicked")
	self:iconLayerUpdate(echatIconLayer.common)
end

function LCGameChatLayer:dealPacket(seed, msg, uiUpdater)
	if seed == eDefMsg.Broadcast then
		--self:worldLayerUpdate()
	elseif seed==eDefMsg.GameChat then
		self.records=clone(self.scene:getMessList())
		for i=#self.records,1,-1 do
			local record = self.records[i]
			if record.types == 4 then
				table.remove(self.records,i)
			end
		end
		self:recordLayerUpdate()
	end
end


LCChatLineCell = class("LCChatLineCell",LCTableViewCell)
function LCChatLineCell:create(...)
	local ret = LCChatLineCell.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCChatLineCell:init(scene, key)
	if not LCTableViewCell.init(self) then return false end
	CC_SYNTHESIZE(self,"datas", nil) -- register when communication on
	CC_SYNTHESIZE(self,"viewScene", scene)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"winWidth", key.width or 1136)
	CC_SYNTHESIZE(self,"len", key.len)
	CC_SYNTHESIZE(self,"iconScale", key.scale or 0.7)

	return true
end

function LCChatLineCell:configWithFriendData(datas)
	self.datas = datas
	self:removeAllChildrenWithCleanup(true)
	for i=1, self.len do
		local cell = nil
		if not self.datas or #self.datas<i then
			if cell then cell:setVisible(false) end
		else
			if not cell and self.datas[i] then
				cell = LCIconCell:create(self.viewScene, {data=self.datas[i], scale=self.iconScale, callback=function (data)
					if self.callback then self.callback(data) end
				end})

				local dis=(i-1)*((self.winWidth-self.len*cell.iconSize.width*self.iconScale)/(self.len))-self.winWidth/2
				cell:setPosition(ccp((i-1)*(cell.iconSize.width*self.iconScale+18)-cell.iconSize.width*1.25, -cell.iconSize.height*cell.iconScale*0.6))
				self:addChild(cell)
			end
		end
	end
end

LCIconCell = class("LCIconCell",LCTableViewCell)
function LCIconCell:create(...)
	local ret = LCIconCell.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCIconCell:init(scene, key)
	if not LCTableViewCell.init(self) then return false end
	if not key then return false end
	CC_SYNTHESIZE(self,"data", key.data)
	CC_SYNTHESIZE(self,"iconScale", key.scale)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"iconSize", CCSizeMake(128, 110))

	local menuItemArray = CCArray:create()
	local spriteNoraml = CCSprite:createWithSpriteFrameName(self.data.name)
	self.iconSize = spriteNoraml:getContentSize()
	local menuPopupItem = CCMenuItemSprite:create(spriteNoraml, spriteNoraml, spriteNoraml)
	menuPopupItem:ignoreAnchorPointForPosition(false)
	menuPopupItem:setAnchorPoint(ccp(0, 0.5))
	menuPopupItem:registerScriptTapHandler(function ()
		if self.callback then self.callback(self.data) end
	end)
	menuItemArray:addObject(menuPopupItem)
	local menuPopup = CCMenu:createWithArray(menuItemArray)
	menuPopup:setPosition(ccp(0, 0))
	menuPopup:setScale(self.iconScale)
	self:addChild(menuPopup)

	return true
end


--================  chat item cell ==============

LCTalkCell = class("LCTalkCell",LCTableViewCell)
function LCTalkCell:create(...)
	local ret = LCTalkCell.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCTalkCell:init(scene, key)
	if not LCTableViewCell.init(self) then return false end
	if not key then return false end
	CC_SYNTHESIZE(self,"data", key.data)
	CC_SYNTHESIZE(self,"callback", key.callback)
	CC_SYNTHESIZE(self,"talkCellSize", key.size or CCSizeMake(400, 60))

	local bg = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
	bg:setPreferredSize(self.talkCellSize)
	local menuItemArray = CCArray:create()
	local menuPopupItem = CCMenuItemSprite:create(bg, bg, bg)
	menuPopupItem:ignoreAnchorPointForPosition(false)
	menuPopupItem:setAnchorPoint(ccp(0, 0))
	menuPopupItem:registerScriptTapHandler(function ()
		if self.callback then self.callback(self.data) end
	end)
	menuItemArray:addObject(menuPopupItem)
	local menuPopup = CCMenu:createWithArray(menuItemArray)
	menuPopup:setPosition(ccp(0, 0))
	self:addChild(menuPopup)

	self.talkCellSize.width=self.talkCellSize.width-5
	local text=self.data.text or ""
	--[[local params = {
		text = "",
		dimensions = self.talkCellSize
	}
	self.richLabe = LCRichLabel:create(params)
	self.richLabe.tables={text=self.data.text, voice=self.data.voice}--]]
	self.richLabe=CCLabelTTF:create(text, DEF_SYSTEM_FONT_NAME, 22)
	local content = self.richLabe:getContentSize()
	self.richLabe:setAnchorPoint(ccp(0, 0.5))
	self.richLabe:setPositionY(self.talkCellSize.height*0.6)
	bg:addChild(self.richLabe)
	self.richLabe:setDimensions(CCSizeMake(self.talkCellSize.width, 0))
	self.richLabe:setHorizontalAlignment(kCCTextAlignmentLeft)

	local line = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_chat_cutline)
	line:setAnchorPoint(ccp(0, 0.5))
	line:setPreferredSize(CCSizeMake(self.talkCellSize.width, 2))
	line:setPositionY(self.talkCellSize.height*0.1)
	bg:addChild(line)

	return true
end


function LCTalkCell:configWithData( data )
	if not data then return end
	self.data=data
	local fontSize = 18
	local text=self.data.text or ""
	--local text = "[fontColor=FFFFFF fontSize="..fontSize.." callback=true]"..self.data.text.."[/fontColor]"
	self.richLabe:setString(text)
	--[[local content=self.richLabe:getContentSize()
	if content.height<self.talkCellSize.height then
		self.richLabe:setPositionY((self.talkCellSize.height-content.height)/2+self.talkCellSize.height/2)
	end--]]
end
return LCGameChatLayer
