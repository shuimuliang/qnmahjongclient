-- ====================== LCGameChatData ====================== --
LCGameChatData = class("LCGameChatData")

function LCGameChatData:ctor()
	CC_SYNTHESIZE(self, "currentUseList", {})
	CC_SYNTHESIZE(self, "customChatList", {})
end

function LCGameChatData:updateChatByPB(data) 
	if not data then return end	
	if data.types~=eChatType.icon1 then return end
	self.currentUseList=self:getCurrentUse()
	local chatData={types=data.types, id=data.messID, text=data.text, name=data.text}	
	if chatData then
		for i=#self.currentUseList, 1, -1 do
			local v=self.currentUseList[i]
			if v.types==chatData.types and v.id==chatData.id then
				table.remove(self.currentUseList, i)
			end
		end
		table.insert(self.currentUseList, chatData)
		
		self:saveChatList()
	end
end

function LCGameChatData:updateCustomList( list ) 
	if not list then return end	
	
	self.customChatList=list
	self:saveCustomList()
end

function LCGameChatData:getCurrentUse()
	if not self.currentUseList or #self.currentUseList<=0 then
		self.currentUseList=LCUserContext:getObjectForKey(eUserDefaultKey.kLocalGameChatUseKey, {})
	end
	return self.currentUseList
end

function LCGameChatData:saveChatList()
	if not self.currentUseList or #self.currentUseList<=0 then return end
	LCUserContext:setObjectForKey(eUserDefaultKey.kLocalGameChatUseKey, self.currentUseList)
end


function LCGameChatData:getCustomList()
	if not self.customChatList or #self.customChatList<=0 then
		self.customChatList=LCUserContext:getObjectForKey(eUserDefaultKey.kLocalCustomChatKey, {})
	end
	return self.customChatList
end

function LCGameChatData:saveCustomList()
	if not self.customChatList then return end
	LCUserContext:setObjectForKey(eUserDefaultKey.kLocalCustomChatKey, self.customChatList)
end




