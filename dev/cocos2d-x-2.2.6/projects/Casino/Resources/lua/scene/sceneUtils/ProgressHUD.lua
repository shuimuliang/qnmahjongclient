local visibleSize = CCDirector:sharedDirector():getVisibleSize()
local centerX, centerY = visibleSize.width/2,visibleSize.height/2

-- ===============  text hud
LCTextHUD = class("LCTextHUD",function() return CCLayer:create() end)
function LCTextHUD:ctor(text)
  CC_SYNTHESIZE(self, "text", text or "")
  CC_SYNTHESIZE(self, "fadeOutTimer", 3)  -- 3s

  CC_SYNTHESIZE(self, "maskLayer",nil) --ccc4(0,0,0,50)
  CC_SYNTHESIZE(self, "bgsprite",nil)
  CC_SYNTHESIZE(self, "stringLabel", nil)

  local function onEvent(event)
    if event == "enter" then self:onEnter()
    elseif event == "exit" then self:onExit() end
  end
  self:registerScriptHandler(onEvent)
end

function LCTextHUD:onEnter()
  self:initView()
  self:updatePosition()
end
function LCTextHUD:onExit()

end

function LCTextHUD:initView()
  self.maskLayer = CCLayerColor:create()
  self:addChild(self.maskLayer)

  self.bgsprite = CCScale9Sprite:createWithSpriteFrameName(eResImage.box_background)
  self:addChild(self.bgsprite)

  self.stringLabel = CCLabelTTF:create(self.text, DEF_SYSTEM_FONT_NAME, 30)
  self.stringLabel:setAnchorPoint(0.5, 0.5)
  self:addChild(self.stringLabel)

  -- local function onTouch(eventType,x,y)
  --   return true
  -- end
  -- self:registerScriptTouchHandler(onTouch, false, eTouchPriority.max, true)
  -- self:setTouchEnabled(true)
end

function LCTextHUD:updatePosition()
    local heightBuffer, widthBuffer = 70 , 45

    local stringSize = self.stringLabel:getContentSize()
    local hudWidth = heightBuffer + stringSize.width
    local hudHeight = widthBuffer + stringSize.height

    self.bgsprite:setContentSize(hudWidth,hudHeight)
    self.bgsprite:setPosition(centerX,centerY)
    self.stringLabel:setPosition(centerX,centerY)
end

function LCTextHUD:showInScene()
  local scene = LCSceneTransit:getRunningScene()
  scene:addChild(self, 1000)
  -- dismiss self
  if self.fadeOutTimer and self.fadeOutTimer > 0 then
    scheduleOnce(scene,function( ... )
      self:dismissSelf()
    end,self.fadeOutTimer)
  end
  return self
end
function LCTextHUD:dismissSelf()
    self:removeFromParentAndCleanup(true)
end

function LCTextHUD:setShowText(str)
  self.text = text
  self:updatePosition()
end

----================  toast hud
LCToastHUD = class("LCToastHUD",LCTextHUD)
function LCToastHUD:showInScene()
  LCToastHUD.super.showInScene(self)
  LCMusicBox:playToast()
end

----================  loading hud
LCLoadingHUD = class("LCLoadingHUD",LCTextHUD)
function LCLoadingHUD:ctor(text)
  LCLoadingHUD.super.ctor(self,text)
  CC_SYNTHESIZE(self, "loadingImage",nil)
  self.fadeOutTimer = 0
end
function LCLoadingHUD:initView()
  LCLoadingHUD.super.initView(self)
  local function onTouch(eventType,x,y)
    return true
  end
  self:registerScriptTouchHandler(onTouch, false, eTouchPriority.max, true)
  self:setTouchEnabled(true)
  self.loadingImage = CCSprite:createWithSpriteFrameName(eResImage.common_loading_circle)
  self:addChild(self.loadingImage)
  self.loadingImage:runAction(CCRepeatForever:create(CCSequence:createWithTwoActions(CCRotateBy:create(0, 30), CCDelayTime:create(0.1))))
