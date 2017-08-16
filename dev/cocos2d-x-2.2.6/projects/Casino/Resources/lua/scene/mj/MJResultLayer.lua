local HU_TYPES = {}
HU_TYPES[0]  = "杠"
HU_TYPES[1]  = "七对x2"
HU_TYPES[2]  = "碰碰胡x2"
HU_TYPES[3]  = "清一色x2"
HU_TYPES[4]  = "清风x2"
HU_TYPES[5]  = "一条龙x2"
HU_TYPES[6]  = "豪华七对x4"
HU_TYPES[7]  = "超级豪华七对x8"
HU_TYPES[8]  = "至尊超级豪华七对x16"
HU_TYPES[9]  = "十三幺x10"
HU_TYPES[10] = "门清x2"
HU_TYPES[11] = "边x2"
HU_TYPES[12] = "卡x2"
HU_TYPES[13] = "吊x2"
HU_TYPES[14] = "杠上开花x2"
HU_TYPES[15] = "海底捞月x2"
HU_TYPES[16] = "捉五魁x4"
HU_TYPES[17] = "素胡x2"
HU_TYPES[18] = "混儿吊x2"
HU_TYPES[19] = "带庄闲，庄家输赢x2"
HU_TYPES[20] = "自摸x2"
HU_TYPES[21] = "点炮"
HU_TYPES[22] = "点杠-3"
HU_TYPES[23] = "明杠+3"
HU_TYPES[24] = "暗杠+6"
HU_TYPES[25] = "补杠+3"

local LCPlayerAvatar  	= require "lua/scene/commons/PlayerAvatar"
local LCMJCardImg    	= require "lua/scene/mj/MJCardImg"
local LCMJEndLayer      = require "lua/scene/mj/MJEndLayer"

local LCMJResultLayer = class( "LCMJResultLayer", function() return CCLayer:create() end )

function LCMJResultLayer:create(...)
	local ret = LCMJResultLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJResultLayer:init( deskStatusMgr,itemList )
	self.deskStatusMgr = deskStatusMgr
	self.itemList = itemList

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjResult,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	self.scale = 0.6
	local nodeMap = {
		{"titleImgLose",		"ccb_titleImgLose",			    "CCSprite"},
		{"titleImgWin",			"ccb_titleImgWin",			    "CCSprite"},
		{"prepairBtn",			"ccb_prepairBtn",			    "CCControlButton"},
		{"endBtn",				"ccb_endBtn",			        "CCControlButton"},
	}

	for i=1,4 do
		table.insert(nodeMap,{"name"..i,			"ccb_name"..i,			"CCLabelTTF"})
		table.insert(nodeMap,{"score"..i,			"ccb_scroe"..i,			"CCLabelTTF"})
		table.insert(nodeMap,{"avatarLayer"..i,		"ccb_avatar"..i,		"CCLayer"})
		table.insert(nodeMap,{"cardLayer"..i,		"ccb_cardLayer"..i,		"CCLayer"})
		table.insert(nodeMap,{"desLab"..i,			"ccb_desLab"..i,		"CCLabelTTF"})
		table.insert(nodeMap,{"totleScore"..i,		"ccb_totleScore"..i,		"CCLabelTTF"})
		table.insert(nodeMap,{"huImg"..i,			"ccb_huImg"..i,			"CCSprite"})	
		table.insert(nodeMap,{"paoImg"..i,			"ccb_paoImg"..i,		"CCSprite"})		
	end
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	table.sort(self.itemList,function(item1,item2)
		return item1.score > item2.score
	end)

	local huPos = 0
	for i=1,#self.itemList do
		local item = self.itemList[i]
		self:initItem(item,i)
		if item.isHu == 1 then
			huPos = item.pos
		end
	end

	if huPos == self.deskStatusMgr:getSelfPos() then
		self.titleImgWin:setVisible(true)
		self.titleImgLose:setVisible(false)
	else
		self.titleImgWin:setVisible(false)
		self.titleImgLose:setVisible(true)
	end

	if self.deskStatusMgr:isLastRound() then
		self.endBtn:setVisible(true)
		self.endBtn:setTouchEnabled(true)
		self.prepairBtn:setVisible(false)
		self.prepairBtn:setVisible(false)
	else
		self.endBtn:setVisible(false)
		self.endBtn:setTouchEnabled(false)
		self.prepairBtn:setVisible(true)
		self.prepairBtn:setVisible(true)
	end

	local function onTouch(eventType,x,y)
		if eventType == "ended" then
            GDebug("wz onTouch")
		end
		return true
	end
	self:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
	self:setTouchEnabled(true)

	return true
