
LCMJSendMgr = {}

function LCMJSendMgr.sendPrepare()
	local pb = LCNetManager:createMessage(eDefMsg.PrepareGame)
	LCNetManager:send(pb)
end

function LCMJSendMgr.sendDiscard(cardNum)
	local pb = LCNetManager:createMessage(eDefMsg.Discard)
	pb._msg.card = cardNum
	LCNetManager:send(pb)
end

function LCMJSendMgr.sendOperation(operationItem)
	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb._msg.operation = operationItem
	LCNetManager:send(pb)


    -- local scene = LCSceneTransit:getRunningScene()
    -- local desk = scene:getDeskView()
    -- local ai = desk.ai
    -- ai:aiOperation(operationItem.type)
end

function LCMJSendMgr.sendChiOperation(chiNumList,keycard,srcPos,desPos)
	local operationItem = {}
	operationItem.type = eMJOperation.chi
	operationItem.keycard = keycard
	operationItem.cardsList = chiNumList
	operationItem.srcPos = srcPos
	operationItem.desPos = desPos

	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb._msg.operation = operationItem
	LCNetManager:send(pb)
end

function LCMJSendMgr.sendAnGangOperation(keycard,srcPos,desPos)
	local operationItem = {}
	operationItem.type = eMJOperation.an
	operationItem.keycard = keycard
	-- operationItem.cardsList = {}
	operationItem.srcPos = srcPos
	operationItem.desPos = desPos

	local pb = LCNetManager:createMessage(eDefMsg.Operation)
	pb._msg.operation = operationItem
	LCNetManager:send(pb)
end

function LCMJSendMgr.sendVoteClose(voteType)
	local pb = LCNetManager:createMessage(eDefMsg.VoteClose)
	pb._msg.action = voteType
	LCNetManager:send(pb)
end


function LCMJSendMgr.sendExitRoom()
	local pb = LCNetManager:createMessage(eDefMsg.ExitRoom)
	LCNetManager:send(pb)
end

function LCMJSendMgr.sendCloseRoom()
	local pb = LCNetManager:createMessage(eDefMsg.CloseRoom)
	LCNetManager:send(pb)
end


return LCMJSendMgr
