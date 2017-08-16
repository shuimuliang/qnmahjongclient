Transition = {}

local ACTION_EASING = {}
ACTION_EASING["BACKIN"]           = {CCEaseBackIn, 1}
ACTION_EASING["BACKINOUT"]        = {CCEaseBackInOut, 1}
ACTION_EASING["BACKOUT"]          = {CCEaseBackOut, 1}
ACTION_EASING["BOUNCE"]           = {CCEaseBounce, 1}
ACTION_EASING["BOUNCEIN"]         = {CCEaseBounceIn, 1}
ACTION_EASING["BOUNCEINOUT"]      = {CCEaseBounceInOut, 1}
ACTION_EASING["BOUNCEOUT"]        = {CCEaseBounceOut, 1}
ACTION_EASING["ELASTIC"]          = {CCEaseElastic, 2, 0.3}
ACTION_EASING["ELASTICIN"]        = {CCEaseElasticIn, 2, 0.3}
ACTION_EASING["ELASTICINOUT"]     = {CCEaseElasticInOut, 2, 0.3}
ACTION_EASING["ELASTICOUT"]       = {CCEaseElasticOut, 2, 0.3}
ACTION_EASING["EXPONENTIALIN"]    = {CCEaseExponentialIn, 1}
ACTION_EASING["EXPONENTIALINOUT"] = {CCEaseExponentialInOut, 1}
ACTION_EASING["EXPONENTIALOUT"]   = {CCEaseExponentialOut, 1}
ACTION_EASING["IN"]               = {CCEaseIn, 2, 1}
ACTION_EASING["INOUT"]            = {CCEaseInOut, 2, 1}
ACTION_EASING["OUT"]              = {CCEaseOut, 2, 1}
ACTION_EASING["RATEACTION"]       = {CCEaseRateAction, 2, 1}
ACTION_EASING["SINEIN"]           = {CCEaseSineIn, 1}
ACTION_EASING["SINEINOUT"]        = {CCEaseSineInOut, 1}
ACTION_EASING["SINEOUT"]          = {CCEaseSineOut, 1}

function Transition.newEasing(action, easingName, more)
    local key = string.upper(tostring(easingName))
    if string.sub(key, 1, 6) == "CCEASE" then
        key = string.sub(key, 7)
    end
    local easing
    if ACTION_EASING[key] then
        local cls, count, default = unpack(ACTION_EASING[key])
        if count == 2 then
            easing = cls:create(action, more or default)
        else
            easing = cls:create(action)
        end
    end
    return easing or action
end

function Transition.sequence(actions)
    if #actions < 1 then return end
    if #actions < 2 then return actions[1] end

    local prev = actions[1]
    for i = 2, #actions do
        prev = CCSequence:createWithTwoActions(prev, actions[i])
    end
    return prev
end