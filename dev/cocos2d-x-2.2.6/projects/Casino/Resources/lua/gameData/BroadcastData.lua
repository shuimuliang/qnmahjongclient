local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

local BroadcastData = {}
EventProtocol.extend(BroadcastData)

local _data = {}
local _handle
BroadcastData.EVENT_BROADCAST = "EVENT_BROADCAST"

function BroadcastData.update(data)
	_data = _handle(data) or _data
	BroadcastData:dispatchEvent{name = BroadcastData.EVENT_BROADCAST}
end

function BroadcastData.getText()
	return _data
end

-- private
_handle = function(data)
	local ret = {}
	for i, t in ipairs(data) do
		ret[i] = t
	end
	return ret
end

return BroadcastData
