
local LCPageScorllView = class("LCPageScorllView", function() return CCLayer:create() end)

function LCPageScorllView:ctor(layerTables, key)
	if layerTables == nil then return false end

	CC_SYNTHESIZE(self, "debug", false)
	CC_SYNTHESIZE(self, "layerTables", layerTables)
	CC_SYNTHESIZE(self, "scrollView", nil)
	CC_SYNTHESIZE(self, "scrollDrection", nil)
	CC_SYNTHESIZE(self, "callback", nil)
	CC_SYNTHESIZE(self, "m_targetPage", 0)
	CC_SYNTHESIZE(self, "m_currPage", 0)
	if key ~= nil then
		self.scrollDrection = key.scrollDrection
		if self.scrollDrection == nil then self.scrollDrection = kCCScrollViewDirectionHorizontal end
		self.callback = key.callback
	end
	if not self.layerTables or  #self.layerTables <= 0 then return true end

	local viewSize = self.layerTables[1]:getContentSize()

	GDebug("%s : %s",viewSize.width,viewSize.height)

	local showLayer = CCLayer:create()
	CC_SYNTHESIZE(self,"showLayer",showLayer)
	for i=1, #self.layerTables do
		if self.scrollDrection == kCCScrollViewDirectionHorizontal then
			self.layerTables[i]:setPosition(ccp((i-1)*viewSize.width, 0))
		else
			self.layerTables[i]:setPosition(ccp(0, (i-1)*viewSize.height))
		end
		showLayer:addChild(self.layerTables[i])
	end

	showLayer:setContentSize(CCSizeMake(#self.layerTables*viewSize.width, viewSize.height))

	self.scrollView = CCScrollView:create(viewSize, showLayer)
	self.scrollView:setDirection(self.scrollDrection)
	self.scrollView:setAnchorPoint(ccp(0.5, 0.5))
	self.scrollView:setBounceable(true)
	self.scrollView:setClippingToBounds(true)
	self:addChild(self.scrollView)


	-- 	onEvent
	local function onEvent(event)
		if event == "enter" then
			self:onEnter()
		elseif event == "exit" then
			self:onExit()
		end
	end
	self:registerScriptHandler(onEvent)

	local parentPosition
	local glPos
	local m_upOffset = false
	local beganOffset
	local m_touchBeganTime = 0
	local move = 0
	local pageDurateion = 0.35
	local function onTouch(eventType,x,y)
		if parentPosition == nil then
			local xs, ys = self:getPosition()
			if self.debug then GDebug("xs="..xs..".ys="..ys) end
			parentPosition = ccp(xs, ys)
		end

		glPos = ccp(x, y)
		if self.debug then GDebug("glox="..glPos.x.."glPos.y="..glPos.y) end

		local touchPoint = self:getParent():convertToNodeSpace(ccp(glPos.x, glPos.y))
		if self.debug then GDebug("touchPointx="..touchPoint.x.."touchPoint.y="..touchPoint.y) end

		if touchPoint.x >= parentPosition.x and touchPoint.x <= parentPosition.x + viewSize.width
			and touchPoint.y >= parentPosition.y and touchPoint.y <= parentPosition.y + viewSize.height then
			m_upOffset = true
			if self.debug then GDebug("parentPosition.x + viewSize.width="..parentPosition.x + viewSize.width.."parentPosition.y + viewSize.height="..parentPosition.y + viewSize.height) end
		else
			m_upOffset = false
		end


		if eventType == "began" then

			beganOffset = self.scrollView:getContentOffset()

			--���������˷�ҳ����, ����ֻ��һ������������ʱ��, ��֧�ַ�ҳ.
			if #self.layerTables == 0 or ( self.scrollDrection ~= kCCScrollViewDirectionHorizontal and self.scrollDrection ~= kCCScrollViewDirectionVertical ) then return end

			--��¼����ʱ����λ��
			m_touchBeganTime = os.clock()
			if self.scrollDrection == kCCScrollViewDirectionHorizontal then
				m_touchBeganOffset = self.scrollView:getContentOffset().x
				move = glPos.x
			else
				m_touchBeganOffset = self.scrollView:getContentOffset().y
				move = glPos.y
			end

			return true
		end

		if eventType == "moved" and m_upOffset then
			if not self.scrollView then return end
			local distance = glPos.x - move
			if self.scrollDrection == kCCScrollViewDirectionHorizontal then
			--	self.scrollView:setContentOffsetInDuration(ccp(self.scrollView:getContentOffset().x + distance, self.scrollView:getContentOffset().y), 0.035)
				move = glPos.x
			else
				distance = glPos.y - move
			--	self.scrollView:setContentOffsetInDuration(ccp(self.scrollView:getContentOffset().x, self.scrollView:getContentOffset().y + distance), 0.035)
				move = glPos.y
			end
		end


		if eventType == "ended" then
			if not self.scrollView then return end

			if #self.layerTables == 0 or ( self.scrollDrection ~= kCCScrollViewDirectionHorizontal and self.scrollDrection ~= kCCScrollViewDirectionVertical ) then return end

			--constant
			local PAGE_DISTENCE
			if self.scrollDrection == kCCScrollViewDirectionHorizontal then
				PAGE_DISTENCE = self.scrollView:getViewSize().width
			else
				PAGE_DISTENCE = self.scrollView:getViewSize().height
			end

			if PAGE_DISTENCE <= 0 then return  end

			local MAX_PAGE
			if self.scrollDrection == kCCScrollViewDirectionHorizontal then
				MAX_PAGE = self.scrollView:getContentSize().width
			else
				MAX_PAGE = self.scrollView:getContentSize().height
			end
			local MIN_PAGE = 0

			local currOffset
			if self.scrollDrection == kCCScrollViewDirectionHorizontal then
				currOffset = self.scrollView:getContentOffset().x
			else
				currOffset = self.scrollView:getContentOffset().y
			end
			local deltaOffset = -(currOffset - m_touchBeganOffset)
			local currTime = os.clock()
			local speed
			if currTime ~= m_touchBeganTime then
				speed = deltaOffset / ( currTime - m_touchBeganTime )
			else
				speed = 0
			end

			self.m_targetPage = self.m_currPage
			if math.abs(deltaOffset) >= 1/2*PAGE_DISTENCE  then
			--������������ĳһ��ֵ.
				if deltaOffset > 0 then
					if self.m_targetPage < #self.layerTables-1 then
						self.m_targetPage = self.m_currPage + 1
					else
						return
					end
				else
					if self.m_targetPage > 0 then
						self.m_targetPage = self.m_currPage - 1
					else
						return
					end
				end
			elseif math.abs(speed) >= 0.035 then
			--�ٶȴ���ĳһ��ֵ.
				if speed > 0  then
					if self.m_targetPage < #self.layerTables-1 then
						self.m_targetPage = self.m_currPage + 1
					else
						return
					end
				else
					if self.m_targetPage > 0 then
						self.m_targetPage = self.m_currPage - 1
					else
						return
					end
				end
			end

			local targetOffset
			if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
				targetOffset = -self.m_targetPage* self.scrollView:getViewSize().width
			else
				targetOffset = -self.m_targetPage* self.scrollView:getViewSize().height
			end

			local pageDurateion = 0.35
			local targetPointOffset
			print("targetOffset"..targetOffset)
			if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
				targetPointOffset = ccp( targetOffset, self.scrollView:getContentOffset().y)
			else
				targetPointOffset = ccp(self.scrollView:getContentOffset().x, targetOffset)
			end
			local function fucl()
				self.scrollView:setContentOffsetInDuration(targetPointOffset, 0.35)
				m_upOffset = false
				if self.callback ~= nil then self.callback(self.m_targetPage+1) end
			end
			scheduleOnce(self, fucl, 0.04)
			self.m_currPage = self.m_targetPage

			return true
		end

	end


	--registerTouch
	if #self.layerTables > 1 then
	--	self.scrollView:setTouchEnabled(false)
		showLayer:registerScriptTouchHandler(onTouch, false, eTouchPriority.pageViewPriority)
		showLayer:setTouchEnabled(true)
	end


	return true
end

function LCPageScorllView:getCurShowPageIndex()
	return self.m_currPage+1
end

function LCPageScorllView:setClippingToBounds(ClippingToBounds)
	if self.scrollView then self.scrollView:setClippingToBounds(ClippingToBounds) end
end

function LCPageScorllView:setToPage(pages)

	if #self.layerTables >= pages and self.scrollView then
		pages = pages-1
		self.m_targetPage = pages
		self.m_currPage = self.m_targetPage
		local targetOffset
		if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
			targetOffset = -pages* self.scrollView:getViewSize().width
		else
			targetOffset = -pages* self.scrollView:getViewSize().height
		end
		local targetPointOffset
		print("targetOffset"..targetOffset)
		if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
			targetPointOffset = ccp( targetOffset, 0)
		else
			targetPointOffset = ccp(0, targetOffset)
		end

		self.scrollView:setContentOffset(targetPointOffset)
	end
end

function LCPageScorllView:scrollToPage(pages)

	if #self.layerTables >= pages and self.scrollView then
		pages = pages-1
		self.m_targetPage = pages
		self.m_currPage = self.m_targetPage
		local targetOffset
		if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
			targetOffset = -pages* self.scrollView:getViewSize().width
		else
			targetOffset = -pages* self.scrollView:getViewSize().height
		end
		local targetPointOffset
		print("targetOffset"..targetOffset)
		if self.scrollDrection == kCCScrollViewDirectionHorizontal  then
			targetPointOffset = ccp( targetOffset, 0)
		else
			targetPointOffset = ccp(0, targetOffset)
		end

		self.scrollView:setContentOffsetInDuration(targetPointOffset, 0.5)
	end
end

function LCPageScorllView:onEnter()
end

function LCPageScorllView:onExit()
	self.showLayer:unregisterScriptTouchHandler()
end

function LCPageScorllView:setTouchEnabled(isEnable)
	isEnable = isEnable or false
	self.scrollView:setTouchEnabled(isEnable)
end

return LCPageScorllView
