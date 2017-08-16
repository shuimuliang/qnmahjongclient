
™?
pf.protomj"R

AbsMessage
token (	Rtoken
msgID (RmsgID
msgBody (RmsgBody"†
Player
id (Rid
nickname (	Rnickname
avatar (	Ravatar
gender (Rgender
coins (Rcoins
cards (Rcards
ip (	Rip"y
Jushu
jushu (Rjushu
coins (Rcoins
cards (Rcards
mjType (RmjType
mjDesc (	RmjDesc"h
InvitePlayer
id (Rid
nickname (	Rnickname
avatar (	Ravatar
state (Rstate"ø
Goods
id (Rid
count (Rcount
extra (Rextra
price (Rprice
iconUrl (	RiconUrl
payType (RpayType
	productID (	R	productID
sort (Rsort"ç
OrderStatus
orderID (	RorderID
status (Rstatus
	errorDesc (	R	errorDesc
jsonStr (	RjsonStr
gemID (RgemID"?
	OrderData
payType (RpayType
jsonStr (	RjsonStr"í

RoomPlayer
pos (Rpos
score (Rscore"
player (2
.mj.PlayerRplayer
latitude (Rlatitude
	longitude (R	longitude"ô
	CardsInfo
pos (Rpos
	pengCards (R	pengCards
chiCards (RchiCards 
anGangCards (RanGangCards$
mingGangCards (RmingGangCards"
discardCards (RdiscardCards
	leftCards (R	leftCards
DrawCard (RDrawCard
paoScore	 (RpaoScore"4
VoteInfo
pos (Rpos
action (Raction"6
MutiHu
pos (Rpos
cardList (RcardList"‰

ResultItem
pos (Rpos
	pengCards (R	pengCards
chiCards (RchiCards
	gangCards (R	gangCards
	leftCards (R	leftCards
drawCard (RdrawCard
score (Rscore
isHu (RisHu
huTypes	 (RhuTypes
huScore
 (RhuScore
	gangScore (R	gangScore
paoScore (RpaoScore
curScore (RcurScore"å

SettleItem
pos (Rpos
score (Rscore
ziMoCnt (RziMoCnt
	jiePaoCnt (R	jiePaoCnt

dianPaoCnt (R
dianPaoCnt"
RecordRound
curRound (RcurRound
roundID (RroundID
	startTime (	R	startTime
	scoreList (R	scoreList"Â

RecordRoom
roomID (RroomID
mjType (RmjType

totalRound (R
totalRound

createTime (	R
createTime
nameList (	RnameList
	scoreList (R	scoreList-
	roundList (2.mj.RecordRoundR	roundList"∑
OperationItem
type (Rtype
keycard (Rkeycard
	cardsList (R	cardsList
srcPos (RsrcPos
desPos (RdesPos*

huInfoList (2
.mj.MutiHuR
huInfoList"’
	LoginSend
channel (Rchannel
version (Rversion
	loginType (R	loginType
session (	Rsession
	machineID (	R	machineID
playerID (RplayerID"
refreshToken (	RrefreshToken"E
	LoginRecv
status (Rstatus 
logicServer (	RlogicServer"

LogoutSend"$

LogoutRecv
status (Rstatus"

AuthSend""
AuthRecv
status (Rstatus"
	DirtySend"
	DirtyRecv
status (Rstatus
roomID (RroomID
highID (RhighID"
player (2
.mj.PlayerRplayer!
jushus (2	.mj.JushuRjushus
wchatID (	RwchatID
module (	Rmodule"
refreshToken (	RrefreshToken"
	ErrorSend"#
	ErrorRecv
status (Rstatus"V
FeedbackSend
playerID (RplayerID
imgUrl (	RimgUrl
text (	Rtext"&
FeedbackRecv
status (Rstatus"
ResourceChangeSend"X
ResourceChangeRecv
status (Rstatus
coins (Rcoins
cards (Rcards"
BroadcastSend";
BroadcastRecv
status (Rstatus
text (	Rtext"$
InviteCodeSend
code (Rcode"V
InviteCodeRecv
status (Rstatus
highID (RhighID
coins (Rcoins"
InviteListSend"T
InviteListRecv
status (Rstatus*
players (2.mj.InvitePlayerRplayers"!
InviteAwardSend
id (Rid"O
InviteAwardRecv
status (Rstatus
coins (Rcoins
id (Rid"
GoodsListSend"H
GoodsListRecv
status (Rstatus
goods (2	.mj.GoodsRgoods"`
OrderApplySend
playerID (RplayerID
channel (Rchannel
waresID (RwaresID"“
OrderApplyRecv
status (Rstatus
coins (Rcoins
cards (Rcards
orderID (	RorderID
price (Rprice
payType (RpayType
count (Rcount
	productID (	R	productID"@
OrderApplyNewSend+
	orderList (2.mj.OrderDataR	orderList"Ü
OrderApplyNewRecv
status (Rstatus
coins (Rcoins
cards (Rcards-
	orderList (2.mj.OrderStatusR	orderList"Û
EnterRoomSend
roomID (RroomID
configs (Rconfigs
mjType (RmjType
costType (RcostType
round (Rround
relink (Rrelink
cheat (Rcheat
latitude (Rlatitude
	longitude	 (R	longitude"–
EnterRoomRecv
status (Rstatus

gameStatus (R
gameStatus
roomID (RroomID

totalRound (R
totalRound
curRound (RcurRound
	leftCards (R	leftCards
	bankerPos (R	bankerPos
curPos (RcurPos
lastPos	 (RlastPos
configs
 (Rconfigs.

playerList (2.mj.RoomPlayerR
playerList3
cardsInfoList (2.mj.CardsInfoRcardsInfoList
hunCard (RhunCard
mjType (RmjType
cheat (Rcheat"
OtherJoinSend"O
OtherJoinRecv
status (Rstatus&
player (2.mj.RoomPlayerRplayer"
ExitRoomSend"8
ExitRoomRecv
status (Rstatus
pos (Rpos"
CloseRoomSend"E
CloseRoomRecv
status (Rstatus
	closeType (R	closeType"'
VoteCloseSend
action (Raction"â
VoteCloseRecv
status (Rstatus
leftTime (RleftTime
firstPos (RfirstPos(
voteList (2.mj.VoteInfoRvoteList"P
GameChatSend
types (Rtypes
messID (RmessID
text (	Rtext"∏
GameChatRecv
status (Rstatus
playerID (RplayerID
nickName (	RnickName
types (Rtypes
messID (RmessID
text (	Rtext
avatar (	Ravatar"
PrepareGameSend";
PrepareGameRecv
status (Rstatus
pos (Rpos"
CancelPrepareSend"=
CancelPrepareRecv
status (Rstatus
pos (Rpos"
StartRoundSend"≤
StartRoundRecv
status (Rstatus
curRound (RcurRound
	leftCards (R	leftCards
	bankerPos (R	bankerPos

cardsCount (R
cardsCount
myCards (RmyCards

totalRound (R
totalRound
	scoreList (R	scoreList
hunCard	 (RhunCard

preHunCard
 (R
preHunCard"
EndRoundSend"R
EndRoundRecv
status (Rstatus*
itemList (2.mj.ResultItemRitemList"
SettlementSend"z
SettlementRecv
status (Rstatus.

SettleList (2.mj.SettleItemR
SettleList 
isVoteClose (RisVoteClose"
GameRecordSend"R
GameRecordRecv
status (Rstatus(
records (2.mj.RecordRoomRrecords"
DrawCardSend"j
DrawCardRecv
status (Rstatus
pos (Rpos
card (Rcard
	tingCards (R	tingCards"!
DiscardSend
card (Rcard"i
DiscardRecv
status (Rstatus
card (Rcard
pos (Rpos
	leftCards (R	leftCards"@
OperationSend/
	operation (2.mj.OperationItemR	operation"X
OperationRecv
status (Rstatus/
	operation (2.mj.OperationItemR	operation"
DiscardNoticeSend"=
DiscardNoticeRecv
status (Rstatus
pos (Rpos"
OperationNoticeSend"f
OperationNoticeRecv
status (Rstatus7
operationList (2.mj.OperationItemRoperationList"
BuyPaoListSend"L
BuyPaoListRecv
status (Rstatus"
paoScoreList (RpaoScoreList"(

BuyPaoSend
paoScore (RpaoScore"R

BuyPaoRecv
status (Rstatus
paoScore (RpaoScore
pos (Rpos*ü

ActionType	
Logind
Logout»	
Auth¨

Dirtyê

ErrorÙ
Feedbackÿ
ResourceChangeº
	Broadcast†

InviteCodeÃ

InviteList∞	
InviteAwardî

	GoodsList¯


OrderApply‹
OrderApplyNew¿
	EnterRoom¥
	OtherJoinò
ExitRoom¸
	CloseRoom‡
	VoteCloseƒ
GameChat®
PrepareGameú
CancelPrepareÄ

StartRound‰
EndRound»

Settlement¨

GameRecordê

BuyPaoListÙ
BuyPaoÿ
DrawCardÑ 
DiscardË 
	OperationÃ!
DiscardNotice∞"
OperationNoticeî#