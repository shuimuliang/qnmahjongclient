local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"

LCMessageBox = class("LCMessageBox")
-- LCMessageBox.__index = LCMessageBox

--key = {text = "",buttonText = "", hideClose = true, callback = function  end , buttonType = ,textColor = }
function LCMessageBox:showConfirm(scene,key)
	return self:showBox(LCMessageBoxConfirm,key)
end
function LCMessageBox:showConfirm1(key)
	return self:showBox(LCMessageBoxConfirm,key)
end
--key = {text = "",okText = "", cancelText = "",hideClose = true,okCallback = function  end,cancelCallback = function  end}
function LCMessageBox:showSelect(scene,key)
	return self:showBox(LCMessageBoxSelect,key)
end
function LCMessageBox:showSelect1(key)
	return self:showBox(LCMessageBoxSelect,key)
end

-- private
function LCMessageBox:showBox(class,key)
	local curScene = LCSceneTransit:getRunningScene()
	key = key or {}
    class.new(key):showInScene()
	if class == LCMessageBoxBJ then
		dialog:setHideBorder(true)
	end

	-- dialog:show()
	return messageBox
end

--=========================  LCMessageBoxBase
LCMessageBoxBase = class("LCMessageBoxBase",DialogDIY)
function LCMessageBoxBase:ctor(key)
	LCMessageBoxBase.super.ctor(self,key)
	CC_SYNTHESIZE(self,"closeButton",nil)
	CC_SYNTHESIZE(self,"hideClose",key.hideClose)
	CC_SYNTHESIZE(self,"buttonTypeMap",{})
	self.buttonTypeMap = {
		[eButtonType.green]  = {bg = eResImage.greenbutton,  font = eUIFont.systembutton,},
		[eButtonType.red]	 = {bg = eResImage.redbutton, 	 font = eUIFont.systembutton,},
		[eButtonType.yellow] = {bg = eResImage.yellowbutton, font = eUIFont.systembutton,}
	}
end

function LCMessageBoxBase:initCloseBtn()
    local s = CCScale9Sprite:createWithSpriteFrameName(eResImage.s_buttontransparent)
    s:setContentSize(CCSizeMake(200, 200))
    self.closeButton = CCMenuItemSprite:create(s, s, s)
    self.closeButton:registerScriptTapHandler(function(...) self:closeButtonClicked(...) end)
    local menu = CCMenu:createWithItem(self.closeButton)
    menu:setPosition(0, 0)
    self:addChild(menu, 2)

    local closeSprite = CCSprite:createWithSpriteFrameName("mj_interface_cpm_btn_close.png")
    self.closeButton:addChild(closeSprite)
    self.closeButton:setAnchorPoint(ccp(0, 0))
    self.closeBtnSize = closeSprite:getContentSize()
    closeSprite:setPosition(self.closeBtnSize.width / 2, self.closeBtnSize.height / 2)


    local contentSize = self.contentView_:getContentSize()
    local winSize = CCDirector:sharedDirector():getWinSize()
    local titleHeight = eUIConst.kCommonDialogTitleHeight
    if not self.hideClose then
        local btnSize = self.closeBtnSize
        local btnx = (winSize.width + contentSize.width) / 2 - btnSize.width/2 - 10 -- - eUIConst.kCommonDialogCloseOffsetX
    	local btny = winSize.height/2 + contentSize.height/2 - btnSize.height / 2 - 10
        self.closeButton:setPosition(btnx, btny)
    else
    	self.closeButton:setVisible(false)
    end
end
function LCMessageBoxBase:onEnter()
	LCMessageBoxBase.super.onEnter(self)
	self:initCloseBtn()
end


--=========================  LCMessageBoxSelect
LCMessageBoxConfirm = class("LCMessageBoxConfirm",LCMessageBoxBase)
function LCMessageBoxConfirm:ctor(key)
	LCMessageBoxConfirm.super.ctor(self,key)

	CC_SYNTHESIZE(self,"text",key.text or "")
	CC_SYNTHESIZE(self,"buttonType",key.buttonType or eButtonType.red)
	CC_SYNTHESIZE(self,"buttonText",key.buttonText or _("确定") )
	CC_SYNTHESIZE(self,"textColor",key.textColor or _APPCOLOR_HEX("5B2C04") )
	CC_SYNTHESIZE(self,"callback",key.callback)
	CC_SYNTHESIZE(self,"minBtnSize",CCSizeMake(177,63))
	CC_SYNTHESIZE(self,"gap",20)
	return true
