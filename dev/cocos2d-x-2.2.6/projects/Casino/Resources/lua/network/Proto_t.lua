PB_TABLE = {
	AbsMessage = {
		{"optional", "string", "token"},
		{"optional", "int32", "msgID"},
		{"optional", "bytes", "msgBody"},
	},
	Player = {
		{"optional", "int32", "id"},
		{"optional", "string", "nickname"},
		{"optional", "string", "avatar"},
		{"optional", "int32", "gender"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "cards"},
		{"optional", "string", "ip"},
	},
	Jushu = {
		{"optional", "int32", "jushu"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "cards"},
		{"optional", "int32", "mjType"},
		{"optional", "string", "mjDesc"},
	},
	InvitePlayer = {
		{"optional", "int32", "id"},
		{"optional", "string", "nickname"},
		{"optional", "string", "avatar"},
		{"optional", "int32", "state"},
	},
	Goods = {
		{"optional", "int32", "id"},
		{"optional", "int32", "count"},
		{"optional", "int32", "extra"},
		{"optional", "int32", "price"},
		{"optional", "string", "iconUrl"},
		{"optional", "int32", "payType"},
		{"optional", "string", "productID"},
		{"optional", "int32", "sort"},
	},
	OrderStatus = {
		{"required", "string", "orderID"},
		{"optional", "int32", "status"},
		{"optional", "string", "errorDesc"},
		{"optional", "string", "jsonStr"},
		{"optional", "int32", "gemID"},
	},
	OrderData = {
		{"required", "int32", "payType"},
		{"required", "string", "jsonStr"},
	},
	RoomPlayer = {
		{"optional", "int32", "pos"},
		{"optional", "int32", "score"},
		{"optional", "Player", "player"},
		{"optional", "double", "latitude"},
		{"optional", "double", "longitude"},
	},
	CardsInfo = {
		{"optional", "int32", "pos"},
		{"optional", "int32", "DrawCard"},
		{"optional", "int32", "paoScore"},
		{"repeated", "int32", "pengCards"},
		{"repeated", "int32", "chiCards"},
		{"repeated", "int32", "anGangCards"},
		{"repeated", "int32", "mingGangCards"},
		{"repeated", "int32", "discardCards"},
		{"repeated", "int32", "leftCards"},
	},
	VoteInfo = {
		{"optional", "int32", "pos"},
		{"optional", "int32", "action"},
	},
	MutiHu = {
		{"optional", "int32", "pos"},
		{"repeated", "int32", "cardList"},
	},
	ResultItem = {
		{"optional", "int32", "pos"},
		{"optional", "int32", "drawCard"},
		{"optional", "int32", "score"},
		{"optional", "int32", "isHu"},
		{"optional", "int32", "huScore"},
		{"optional", "int32", "gangScore"},
		{"optional", "int32", "paoScore"},
		{"optional", "int32", "curScore"},
		{"repeated", "int32", "pengCards"},
		{"repeated", "int32", "chiCards"},
		{"repeated", "int32", "gangCards"},
		{"repeated", "int32", "leftCards"},
		{"repeated", "int32", "huTypes"},
	},
	SettleItem = {
		{"optional", "int32", "pos"},
		{"optional", "int32", "score"},
		{"optional", "int32", "ziMoCnt"},
		{"optional", "int32", "jiePaoCnt"},
		{"optional", "int32", "dianPaoCnt"},
	},
	RecordRound = {
		{"optional", "int32", "curRound"},
		{"optional", "int32", "roundID"},
		{"optional", "string", "startTime"},
		{"repeated", "int32", "scoreList"},
	},
	RecordRoom = {
		{"optional", "int32", "roomID"},
		{"optional", "int32", "mjType"},
		{"optional", "int32", "totalRound"},
		{"optional", "string", "createTime"},
		{"repeated", "string", "nameList"},
		{"repeated", "int32", "scoreList"},
		{"repeated", "RecordRound", "roundList"},
	},
	OperationItem = {
		{"optional", "int32", "type"},
		{"optional", "int32", "keycard"},
		{"optional", "int32", "srcPos"},
		{"optional", "int32", "desPos"},
		{"repeated", "int32", "cardsList"},
		{"repeated", "MutiHu", "huInfoList"},
	},
	LoginSend = {
		{"optional", "int32", "channel"},
		{"optional", "int32", "version"},
		{"optional", "int32", "loginType"},
		{"optional", "string", "session"},
		{"optional", "string", "machineID"},
		{"optional", "int32", "playerID"},
	},
	LoginRecv = {
		{"optional", "int32", "status"},
		{"optional", "string", "logicServer"},
	},
	LogoutSend = {
	},
	LogoutRecv = {
		{"optional", "int32", "status"},
	},
	AuthSend = {
	},
	AuthRecv = {
		{"optional", "int32", "status"},
	},
	DirtySend = {
	},
	DirtyRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "roomID"},
		{"optional", "int32", "highID"},
		{"optional", "Player", "player"},
		{"optional", "string", "wchatID"},
		{"optional", "string", "module"},
		{"repeated", "Jushu", "jushus"},
	},
	ErrorSend = {
	},
	ErrorRecv = {
		{"optional", "int32", "status"},
	},
	FeedbackSend = {
		{"optional", "int32", "playerID"},
		{"optional", "string", "imgUrl"},
		{"optional", "string", "text"},
	},
	FeedbackRecv = {
		{"optional", "int32", "status"},
	},
	ResourceChangeSend = {
	},
	ResourceChangeRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "cards"},
	},
	BroadcastSend = {
	},
	BroadcastRecv = {
		{"optional", "int32", "status"},
		{"repeated", "string", "text"},
	},
	InviteCodeSend = {
		{"optional", "int32", "code"},
	},
	InviteCodeRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "highID"},
		{"optional", "int32", "coins"},
	},
	InviteListSend = {
	},
	InviteListRecv = {
		{"optional", "int32", "status"},
		{"repeated", "InvitePlayer", "players"},
	},
	InviteAwardSend = {
		{"optional", "int32", "id"},
	},
	InviteAwardRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "id"},
	},
	GoodsListSend = {
	},
	GoodsListRecv = {
		{"optional", "int32", "status"},
		{"repeated", "Goods", "goods"},
	},
	OrderApplySend = {
		{"optional", "int32", "playerID"},
		{"optional", "int32", "channel"},
		{"optional", "int32", "waresID"},
	},
	OrderApplyRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "cards"},
		{"optional", "string", "orderID"},
		{"optional", "int32", "price"},
		{"optional", "int32", "payType"},
		{"optional", "int32", "count"},
		{"optional", "string", "productID"},
	},
	OrderApplyNewSend = {
		{"repeated", "OrderData", "orderList"},
	},
	OrderApplyNewRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "coins"},
		{"optional", "int32", "cards"},
		{"repeated", "OrderStatus", "orderList"},
	},
	EnterRoomSend = {
		{"optional", "int32", "roomID"},
		{"optional", "int32", "mjType"},
		{"optional", "int32", "costType"},
		{"optional", "int32", "round"},
		{"optional", "int32", "relink"},
		{"optional", "bool", "cheat"},
		{"optional", "double", "latitude"},
		{"optional", "double", "longitude"},
		{"repeated", "int32", "configs"},
	},
	EnterRoomRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "gameStatus"},
		{"optional", "int32", "roomID"},
		{"optional", "int32", "totalRound"},
		{"optional", "int32", "curRound"},
		{"optional", "int32", "leftCards"},
		{"optional", "int32", "bankerPos"},
		{"optional", "int32", "curPos"},
		{"optional", "int32", "lastPos"},
		{"optional", "int32", "hunCard"},
		{"optional", "int32", "mjType"},
		{"optional", "bool", "cheat"},
		{"repeated", "int32", "configs"},
		{"repeated", "RoomPlayer", "playerList"},
		{"repeated", "CardsInfo", "cardsInfoList"},
	},
	OtherJoinSend = {
	},
	OtherJoinRecv = {
		{"optional", "int32", "status"},
		{"optional", "RoomPlayer", "player"},
	},
	ExitRoomSend = {
	},
	ExitRoomRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "pos"},
	},
	CloseRoomSend = {
	},
	CloseRoomRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "closeType"},
	},
	VoteCloseSend = {
		{"optional", "int32", "action"},
	},
	VoteCloseRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "leftTime"},
		{"optional", "int32", "firstPos"},
		{"repeated", "VoteInfo", "voteList"},
	},
	GameChatSend = {
		{"optional", "int32", "types"},
		{"optional", "int32", "messID"},
		{"optional", "string", "text"},
	},
	GameChatRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "playerID"},
		{"optional", "string", "nickName"},
		{"optional", "int32", "types"},
		{"optional", "int32", "messID"},
		{"optional", "string", "text"},
		{"optional", "string", "avatar"},
	},
	PrepareGameSend = {
	},
	PrepareGameRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "pos"},
	},
	CancelPrepareSend = {
	},
	CancelPrepareRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "pos"},
	},
	StartRoundSend = {
	},
	StartRoundRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "curRound"},
		{"optional", "int32", "leftCards"},
		{"optional", "int32", "bankerPos"},
		{"optional", "int32", "totalRound"},
		{"optional", "int32", "hunCard"},
		{"optional", "int32", "preHunCard"},
		{"repeated", "int32", "cardsCount"},
		{"repeated", "int32", "myCards"},
		{"repeated", "int32", "scoreList"},
	},
	EndRoundSend = {
	},
	EndRoundRecv = {
		{"optional", "int32", "status"},
		{"repeated", "ResultItem", "itemList"},
	},
	SettlementSend = {
	},
	SettlementRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "isVoteClose"},
		{"repeated", "SettleItem", "SettleList"},
	},
	GameRecordSend = {
	},
	GameRecordRecv = {
		{"optional", "int32", "status"},
		{"repeated", "RecordRoom", "records"},
	},
	DrawCardSend = {
	},
	DrawCardRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "pos"},
		{"optional", "int32", "card"},
		{"repeated", "int32", "tingCards"},
	},
	DiscardSend = {
		{"optional", "int32", "card"},
	},
	DiscardRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "card"},
		{"optional", "int32", "pos"},
		{"repeated", "int32", "leftCards"},
	},
	OperationSend = {
		{"optional", "OperationItem", "operation"},
	},
	OperationRecv = {
		{"optional", "int32", "status"},
		{"optional", "OperationItem", "operation"},
	},
	DiscardNoticeSend = {
	},
	DiscardNoticeRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "pos"},
	},
	OperationNoticeSend = {
	},
	OperationNoticeRecv = {
		{"optional", "int32", "status"},
		{"repeated", "OperationItem", "operationList"},
	},
	BuyPaoListSend = {
	},
	BuyPaoListRecv = {
		{"optional", "int32", "status"},
		{"repeated", "int32", "paoScoreList"},
	},
	BuyPaoSend = {
		{"optional", "int32", "paoScore"},
	},
	BuyPaoRecv = {
		{"optional", "int32", "status"},
		{"optional", "int32", "paoScore"},
		{"optional", "int32", "pos"},
	},
}