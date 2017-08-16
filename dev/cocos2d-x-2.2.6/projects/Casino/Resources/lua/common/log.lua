
local DEF_LOG_LEVEL = {
	DEBUG = 0,
	INFO = 1,
	WARN = 2,
	ERROR = 3,
	FATAL = 4,
}


function setGLogLevel(level)  
	if not level then return end
	CELog:getInstance():setLogLevel(level)
end


local logFile = nil
function initLogFile()
	if logFile then GWarn("can not init log file twice") return  end
	local sysCfg = LCUserContext:getSystemConfig()
	logFile = string.format("%s_%d",DEF_GAME_NAME, sysCfg.logIndex)
	
	-- save index + 1
	sysCfg.logIndex = sysCfg.logIndex + 1
	if sysCfg.logIndex > DEF_MAX_LOG_COUNT then sysCfg.logIndex = 1 end
	LCUserContext:saveSystemConfig()
	
	-- output 
	CELog:getInstance():createFile(logFile)
end	
	

local function trace(level)
	local info = debug.getinfo(level,"Sln")
	local path = info.short_src
	local file = nil
	
	local c1 = string.byte("/")
	local c2 = string.byte("\\")
	local i=string.len(path)
	while i > 0 do
		local c = string.byte(path,i)
		if  c == c1 or c == c2 then
			file = string.sub(path, i+1, string.len(path))
			break
		end
		i = i - 1
	end
	if not file then file = path end
	return file, info.currentline, info.name
end	

local function GLog(level, format, ...)
	-- file line and function
	local detail = ""
	local src,line,func = trace(4)
	if not func then func = "GLOBAL" end
	
	local content = string.format(format, ...)
	
	-- construct
	--local str = string.format("[%s %s] [%s:%d %s]: %s",time, tag, src, line, func, content)	
	local str = string.format("[%s:%d %s]: %s",src, line, func, content)	
	
	-- output
	CELog:getInstance():write(level,str)
end

function GDebug(...) GLog(DEF_LOG_LEVEL.DEBUG, ...) end
function GInfo(...) GLog(DEF_LOG_LEVEL.INFO, ...) end
function GWarn(...) GLog(DEF_LOG_LEVEL.WARN, ...) end
function GError(...)
	GLog(DEF_LOG_LEVEL.ERROR, ...) 
end
function GFatal(...) GLog(DEF_LOG_LEVEL.FATAL, ...) end


function posstring(pos)
	if not pos then return "" end
	local str = "(" .. pos.x .. "," .. pos.y .. ")"	
	return str
end

function sizestring(size)
	if not size then return "" end
	local str = string.format("(%d,%d)",size.width, size.height)
	return str
end

function rectstring(rect)
	if not rect then return "" end
	local str = string.format("origin(%d,%d) size(%d,%d)",
	rect.origin.x,
	rect.origin.y,
	rect.size.width,
	rect.size.height)
	return str
end	 

function arraystring(list)
	local str = ""
	for i=1,table.getn(list) do str = str .. list[i] .. " " end
	return str
end	

function matrixstring(matrix, r, l)
	local rc = r or table.getn(matrix)
	if not matrix then return "" end
	local str = "\n"
	for i=1, rc do
		local row = matrix[i]
		local lc = l or table.getn(row)
		for j=1, lc do
			if not row[j] then str = str .. "nil "
			else str = str .. row[j] .. " " end
		end
		str = str .. "\n"
	end
	return str
end


