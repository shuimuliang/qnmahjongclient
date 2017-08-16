
local LCRichLabel = class("LCRichLabel", function(...)
    local node = CCNode:create(...)
    return node
end)

--[[ 公共方法
	创建方法 create

	set:
	设置文字方法 setLabelString
	设置尺寸方法 setDimensions

	get:
	获得文字实际占用尺寸 getLabelSize
]]--

--播放状态 1 表示未开始 2 表示播放中 3 表示已经播放完毕 
--LCRichLabel._labelStatus = 1

--创建方法
--[[
	local params = {
		fontName = "Arial",
		enableStroke = "1",
		fontSize = 30,
		fontColor = ccc3(255, 0, 0),
		dimensions = CCSize(300, 200),
		text = [fontColor=f75d85 fontSize=20]hello world[/fontColor],
		key = {}
	}

	text 目前支持参数
		文字
		fontName  : font name
		fontSize  : number
		fontColor : 十六进制
		enableStroke = "1",
		callback  :  bool

		图片
		image : "xxx.png"
		scale : number
		callback  :  bool

		数据(_key)
		格式要求[xx=xx yy=yy][/xx], 结尾格式必须与开头第一个=前名相同,即xx
]]--
function LCRichLabel:create(params)
	if not params then return nil end
	local ret = LCRichLabel.new()
	ret:init_(params)
	return ret
end

local str  = string
local tb   = table
local xstr = xstr

local eSizeTb = eRichLocalSize

function LCRichLabel:init_(params)
	--如果text的格式指定字体则使用指定字体，否则使用默认字体
	--大小和颜色同理
	CC_SYNTHESIZE(self,"_fontName", params.fontName or DEF_SYSTEM_FONT_NAME)--默认字体
	CC_SYNTHESIZE(self,"_fontSize", params.fontSize or 30)--默认大小
	CC_SYNTHESIZE(self,"_fontColor", params.fontColor or ccc3(255, 255, 255))--默认白色
	CC_SYNTHESIZE(self,"_dimensions", params.dimensions or CCSize(0, 0))--默认无限扩展，即沿着x轴往右扩展
	CC_SYNTHESIZE(self,"text", params.text)
	CC_SYNTHESIZE(self,"_key", params.key or {})--数据
	CC_SYNTHESIZE(self,"stroke", params.enableStroke)
	CC_SYNTHESIZE(self,"_containLayer", nil)
	CC_SYNTHESIZE(self,"_spriteArray", nil)--精灵数组
	CC_SYNTHESIZE(self,"_textStr", "")
	CC_SYNTHESIZE(self,"_maxWidth", nil)
	CC_SYNTHESIZE(self,"_maxHeight", nil)
	CC_SYNTHESIZE(self,"callbackTable", {})
	CC_SYNTHESIZE(self,"lineHeight", nil)
	CC_SYNTHESIZE(self,"spriteTables", {})

	--装文字和图片精灵
	self._containLayer = CCLayerColor:create()
	self:addChild(self._containLayer)

    self:setLabelString(self.text)
end

local specialTable={
	{ch="，", en=","},
	{ch="。", en="."},
	{ch="？", en="?"},
	{ch="！", en="!"},
	{ch="（", en="("},
	{ch="）", en=")"},
	{ch="、", en="."},
	{ch="：", en=":"},
	{ch="；", en=";"},
}

function LCRichLabel:setString(text)
	--local t1 = getTickCount()
	self:setLabelString(text)
	--local t2 = getTickCount()
	--GDebug("------disT1="..(t2-t1))
end

--设置text
function LCRichLabel:setLabelString(text)
	if not text then return end
	--GDebug("-----text="..text)
	text = tostring(text)
	for _,v in pairs(specialTable) do
		text = str.gsub(text, v.ch, v.en)		
	end
	
	if self._textStr == text then
		return 									--相同则忽略
	end

	if self._textStr then 						--删除之前的string
		self._spriteArray = nil
		self._containLayer:removeAllChildrenWithCleanup(true)
	end

	--self._labelStatus = 1 						--未开始
	--self:removeNodeEventListenersByEvent(NODE_ENTER_FRAME_EVENT)
	--self:unscheduleUpdate() --init

	self._textStr = text

	--转化好的数组
	local parseArray = self:parseString_(text)

	self.parseArray=parseArray

	self:createStart()
	
	eRichLocalSize = eSizeTb
