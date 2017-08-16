local LCTableViewCell = require("lua/scene/sceneUtils/TableViewCell")
local LCRichLabel = require("lua/scene/sceneUtils/RichLabel")


local avatarScale=0.18
local tb   = table
local str  = string
local xstr = xstr

local LCBroadCell = class("LCBroadCell",LCTableViewCell)

function LCBroadCell:create(...)
	local ret = LCBroadCell.new()
	if not ret:init(...) then return nil end
	return ret
end
function LCBroadCell:init(key)
	if not LCTableViewCell.init(self) then return false end
	if not key then key = {} end
	CC_SYNTHESIZE(self,"callback",key.callback)
	CC_SYNTHESIZE(self,"boardData", key.boardData)
	CC_SYNTHESIZE(self,"cellSize", key.cellSize or eBroadCellSize)
	CC_SYNTHESIZE(self,"gapLeft", key.gapLeft or 5)

	-- const
	CC_SYNTHESIZE(self,"fontSize", key.fontSize or 25)
	-- ui releated

	CC_SYNTHESIZE(self,"avatar", nil)
	CC_SYNTHESIZE(self,"cellContentSize",self.cellSize)

	CC_SYNTHESIZE(self,"contentLabel3", nil)

	-- ui create
	self.avatar =  LCPlayerAvatar:create(eDefaultAvatar.min,function(avatar,url)
		self:infoCallback()
	end, {isNotClip=false})
	self.avatar:setAvatarScale(avatarScale)
    self:addChild(self.avatar)

	local line = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_chat_cutline)
	--line:setAnchorPoint(ccp(0.5, 0))
	line:setPreferredSize(CCSizeMake(self.cellSize.width, 2))
	line:setPosition(self.cellSize.width/2, 0)
	self:addChild(line)

	-- update with data
	self:configWithData(self.boardData)

	return true
end
function LCBroadCell:infoCallback( data )
	GDebug("info clicked")
	if self.callback then self.callback( data or self.boardData ) end
end

function LCBroadCell:setDimensions(size)
	if not size then return end
	self.cellSize = size
	if self.contentLabel3 then self.contentLabel3:setDimensions(size) end
	self:configWithData(self.boardData)
end

