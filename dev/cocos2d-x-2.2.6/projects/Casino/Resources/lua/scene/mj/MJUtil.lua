
LCMJUtil = {}

function LCMJUtil.sortCardDataList(cardDataList,hunNum)
	for i=1,#cardDataList do
		local data = cardDataList[i]
		if data.num == hunNum then data.num = -10 end
		if data.num == 10 then data.num = 100 end
		if data.num == 20 then data.num = 200 end
		if data.num == 30 then data.num = 300 end
	end

	table.sort(cardDataList,function(data1,data2)
		return data1.num < data2.num
	end)

	for i=1,#cardDataList do
		local data = cardDataList[i]
		if data.num == -10 then data.num = hunNum end
		if data.num == 100 then data.num = 10 end
		if data.num == 200 then data.num = 20 end
		if data.num == 300 then data.num = 30 end
	end
	
	return cardDataList
end

function LCMJUtil.sortCardNumList(numList,hunCard)
	for i=1,#numList do
		if numList[i] == hunCard then numList[i] = -10 end
		if numList[i] == 10 then numList[i] = 100 end
		if numList[i] == 20 then numList[i] = 200 end
		if numList[i] == 30 then numList[i] = 300 end
	end

	table.sort(numList,function(num1,num2)
		return num1 < num2
	end)

	for i=1,#numList do
		if numList[i] == -10 then numList[i] = hunCard end
		if numList[i] == 100 then numList[i] = 10 end
		if numList[i] == 200 then numList[i] = 20 end
		if numList[i] == 300 then numList[i] = 30 end
	end

	return numList
end

function LCMJUtil.checkCardsWithNums(cards,nums,hunNum)
	--1.把后端数据按照前端规则进行排序
	local numList = LCMJUtil.sortCardNumList(nums,hunNum)

	--2.如果两个table个数不一致，直接返回false
	if #cards ~= #numList then return false end

	--3.检查每一个元素是否一致
	for i=1,#cards do
		local card = cards[i]
		local num = numList[i]
		if card.data.num ~= num then return false end
	end

	return true
end	

function LCMJUtil.getChangeNum(num,hunNum)
	if num == hunNum then num = -10 end	
	if num == 10 then num = 100 end
	if num == 20 then num = 200 end
	if num == 30 then num = 300 end
	return num
end

function LCMJUtil.getPengList(nums)
	local t = {}
	for i=1,#nums do
		if i % 3 == 0 then
			table.insert(t,nums[i])
		end
	end
	return t
end

function LCMJUtil.getGangList(nums)
	local t = {}
	for i=1,#nums do
		if i % 4 == 0 then
			table.insert(t,nums[i])
		end
	end
	return t
end

function LCMJUtil.getImgPrefixAndNumByNum(num)
	local prefix = ""
	if num < 10 then
		prefix = "w_"
	elseif num > 10 and num < 20 then
		num = num - 10
		prefix = "tong_"
	elseif num > 20 and num < 30 then
		num = num - 20
		prefix = "tiao_"
	end
	return prefix,num
end

function LCMJUtil.getHunImgByNum(num)
	local prefix,num = LCMJUtil.getImgPrefixAndNumByNum(num)
	return "mj_"..prefix..num..".png"
end

return LCMJUtil
