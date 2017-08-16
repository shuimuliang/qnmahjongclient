
function scheduleOnce(node, callback, delay, notFrameRate)
	if not node then GError("node should not nil") return end
	if notFrameRate then delay=delay*CCDirector:sharedDirector():getAnimationInterval()/(1/60) end
    local delay = CCDelayTime:create(delay)
    local callfunc = CCCallFunc:create(callback)
    local sequence = CCSequence:createWithTwoActions(delay, callfunc)
    node:runAction(sequence)
    return sequence
end

function unschedule(node,action)
	if not action then return end
	node:stopAction(action)
end

function handler(obj, method)
    return function(...)
        return method(obj, ...)
    end
end

function _create(clsObj, ...)
	local ret = clsObj.new()
	if not ret:init(...) then return nil end
	return ret
end

local function CC_SYNTHESIZE_DEF(rw, repeated, clsObj, varName, dft)
	if type(varName) ~= "string" then
		GError("varName type error, type is %s",type(varName))
		return
	end

	-- upper first letter
	local first = string.sub(varName,1,1)
	local last = string.sub(varName,2,string.len(varName))
	first = string.upper(first)
	local Fname = first .. last

	-- check r w tag
	local r,w = false,false
	if rw == 1 then w = true
	elseif rw == 2 then r = true
	elseif rw == 3 then r,w = true,true
	else end

	-- parmeter
	if clsObj[varName] and not repeated then GError("%s has exsist",varName) return end
	clsObj[varName] = dft

	-- r
	if r then
		local getfunc = "get" .. Fname
		if clsObj[getfunc] and not repeated then
			GError("%s has exsist",getfunc)
			return
		end
		clsObj[getfunc] = function(self) return self[varName] end
	end

	-- w
	if w then
		local setfunc = "set" .. Fname
		if clsObj[setfunc] and not repeated then GError("%s has exsist",setfunc) return end
		clsObj[setfunc] = function(self,var) self[varName] = var end
	end
end

function CC_SYNTHESIZE(clsObj,varName,dft) CC_SYNTHESIZE_DEF(3,false,clsObj,varName,dft) end
function CC_SYNTHESIZE_RO(clsObj,varName,dft) CC_SYNTHESIZE_DEF(2,false,clsObj,varName,dft) end


local function _diyWidget(super, classname)

	super.addChildToMid = function (_,child,...)
		local achor = super:getAnchorPoint()
		local size = super:getContentSize()
		local x = size.width * (1 - achor.x - 0.5)
		local y = size.height * (1 - achor.y - 0.5)

		child:setPosition(ccp(x,y))
		super:addChild(child,...)
	end

	if classname == "CCLabelTTF" then
		-- set word id
		super.setWordID = function (_, id, ...)
			local str = _ID2WORD(id)
			local t = {...}
			if #t > 0 then str = string.format(str, ...) end
			-- set text
			super:setString(str)
		end
	elseif classname == "CCScale9Sprite" then
		super.setFrame = function (_,image)
			local size = super:getContentSize()
			local top,right,left,bottom = super:getInsetTop(),super:getInsetRight(),super:getInsetLeft(),super:getInsetBottom()
			local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(image)
			super:setSpriteFrame(frame)
			super:setContentSize(size)
			super:setInsetLeft(left)
			super:setInsetTop(top)
			super:setInsetRight(right)
			super:setInsetBottom(bottom)
		end
	end
end

