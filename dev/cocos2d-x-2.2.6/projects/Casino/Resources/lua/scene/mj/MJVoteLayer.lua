
local LCMJVoteLayer = class( "LCMJVoteLayer", function() return CCLayer:create() end )

function LCMJVoteLayer:create(...)
	local ret = LCMJVoteLayer.new()
	if not ret:init(...) then return  nil end
	return ret
end

function LCMJVoteLayer:init(deskStatusMgr)
	self.deskStatusMgr = deskStatusMgr

	local function onTouch(eventType,x,y)
		if eventType == "ended" then
            GDebug("wz onTouch")
		end
		return true
	end
	self:registerScriptTouchHandler(onTouch, false, eTouchPriority.maskLayer, true)
	self:setTouchEnabled(true)

	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.mjVote,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
	self:addChild(layer)

	local nodeMap = {
		{"refuseBtn",		"ccb_refuseBtn",			"CCControlButton"},
		{"agreeBtn",		"ccb_agreeBtn",			    "CCControlButton"},
		{"timeLab",			"ccb_timeLab",			    "CCLabelTTF"},
	}

	for i=1,4 do
		table.insert(nodeMap,{"lab"..i,			"ccb_playerLab"..i,		"CCLabelTTF"})
	end

	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)

	return true
end

function LCMJVoteLayer:ccb_agreeBtnClicked()
	LCMJSendMgr.sendVoteClose(eMJVoteType.agree)
end

function LCMJVoteLayer:ccb_refuseBtnClicked()
	LCMJSendMgr.sendVoteClose(eMJVoteType.refuse)
end

function LCMJVoteLayer:updateByMsg(msg)
	local firstPos 	= msg.firstPos
	local infoList  = {}
	local result    = eMJVoteType.noVote 
	local name 		= ""

	for i=1,#msg.voteList do
		local info = msg.voteList[i]
		if info.action == eMJVoteType.refuse then
			local pos = info.pos
			local player = self.deskStatusMgr:getPlayerByPos(pos)

			return eMJVoteType.refuse, player.nickName
		end

		if info.pos == firstPos then
			table.insert(infoList,1,info)
		else
			table.insert(infoList,info)
		end
	end
	
	for i=1,#infoList do
		local info = infoList[i]
		local pos = info.pos
		local player = self.deskStatusMgr:getPlayerByPos(pos)

		self:setLab(i,player.nickName,info.action)

		if info.action ~= eMJVoteType.noVote and pos == self.deskStatusMgr:getSelfPos() then
			self.agreeBtn:setEnabled(false)
			self.refuseBtn:setEnabled(false)
		end
	end

	self:setTime(msg.leftTime)

	return result,name
end

function LCMJVoteLayer:setLab(index,nickName,action)
	local str = ""
	if index == 1 then
		str = string.format("玩家%s申请解散房间",nickName)
	else
		if action == 1 then
			str = string.format("%s [已同意]",nickName)
			self["lab"..index]:setColor(_APPCOLOR_HEX("1A687E"))
		else
			str = string.format("%s [等待选择]",nickName)
			self["lab"..index]:setColor(_APPCOLOR_HEX("6F3A02"))
		end
	end
	self["lab"..index]:setString(str)
end

function LCMJVoteLayer:setTime(time)
	if self.schedule then return end

	local function changeTime()
		if self.time < 0 then return end
		self.timeLab:setString(self.time)
		self.time = self.time - 1
	end

	self.time = time
	changeTime()
	self.schedule = schedule(self, function() changeTime() end, 1)
end

return LCMJVoteLayer
