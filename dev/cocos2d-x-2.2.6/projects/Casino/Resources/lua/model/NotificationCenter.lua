
LCNotificationCenter = {}
LCNotificationCenter.__index = LCNotificationCenter

function LCNotificationCenter:start()
	CC_SYNTHESIZE(self,"observerMap",{})
end	

function LCNotificationCenter:addObserver(observer,selector,aName)
	if not aName then GError("notification name should not nil xxx") end
	self.observerMap[aName] = self.observerMap[aName] or {}
	table.insert(self.observerMap[aName],{observer = observer,selector = selector})
end

function LCNotificationCenter:removeObserver(observer,aName)
	if not aName then GError("notification name should not nil xxx") end
	local observerList = self.observerMap[aName]
	if not observerList then return end

	for i= #observerList,1,-1 do
		if (observer == observerList[i].observer) then 	
 			table.remove(self.observerMap[aName],i)
 	 	end
	end
end

function LCNotificationCenter:postNotificationName(aName,userInfo)
	local notification = {name = aName,userInfo = userInfo}
	local observerList = self.observerMap[aName]
	if not observerList then return end
	for __,observerInfo in pairs(observerList) do
		local observer = observerInfo.observer
		local selector = observerInfo.selector
		if (observer and selector) then 				
 			selector(observer,userInfo) 
 		end
	end
	return true
end