local function _initWidget(super, classname, parms)
	-- check
	if not parms then return end

	-- visible
	if parms.visible ~= nil then  super:setVisible(parms.visible) end

	-- font and Label
	if  classname == "CCLabelTTF" then
		if parms.wordID then
			if type(parms.wordID) == "table" then
				super:setWordID(unpack(parms.wordID))
			else
				super:setWordID(parms.wordID)
			end
		end
		if parms.strokeColor then
			super:enableStroke(parms.strokeColor,1,true)
		end
	elseif classname == "CCSprite" then
		if parms.default then
			local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(parms.default)
			super:setDisplayFrame(frame)
		end
	elseif classname == "CCScale9Sprite" then
		if parms.default then
			super:setFrame(parms.default)
		end

	elseif classname == "CCControlButton" then
		if parms.title then
			_CBTN_CTEXTF(super, parms.title, eUIFont.systembutton)
			super.setTitle = function (_,text)
				super:setTitleForState(CCString:createWithData(text,#text),CCControlStateNormal)
				super:setTitleForState(CCString:createWithData(text,#text),CCControlStateHighlighted)
				super:setTitleForState(CCString:createWithData(text,#text),CCControlStateDisabled)
			end
		end

	elseif classname == "CCLayer" then
		if parms.richText then
			local anchorPt = super:getAnchorPoint()
			local anchorPtX = anchorPt.x
			local anchorPtY = anchorPt.y
			local size = super:getContentSize()
			local key = {
					text = "",
					dimensions = size
				}
			local LCRichLabel = require("lua/scene/sceneUtils/RichLabel")
			local richText = LCRichLabel:create(key)
			super:addChild(richText)
			richText:setPositionY(size.height)
			richText._vAlign = kCCVerticalTextAlignmentCenter
			richText._hAlign = kCCTextAlignmentCenter
			richText.setVerticalAlignment = function (_,align)
				richText._vAlign = align
				richText:setContent()
			end
			richText.setHorizontalAlignment = function (_,align)
				richText._hAlign = align
				richText:setContent()
			end

			-- bind func
			richText.setContent = function (_,txt)
				-- set word
				if txt then
					richText:setLabelString(txt)
				end
				local curSize = richText:getLabelSize()
				local x,y = 0,0
				-- X
				if richText._hAlign == kCCTextAlignmentLeft then
					x = 0
				elseif richText._hAlign == kCCTextAlignmentCenter then
					x = math.max( (size.width - curSize.width)/2 , 0)
				elseif richText._hAlign == kCCTextAlignmentRight then
					x = size.width
				end

				-- Y
				if richText._vAlign == kCCVerticalTextAlignmentTop then
					y = size.height
				elseif richText._vAlign == kCCVerticalTextAlignmentCenter then
					y = size.height - math.max( (size.height - curSize.height)/2,0)
				elseif richText._vAlign == kCCVerticalTextAlignmentBottom then
					y = 0
				end
				richText:setPosition(x, y)
			end
			return richText
		end
	end
end

local function _superWidget(ccbNode,class, parms)
	if (not ccbNode) or (not class) then return end
	local super =  tolua.cast(ccbNode,class)
	_diyWidget(super, class)
	local richText = _initWidget(super, class, parms)
	return super,richText
end

function CE_SYNTHESIZE_CCB_MAP(obj, rootNode, map)
	if not obj or not rootNode or not map then
		GError("invalid argument")
		return
	end

	for i=1, table.getn(map) do
		-- get parms
		local item = map[i]
		local varName = item[1]
		local ccbName = item[2]
		local class = item[3]
		local parms = item[4]
		if not class then class = "CCNode" end

		-- init
		local ccbNode = obj[ccbName]
		if ccbNode then
			local super,richText = _superWidget(ccbNode,class, parms)

			CC_SYNTHESIZE(obj, varName, super )
			-- richText
			if richText then
				super["richText"] = richText
			end
		else
			GError("can not get node from ccb file :%s" ,ccbName)
		end

	end -- for
end

function serialize(obj)
	local lua = ""
	local t = type(obj)
	if t == "number" then
		lua = lua .. obj
	elseif t == "boolean" then
		lua = lua .. tostring(obj)
	elseif t == "string" then
		lua = lua .. string.format("%q", obj)
	elseif t == "table" then
		lua = lua .. "{\n"
	for k, v in pairs(obj) do
		lua = lua .. "[" .. serialize(k) .. "]=" .. serialize(v) .. ",\n"
	end
	local metatable = getmetatable(obj)
		if metatable ~= nil and type(metatable.__index) == "table" then
		for k, v in pairs(metatable.__index) do
			lua = lua .. "[" .. serialize(k) .. "]=" .. serialize(v) .. ",\n"
		end
	end
		lua = lua .. "}"
	elseif t == "nil" then
		return nil
	else
		error("can not serialize a " .. t .. " type.")
	end
	return lua
end

function unserialize(lua)
	local t = type(lua)
	if t == "nil" or lua == "" then
		return nil
	elseif t == "number" or t == "string" or t == "boolean" then
		lua = tostring(lua)
	else
		error("can not unserialize a " .. t .. " type.")
	end
	lua = "return " .. lua
	local func = loadstring(lua)
	if func == nil then
		return nil
	end
	return func()
end

function splitStr(str, reps)
    local resultStrsList = {}
    string.gsub(str, '[^' .. reps ..']+', function(w) table.insert(resultStrsList, w) end )
    return resultStrsList
end

function GET_ADAPTER_SCALE()
	return CCEGLView:sharedOpenGLView():getScaleY()
end

function CE_OBJECT_INVOKE(object, f_name, ...)
	if not object or not f_name then return end
	local f = object[f_name]
	if not f then return end
	return f(object, ...)
end

function CE_GET_WORLD_POSITION(node,anchor,adapterScale)
	if not node then return end
	adapterScale = adapterScale or 1

	local pos = ccp(node:getPosition())
	local worldPos = node:getParent():convertToWorldSpace(pos)
	if anchor then
		local nodeAnchor = node:getAnchorPoint()
		anchor = ccp(anchor.x - nodeAnchor.x,anchor.y - nodeAnchor.y)
		local size = node:getContentSize()
		local scaleX ,scaleY = node:getScaleX(),node:getScaleY()
		size = CCSizeMake(size.width * scaleX * adapterScale , size.height * scaleY * adapterScale)
		return ccp(worldPos.x + size.width * anchor.x,worldPos.y + size.height * anchor.y)
	else
		return worldPos
	end
end

function CE_GET_STRING_SHOW_LENGTH(str)
	local list_chs,list_eng,_,_ = CE_DISASSEMBLE_STRING_TO_CHAR_TABLE(str)
	return (#list_chs) * 2 + #list_eng
end

function CE_DISASSEMBLE_STRING_TO_CHAR_TABLE(str)
    local list_chs = {}
    local list_eng = {}
    local list_len = {}
    local list_char = {}

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

		--print("shift is ",shift)
		if shift > 1 then
			table.insert(list_chs,char)
			table.insert(list_len,2)
		else
			table.insert(list_eng,char)
			table.insert(list_len,1)
		end
		table.insert(list_char,char)
    end
	return list_chs,list_eng,list_char,list_len
end

function CE_STRING_CUT_OFF(str,leftLen)
	local _,_,list_char,list_len = CE_DISASSEMBLE_STRING_TO_CHAR_TABLE(str)
	local curStr = ""
	local curLen = #curStr
	for i,showLen in ipairs(list_len) do
		if curLen + showLen <= leftLen then
			curStr = curStr .. list_char[i]
			curLen = curLen + showLen
		else
			break
		end
	end
	return curStr
end

function CE_SPACE_WIDTH(fontSize,fontName)
	fontSize = fontSize or 24
	fontName = fontName or DEF_SYSTEM_FONT_NAME
	local label = CCLabelTTF:create(" ", fontName, fontSize)
	local characterSize = label:getContentSize()
	return characterSize.width
end


__ceSpaceWidth25 = CE_SPACE_WIDTH(25)