end

function LCLoadingHUD:updatePosition()
  local hudWidth,hudHeight = 300,160
  local heightBuffer, widthBuffer = 46 , 45
  local loadingSize = self.loadingImage:getContentSize()
  local stringSize = self.stringLabel:getContentSize()

  local deltaW = math.max(stringSize.width + widthBuffer,hudWidth) - hudWidth
  local deltaH = 0  --stringSize.height
  hudHeight = hudHeight + deltaH
  hudWidth  = hudWidth + deltaW

  local labelY = centerY + hudHeight/2 - heightBuffer/2 - stringSize.height/2
  local loadingY = centerY - hudHeight/2 + heightBuffer/2 + loadingSize.height/2
  self.bgsprite:setContentSize(hudWidth,hudHeight)
  self.bgsprite:setPosition(centerX, centerY)
  self.stringLabel:setPosition(centerX, labelY)
  self.loadingImage:setPosition(centerX, loadingY)
end

----================  tip hud
local resMap = {
  [eHudType.hudInfo] = {bg = eResImage.c_frame_03, icon = eResImage.box_buy_background02, color = _APPCOLOR_HEX("feda6a")},
  [eHudType.hudError] = {bg = eResImage.c_frame_03, icon = eResImage.box_buy_background02,  color = _APPCOLOR_HEX("feda6a")},
  [eHudType.hudSucc] = {bg = eResImage.c_frame_03, icon = eResImage.box_buy_background02, color = _APPCOLOR_HEX("feda6a")}
}
local frameCache = CCSpriteFrameCache:sharedSpriteFrameCache()

LCTipHUD = class("LCTipHUD",LCTextHUD)
function LCTipHUD:ctor(text, hudType)
  LCTipHUD.super.ctor(self,text)
  CC_SYNTHESIZE(self, "hudType",hudType or eHudType.hudSucc)
  CC_SYNTHESIZE(self, "icon",nil)
end
function LCTipHUD:initView()
  LCTipHUD.super.initView(self)
  local res = resMap[self.hudType]
  local frame = frameCache:spriteFrameByName(res.bg)
  self.bgsprite:setSpriteFrame(frame)
  self.icon = CCSprite:createWithSpriteFrameName(res.icon)
  self:addChild(self.icon)
  self.stringLabel:setColor(res.color)
end

function LCTipHUD:updatePosition()
  local heightBuffer, widthBuffer , gap = 46 , 45 , 20
  local iconSize = self.icon:getContentSize()
  local stringSize = self.stringLabel:getContentSize()

  local hudWidth = iconSize.width + stringSize.width + widthBuffer + gap
  local hudHeight = stringSize.height + heightBuffer

  local iconX = centerX - hudWidth/2 + widthBuffer/2 + iconSize.width/2
  local labelX = centerX + hudWidth/2 - widthBuffer/2 - stringSize.width/2

  self.bgsprite:setContentSize(hudWidth,hudHeight)
  self.bgsprite:setPosition(centerX, centerY)
  self.stringLabel:setPosition(labelX, centerY)
  self.icon:setPosition(iconX, centerY)
end


----================  LCEmbedLoadingHUD hud
LCEmbedLoadingHUD = class("LCEmbedLoadingHUD",LCTextHUD)
function LCEmbedLoadingHUD:ctor(text,parentNode)
  LCEmbedLoadingHUD.super.ctor(self,text)
  local scene = LCSceneTransit:getRunningScene()
  CC_SYNTHESIZE(self, "parentNode",parentNode or scene)
  CC_SYNTHESIZE(self, "loadingImage",nil)
  self.fadeOutTimer = 0