end

function LCRichLabel:createStart()
	local parseArray=self.parseArray
	if not parseArray then return end
	--创建精灵
	local widthArray, heightArray, callbackTable = self:maxSprite_(parseArray, false)
	
	self:reMax(widthArray, heightArray)
end

--设置尺寸
function LCRichLabel:setDimensions(dimensions)
	if not dimensions then return end
	local width  = dimensions.width
	local height = dimensions.height
	if not width or not height or
	 (width==self._dimensions.width and height==self._dimensions.height) then
		return
	end
	self._containLayer:setContentSize(dimensions)
	self._dimensions = dimensions
end

--获得label尺寸
function LCRichLabel:getLabelSize()
	local width  = self._maxWidth or 0
	local height = self._maxHeight or 0
	return CCSizeMake(width, height)
end

function LCRichLabel:getContentSize()
	return self:getLabelSize()
end

function LCRichLabel:getChildByIndex(index)
	local _spriteArray = self._spriteArray or {}
	if not index or index>#_spriteArray then return end
	return _spriteArray[index]
end

function LCRichLabel:getSpriteByIndex(index)
	local spriteTables=self.spriteTables or {}
	if not index  or index>#spriteTables then return end
	return spriteTables[index]
end

function LCRichLabel:getOneLineHeight()
	local height=1
	local arr = self._spriteArray or {}
	if #arr>=1 then
		height=arr[1]:getContentSize().height
	end
	return height
end

function LCRichLabel:getLines()
	return self.rowIndex or 0
end

function LCRichLabel:getSizeOf_(sizeArray)
	local widthArr = {} --宽度数组
	local heightArr = {} --高度数组

	--精灵的尺寸
	for i, size in pairs(sizeArray) do
		widthArr[i] = size.width
		heightArr[i] = size.height
	end
	
	return widthArr, heightArr
end

--获得每个精灵的尺寸
function LCRichLabel:getSizeOfSprites_(spriteArray)
	local widthArr = {} --宽度数组
	local heightArr = {} --高度数组

	--精灵的尺寸
	for i, sprite in pairs(spriteArray) do	
		local rect   = sprite:boundingBox()
		widthArr[i]  = rect.size.width
		heightArr[i] = rect.size.height
	end
	return widthArr, heightArr

end

