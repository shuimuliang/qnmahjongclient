local JushuConfig = {}

local _data = {}
local _handleData

function JushuConfig.update(data)
    _data = _handleData(data) or _data
end

function JushuConfig.getAllJushuByMjType(mjType)
    if not isset(_data, mjType) then return {} end
    local keys = table.keys(_data[mjType].jushu)
    table.sort(keys)
    return keys
end

function JushuConfig.getCoinsByTypeJushu(mjType, count)
    if isset(_data, mjType) and isset(_data[mjType].jushu, count) then
        return _data[mjType].jushu[count].coins
    end
    return 0
end

function JushuConfig.getCardsByTypeJushu(mjType, count)
    if isset(_data, mjType) and isset(_data[mjType].jushu, count) then
        return _data[mjType].jushu[count].cards
    end
    return 0
end

function JushuConfig.getAllMjType()
    return table.keys(_data)
end

function JushuConfig.getNameByType(mjType)
    -- print_lua_table(_data)
    -- GDebug("wz : mjType = %s, type is %s", mjType, type(mjType))
    if isset(_data, mjType) then
        return _data[mjType].name
    end
    return ""
end

-- private
_handleData = function(data)
    if not data then return end
    local ret = {}
    for _, t in ipairs(data) do
        ret[t.mjType] = ret[t.mjType] or {name = t.mjDesc, jushu = {}}
        ret[t.mjType].jushu[t.jushu] = {coins = t.coins, cards = t.cards}
    end
    return ret
end

return JushuConfig