end
function LCEmbedLoadingHUD:initView()
  self.maskLayer = CCLayerColor:create() --_APPCOLOR_HEX("FF000080")
  self:addChild(self.maskLayer)

  self.stringLabel = CCLabelTTF:create(self.text, DEF_SYSTEM_FONT_NAME, 30)
  self.stringLabel:setAnchorPoint(0.5, 0.5)
  self:addChild(self.stringLabel)

  self.loadingImage = CCSprite:createWithSpriteFrameName(eResImage.common_loading_circle)
  self:addChild(self.loadingImage)
  self.loadingImage:runAction(CCRepeatForever:create(CCSequence:createWithTwoActions(CCRotateBy:create(0, 30), CCDelayTime:create(0.1))))

  local function onTouch(eventType,x,y)
    local glPos = self:convertToNodeSpace( ccp( x, y ) )
    local bBox = self.maskLayer:boundingBox()
    if bBox:containsPoint(glPos) then
      return true
    end
    return false
  end
  self:registerScriptTouchHandler(onTouch, false, eTouchPriority.max, true)
  self:setTouchEnabled(true)
end

function LCEmbedLoadingHUD:updatePosition()
  local visibleSize = self.parentNode:getContentSize()
  local centerX, centerY = visibleSize.width/2,visibleSize.height/2
  self.maskLayer:setContentSize(visibleSize)
  local hudWidth,hudHeight = 300,160
  local heightBuffer, widthBuffer = 46 , 45
  local loadingSize = self.loadingImage:getContentSize()
  local stringSize = self.stringLabel:getContentSize()

  local deltaW = math.max(stringSize.width + widthBuffer,hudWidth) - hudWidth
  local deltaH = 0  --stringSize.height
  hudHeight = hudHeight + deltaH
  hudWidth  = hudWidth + deltaW

  local loadingY = centerY + hudHeight/2 - heightBuffer/2 - loadingSize.height/2
  local labelY = centerY - hudHeight/2 + heightBuffer/2 + stringSize.height/2
  self.stringLabel:setPosition(centerX, labelY)
  self.loadingImage:setPosition(centerX, loadingY)
end
function LCEmbedLoadingHUD:show()
  self.parentNode:addChild(self,1000)
  return self
end

-- LCProgressHUD = class("LCProgressHUD",function() return CCLayer:create() end)
-- function LCProgressHUD:ctor(str,hudType)

--  CC_SYNTHESIZE(self, "scene", LCSceneTransit:getRunningScene())
--   CC_SYNTHESIZE(self, "str", str or "")
--   CC_SYNTHESIZE(self, "hudType", hudType or eHudType.hudWait)

--  CC_SYNTHESIZE(self, "maskType", nil)
--  CC_SYNTHESIZE(self, "fadeOutTimer", 3)  -- 3s

--   CC_SYNTHESIZE(self, "maskLayer",CCLayerColor:create()) --ccc4(0,0,0,50)
--   CC_SYNTHESIZE(self, "loadingBg",nil)
--   CC_SYNTHESIZE(self, "loadingImage",nil)
--   CC_SYNTHESIZE(self, "loadingImage2",nil)
--   CC_SYNTHESIZE(self, "typeImage",nil)
--   CC_SYNTHESIZE(self, "stringLabel", nil)
--   self:addChild(self.maskLayer)
--   self:initView()
--  return true
-- end

-- -- private method

-- function LCProgressHUD:initView()

--   self.loadingBg = CCScale9Sprite:createWithSpriteFrameName(eResImage.box_background)
--   self.loadingBg:setPosition(centerX, centerY)
--   self:addChild(self.loadingBg)

--   self.stringLabel = CCLabelTTF:create("", DEF_SYSTEM_FONT_NAME, 30)
--   self.stringLabel:setAnchorPoint(0.5, 0)
--   self.stringLabel:setPosition(centerX, centerY)
--   self:addChild(self.stringLabel)