--获得每个精灵的位置
function LCRichLabel:getPointOfSprite_(widthArr, heightArr, dimensions)
	local totalWidth = dimensions.width
	local totalHight = dimensions.height

	local maxWidth   = 0
	local maxHeight  = 0

	local spriteNum  = #widthArr
	local language   = DEF_DESGIN_LANGUAGE

	--从左往右，从上往下拓展
	local curX = 0 								--当前x坐标偏移
	if language == kLanguageArabic then
		totalWidth    = totalWidth-10
		curX          = totalWidth
	end

	local curIndexX   = 1 						--当前横轴index
	local curIndexY   = 1 						--当前纵轴index

	local pointArrX   = {} 						--每个精灵的x坐标

	local rowIndexArr = {} 						--行数组，以行为index储存精灵组
	local indexArrY   = {} 						--每个精灵的行index

	--计算宽度，并自动换行
	for i, spriteWidth in pairs(widthArr) do
		local nexX = curX + spriteWidth
		if language == kLanguageArabic then
			nexX = curX - spriteWidth
		else
			nexX = curX + spriteWidth
		end
		local pointX
		local rowIndex = curIndexY

		local halfWidth = spriteWidth * 0.5
		if language == kLanguageArabic then
			if nexX < -spriteWidth+10 and totalWidth ~= 0 then --超出界限了
				pointX = totalWidth
				if curIndexX == 1 then 			--当前是第一个，
					curX = totalWidth 			-- 重置x
				else --不是第一个，当前行已经不足容纳
					rowIndex = curIndexY + 1 	--换行
					curX = totalWidth
				end
				curIndexX = 1 					--x坐标重置
				curIndexY = curIndexY + 1 		--y坐标自增
			else
				pointX = curX
				if curIndexX ~=1 then pointX=curX-halfWidth end --精灵坐标x
				curX = pointX - halfWidth 		--精灵最右侧坐标
				curIndexX = curIndexX + 1
			end
		else
			if nexX > totalWidth and totalWidth ~= 0 then --超出界限了
				pointX = halfWidth
			--[[if curIndexX == 1 then 		    --当前是第一个，
					curX  = 0					-- 重置x
				else 							--不是第一个，当前行已经不足容纳--]]
				rowIndex  = curIndexY + 1 	    --换行
				curX      = spriteWidth
				--end
				curIndexX = 1 					--x坐标重置
				curIndexY = curIndexY + 1 		--y坐标自增
			else
				pointX = curX + halfWidth 		--精灵坐标x
				curX = pointX + halfWidth 		--精灵最右侧坐标
				curIndexX = curIndexX + 1
			end
		end
		pointArrX[i] = pointX 					--保存每个精灵的x坐标

		indexArrY[i] = rowIndex 				--保存每个精灵的行

		local tmpIndexArr = rowIndexArr[rowIndex]

		if not tmpIndexArr then 				--没有就创建
			tmpIndexArr = {}
			rowIndexArr[rowIndex] = tmpIndexArr
		end
		tb.insert(tmpIndexArr, i)				--保存相同行对应的精灵
		
		if curX > maxWidth then
			maxWidth = curX
		end
	end

	local curY 			= 0
	local rowHeightArr = {} 					--每一行的y坐标

	--计算每一行的高度
	for i, rowInfo in pairs(rowIndexArr) do
		local rowHeight = 0
		for j, index in pairs(rowInfo) do 		--计算最高的精灵
			local height = heightArr[index]
			if height > rowHeight then
				rowHeight = height
			end
		end
		local pointY = curY + rowHeight * 0.5 	--当前行所有精灵的y坐标（正数，未取反）
		tb.insert(rowHeightArr, -pointY)     	--从左往右，从上到下扩展，所以是负数
		curY = curY + rowHeight 				--当前行的边缘坐标（正数）

		if curY > maxHeight then
			maxHeight = curY
		end
	end

	self._maxWidth   = maxWidth
	self._maxHeight  = maxHeight

	local pointArrY  = {}

	for i = 1, spriteNum do
		local indexY = indexArrY[i] 			--y坐标是先读取精灵的行，然后再找出该行对应的坐标
		local pointY = rowHeightArr[indexY]
		pointArrY[i] = pointY
	end
	self._containLayer:changeWidthAndHeight(self._maxWidth, self._maxHeight)
	return pointArrX, pointArrY
end

function LCRichLabel:reMax(widthArr, heightArr)
	local newArr = self:getPointOfSprite2_(widthArr, self.callbackTable, heightArr, self._dimensions)

	--创建精灵
	local spriteArray , callbackTable= self:createSprite_(newArr, newArr)

	self:adjustPosition_()
end

--调整位置（设置文字和尺寸都会触发此方法）
function LCRichLabel:adjustPosition_()

	local spriteArray = self._spriteArray

	if not spriteArray then --还没创建
		return
	end

	--获得每个精灵的宽度和高度
	local widthArr  = self.widthArr
	local heightArr = self.heightArr
	if not heightArr or #heightArr<=0 or not widthArr or #widthArr<=0 then
		self.widthArr, self.heightArr = self:getSizeOfSprites_(spriteArray)
	end

	--获得每个精灵的坐标
	local pointArrX, pointArrY = self:getPointOfSprite_(widthArr, heightArr, self._dimensions)

	local index   = 0
	local arrXLen = #pointArrX
	local arrYLen = #pointArrY
	local sprLen  = #spriteArray
	
	local function cal()
		index = index+1
		if index>sprLen then return end
		local sprite=spriteArray[index]
		local posX, posY = 0, 0
		if arrXLen>=index and arrYLen>=index then posX=pointArrX[index] posY=pointArrY[index] end
		sprite:setPosition(posX, posY)
		if not self.lineHeight then
			local content=sprite:getContentSize()
			self.lineHeight=content.height
		end
		cal()
	end

	cal()
end

