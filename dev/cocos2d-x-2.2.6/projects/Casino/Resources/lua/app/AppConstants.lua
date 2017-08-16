eNotificationName = {
	kClientMsg = "kClientMsg",
	kDayChanged = "kDayChanged",
	kReportPlayer = "kReportPlayer",
	kGoldChange = "kGoldChange",
	kMessChange = "kMessChange",
	kVIPChange = "kVIPChange",
	kSignInChange = "kSignInChange",
	kFlashMsg = "kFlashMsg",
	kFriendChange = "kFriendChange",
	kFirstCharge = "kFirstCharge",
}

eUserDefaultKey = {
	kLocalServerPlayer = "kLocalServerPlayer",
	kLocalServerInfoExt = "kLocalServerInfoExt",
	kLocalServerTreasures = "kLocalServerTreasures",

	kLocalMessageKey = "kLocalMessageKey",
	kLocalChatListKey = "kLocalChatListKey",
	kLocalChatCountKey = "kLocalChatCountKey",
	kLocalReportListKey = "kLocalReportListKey",
	kLocalReportGetKey = "kLocalReportGetKey",
	kLocalIsRankKey = "kLocalIsRankKey",
	kLocalGameChatLastType = "kLocalGameChatLastType",
	kLocalGameChatUseKey = "kLocalGameChatUseKey",
	kLocalLastInputKey = "kLocalLastInputKey",
	kLocalCustomChatKey = "kLocalCustomChatKey",
	kLocalCountryCodeIndex = "kLocalCountryCodeIndex",
	kLocalPhoneNum = "kLocalPhoneNum",
	kLocalPhoneP = "kLocalPhoneP",
}

eUIFont = {
	systembutton = "systembutton",
	paneltitle   = "paneltitle",
	yellowbutton = "yellowbutton",
	poker_black  = "poker_black",
	poker_red    = "poker_red",
	bigwin    	 = "bigwin",
	te1			 = "te1",

	ba_chipsdigital = "ba_chipsdigital",
	ba_tablebet 	= "ba_tablebet",

	vipdigital = "vipdigital",

	fonts_money_own = "fonts_money_own",
	fonts_greenbutton = "fonts_greenbutton",
	fonts_redbutton = "fonts_redbutton",
	fonts_yellowbutton = "fonts_yellowbutton",
	fonts_buqian = "fonts_buqian",
	mjscore = "Arial",

}

eIsShock = {
	agree=1,
	never=0,
}

eUIConst = {
	kCommonDialogTitleHeight  = 60,
	kCommonDialogCloseOffsetX = 13,

	kRankCellHeight = 65,
	kRankCellHeight2 = 100,

	kShopGoldCellHeight = 90,
	kShopPropertyCellHeight = 90,
	kShopBoxCellHeight = 130,
	kShopSellCellHeight = 90,
	kShopSellCellHeight2 = 492,

	screenWidth = 1136,
	screenHeight = 640,

	kTabHeaderLeftGap = 4.5,
	kTabHeaderFlagWidth1 = 99,
	kTabHeaderFlagWidth2 = 124,
	kTabHeaderFlagHeight = 4,
	kTabHeaderHeight = 51,
	kTabHeaderHeight2 = 53,
	kTabHeaderBottomLineHeight = 3,
}

eADPlayStatus = {
	none = 0,
	succ = 1,
	failed = 2,
	half = 3,
}

eTouchPriority = {
	max = -128,
	maskLayer = 1,
	zero = 0,
	pageViewPriority = 1,
}

eHttpResponseCode = {
	ok = 200,
}

eDirect2D = {left = -1, right = 1}

--富文本专用
eRichLocalSize={
	ch = {},
	en = {},
	sp = {},
	img= {},
}

-- ==============  logic related ==============
ePlayerGender = {
	unknown = 0,
	male = 1,
	female = 2,
}

eGoodsType = {
	gold = 1,
	box = 2,
	property = 3,
	pack = 4,
	monthCard = 5,
}

eStoreItemType = {
	goods = 1,
	gems = 2,
	assets = 3,
}

