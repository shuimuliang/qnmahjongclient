local LCPlayerAvatar  	= require "lua/scene/commons/PlayerAvatar"
local LCMJCardImg     = require("lua/scene/mj/MJCardImg")

local LCMJEndLayer = class( "LCMJEndLayer", function() return CCLayer:create() end )

function LCMJEndLayer:create(...)
	local ret = LCMJEndLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJEndLayer:init( deskStatusMgr,itemList )
	self.deskStatusMgr = deskStatusMgr
	self.itemList = itemList

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjEndGame,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	self.scale = 0.6
	local nodeMap = {
	}

	for i=1,4 do
		table.insert(nodeMap,{"name"..i,			"ccb_name"..i,			"CCLabelTTF"})
		table.insert(nodeMap,{"score"..i,			"ccb_scroe"..i,			"CCLabelTTF"})
		table.insert(nodeMap,{"avatarLayer"..i,		"ccb_avatar"..i,		"CCLayer"})
		table.insert(nodeMap,{"desLab"..i,			"ccb_desLab"..i,		"CCLabelTTF"})
		table.insert(nodeMap,{"addScore"..i,		"ccb_addScore"..i,		"CCLabelTTF"})
		table.insert(nodeMap,{"huImg"..i,			"ccb_huImg"..i,			"CCSprite"})		
		table.insert(nodeMap,{"paoImg"..i,			"ccb_paoImg"..i,		"CCSprite"})		
	end
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	self.maxPao = 0

	table.sort(self.itemList,function(item1,item2)
		return item1.score > item2.score
	end)

	local tempT = clone(self.itemList)
	table.sort(tempT,function(item1,item2)
		return item1.dianPaoCnt > item2.dianPaoCnt
	end)
	self.maxPao = tempT[1].dianPaoCnt

	for i=1,#self.itemList do
	 	local item = self.itemList[i]
	 	self:initItem(item,i)
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

function LCMJEndLayer:initItem(item,index)
	local pos = item.pos
	local player = self.deskStatusMgr:getPlayerByPos(pos)
	if player then
		local key = {}
		key.clipMode = "mj_head_bg.png"
		key.isMJ = true
		key.hidScore = true
		local avatar = LCPlayerAvatar:create(player.avatar,function() end,key)
		self["avatarLayer"..index]:addChild(avatar)
		self["name"..index]:setString(player.nickName)
		self["score"..index]:setString(player.id)
	end

	self["addScore"..index]:setString(item.score)
	if tonumber(item.score) < 0 then
		self["addScore"..index]:setColor(_APPCOLOR_HEX("971B20"))
		self["addScore"..index]:setString(item.score)
		self["huImg"..index]:setVisible(false)
	elseif tonumber(item.score) > 0 then 
		self["addScore"..index]:setString("+"..item.score)
		self["huImg"..index]:setVisible(true)
	else
		self["addScore"..index]:setString("+"..item.score)
		self["huImg"..index]:setVisible(false)		
	end

	local str = "自摸次数： %d      接炮次数： %d      点炮次数： %d  "
	str = string.format(str,item.ziMoCnt,item.jiePaoCnt,item.dianPaoCnt)
	self["desLab"..index]:setString(str)

	if item.dianPaoCnt == self.maxPao and item.dianPaoCnt ~= 0 then
		self["paoImg"..index]:setVisible(true)
	else
		self["paoImg"..index]:setVisible(false)
	end

end

function LCMJEndLayer:ccb_backBtnClicked()
    LCSceneTransit:replaceScene(eSceneName.main)
end

function LCMJEndLayer:ccb_screenShootClicked()
    local succ = APP_SCREEN_CAPTURE()
    if succ then
        LCToastHUD.new("截屏成功！已保存到您的相册中"):showInScene()
    end
end


return LCMJEndLayer
