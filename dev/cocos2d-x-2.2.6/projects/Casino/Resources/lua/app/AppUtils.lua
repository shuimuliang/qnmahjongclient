function _ID2WORD(id, default)
	if not id then return end
	if not LOCALIZABLE_WORDS then GError("no local world file") return end
	-- if type(id) ~= "string" then GError("invalid id type %s",type(id)) return end
	local word = LOCALIZABLE_WORDS[tostring(id)]
	local errorCode = ERROR_CODE[tostring(id)]
	if errorCode then return errorCode end
	
	if not word then
		if default then return tostring(default)
		else return tostring(id) end
	end
	return word
end

function _APP_UI_FONT(__name,img)
	if not __name then return end
	if not img then
		return DEF_PATH.uifont..__name..eResSuffix.uifont
	else
		return DEF_PATH.uifont..__name..eResSuffix.png
	end
end

function _APPHTML(__name)
	if not __name then return "" end
	local fullPath = CCFileUtils:sharedFileUtils():fullPathForFilename(__name)
	if fullPath then fullPath = "file:///"..fullPath end
	return fullPath or ""
end

function _APPCOLOR_RGB(r,g,b,a)
	if not a then return ccc3(r,g,b) end
	return ccc4(r,g,b,a)
end
function _APPCOLOR_HEX(value)
	if not value then return end
	local r = tonumber("0x"..string.sub(value,1,2))
	local g = tonumber("0x"..string.sub(value,3,4))
	local b = tonumber("0x"..string.sub(value,5,6))
	if(#value == 8) then
		local a = tonumber("0x"..string.sub(value,7,8))
		return ccc4(r,g,b,a)
	end
	return ccc3(r,g,b)
end

__app_render_texture = nil
function _APP_RENDER_TEXTURE(__name)
	if  DEF_TARGET_PLATFORM == DEF_PLATFORM_IOS then
		if not __app_render_texture then
			local size = CCDirector:sharedDirector():getWinSize()
			__app_render_texture = CCRenderTexture:create(size.width,size.height,0)
			__app_render_texture:retain()
		end
	else
		local size = CCDirector:sharedDirector():getWinSize()
		__app_render_texture = CCRenderTexture:create(size.width,size.height,0)
	end
	return __app_render_texture
end

function APP_SCREEN_CAPTURE()
	local renderTexture = _APP_RENDER_TEXTURE()
    renderTexture:begin()
    CCDirector:sharedDirector():getRunningScene():visit()
    renderTexture:endToLua()

    local imgName = "album_screen.jpg"
    if  DEF_TARGET_PLATFORM == DEF_PLATFORM_ANDROID then
    	imgName =  CCEPlugin:getAndroidSDCardPath() .. getTickCount() .. imgName
    end

    local succ = renderTexture:saveToFile(imgName)
    return succ
end

function _NICE_MONEY(money)
	if not money or money == 0 then return "0" end
	absMoney = math.abs(money)
	local str = ""
	local left = absMoney
	local maxDigits = 15
	local count,totalCount = 0,0
	while left > 0 do
		count = count + 1
		totalCount = totalCount + 1
		if totalCount > maxDigits then break end
		str = (left%10)..str
		left = math.floor(left/10)
		if count == 3 and left > 0 then str = ","..str  count = 0 end
	end
	if money < 0 then
		str = "-"..str
	end
	return str
end

function _NICE_MONEY2(money)
	if not money then return "" end
	if type(money) ~= "number" then GError("error money :%s",money) return "" end
	local absMoney = math.abs(money)

	local function niceMoney( moneyStr,suffix )
		if not moneyStr or not suffix then return "" end
		local len = #moneyStr
		if len <= 2 then return "" end
		local last1 = string.sub(moneyStr,len,len)
		local last2 = string.sub(moneyStr,len-1,len-1)

		if last1 ~= "0" then return moneyStr..suffix end
		moneyStr = string.sub(moneyStr,1,len-1)
		if last2 ~= "0" then return moneyStr..suffix end
		moneyStr = string.sub(moneyStr,1,len-3)
		return moneyStr..suffix
	end

	local format, moneyStr, suffix = "%.2f", "" , ""
	if (absMoney >= 100000000) then
		moneyStr = string.format(format,absMoney / 100000000)
		suffix = _("亿")
 	elseif (absMoney >= 10000000) then
    	moneyStr = string.format(format,absMoney / 10000000)
    	suffix = _("千万")
  	-- elseif (money >= 1000000) then
  	-- 	moneyStr = string.format(format,money / 1000000)
   --  	suffix = _("百万")
  	elseif (absMoney >= 10000) then
  		moneyStr = string.format(format,absMoney / 10000)
    	suffix = _("万")
  	elseif (absMoney >= 1000) then
  		moneyStr = string.format(format,absMoney / 1000)
    	suffix = _("千")
  	else
    	return _NICE_MONEY(money)
 	end
	local resultStr = niceMoney(moneyStr,suffix)
	if money < 0 and resultStr then resultStr = "-"..resultStr end
 	return resultStr or ""
end

-- 10k 100k 1m 100m
function _NICE_MONEY3(money)
		if not money then return "" end
	if type(money) ~= "number" then GError("error money :%s",money) return "" end
	local absMoney = math.abs(money)

	local function isChinese()
		return DEF_DESGIN_LANGUAGE == kLanguageChinese
            or DEF_DESGIN_LANGUAGE == kLanguageChineseTraditional
            or DEF_DESGIN_LANGUAGE == kLanguageChineseTraditionalTaiwan

	end

	if isChinese() then return _NICE_MONEY2(money) end

	local resultStr = ""
	if (absMoney >= 1000000000) then
		local format = absMoney % 1000000000 == 0 and "%.0fB" or "%.1fB"
    	resultStr = string.format(format,absMoney / 1000000000)
  	elseif (absMoney >= 1000000) then
  		local format = absMoney % 1000000 == 0 and "%.0fM" or "%.1fM"
    	resultStr = string.format(format,absMoney / 1000000)
    elseif (absMoney >= 1000) then
    	local format = absMoney % 1000 == 0 and "%.0fK" or "%.1fK"
    	resultStr = string.format(format,absMoney / 1000)
  	else
    	return _NICE_MONEY(money)
 	end
	if money < 0 and resultStr then resultStr = "-"..resultStr end
 	return resultStr or ""
end

function _FRAMERATE()
	return CCDirector:sharedDirector():getAnimationInterval()/(1/60)
end

--CCMenuItemImage change frame
function _CMENUITEM_FRAME(node, name)
	if not node then GError("AppUtils _CMENUITEM_FRAME 130 node should not nil") return end
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(name)
	node:setNormalSpriteFrame(frame)
	node:setSelectedSpriteFrame(frame)
	node:setDisabledSpriteFrame(frame)
end

--CCControlButton change frame
function _CBTN_CFRAME(node, name, resize)
	if not node then GError("AppUtils _CBTN_CFRAME node should not nil") return end
	if name == nil or name == "" then return end
	local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(name)
	if resize then
		local size = frame:getRect().size
		node:setPreferredSize(size)
	end

	node:setBackgroundSpriteFrameForState(frame, CCControlStateNormal)
	node:setBackgroundSpriteFrameForState(frame, CCControlStateHighlighted)
	node:setBackgroundSpriteFrameForState(frame, CCControlStateSelected)
	node:setBackgroundSpriteFrameForState(frame, CCControlStateDisabled)
end

--CCControlButton change title color
function _CBTN_TITLECOLOR(node, titleColor)
	if not node then GError("AppUtils _CBTN_TITLECOLOR node should not nil") return end
	node:setTitleColorForState(titleColor, CCControlStateNormal)
	node:setTitleColorForState(titleColor, CCControlStateHighlighted)
	node:setTitleColorForState(titleColor, CCControlStateSelected)
	node:setTitleColorForState(titleColor, CCControlStateDisabled)
end

--CCControlButton change font and text
function _CBTN_CTEXTF(node, text, font, labelAnc)
	if not node then GError("AppUtils _CBTN_CTEXTF node should not nil") return end
	local sprite = nil

	if text then
		text= tostring(text)

		if sprite then text = "" end
		node:setTitleForState(CCString:createWithData(text,#text),CCControlStateNormal)
		node:setTitleForState(CCString:createWithData(text,#text),CCControlStateHighlighted)
		node:setTitleForState(CCString:createWithData(text,#text),CCControlStateDisabled)
		-- else
		if sprite then
			if node.isadded then
				local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(sprite)
				if node.isadded.setDisplayFrame then
					node.isadded:setDisplayFrame(frame)
				else
					GError("fuck,error here!!!!  %s",type(node))
				end
			else
				local size = node:getContentSize()
				local temp = CCSprite:createWithSpriteFrameName(sprite)
				node:addChild(temp, 3, eAraTag)
				local tempSize=temp:getContentSize()
				if tempSize.width>=size.width and tempSize.width<size.width*2 then
					temp:setScale(size.width/tempSize.width-0.1)
				end
				temp:setPosition(ccp(size.width/2,size.height/2))
				node.isadded = temp
			end
		end
	end
	local fontAnc = nil
	if font and not sprite and font~="" then
		node:setTitleBMFontForState(_APP_UI_FONT(font),CCControlStateNormal)
		node:setTitleBMFontForState(_APP_UI_FONT(font),CCControlStateHighlighted)
		node:setTitleBMFontForState(_APP_UI_FONT(font),CCControlStateDisabled)

		fontAnc = node:getLabelAnchorPoint()
	end
	if labelAnc then
		node:setLabelAnchorPoint(labelAnc)
		fontAnc = labelAnc
	end

	if fontAnc then
		_CBTN_CTEXTA(node, fontAnc)
	end
end

function _CBTN_CTEXTA(node, fontAnc)
	if not node or not fontAnc then return end
	local bmtLabel_N = node:getTitleLabelForState(CCControlStateNormal)
	bmtLabel_N:setAnchorPoint(fontAnc)
	local bmtLabel_H = node:getTitleLabelForState(CCControlStateHighlighted)
	bmtLabel_H:setAnchorPoint(fontAnc)
	local bmtLabel_D = node:getTitleLabelForState(CCControlStateDisabled)
	bmtLabel_D:setAnchorPoint(fontAnc)
end

-- isRound 是否是圆形的
function _BTNTouch(btn, touch,isRound,zOrder)
	if not btn then return end
	local isGray=touch
	if not touch then
		btn:setTouchEnabled(false)
		isGray=true
	else
		isGray=false
		btn:setTouchEnabled(true)
	end
	_NODEGREY(btn, isGray,isRound,zOrder)
end

function _NODEGREY(node, gray, isRound, zOrder, imageName, pos, alpha)
	if gray then
		if not node.maskLayer then
			local btnSize = node:getContentSize()
			local scale = node:getScale()
			btnSize = CCSizeMake(btnSize.width*scale, btnSize.height*scale)
			local maskName = isRound and  eResImage.ba_chips_mask or eResImage.c_yellowtbutton_normal_mask
			if imageName then maskName=imageName end
			zOrder = zOrder or 4
			local maskSprite = CCScale9Sprite:createWithSpriteFrameName(maskName)
			maskSprite:setContentSize(btnSize)
			if alpha then
				maskSprite:setOpacity(alpha)
			end
			local ccpPos=ccp(btnSize.width/2,btnSize.height/2)
			if pos then ccpPos=pos end
			maskSprite:setPosition(ccpPos)
			node:addChild(maskSprite,zOrder)
			node.maskLayer = maskSprite
		end
	else
		if node.maskLayer then
			node.maskLayer:removeFromParentAndCleanup(true)
			node.maskLayer = nil
		end
	end
end

function _SET_TEXTURE_RECT(sprite,rect)
  if not sprite or not rect then return end
  local isRotated = sprite:isTextureRectRotated()
  local oldRect = sprite:getTextureRect()
  local contentSize = sprite:getContentSize()

  local newX = oldRect.origin.x + ( isRotated and (oldRect.size.height - rect.size.height) or 0 ) + rect.origin.x
  local newY = oldRect.origin.y
  local newWidth = rect.size.width
  local newHeight = rect.size.height

  -- GDebug("  %s %s     %s      %s     %s    %s", (isRotated and "true" or "false"), rectstring(rect), rectstring(oldRect),newX,newY,sizestring(contentSize))

  local newRect = CCRectMake(newX, newY, newWidth, newHeight)
  sprite:setTextureRect(newRect, isRotated, contentSize)
end

function GET_SELF_GENDER()
	local player =  LCAppGlobal:getLoginPlayer()
	if player then
		return player.gender
	end
	return ePlayerGender.male
end

-- return minute and secod 00:00
function _TIME_2_MS(time)
	if type(time) ~= "number" then return end
	local min = math.floor(time / 60)
	local sec = math.floor(time % 60)
	return string.format("%02d:%02d",min,sec)
end

function _TIME_2_HMS(time)
	if type(time) ~= "number" then return end
	local hour=0
	if time>3600 then
		hour = math.floor(time / 3600)
		time=time % 3600
	end

	local min = math.floor(time / 60)
	local sec = math.floor(time % 60)

	if hour>0 then return string.format("%02d:%02d:%02d",hour, min, sec)

	end
	return string.format("%02d:%02d",min,sec)
end

function _isRightEmail(str)
    if string.len(str or "") < 6 then return false end
    local b,e = string.find(str or "", '@')
    local bstr = ""
    local estr = ""
    if b then
        bstr = string.sub(str, 1, b-1)
        estr = string.sub(str, e+1, -1)
    else
        return false
    end

    -- check the string before '@'
    local p1,p2 = string.find(bstr, "[%w_]+")
    if (p1 ~= 1) or (p2 ~= string.len(bstr)) then return false end

    -- check the string after '@'
    if string.find(estr, "^[%.]+") then return false end
    if string.find(estr, "%.[%.]+") then return false end
    if string.find(estr, "@") then return false end
    if string.find(estr, "[%.]+$") then return false end

    local _,count = string.gsub(estr, "%.", "")
    if (count < 1 ) or (count > 3) then
        return false
    end

    return true
end

function _isEasyPassword(str)
	local pwdList ={ "000000","111111","11111111","112233","123123",
					 "123321","123456","12345678","654321","666666",
					 "888888","abcdef","abcabc","abc123","a1b2c3","aaa111",
					 "123qwe","qwerty","qweasd","admin","password","passwd",
					 "iloveyou","5201314"}
	local find = false
	for i=1,#pwdList do
		if str == pwdList[i] then find = true end
	end
	return find
end


function _isRightPassword(str)
	if string.len(str or "") < 6 then return false end
	return true
end

function _isRightPhone(str)
	local isNum = tonumber(str)
	if isNum then return true end
	return false
end

-- »ñÈ¡¹Ì¶¨³¤¶È×Ö·û
function STRING2LEN(str, len)
	if not str then return "" end
	local list, len2=STRING2CHAR(str)
	if not list then return "" end
	local str2=str
	if len2>len then
		str2=""
		for k,v in pairs(list) do
			if k<=len then
				str2=str2..v
			end
		end
	end
	return str2
end

-- ²ð·Ö³öµ¥¸ö×Ö·û
function STRING2CHAR(str)
	if not str then return {}, 0 end
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
        local char = string.sub(str, i, i+shift-1)
        i = i + shift
        table.insert(list, char)
    end
	return list, len
end

function _FIND_GOLD_FROM_PAIRLIST( pairList )
	local isfindGold = false
	for _, pair in ipairs( pairList ) do
		if pair.key == ePairKey.GOLD then
			isfindGold = true
			return pair.value,isfindGold
		end
	end
	return 0,isfindGold
end


function table.isEmpty(hashtable)
	if not hashtable then GDebug("input not a table.") return true end
	for k, v in pairs(hashtable) do
		return false
	end
	return true
end

--[[--

返回指定表格中的所有键

~~~ lua

local hashtable = {a = 1, b = 2, c = 3}
local keys = table.keys(hashtable)
-- keys = {"a", "b", "c"}

~~~

@param table hashtable 要检查的表格

@return table

]]
function table.keys(hashtable)
    local keys = {}
    for k, v in pairs(hashtable) do
        keys[#keys + 1] = k
    end
    return keys
end

--[[--

返回指定表格中的所有值

~~~ lua

local hashtable = {a = 1, b = 2, c = 3}
local values = table.values(hashtable)
-- values = {1, 2, 3}

~~~

@param table hashtable 要检查的表格

@return table

]]
function table.values(hashtable)
    local values = {}
    for k, v in pairs(hashtable) do
        values[#values + 1] = v
    end
    return values
end

--[[--

将来源表格中所有键及其值复制到目标表格对象中，如果存在同名键，则覆盖其值

~~~ lua

local dest = {a = 1, b = 2}
local src  = {c = 3, d = 4}
table.merge(dest, src)
-- dest = {a = 1, b = 2, c = 3, d = 4}

~~~

@param table dest 目标表格
@param table src 来源表格

]]
function table.merge(dest, src)
    for k, v in pairs(src) do
        dest[k] = v
    end
end

--[[

从表格中查找指定值，返回其索引，如果没找到返回 false

~~~ lua

local array = {"a", "b", "c"}
print(table.indexof(array, "b")) -- 输出 2

~~~

@param table array 表格
@param mixed value 要查找的值
@param [integer begin] 起始索引值

@return integer

]]
function table.indexof(array, value, begin)
    for i = begin or 1, #array do
        if array[i] == value then return i end
    end
	return false
end

--[[--

从表格中查找指定值，返回其 key，如果没找到返回 nil

~~~ lua

local hashtable = {name = "dualface", comp = "chukong"}
print(table.keyof(hashtable, "chukong")) -- 输出 comp

~~~

@param table hashtable 表格
@param mixed value 要查找的值

@return string 该值对应的 key

]]
function table.keyof(hashtable, value)
    for k, v in pairs(hashtable) do
        if v == value then return k end
    end
    return nil
end

--[[--

如果表格中指定 key 的值为 nil，或者输入值不是表格，返回 false，否则返回 true

@param table hashtable 要检查的表格
@param mixed key 要检查的键名

@return boolean

]]
function isset(hashtable, key)
    local t = type(hashtable)
    return (t == "table" or t == "userdata") and hashtable[key] ~= nil
end

function ADDCOLON( str )
	if DEF_DESGIN_LANGUAGE ~= kLanguageArabic then
		str=str..":"
	end
	return str
end


function POINTS_2_DISTANCE(lat1,lon1,lat2,lon2)
	local EARTH_RADIUS = 6371000
	
	local dLat = (lat2-lat1)*(math.pi/180)
	local dLon = (lon2-lon1)*(math.pi/180)

	local l1 = lat1*(math.pi/180)
	local l2 = lat2*(math.pi/180)

	local a1 = math.sin(dLat/2) * math.sin(dLat/2)
	local a2 = math.sin(dLon/2) * math.sin(dLon/2) * math.cos(l1) * math.cos(l2)

	local a = a1 + a2
	local c = 2 * math.atan2(math.sqrt(a),math.sqrt(1-a))

	return EARTH_RADIUS * c
end

--[[--

遍历表格，确保其中的值唯一

~~~ lua

local t = {"a", "a", "b", "c"} -- 重复的 a 会被过滤掉
local n = table.unique(t)

for k, v in pairs(n) do
    print(v)
end

-- 输出
-- a
-- b
-- c

~~~

@param table t 表格

@return table 包含所有唯一值的新表格

]]
function table.unique(t)
    local check = {}
    local n = {}
    for k, v in pairs(t) do
        if not check[v] then
            n[k] = v
            check[v] = true
        end
    end
    return n
end

--[==========[生成stencil图片（圆角图片等）]==========]
--[===[stencilName : 模板图片名称]===]
--[===[spriteName : 底板图片名称]===]
function _CREATE_STENCILED_IMAGE(stencilName, spriteName)
	if not spriteName then GError("no spriteName to create") return end

	--create clippingNode
	local holesClipper = CCClippingNode:create()
	holesClipper:setInverted(false)
	holesClipper:setAlphaThreshold(0.9)

	--create sprite
	local sprite = nil
	if type(spriteName) == "string" then
		sprite = CCSprite:createWithSpriteFrameName(spriteName)
	elseif tolua.type(spriteName) == "CCSprite" then
		sprite = spriteName
	end
	if not sprite then GError("no sprite create with frame name " .. spriteName) return end
	sprite:setAnchorPoint(ccp(0.5,0.5))
	-- sprite:getTexture():setTexParameters()
	local tp = ccTexParams()
    tp.minFilter = GL_LINEAR
    tp.magFilter = GL_NEAREST
    tp.wrapS = GL_CLAMP_TO_EDGE
    tp.wrapT = GL_CLAMP_TO_EDGE

	-- sprite:getTexture():setTexParameters(tp)

	--create stencil
	local stencil = CCSprite:createWithSpriteFrameName(stencilName)
	stencil:setAnchorPoint(ccp(0.5,0.5))
	stencil:setPosition(sprite:getPosition())
    local tp = ccTexParams()
    tp.minFilter = GL_LINEAR
    tp.magFilter = GL_NEAREST
    tp.wrapS = GL_CLAMP_TO_EDGE
    tp.wrapT = GL_CLAMP_TO_EDGE

	-- stencil:getTexture():setTexParameters(tp)
	holesClipper:setStencil(stencil)
	holesClipper:addChild(sprite)

	return holesClipper
end

oldStringFormat = string.format
string.format = function (format,...)
	local r,r1,r2 = pcall(oldStringFormat,format,...)
	if r then return r1 end
	print("string format error : ", r1,r2)
	return ""
end

--[[--

检查并尝试转换为布尔值，除了 nil 和 false，其他任何值都会返回 true

@param mixed value 要检查的值

@return boolean

]]
function checkbool(value)
    return (value ~= nil and value ~= false)
end

--[[--

载入一个模块

import() 与 require() 功能相同，但具有一定程度的自动化特性。

假设我们有如下的目录结构：

~~~

app/
app/classes/
app/classes/MyClass.lua
app/classes/MyClassBase.lua
app/classes/data/Data1.lua
app/classes/data/Data2.lua

~~~

MyClass 中需要载入 MyClassBase 和 MyClassData。如果用 require()，MyClass 内的代码如下：

~~~ lua

local MyClassBase = require("app.classes.MyClassBase")
local MyClass = class("MyClass", MyClassBase)

local Data1 = require("app.classes.data.Data1")
local Data2 = require("app.classes.data.Data2")

~~~

假如我们将 MyClass 及其相关文件换一个目录存放，那么就必须修改 MyClass 中的 require() 命令，否则将找不到模块文件。

而使用 import()，我们只需要如下写：

~~~ lua

local MyClassBase = import(".MyClassBase")
local MyClass = class("MyClass", MyClassBase)

local Data1 = import(".data.Data1")
local Data2 = import(".data.Data2")

~~~

当在模块名前面有一个"." 时，import() 会从当前模块所在目录中查找其他模块。因此 MyClass 及其相关文件不管存放到什么目录里，我们都不再需要修改 MyClass 中的 import() 命令。这在开发一些重复使用的功能组件时，会非常方便。

我们可以在模块名前添加多个"." ，这样 import() 会从更上层的目录开始查找模块。

~

不过 import() 只有在模块级别调用（也就是没有将 import() 写在任何函数中）时，才能够自动得到当前模块名。如果需要在函数中调用 import()，那么就需要指定当前模块名：

~~~ lua

# MyClass.lua

# 这里的 ... 是隐藏参数，包含了当前模块的名字，所以最好将这行代码写在模块的第一行
local CURRENT_MODULE_NAME = ...

local function testLoad()
    local MyClassBase = import(".MyClassBase", CURRENT_MODULE_NAME)
    # 更多代码
end

~~~

@param string moduleName 要载入的模块的名字
@param [string currentModuleName] 当前模块名

@return module

]]
function import(moduleName, currentModuleName)
    local currentModuleNameParts
    local moduleFullName = moduleName
    local offset = 1

    while true do
        if string.byte(moduleName, offset) ~= 46 then -- .
            moduleFullName = string.sub(moduleName, offset)
            if currentModuleNameParts and #currentModuleNameParts > 0 then
                moduleFullName = table.concat(currentModuleNameParts, "/") .. "/" .. table.concat(string.split(moduleFullName, "."), "/")
            end
            break
        end
        offset = offset + 1

        if not currentModuleNameParts then
            if not currentModuleName then
                local n,v = debug.getlocal(3, 1)
                currentModuleName = v
            end

            currentModuleNameParts = string.split(currentModuleName, "/")
        end
        table.remove(currentModuleNameParts, #currentModuleNameParts)
    end
    GDebug(moduleFullName)
    return require(moduleFullName)
end

--[[--

用指定字符或字符串分割输入字符串，返回包含分割结果的数组

~~~ lua

local input = "Hello,World"
local res = string.split(input, ",")
-- res = {"Hello", "World"}

local input = "Hello-+-World-+-Quick"
local res = string.split(input, "-+-")
-- res = {"Hello", "World", "Quick"}

~~~

@param string input 输入字符串
@param string delimiter 分割标记字符或字符串

@return array 包含分割结果的数组

]]
function string.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end


--[[
	io
]]
function io.exists(path)
    local file = io.open(path, "r")
    if file then
        io.close(file)
        return true
    end
    return false
end

function io.readfile(path)
    local file = io.open(path, "r")
    if file then
        local content = file:read("*a")
        io.close(file)
        return content
    end
    return nil
end

function io.writefile(path, content, mode)
    mode = mode or "w+b"
    local file = io.open(path, mode)
    if file then
        print("file is ok ok ok  ok ok ")
        if file:write(content) == nil then print("file is bad bad bad bad ") return false end
        io.close(file)
        return true
    else
        return false
    end
end

--[[
	打印table，支持多层嵌套，indent可不传
]]
function print_lua_table (lua_table, indent)
    indent = indent or 0
    for k, v in pairs(lua_table) do
        if type(k) == "string" then
            k = string.format("%q", k)
        end
        local szSuffix = ""
        if type(v) == "table" then
            szSuffix = "{"
        end
        local szPrefix = string.rep("    ", indent)
        formatting = szPrefix.."["..k.."]".." = "..szSuffix
        if type(v) == "table" then
            GDebug(formatting)
            print_lua_table(v, indent + 1)
            GDebug(szPrefix.."},")
        else
            local szValue = ""
            if type(v) == "string" then
                szValue = string.format("%q", v)
            else
                szValue = tostring(v)
            end
            GDebug(formatting..szValue..",")
        end
    end
end

--秒转换“00:00”格式
function gettimeS(m_time)
    					local m_h=math.floor(m_time/60/60)
    					local m_m=math.floor((m_time-m_h*60*60)/60)
    					local m_s=m_time-m_h*60*60-m_m*60
    					if m_h>0 then
        					return (m_h<10 and "0" or "")..m_h..":"..(m_m<10 and "0" or "")..m_m..":"..(m_s<10 and "0" or "")..m_s
    					else
        					return (m_m<10 and "0" or "")..m_m..":"..(m_s<10 and "0" or "")..m_s
    					end

end
-- function os.printTimezone()
--     local t = os.time()
--     local ut = os.date('!*t',t)
--     local lt = os.date('*t',t)
--     local tzdt = (lt.hour - ut.hour) * 3600 + (lt.min - ut.min) * 60
--     print(string.format("本地时间与标准时间差 %d(%.1f小时) 分钟", tzdt, tzdt / 3600))
-- end
--活的当前服务器时间，秒为单位，即从凌晨至现在一共经历里多少秒
function gettime()
 -- local t = os.time()
 --    local ut = os.date('!*t',t)
 --    local lt = os.date('*t',t)
 --    local tzdt = (lt.hour - ut.hour) * 3600 + (lt.min - ut.min) * 60+(lt.sec - ut.sec)
    					-- local t1 = os.date("!*t", LCNetManager:getServerTime())
    					-- local t1 = os.date("*t", LCNetManager:getServerTime())
    					-- local m_datatime=LCNetManager:getDateTime()
    					-- local m_time=m_datatime
    					return LCNetManager:getDateTime()
end