end

function LCMJResultLayer:initItem(item,index)
	local pos = item.pos
	local player = self.deskStatusMgr:getPlayerByPos(pos)
	local bankerPos = self.deskStatusMgr:getBankerPos()
	if player then
		local key = {}
		key.clipMode = "mj_head_bg.png"
		key.isMJ = true
		key.hidScore = true
		local avatar = LCPlayerAvatar:create(player.avatar,function() end,key)
		self["avatarLayer"..index]:addChild(avatar)
		self["name"..index]:setString(player.nickName)
		self["score"..index]:setString(player.id)

		if bankerPos == pos then
			avatar:setMJBanker(true)
		end
	end

	local offX = 0

	local data = LCMJCardData.new(1)
	local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
	local cellW = img:getContentSize().width * self.scale - 2

	if item.pengCards and #item.pengCards >= 3 then
		local num = #item.pengCards/3
		local j = 0
		for i=1,num do
			for k=1,3 do
				local data = LCMJCardData.new(item.pengCards[k + (i-1) * 3])
				local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
				img:setScale(self.scale) 
				img:setAnchorPoint(0,0)
				self["cardLayer"..index]:addChild(img)
				img:setPositionX(cellW*(k-1) + offX)				
				if k == 3 then offX = img:getPositionX() + cellW + 10 end
			end
		end
	end	

	if item.chiCards and #item.chiCards >= 3 then
		local num = #item.chiCards/3
		local j = 0
		for i=1,num do
			for k=1,3 do
				local data = LCMJCardData.new(item.chiCards[k + (i-1) * 3])
				local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
				img:setScale(self.scale) 
				img:setAnchorPoint(0,0)
				self["cardLayer"..index]:addChild(img)
				img:setPositionX(cellW*(k-1) + offX)				
				if k == 3 then offX = img:getPositionX() + cellW + 10 end
			end
		end
	end

	if item.gangCards and #item.gangCards >= 4 then
		local num = #item.gangCards/4
		local j = 0
		for i=1,num do
			for k=1,4 do
				local data = LCMJCardData.new(item.gangCards[k + (i-1) * 4])
				local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
				img:setScale(self.scale) 
				img:setAnchorPoint(0,0)
				self["cardLayer"..index]:addChild(img)
				img:setPositionX(cellW*(k-1) + offX)				
				if k == 4 then offX = img:getPositionX() + cellW + 10 end
			end
		end
	end

	if item.leftCards and #item.leftCards >= 1 then
		if item.isHu ~= 1 then
			local hunNum = self.deskStatusMgr:getHunCard()
			item.leftCards = LCMJUtil.sortCardNumList(item.leftCards,hunNum)	
		end

		for i=1,#item.leftCards do
			local data = LCMJCardData.new(item.leftCards[i])
			local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
			img:setScale(self.scale) 
			img:setAnchorPoint(0,0)
			self["cardLayer"..index]:addChild(img)
			img:setPositionX(cellW*(i-1) + offX)	
			if i == #item.leftCards then offX = img:getPositionX() + cellW + 10 end
		end
	end

	if item.drawCard ~= 0 then
		local data = LCMJCardData.new(item.drawCard)
		local img = LCMJCardImg.new(data,eMJPType.bottom,eMJImgType.pair)
		img:setScale(self.scale) 
		img:setAnchorPoint(0,0)
		self["cardLayer"..index]:addChild(img)
		img:setPositionX(offX)
	end

	self["totleScore"..index]:setString(item.score)
	if tonumber(item.score) < 0 then
		self["totleScore"..index]:setColor(_APPCOLOR_HEX("971B20"))
		self["totleScore"..index]:setString(item.score)
	else
		self["totleScore"..index]:setString("+"..item.score)
	end

	if item.isHu == 1 then
		self["huImg"..index]:setVisible(true)
	else
		self["huImg"..index]:setVisible(false)
	end

	local str = ""

	for i=1,#item.huTypes do
		local hu = item.huTypes[i]
		if HU_TYPES[hu] then
			str = str .. HU_TYPES[hu] .. "  " 
		end
	end

	self["desLab"..index]:setString(str)

end

function LCMJResultLayer:ccb_prepairBtnClicked()
	 LCMJSendMgr.sendPrepare()
	 self:removeFromParentAndCleanup(true)
end

function LCMJResultLayer:ccb_endGameBtnClicked()
	local msg = self.deskStatusMgr.settlementMsg
	if msg then
		local endLayer = LCMJEndLayer:create(self.deskStatusMgr,msg.SettleList)
	    self:addChild(endLayer)
	end
end

return LCMJResultLayer
