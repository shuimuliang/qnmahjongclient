LOCALIZABLE_WORDS = {
--  ======= goods =========
	["goods.noExchange"] = "您好像什么奖品都没有兑换过哦！",
	["goods.confirm"] = "是否确定花%s个勋章兑换%s?",
	["goods.end.desc"] = "本期兑换已经结束",
	["goods.next.desc"] = "下期兑换于%s开启",
	["goods.left.desc"] = "距离活动结束还有 : %s",
	["goods.declare"] = "该活动和苹果公司无关",
	["goods.ymd"] = "%s年%s月%s日",
	["goods.dhm"] = "%s天%s时%s分",
	["goods.record.no"] = "暂无兑换记录，赶快来兑换吧！",
	["goods.record.yes"] = "该礼物兑换次数 : %s",
	["goods.tip"] = "您目前暂未兑换奖品，为了你喜欢的礼物奋斗吧！",
	["goods.exchange"] = "兑换",

	["goods.status"] = "兑换状态 ：",
	["goods.time"] = "兑换时间 ：%s",
	["goods.status.des1"] = "提交审核",
	["goods.status.des2"] = "已完成",
	["goods.user.name"] = "收件人 ：%s",
	["goods.user.phone"] = "电话 ：%s",
	["goods.user.address"] = "收件地址 : ",

	["goods.placeholder.name"] = "请填写姓名",
	["goods.placeholder.phone"] = "请留电话方便联系",
	["goods.placeholder.address"] = "请填写真实有效的收货地址",
	["goods.tip.phone"] = "请输入正确的电话",
	["goods.tip.name"] = "请输入正确的姓名",
	["goods.tip.address"] = "请输入正确的地址",
	["goods.tip.long.address"] = "您输入的地址信息过长！",
	["goods.tip.long.phone"] = "您输入的电话信息过长！",
	["goods.tip.long.name"] = "您输入的姓名信息过长！",

--  =======goods exchange ======
	["goods.exchange.title1"] = "奖品兑换",
	["goods.exchange.title2"] = "我的奖品",

	--========= zjh ======
	["zjh.result.win"] = "胜利",
	["zjh.result.lost"] = "失败",
	["zjh.giveUp"] = "放弃",
	["zjh.pk"] = "比牌",
	["zjh.look"] = "看牌",
	["zjh.raise"] = "加注",
	["zjh.follow"] = "跟注",
	["zjh.autoBet"] = "自动跟注",
	["zjh.betAll"] = "押满",
	["zjh.betAllRequest"] = "当前可以押满的金额为：%s",
	-- ["zjh.betAllResponse"] = "对方押满，您跟注的金额为：%s",
	["zjh.changeDesk"] = "换桌",
	["zjh.continue"] = "继续",
	-- ["zjh.roundLimit"] = "押注轮数已达上限，将自动比牌。",
	["zjh.autoFold"] = "筹码不足，如不购买筹码，则将自动放弃。",
    ["zjh.willPk"] = "筹码不足完成游戏，是否比牌？",
}

if DEF_PATH then
	if DEF_DESGIN_LANGUAGE ~= kLanguageChinese then
		LOCALIZABLE_WORDS = {}
	end

	local script = "function openLua() return require " .. "\"" .. DEF_PATH.db .. "bwordid" .. "\" end"
	local func, err = loadstring(script)
	if err then GError(err) return end
	func()

	-- load data
	local dataList = openLua()
	local count = table.getn(dataList)

	for i=1,#dataList do
		local data = dataList[i]
		local key = tostring(data["wordid"])
		LOCALIZABLE_WORDS[key] = data["wordstr"]
	end
else
	return LOCALIZABLE_WORDS
end