--字符串拼接
function LCRichLabel:getPointOfSprite2_(widthArr, dicArr, heightArr, dimensions)
	local totalWidth = dimensions.width
	local totalHight = dimensions.height
	local language   = DEF_DESGIN_LANGUAGE
	local maxWidth   = 0
	local maxHeight  = 0

	--从左往右，从上往下拓展
	local curX = 0 --当前x坐标偏移
	local curIndexX = 1 --当前横轴index
	local curIndexY = 1 --当前纵轴index
	local newArrays = {}
	local wArrNum   = #widthArr
	
	--计算宽度，并自动换行
	for i, spriteWidth in pairs(widthArr) do
		local nexX = curX + spriteWidth
		local pointX
		local rowIndex  = curIndexY
		local halfWidth = spriteWidth*0.5
		local newArr=dicArr[i]
		local newDic=newArr.dic
		if nexX >= totalWidth and totalWidth ~= 0 then --超出界限了
			pointX = halfWidth
			if curIndexX == 1 then --当前是第一个，
				if language == kLanguageArabic then
					if wArrNum <= 1 then curX = spriteWidth end
				else
					curX = 0-- 重置x
				end
			else --不是第一个，当前行已经不足容纳
				rowIndex = curIndexY + 1 --换行
				curX 	 = spriteWidth
			end
			curIndexX = 1 --x坐标重置
			curIndexY = curIndexY + 1 --y坐标自增
			if i>1 then
				local last    = dicArr[i-1]
				local lastDic = last.dic
				if lastDic.type == 1 then
					lastDic.text=last.word
				else
					lastDic.image=last.word
				end
				lastDic.pass=true
				tb.insert(newArrays, clone(lastDic))
			end
		else
			if i>1 then
				local last    = dicArr[i-1]
				local lastDic = last.dic
				if lastDic.type == newDic.type
					and lastDic.type     	== 1
					and lastDic.fontName 	== newDic.fontName
					and lastDic.fontSize 	== newDic.fontSize
					and lastDic.fontColor 	== newDic.fontColor
					and lastDic.enableStroke== newDic.enableStroke
					and lastDic.callback 	== newDic.callback then
					 newArr.word=tb.concat({last.word, newArr.word})
				--	GDebug("-----last.word="..last.word)
				--	GDebug("-----newArr.word="..newArr.word)
				elseif not lastDic.pass then
					lastDic.pass=true
					if lastDic.type == 1 then
						lastDic.text=last.word
					else
						lastDic.image=last.word
					end
					tb.insert(newArrays, clone(lastDic))
				end
			end
			pointX = curX + halfWidth --精灵坐标x
			curX = pointX + halfWidth --精灵最右侧坐标
			curIndexX = curIndexX + 1
		end
		if i == wArrNum then
			newDic.pass=true
			if newDic.type == 1 then
				newDic.text=newArr.word
			else
				newDic.image=newArr.word
			end
			tb.insert(newArrays, clone(newDic))
		end

		if curX > maxWidth then
			maxWidth = curX
		end
		if i == wArrNum then
			self.rowIndex=rowIndex --总行数
		end
	end
	
	--GDebug("-----len="..(#newArrays))
	return newArrays
end

--创建精灵
function LCRichLabel:createSprite_(parseArray, nameArray)
	local spriteArray 	= {}
	local callbackTable = {}
	local widthArr		= {}
	local heightArr		= {}
	local spriteTables	= {}

	local function addCallback(dic, index, word, node)
		local item={node=node, word=word, name=nameArray[index], dic=dic}
		tb.insert(callbackTable, item)
	end

	for i, dic in pairs(parseArray) do
		local textArr = dic.text
		if textArr and #textArr > 0 then --创建文字
			local fontName 		= dic.fontName or self._fontName
			local fontSize 		= dic.fontSize or self._fontSize
			local fontColor 	= dic.fontColor or self._fontColor
			local stroke 		= dic.enableStroke or self.stroke
			
			dic.type = 1
			dic.text = textArr
			
			local label = CCLabelTTF:create(textArr, fontName, fontSize)
			if stroke then
				label:enableStroke(fontColor or self._fontColor, stroke, true)
			end
			label:setColor(fontColor)
			self._containLayer:addChild(label)
			
			tb.insert(spriteArray, label)
			local rect = label:getContentSize()
			tb.insert(widthArr, rect.width)
			tb.insert(heightArr, rect.height)
			addCallback(dic, i, textArr, label)
		elseif dic.image then
			local sprite = CCSprite:createWithSpriteFrameName(dic.image)
			if sprite then
				local scale = dic.scale or 1
				sprite:setScale(scale)
				self._containLayer:addChild(sprite)
				
				tb.insert(spriteArray, sprite)
				local rect = sprite:boundingBox()
				tb.insert(widthArr, rect.size.width)
				tb.insert(heightArr, rect.size.height)
				tb.insert(spriteTables, sprite)
			end
			dic.type=2
			addCallback(dic, i, dic.image, sprite)
		else
			self._key = dic
		end
	end

	self._spriteArray  = spriteArray
	self.callbackTable = callbackTable
	self.spriteTables  = spriteTables

	self.widthArr  = widthArr
	self.heightArr = heightArr

	return spriteArray, callbackTable
end

function LCRichLabel:isCommonSp(text)
	for _,v in pairs(specialTable) do
		if text == v.en then return true end
	end
	return false
end

function LCRichLabel:getLabSize(dic, text)
	if not dic then return end
	local text     = text or ""
	local nText	   = ""
	local fontName = dic.fontName or self._fontName
	local fontSize = dic.fontSize or self._fontSize
	local stroke   = dic.enableStroke or self.stroke
	
	local isChinese = xstr.isChinese(text)
	local isalnum   = false
	local isSpecial = false
	local ret
	if isChinese then
		if not eSizeTb.ch[fontSize] then
			ret=CCSizeMake(fontSize, fontSize+10)
		end
		ret=eSizeTb.ch[fontSize]
	end
	
	if not ret then
		isalnum = xstr.isalnum(text)
		if isalnum then 
			if not eSizeTb.en[fontSize] then
				ret=CCSizeMake(fontSize/2, fontSize+10)
			end		
			ret=eSizeTb.en[fontSize] 
		end
	end
	
	if not ret then
		nText = xstr.trim(text) or ""
		if nText~="" then
			local key=tb.concat({fontSize, text}) 
			ret=eSizeTb.sp[key]
		end
	end

	if not ret then
		local label = CCLabelTTF:create(text, fontName, fontSize)
		if stroke then
			label:enableStroke(fontColor or self._fontColor, stroke, true)
		end
		ret=label:getContentSize()
		local item=CCSizeMake(ret.width, ret.height)
		if isChinese then
			eSizeTb.ch[fontSize]=item
			--GDebug("---11")
		elseif isalnum then
			eSizeTb.en[fontSize]=item
		--	GDebug("---22")
		elseif nText~="" then
			local key=tb.concat({fontSize,text}) 
			eSizeTb.sp[key]=item
		end
	end

	return ret
end

function LCRichLabel:maxSprite_(parseArray, add)
	local spriteArray   = {}
	local widthArray    = {}
	local heightArray   = {}
	local callbackTable = {}

	local function addCallback(dic, word, size)
		local item = {size=size, word=word, name=word, dic=dic}
		tb.insert(callbackTable, item)
	end

	for i, dic in pairs(parseArray) do
		local dic=clone(dic)
		local text = dic.text
		if text and #text~=0 then --创建文字
			dic.type=1
			dic.pass=false
			--local size=getLabSize(dic, textArr[1])
			local textArr=self:formatText2String_(text) or {}
			for j, word in pairs(textArr) do
				local size=self:getLabSize(dic, word)
				tb.insert(spriteArray, size)
				tb.insert(widthArray, size.width)
				tb.insert(heightArray, size.height)
				dic.text=word
				addCallback(dic, word, size)
			end
			--GDebug("-----dic.text="..dic.text)
		elseif dic.image then
			local imgName=dic.image
			local scale = dic.scale or 1
			if not eSizeTb.img.imgName then
				local sprite = CCSprite:createWithSpriteFrameName(dic.image)
				local content=sprite:getContentSize()
				eSizeTb.img.imgName = CCSizeMake(content.width*scale, content.height*scale)
			end
			local size=eSizeTb.img.imgName
			tb.insert(spriteArray, size)
			tb.insert(widthArray, size.width)
			tb.insert(heightArray, size.height)
			dic.type=2
			addCallback(dic, dic.image, size)
		--	GDebug("-----image")
		else
			self._key = dic
		end
	end
	--print("callbackTable len="..#callbackTable)

	self.callbackTable = callbackTable
	return widthArray, heightArray, callbackTable
end

--将字符串转换成一个个字符
function LCRichLabel:formatString_(parseArray)
	for i,dic in pairs(parseArray) do
		local text = dic.text
		if text then
			local textArr = self:stringToChar_(text)
			dic.textArray = textArr
		end
	end
end

function LCRichLabel:formatText2String_(text)
	if text then
		local textArr = self:stringToChar_(text)
		return textArr
	end
	return {}
end

--文字解析，按照顺序转换成数组，每个数组对应特定的标签
function LCRichLabel:parseString_(str)
	if not str then return {} end
	
	local clumpheadTab = {} -- 标签头
	--作用，取出所有格式为[xxxx]的标签头
	for w in str.gfind(str, "%b[]") do
		if  str.sub(w,2,2) ~= "/" then-- 去尾
			tb.insert(clumpheadTab, w)
		end
	end

	-- 解析标签
	local totalTab = {}
	for k,ns in pairs(clumpheadTab) do
		local tab = {}
		local tStr
		-- 第一个等号前为块标签名
		str.gsub(ns, str.sub(ns, 2, #ns-1), function (w)
			local n = str.find(w, "=")
			if n then
				local temTab = self:stringSplit_(w, " ") -- 支持标签内嵌
				for k,pstr in ipairs(temTab) do
					local temtab1 = self:stringSplit_(pstr, "=")
					local pname = temtab1[1]

					if k == 1 then
						tStr = pname
					end -- 标签头

					local js = temtab1[2]
					if not js then break end
					local p = str.find(js, "[^%d.]")
        			if not p then
        				js = tonumber(js)
        			end

					local switchState = {
						["fontColor"]	 = function()
							tab["fontColor"] = self:convertColor_(js)
						end,
					} --switch end

					local fSwitch = switchState[pname] --switch 方法

					--存在switch
					if fSwitch then
						--目前只是颜色需要转换
						local result = fSwitch() --执行function
					else
						if js then tab[pname] = js end
						--如果没有枚举
						--return
					end
				end
			end
		end)
		if tStr then
			-- 取出文本
			local beginFind,endFind = str.find(str, tb.concat({"%[%/", tStr, "%]"}))
			local endNumber = beginFind-1
			local gs = str.sub(str, #ns+1, endNumber)
			if str.find(gs, "%[") then
				tab["text"] = gs
			else
				gs = str.gsub(gs, "(%W)", "%%%1")
				str.gsub(str, gs, function (w)
					tab["text"] = w
				end)
			end
			-- 截掉已经解析的字符
			str = str.sub(str, endFind+1, #str)
			tb.insert(totalTab, tab)
		end
	end
	-- 普通格式label显示
	if #clumpheadTab == 0 then
		local ptab = {}
		ptab.text  = str
		tb.insert(totalTab, ptab)
	end
	return totalTab
end


--[[解析16进制颜色rgb值]]
function  LCRichLabel:convertColor_(xStr)
    local function toTen(v)
        return tonumber(tb.concat({"0x", v}))
    end

    local b = str.sub(xStr, -2, -1)
    local g = str.sub(xStr, -4, -3)
    local r = str.sub(xStr, -6, -5)

    local red   = toTen(r) or self._fontColor.r
    local green = toTen(g) or self._fontColor.g
    local blue  = toTen(b) or self._fontColor.b
    return ccc3(red, green, blue)
end

-- string.split()
function LCRichLabel:stringSplit_(str, flag)
	local tab = {}
	while true do
		local n = str.find(str, flag)
		if n then
			local first = str.sub(str, 1, n-1)
			str = str.sub(str, n+1, #str)
			tb.insert(tab, first)
		else
			tb.insert(tab, str)
			break
		end
	end
	return tab
end

-- 拆分出单个字符
function LCRichLabel:stringToChar_(str)
	--[[if DEF_DESGIN_LANGUAGE == kLanguageArabic then
		 local list = {}
		 local len = string.len(str)
		 table.insert(list, str)
		return list, len
	else--]]
		local list = {}
	    local len = string.len(str)
	    local i = 1
	    while i <= len do
	        local c = string.byte(str, i)
	        local shift = 1
	        if c > 0 and c <= 127 then
	            shift = 1
	        elseif (c >= 192 and c <= 223) then
	            shift = 2
	        elseif (c >= 224 and c <= 239) then
	            shift = 3
	        elseif (c >= 240 and c <= 247) then
	            shift = 4
	        end
	        local char = str.sub(str, i, i+shift-1)
	        i = i + shift
	       tb.insert(list, char)
	    end
		return list, len
	--end
end

--[[-------------------
    ---Private Method-----
    ---------------------]]
function LCRichLabel:setTouchEnabled(able)
	if able==nil then return end
	self._containLayer:setTouchEnabled(able)
end

function LCRichLabel:registerScriptTouchHandler(nHandler, bIsMultiTouches, nPriority, bSwallowsTouches)
	self.callbackFun = nHandler
	self._containLayer:registerScriptTouchHandler(function( ... )
		return self:onTouch(...)
	end, bIsMultiTouches or false, nPriority or eTouchPriority.maskLayer, bSwallowsTouches or false)
	self._containLayer:setTouchEnabled(true)
end

local beganpPos  = ccp(0, 0)
local touchIndex = 1
function LCRichLabel:onTouch( eventType,x,y )
	local layerPos = self._containLayer:convertToNodeSpace(ccp(x, y))

	if eventType == "began" then
		for i=1, #self.callbackTable do
			local bBox = self.callbackTable[i].node:boundingBox()
			if bBox:containsPoint(layerPos) then
				touchIndex = i
				beganpPos = ccp(x, y)
				return true
			end
		end
		return false
	elseif eventType == "moved" then
		local moves = 0
		for i=1, #self.callbackTable do
			local bBox = self.callbackTable[i].node:boundingBox()
			if bBox:containsPoint(layerPos) then
				moves = i
			end
		end
		if moves<=0 or moves > #self.callbackTable then touchIndex = 0 end
	elseif eventType == "ended" then
		if touchIndex > 0 and touchIndex <= #self.callbackTable then
			if (math.abs(beganpPos.x-x)<10 and math.abs(beganpPos.y-y)<10) then
				local data=self.callbackTable[touchIndex]
				if self.callbackFun and data.dic.callback then
					self.callbackFun(data)
				end
			end
		end
		touchIndex = 1
	end
end

--是否在播放动画
function LCRichLabel:isRunningAmim()
	--[[local isRunning = false
	if self._labelStatus == 2 then
		isRunning = true
	end
	return isRunning--]]
end

--强制停止播放动画
function LCRichLabel:playEnded()
	--[[print("play ended")
	self:removeNodeEventListenersByEvent(NODE_ENTER_FRAME_EVENT)
	self:unscheduleUpdate()

	self._labelStatus = 3 --播放完毕
	for i, sprite in pairs(self._spriteArray) do
		sprite:stopActionByTag(99)
		sprite:setOpacity(255)
	end--]]
end

--播放fade in 动画
function LCRichLabel:playFadeInAnim(wordPerSec)
	--[[local spriteArray = self._spriteArray

	if spriteArray then

		if self._labelStatus == 2 then --上一个动画播放中
			self:playEnded()
		end

		self._labelStatus = 2--播放中

		wordPerSec = wordPerSec or 15 --每秒多少个字

		local delay = 1 / wordPerSec

		do -- next action
			local curTime = 0

			local totalNum = #spriteArray

			if totalNum == 0 then
				self._labelStatus = 3 --播放完毕
				return
			end

			local totalTime = totalNum * delay
			local curIntIndex = 1

			--init
			for i, sprite in pairs(spriteArray) do
				sprite:setOpacity(0)
			end

		    local function updatePosition(dt)

		                curTime = curTime + dt

		                --这个类似动作里面的update的time参数
		                local time = curTime / totalTime

		                local fIndex = (totalNum - 1) * time + 1 --从1开始
		                local index  = math.floor(fIndex)

		                if index < totalNum then


		                else --最后一个点
		                	self._labelStatus = 3 --播放完毕
		                	self:removeNodeEventListenersByEvent(NODE_ENTER_FRAME_EVENT)
		                	self:unscheduleUpdate()
		                end

	                	if index >= curIntIndex then
	                		for i = curIntIndex, index do
	                			local sprite = spriteArray[i]

	                			if sprite then
	                				local action = CCFadeIn:create(0.2)
	                				action:setTag(99)
	                				sprite:runAction(action)
	                			else
	                				print("Error: sprite not exist")
	                			end
	                		end

	                		curIntIndex = index + 1
	                	end

		    end

		    self:removeNodeEventListenersByEvent(NODE_ENTER_FRAME_EVENT)
		    self:unscheduleUpdate()
		    self:registerScriptTouchHandler(NODE_ENTER_FRAME_EVENT, updatePosition)
		    self:scheduleUpdate_()
		end
	end--]]
end

return LCRichLabel