--   self.loadingImage = CCSprite:createWithSpriteFrameName(eResImage.common_loading_circle)
--   self.loadingImage:setPosition(centerX, centerY)
--   self.loadingImage:setVisible(false)
--   self:addChild(self.loadingImage)

--   local typeImageName = eResImage.hudSuccIcon
--   if self.hudType == eHudType.hudError then typeImageName = eResImage.hudErrorIcon end
--   self.typeImage = CCSprite:createWithSpriteFrameName(typeImageName)
--   self.typeImage:setPosition(centerX, centerY)
--   self.typeImage:setVisible(false)
--   self:addChild(self.typeImage)
--   if self.hudType == eHudType.hudWait then
--     self.loadingImage:setVisible(true)
--     self.loadingImage:runAction(CCRepeatForever:create(CCSequence:createWithTwoActions(CCRotateBy:create(0, 30), CCDelayTime:create(0.1))))
--   end

--   if self.hudType == eHudType.hudInfo or
--      self.hudType == eHudType.hudError or
--      self.hudType == eHudType.hudSucc then
--     self.typeImage:setVisible(true)
--   end

--   local function onTouch(eventType,x,y)
--     return true
--   end
--   self:registerScriptTouchHandler(onTouch, false, eTouchPriority.max, true)
--   self:setTouchEnabled(true)
--   self:updatePosition()
-- end

-- function LCProgressHUD:updatePosition()
--  local hudWidth,hudHeight = 310,150
--  local stringHeightBuffer = 15
--   local stringWidthBuffer = 60
--   local bgPos = ccp(centerX, centerY)
--   local loadingImagePos = ccp(centerX, centerY)
--   local labelPos = ccp(centerX, centerY)
--   local typeImagePos = ccp(centerX, centerY)
--   local loadingSize = self.loadingImage:getContentSize()
--   self.stringLabel:setString(self.str)

--  if self.str and #self.str > 0 then
--    local stringSize = self.stringLabel:getContentSize()
--     local deltaH = stringSize.height
--     local deltaW = math.max(stringSize.width,hudWidth) - hudWidth
--     hudHeight = hudHeight + deltaH
--     hudWidth = hudWidth + deltaW + stringWidthBuffer
--     loadingImagePos.y = loadingImagePos.y - loadingSize.height/2  - stringHeightBuffer/2 --+ deltaH
--     labelPos.y = labelPos.y  + stringHeightBuffer/2  --+ (loadingSize.height/2 - deltaH) + stringHeightBuffer
--     typeImagePos.y = typeImagePos.y - deltaH
--  end
--   self.loadingBg:setContentSize(hudWidth,hudHeight)
--   self.loadingBg:setPosition(bgPos)
--   self.loadingImage:setPosition(loadingImagePos)
--   self.stringLabel:setPosition(labelPos)
--   self.stringLabel:setVisible(self.str and #self.str > 0)
--   self.typeImage:setPosition(typeImagePos)
-- end

-- -- public method
-- function LCProgressHUD:setShowText(str)
--   self.str = str
--   self:updatePosition()
-- end
-- function LCProgressHUD:showInfo(str)
--     self:showInScene(str,eHudType.hudInfo,1)
-- end

-- function LCProgressHUD:showSucc(str)
--   self:showInScene(str,eHudType.hudSucc,1)
-- end

-- function LCProgressHUD:showError(str)
--   self:showInScene(str,eHudType.hudError,1)
-- end

-- function LCProgressHUD:showInScene(str,hudType,delay)
--   local scene = LCSceneTransit:getRunningScene()
--   local progressHUD = LCProgressHUD.new(str,hudType)
--   scene:addChild(progressHUD,1000)
--   if delay and type(delay) == "number" then
--     scheduleOnce(scene,function( ... )
--       progressHUD:dismissSelf()
--     end,delay)
--   end
--  return progressHUD
-- end
-- function LCProgressHUD:dismissSelf()
--  self:removeFromParentAndCleanup(true)
-- end