ePairKey = {
	-- 资源1001 ～ 1999
	MEDAL  		= 1001,		--勋章数
	VIP_POINT 	= 1002,		--VIP点
	GOLD 		= 1003,		--筹码
	DIAMOND 	= 1004,		--钻石

	PEGASUS 		= 1101,		--名表
	SOLDIER 		= 1102,		--汽车
	VALLEY_FIELD 	= 1103,		--豪宅
	CASTLE 			= 1104,		--游艇

	-- 商品2001 ～ 2999
	ELEMENTARY_GEM_CARD = 2001, -- 低级宝石卡 6人民币 	->	10元宝
	NORMAL_GEM_CARD     = 2002, -- 普通宝石卡 50人民币 	->	100元宝
	HIGH_GEM_CARD       = 2003, -- 高级宝石卡 200人民币 ->	500元宝

	--  房间内礼物3001~
	gifts_01			= 3001,
	CAKE				= 3101, --蛋糕(节日礼物) 3101~

	-- 道具
	VIP_TICKT 		= 4001, -- 贵宾场入场券
	KICK_CARD 		= 4101, -- 踢人卡
	CHAIR_CARD		= 4102, -- 占座卡
	BROADCAST     	= 4103, -- 小喇叭
	EDIT_CARD     	= 4104, -- 改名卡
	ROOM_CARD 		= 4105, -- 房卡

	COUNT			= 1,     --个数
}

ePosType = {
	center  = 0,
	left    = 1,
	right   = 2,
	top		= 3,
	bottom	= 4,
	leftTop	= 5,
	rightTop= 6,
	leftBottom=7,
	rightBottom=8,
}

ePlayerType = {
	vertical=0,
	horizontal=1,
}

eAraTag = 1

eAcceptFriendType = {
	agree = 1,
	pass = 2,
}

eMsgMgrKey = {
	vipAward = "vipAward",
}



eFriendType={
	friend=1,
	inviteFriend=2,
	marry = 3,
}

eRoleInfoType = {
	hallInfo = 1,
	roomInfo = 2,
	friendInfo = 3,
	rankInfo = 4,
	email = 6,
	CustomizeIndex=14,
}

eHudType = {
	hudWait = 1,
	hudInfo = 2,
	hudSucc = 3,
	hudError = 4,
}

eBroadcastType = {
	system = 1,
	active = 2,
	game   = 3,
	common = 4,
	marry  = 5,
	noTitle = 10,

	player = 100,
	room   = 101,
}

eBroadCellSize=CCSizeMake(740, 52)
eBroadCellSize170=CCSizeMake(740, 52)
eBroadCellSize190=CCSizeMake(686, 54)
eGameCellSize=CCSizeMake(430, 65)

eChatType = {
	edit=1,
	talk=2,
	icon1=3,
	voice=4,
}

eEnterRoomProxy = {
	normal = 0,
	reEnter = 1,
	forceEnter = 2,
	change = 3,
}

eDefaultAvatar = {min = 1,max = 2}
eAvatarSize   = CCSizeMake(225,225)
eBroadcastLen = 20

eErrorCode ={
	-- common
	succ = 0,
	NOT_EXIST_PLAYER 	= 1, -- 玩家不存在
	USER_UNBIND   	 	= 2, -- 该设备游客账户未绑定
	USER_UPGRADED       = 3, -- 游客账户已升级
}

eRoomType = {
	all 		 = 100,		--排行、快速开始使用
	goodsChange	 = 101,		--实物兑换
	slotRoom 	 = 1,		--slot
	BJRoom 		 = 2,		--blackJack
	THRoom 		 = 3,		--Texas Hold'em
	RouletRoom 	 = 4,		--Roulette
	BaccaratRoom = 5, 		--baccarat
	Zhajinhua 	 = 6,		--炸金花
	BJRandom 	 = 8,   	--blackJack 随机庄家

	more = 102,
	quick = 1000,
}

eLeaveType = {
	atOnce = 0,
	gameOver = 1,
	kickOut = 2, -- kick out room
	kickByPlayer = 3,
	closeRoom = 4,
	tournamentEnd = 5,
	machineOccupy = 6,
}


