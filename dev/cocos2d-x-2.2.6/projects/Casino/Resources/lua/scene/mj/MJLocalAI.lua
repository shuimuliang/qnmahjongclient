
local LCMJLocalAI = class( "LCMJLocalAI")

function LCMJLocalAI:ctor(tableView)
	self.tableView = tableView
end

-- function LCMJLocalAI:testDraw()
-- 	local num = math.random(1,29)
-- 	while(num == 10 or num == 20) do
-- 		num = math.random(1,29)
-- 	end
-- 	local data = LCMJCardData.new(num)
-- 	self.tableView:drawCard(eMJPType.bottom, data)
-- end


function LCMJLocalAI:testPeng(pos)
	if not pos then pos = 2 end
	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.operation = {type = 1 , keycard = 2, srcPos = 2, desPos = pos}

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperation(pb)
end

function LCMJLocalAI:test1(pos )
	if not pos then pos = 2 end
	local pb = LCNetManager:createMessage(eDefMsg.Discard)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.card = 4
	pb.msg.pos = pos

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgDiscard(pb)
end

function LCMJLocalAI:testDraw( )
	local pb = LCNetManager:createMessage(eDefMsg.DrawCard)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.card = 3
	pb.msg.pos = 1

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgDrawCard(pb)

	-- self:aiDiscardNotice()
end


function LCMJLocalAI:testInit()
	local pb = LCNetManager:createMessage(eDefMsg.StartRound)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.curRound = 0
	pb.msg.leftCards = 0
	pb.msg.bankerPos = 0
	pb.msg.cardsCount = {13,13,13,13}
	pb.msg.myCards = {1,2,3,2,2,20,30,25,17,21,22,32,6}
	-- pb.msg.myCards = {2,2,2,2,2,2,2,2,2,2,2,2,2}
	pb.msg.totalRound = 8

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgStartRound(pb)
end

-- function LCMJLocalAI:aiDiscardNotice()
-- 	local pb = LCNetManager:createMessage(eDefMsg.DiscardNotice)
-- 	pb.msg = {}
-- 	pb.msg.status = 0
-- 	pb.msg.pos = 1

-- 	local scene = LCSceneTransit:getRunningScene()
-- 	local messageMgr = scene.messagesMgr
-- 	messageMgr:onMsgDiscardNotice(pb)
-- end


function LCMJLocalAI:testChi(pos)
	if not pos then pos = 2 end

	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.operation = {type = 3,keycard = 2, srcPos = 3, desPos = pos, cardsList = {1,2,3} }

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperation(pb)
end

function LCMJLocalAI:aiDiscardNotice()
	local pb = LCNetManager:createMessage(eDefMsg.OperationNotice)
	pb.msg = {}
	pb.msg.status = 0
	local item1 = {type = 1,keycard = 3, srcPos = 3, desPos = 2}
	local item2 = {type = 3,keycard = 3, srcPos = 3, desPos = 2, cardsList = {1,2,3,2,3,4,3,4,5} }
	-- local item3 = {type = 3,keycard = 2, srcPos = 3, desPos = 2}
	-- local item4 = {type = 3,keycard = 2, srcPos = 3, desPos = 2}

	pb.msg.operationList = {item2,item1}

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperationNotice(pb)
end

function LCMJLocalAI:aiOperation(opType)
	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.operation = {type = opType , keycard = 2, srcPos = 3, desPos = 2}

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperation(pb)
end

function LCMJLocalAI:aiPengGang()
	local pb = LCNetManager:createMessage(eDefMsg.OperationNotice)
	pb.msg = {}
	pb.msg.status = 0
	local item1 = {type = 8,keycard = 2, srcPos = 3, desPos = 1}
	local item4 = {type = 6,keycard = 2, srcPos = 3, desPos = 1}

	pb.msg.operationList = {item4,item1}

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperationNotice(pb)
end


function LCMJLocalAI:aiHu(pos )
	if not pos then pos = 2 end
	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.operation = {type = 2 , keycard = 2, srcPos = 3, desPos = pos, cardsList = {2,3,4,5,6,7,8,9,10,11,12,13,12}}

	local scene = LCSceneTransit:getRunningScene()
	local messageMgr = scene.messagesMgr
	messageMgr:onMsgOperation(pb)
end

function LCMJLocalAI:aiRelink()
	local mjplayer1 = {player = {id = 21211, nickname = "a" , avatar = "" , coins = 3, cards = 3} ,score = 10, pos = 1}
	local mjplayer2 = {player = {id = 21211, nickname = "b" , avatar = "" , coins = 3, cards = 3} ,score = 20, pos = 2}
	local mjplayer3 = {player = {id = 21211, nickname = "c" , avatar = "" , coins = 3, cards = 3} ,score = 30, pos = 3}
	local mjplayer4 = {player = {id = 21211, nickname = "d" , avatar = "" , coins = 3, cards = 3} ,score = 40, pos = 4}


	local cardInfo1 = {pos = 1, pengCards = {1,1,1}, chiCards = {11,12,13} ,anGangCards= {7,7,7,7}, mingGangCards = {21,21,21,21}, discardCards = {3,4,5,6,7} , leftCards = {21,22,23,24,25,26} }
	local cardInfo2 = {pos = 2, pengCards = {1,1,1}, chiCards = {11,12,13} ,anGangCards= {7,7,7,7}, mingGangCards = {21,21,21,21}, discardCards = {3,4,5,6,7} , leftCards = {21,22,23,24,25,26} }
	local cardInfo3 = {pos = 3, pengCards = {1,1,1}, chiCards = {11,12,13} ,anGangCards= {7,7,7,7}, mingGangCards = {21,21,21,21}, discardCards = {3,4,5,6,7} , leftCards = {21,22,23,24,25,26} }
	local cardInfo4 = {pos = 4, pengCards = {1,1,1}, chiCards = {11,12,13} ,anGangCards= {7,7,7,7}, mingGangCards = {21,21,21,21}, discardCards = {3,4,5,6,7} , leftCards = {21,22,23,24,25,26} }

	local pb = LCNetManager:createMessage(eDefMsg.EnterRoom)
	pb.msg = {}
	pb.msg.status = 0
	pb.msg.gameStatus = 1
	pb.msg.roomID = 13404
	pb.msg.totalRound = 8
	pb.msg.curRound = 1
	pb.msg.leftCards = 136
	pb.msg.bankerPos = 1
	pb.msg.curPos = 1
	pb.msg.configs = {1,3,5,6,7,12}
	pb.msg.playerList = {mjplayer1,mjplayer2,mjplayer3,mjplayer4}
	pb.msg.cardsInfoList = {cardInfo1,cardInfo2,cardInfo3,cardInfo4}

	

end

return LCMJLocalAI