function LCBroadCell:configWithData(boardData)
	if not boardData then return end
	self.boardData = boardData

	self.avatar:setVisible(false)
	self.avatar:setTouchEnabled(false)

	local broadType    = boardData.broadTypes
	local title = eBroadName[broadType] or _("[ 游戏公告 ]  ")
	local titleColor   = "77f4ff"
	local nameColor    = "77f4ff"
	local contentColor = "C8C8C8"
	if broadType==eBroadcastType.active then titleColor="00f450"
	elseif broadType==eBroadcastType.game
	or broadType==eBroadcastType.common then
		titleColor="ffcb67"
	end
	local avatarSize
	local leftDis = 0
	local dimensions=self.cellSize
	local textShow=""
	-- relayout
	if broadType == eBroadcastType.system or broadType == eBroadcastType.active then
		local part1 = tb.concat({"[fontColor=", titleColor, " fontSize=", self.fontSize, "]", title, "[/fontColor]"})
		local part2 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", boardData.text, "[/fontColor]"})
		textShow    = tb.concat({part1, part2})
	elseif broadType == eBroadcastType.game then
		local roomType = boardData.game
		local gameName = eGameName[roomType] and eGameName[roomType].name or _("神秘游戏")
		local text2 = str.format(_("在%s中赢取%s筹码！"), gameName or "", _NICE_MONEY3(boardData.winGold or 0))
		if DEF_DESGIN_LANGUAGE == kLanguageEnglish then
			text2 = string.format(_("在%s中赢取%s筹码！"), _NICE_MONEY3(boardData.winGold or 0), gameName or "")
		end
		local part1 = tb.concat({"[fontColor=", titleColor, " fontSize=", self.fontSize, "]", title, "[/fontColor]"})
		local part2 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", _("恭喜"), "[/fontColor]"})
		local part3 = tb.concat({"[fontColor=", nameColor, " fontSize=", self.fontSize, " callback=true]", boardData.nickName, "[/fontColor]"})
		local part4 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", text2, "[/fontColor]"})
		textShow    = tb.concat({part1, part2, part3, part4})
	elseif broadType == eBroadcastType.player then
	--	GDebug("--------boardData.nickName="..boardData.nickName)
		self.avatar:setVisible(true)
		self.avatar:setTouchEnabled(true)
		self.avatar:updateURL(boardData.avatar)
		local originAvatarSize = self.avatar:getContentSize()
		avatarSize = CCSizeMake(originAvatarSize.width * avatarScale, originAvatarSize.height * avatarScale)
		local avatarPosX, avatarPosY = self.gapLeft - originAvatarSize.width * (1-avatarScale)/2 , -(1-avatarScale)*originAvatarSize.height/2+10
		self.avatar:setPosition(ccp(avatarPosX,avatarPosY))

		local part1 = tb.concat({"[fontColor=", nameColor, " fontSize=", self.fontSize, " callback=true]", boardData.nickName, ":", "[/fontColor]"})
		local part2 = tb.concat({"[fontColor=FFFFFF fontSize=", self.fontSize, "]", boardData.text, "[/fontColor]"})
		if boardData.types and boardData.types==eChatType.icon1 then
			part2 = tb.concat({"[image=", boardData.text, " scale=0.5][/image]"})
		end

		leftDis     = (avatarSize.width+self.gapLeft)
		dimensions  = CCSizeMake(self.cellSize.width-avatarSize.width-self.gapLeft*2, self.cellSize.height)

		textShow    = tb.concat({part1, part2})
	elseif broadType == eBroadcastType.common then
		local text  = boardData.text or ""
		local name  =  boardData.nickName or ""

		local startIndex, endIndex   = str.find(text, "{{{")
		local startIndex1, endIndex1 = str.find(text, "}}}")

		local part1 = tb.concat({"[fontColor=", titleColor, " fontSize=", self.fontSize, "]", title, "[/fontColor]"})
		local part2 = ""
		local part3 = ""
		local part4 = ""
		if startIndex then
			local content1=str.sub(text, 1, startIndex-1)
			part2 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", content1, "[/fontColor]"})
			part3 = tb.concat({"[fontColor=", nameColor, " fontSize=", self.fontSize, " callback=true]", name, "[/fontColor]"})
		end
		if endIndex1 then
			local content2=str.sub(text, endIndex1+1, #text)
			part4 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", content2, "[/fontColor]"})
		end
		if not startIndex and not endIndex1 then
			part1 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", text, "[/fontColor]"})
			textShow=part1
		else
			textShow = tb.concat({part1, part2, part3, part4})
		end
	elseif broadType == eBroadcastType.noTitle then
		part1 = tb.concat({"[fontColor=", contentColor, " fontSize=", self.fontSize, "]", boardData.text, "[/fontColor]"})
		textShow=part1
	end
	if not self.contentLabel3 then
		local key = { text = textShow,
			dimensions = dimensions}
		self.contentLabel3 = LCRichLabel:create(key)
		self.contentLabel3:registerScriptTouchHandler(function (tables)
			local tables=tables or {}
			local dic = tables.dic or {}
			local playerID=dic.playerID
			local data=clone(self.boardData)
			if playerID then
				data.playerID = playerID
			end
			self:infoCallback(data)
		end)
		self:addChild(self.contentLabel3)
	else
		self.contentLabel3:setDimensions(dimensions)
		self.contentLabel3:setString(textShow)
	end
	local content=self.contentLabel3:getContentSize()
	local lineHeight=self.contentLabel3:getOneLineHeight()
	local tHeight  =content.height
	local maxHeight
	if broadType == eBroadcastType.player and avatarSize then
		maxHeight=avatarSize.height
		if content.height>maxHeight then
			maxHeight=content.height
			tHeight=maxHeight-lineHeight/2
	--		self.avatar:setPositionY(maxHeight/2-avatarSize.height)
		else
			tHeight=tHeight
			maxHeight=avatarSize.height
		end
	else
		local lines=self.contentLabel3:getLines()
		if lines>1 then
			maxHeight=content.height
			tHeight=maxHeight-lineHeight/2
		else
			tHeight=tHeight
			maxHeight=tHeight
		end
	end
	tHeight=tHeight+lineHeight/2
	maxHeight = maxHeight+lineHeight/2
	local width=content.width
	if avatarSize then width=width+avatarSize.width end
	--self.contentLabel3:setVisible(true)
	self.contentLabel3:setPosition(ccp((self.gapLeft+10)*(1-avatarScale)+leftDis, tHeight))
	self:changeWidthAndHeight(content.width, maxHeight)
	self.cellContentSize=(CCSizeMake(width, maxHeight))
end

function LCBroadCell:getHeight()
	local height=self.cellContentSize.height
	return height
end

return LCBroadCell