eOnOffModelType = {
	vip = 1,
	report = 2,
	sendGift = 3,
	buyGift = 4,
	soldGift = 5,
	kick = 6,
	talkCard = 8,
	gameStore = 9,
	iappay = 10,
	broadcast= 11,
	editCard = 12,
	inviteFriend = 13,
	playerInfo = 14,
	agreement = 15,
	ownSeat = 16,
	findKey = 18,
	paypal = 20,
	paymentWall = 21,
	freeCoin = 23,
	vipRank = 30,
	molPay = 31,
	createRoom = 32,
	introduce = 33,
	inviteKey = 34,
}

eResType = {
	image = 1,
	pvr = 2,
	music = 3,
	effect = 4,
	font = 5,
}

--首冲状态
eFirstChargeStatus = {
    noCharge = 0,
    canGet = 1,
    haveGot = 2,
    noData = -1,
}

eStencilMaskType = {
	round = 1,   -- 圆形
	square  = 2,  --方形
	rect  = 3,    -- 长方形
	newRect = 4,  -- 170版本新手引导
}

eButtonType = {
	green = 1,
	red = 2,
	yellow = 3,
}

eIntroduceStatus = {
	canAward = 0,
	dealing = 1,
	done = 2,
}

eMJCardImgType = {
	top = 0
}

eMJImgType = {
	discard  = 1,      --已出的牌
	pair     = 2,	   --碰或者杠的牌
	stand    = 3,      --立着的牌（还在牌库中）
	back 	 = 4,      --扣着的牌（用于暗杠）
}

eMJPType = {
	bottom 	= 1,		-- 自己
	right 	= 2,		-- 右家
	top 	= 3,		-- 上家
	left 	= 4,		-- 左家
}


eMJDirect = {
	east 	= 1,		-- 东
	south 	= 2,		-- 南
	west 	= 3,		-- 西
	north 	= 4,		-- 北
}


eMJOperation = {
	peng     = 1,		--碰
	hu       = 2,		--胡
	chi      = 3,       --吃
	ming     = 4,		--明杠
	an       = 5,		--暗杠
	guo      = 6,     	--过
	multiHu  = 7, 		--一炮多响
	pengGang = 8,		--碰后杠
	otherOp  = 9,		--别人正在操作
	ziHu     = 10,      --自摸胡
	paoHu    = 11,      --点炮胡
}

eMJVoteType = {
	noVote = 0,        --没有投票
	agree  = 1,        --赞同
	refuse = 2,	       --拒绝
}

eMJCloseRoom = {
	gameOver 	= 0, 		--牌局正常结束
	forceClose 	= 1,		--开始游戏前房主强制解散房间
	voteClose 	= 2,		--投票通过解散房间
}

eMJGameStatus = {
	playingRound = 1,
	oneRoundEnd  = 2,
	allRoundEnd  = 3,
	beforeRound  = 4,
}

ePayType = {
	appstore = 1,
	iappay   = 2,
}

-- 邀请奖励是否领取
eInviteAwardState = {
	available = 0,
	unavailable = 1,
}

eMJRuleItem = {
	--河北麻将
	[1] = {
		[1]  = "带风牌",
		[2]  = "可吃牌",
		[3]  = "点炮可胡",
		[4]  = "黄庄黄杠",
		[5]  = "带庄闲",
		[6]  = "门清",
		[7]  = "边卡吊",
		[8]  = "捉五魁",
		[9]  = "带混儿",
		[10] = "素胡",
		[11] = "混儿吊",
		[12] = "点炮一家出",
		[13] = "点炮三家出",
		[14] = "点炮大包",
	},
	--郑州麻将
	[2] = {
		[1] = "带风牌",
		[2] = "带混儿",
		[3] = "买跑",
		[4] = "可点炮胡",
		[5] = "七对加倍",
		[6] = "杠上花加倍",
		[7] = "庄家加底",
		[8] = "杠跑",
	},
	--推倒胡
	[3] = {
		[1] = "点炮可胡",
		[2] = "只能自摸胡",
		[3] = "买跑",
		[4] = "杠上花加倍",
		[5] = "杠跑",
	},
	--开封麻将
	[4] = {
		[1] = "点炮可胡",
		[2] = "只能自摸胡",
		[3] = "买跑",
		[4] = "自摸翻倍",
		[5] = "暗杠翻倍",
		[6] = "杠上花加倍",
		[7] = "七对翻倍",
		[8] = "杠跑",
	}}
