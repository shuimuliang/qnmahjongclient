local SystemSwitches = {}

local _data = {}

function SystemSwitches.update(data)
	_data = data or _data
end

function SystemSwitches.closeWeixinLogin()
	return checkbool(_data.weixin_login)
end

function SystemSwitches.closeInvite()
	return checkbool(_data.invite_award)
end

return SystemSwitches