end
function LCMessageBoxConfirm:initContentView()
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.messageboxConfirm,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local size = layer:getContentSize()
 --  	self:changeWidthAndHeight(size.width,size.height)
	self:addChild(layer)

	local nodeMap = {
		{"labText","ccb_text","CCLabelTTF"},
		{"btnOK","ccb_button","CCControlButton"},
	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)
	local layerSize = layer:getContentSize()
	local maxTextWidth = layerSize.width * 2/3

	self.labText:setString(self.text)
	local textSize = self.labText:getContentSize()
	local textWidth = math.min(maxTextWidth, textSize.width/1.5)
	if textWidth <= maxTextWidth then
		textWidth = maxTextWidth
	end
	self.labText:setDimensions(CCSizeMake( textWidth, 0))
	self.labText:setColor(self.textColor)
	_CBTN_CTEXTF(self.btnOK,self.buttonText)
	-- _CBTN_CFRAME(self.btnOK, self.buttonTypeMap[self.buttonType].bg)

	local label = self.btnOK:getTitleLabelForState(CCControlStateNormal)
	local size = label:getContentSize()
	local width = size.width + self.gap*2
	width = math.max(self.minBtnSize.width,width)
	self.btnOK:setPreferredSize(CCSizeMake(width,self.minBtnSize.height))
	return layer
end

function LCMessageBoxConfirm:ccb_okButtonClicked()
	if self.callback then self.callback() end
	self:dismissSelf()
end


--=========================  LCMessageBoxSelect
LCMessageBoxSelect = class("LCMessageBoxSelect",LCMessageBoxBase)
function LCMessageBoxSelect:ctor(key)
	LCMessageBoxSelect.super.ctor(self,key)

	CC_SYNTHESIZE(self,"text",key.text or "")
	CC_SYNTHESIZE(self,"button1Text",key.okText or  _ID2WORD(11001))
	CC_SYNTHESIZE(self,"button2Text",key.cancelText or  _ID2WORD(11002))
	CC_SYNTHESIZE(self,"okCallback",key.okCallback)
	CC_SYNTHESIZE(self,"cancelCallback",key.cancelCallback)

	CC_SYNTHESIZE(self,"minBtnSize",CCSizeMake(177,63))
	CC_SYNTHESIZE(self,"gap",20)

	return true
end

function LCMessageBoxSelect:initContentView()
	local  proxy = CCBProxy:create()
    local  node  = CCBuilderReaderLoad(eResCCBI.messageboxSelect,proxy,self)
    local  layer = tolua.cast(node,"CCLayer")
    local contentSize = layer:getContentSize()
 --  	self:changeWidthAndHeight(contentSize.width,contentSize.height)
	self:addChild(layer)

	local nodeMap = {
		{"labText","ccb_text","CCLabelTTF"},
		{"btnOK","ccb_okButton","CCControlButton"},
		{"btnCancel","ccb_cancelButton","CCControlButton"},
	}
	CE_SYNTHESIZE_CCB_MAP(self,layer,nodeMap)
	local layerSize = layer:getContentSize()

	self.labText:setString(self.text)
	local textSize = self.labText:getContentSize()
	local textWidth = math.min(layerSize.width * 2/3, textSize.width)
	self.labText:setDimensions(CCSizeMake( textWidth, 0))

	_CBTN_CTEXTF(self.btnOK,self.button1Text)
	_CBTN_CTEXTF(self.btnCancel,self.button2Text)
	local label = self.btnOK:getTitleLabelForState(CCControlStateNormal)
	local size = label:getContentSize()
	local width = size.width + self.gap*2
	width = math.max(self.minBtnSize.width,width)
	width = math.min(width,contentSize.width)

	local label1 = self.btnCancel:getTitleLabelForState(CCControlStateNormal)
	local size = label1:getContentSize()
	local width1 = size.width + self.gap*2
	width1 = math.max(self.minBtnSize.width,width1)

	-- width = math.max(width,width1)
	width1 = math.min(width1,contentSize.width)
	self.btnOK:setPreferredSize(CCSizeMake(width,self.minBtnSize.height))
	self.btnCancel:setPreferredSize(CCSizeMake(width1,self.minBtnSize.height))
	local btnOKPosX,btnOKPosY = self.btnOK:getPosition()
	local btnCancelX,btnCancelY = self.btnCancel:getPosition()

	local leftWidth = contentSize.width - width - width1
	btnCancelX = leftWidth/3 + width1/2
	btnOKPosX = leftWidth/3*2 + width1 + width/2

	self.btnOK:setPosition(ccp(btnOKPosX,btnOKPosY))
	self.btnCancel:setPosition(ccp(btnCancelX,btnCancelY))
	return layer
end


function LCMessageBoxSelect:ccb_okButtonClicked()
	if self.okCallback then self.okCallback() end
	self:dismissSelf()
end
function LCMessageBoxSelect:ccb_cancelButtonClicked()
	if self.cancelCallback then self.cancelCallback() end
	self:dismissSelf()
end
