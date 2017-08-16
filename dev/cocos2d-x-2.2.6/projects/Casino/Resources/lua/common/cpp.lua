

function _enum(name,val)
	if not name then assert(false,"name is nil") return end
	if _G[name] then assert(false,name .. " is defined again") return end
	_G[name] = val
end


function printf(format,...)
	print(string.format(format,...))
end

-- =====> stlmap

stlmap = class("stlmap")

function stlmap:ctor()
	self._map = {}
	self._size = 0
end

function stlmap:size() return self._size end

function stlmap:insert(k,v)
	if not k or not v then return end
	local p = self._map[k]
	self._map[k] = v
	if not p then self._size = self._size + 1 end
end

function stlmap:erase(k)
	local p = self._map[k]
	self._map[k] = nil
	if p then self._size = self._size - 1 end
end

function stlmap:find(k) return self._map[k] end

function stlmap:foreach(callback)
	if not callback then return end
	for k,v in pairs(self._map) do
		local isBreak = callback(k,v)
		if isBreak then break end
	end
end


-- =====> stllist

stllist = class("stllist")

function stllist:ctor()
	self._list = {}
end

function stllist:size() return table.getn(self._list) end

function stllist:front()
	if table.getn(self._list) == 0 then return end
	return self._list[1]
end

function stllist:back()
	local s = table.getn(self._list)
	if s == 0 then return end
	return self._list[s]
end

function stllist:push_back(v)
	if not v then return end
	table.insert(self._list,v)
end

function stllist:push_front(v)
	if not v then return end
	table.insert(self._list,1,v)
end

function stllist:pop_back()
	return table.remove(self._list, table.getn(self._list))
end

function stllist:pop_front()
	return table.remove(self._list, 1)
end

function stllist:clear()
	self._list = {}
end

function stllist:erase(index)
	return table.remove(self._list, index)
end

function stllist:foreach(callback)
	if not callback then return end
	for i=1, #self._list do
		local v = self._list[i]
		local isBreak = callback(i, v)
		if isBreak then break end
	end
end


function stllist:foreach_reverse(callback)
	if not callback then return end
	for i=#self._list, 1, -1 do
		local v = self._list[i]
		local isBreak = callback(i, v)
		if isBreak then break end
	end
end

function stllist:find(i) return self._list[i] end



-- =====> flatmap
flatmap = class("flatmap")

function flatmap:ctor(level)
	self._map = {}
	self._level = level
	self._size = 0
end

function flatmap:level() return self._level end

function flatmap:size() return self._size end

function flatmap:insert(v, ...)
	local keys = {...}
	if not v or #keys ~= self._level or #keys == 0 then return end

	local level = 1
	local function _insert(map)
		local k = keys[level]

		-- end of level
		if level >= self._level then
			if not map[k] then self._size = self._size + 1 end
			map[k] = v
			return
		end

		-- find next
		local nmap = map[k]
		if not nmap then
			nmap = {}
			map[k] = nmap
		end

		level = level + 1
		return _insert(nmap)
	end

	_insert(self._map)
end

function flatmap:erase(...)
	local keys = {...}
	if #keys ~= self._level or #keys == 0 then return end

	local level = 1
	local function _erase(map)
		local k = keys[level]

		-- end of level
		if level >= self._level then
			if map[k] then self._size = self._size - 1 end
			map[k] = nil
			return
		end

		-- find next
		local nmap = map[k]
		if not nmap then return end

		level = level + 1
		return _insert(nmap)
	end

	return _erase(self._map)
end

function flatmap:find(...)
	local keys = {...}
	if #keys ~= self._level or #keys == 0 then return end

	local level = 1
	local function _find(map)
		local k = keys[level]

		-- end of level
		if level == self._level then return map[k] end

		-- find next
		local nmap = map[k]
		if not nmap then return end

		level = level + 1
		return _find(nmap)
	end

	return _find(self._map)
end
