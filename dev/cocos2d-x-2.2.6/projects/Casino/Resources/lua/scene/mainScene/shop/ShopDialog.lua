local DialogDIY = require "lua/scene/commons/dialog/DialogDIY"
local TableView = require "lua/scene/sceneUtils/TableView"
local Iappay    = require "lua/model/Iappay"
local LCPaymentMgr  = require("lua/model/Payment")

local ShopCell  = import(".ShopCell")

local ShopDialog = class("ShopDialog", DialogDIY)

local _test
local _initTableView, _request

function ShopDialog:ctor(key)
    ShopDialog.super.ctor(self, key)
    self._tableView = nil
    self._goods = {}
    self._shopMgr = LCAppGlobal:getGameData():getShopMgr()
    self._switchMgr = LCAppGlobal:getGameData():getSwitchMgr()
end

function ShopDialog:initContentView()
    local proxy = CCBProxy:create()
    local layer = CCBuilderReaderLoad("shop", proxy, self)
    self:addChild(layer)

    _initTableView(self)
    -- _test(self)

    _request(self)

    return layer
end

function ShopDialog:ccb_onClose()
    self:dismissSelf()
end

function ShopDialog:dealPacket(seed, msg)
    if seed == eDefMsg.GoodsList then
        LCSceneTransit.getRunningScene():hideLoading()
        local status = msg.status
        if status == eErrorCode.succ then
            local payType = ePayType.iappay
            if self._switchMgr:closeWeixinLogin() then
                payType = ePayType.appstore
            end
            local gemList = self._shopMgr:getGemsByType(payType)
            self._goods = gemList
            table.sort(self._goods, function(a, b) return a.money < b.money end) -- 按价格排序
            for _, g in pairs(self._goods) do -- 除以 100，得到 RMB 价格
                g.money = g.money / 100
            end
            self._tableView:reloadData()
        else
        end
    elseif seed == eDefMsg.OrderApply then
        LCSceneTransit.getRunningScene():hideLoading()
        LCToastHUD.new("支付成功"):showInScene()
    end
end

-- private
_request = function(self)
    LCSceneTransit.getRunningScene():showLoading("发送中...")
    local pb = LCNetManager:createMessage(eDefMsg.GoodsList)
    LCNetManager:send(pb)
end

_test = function(self)
    LCSceneTransit.getRunningScene():showLoading("发送中...")
    performWithDelay(self, function()
        self._goods = {
            {
                id = 1,
                count = 1,
                extra = 1,
                price = "1",
                iconUrl = "http://tse1.mm.bing.net/th?id=OIP.z35A3ivA68jUfYfbsflyAQCWCW&pid=15.1",
            },
            {
                id = 2,
                count = 2,
                extra = 2,
                price = "2",
                iconUrl = "http://mt2.haibao.cn/img/150_150_100_0/yaNB1Cy0nat5c/celebrity/201411/yaNB1Cy0nat5c.png",
            },
            {
                id = 3,
                count = 3,
                extra = 3,
                price = "3",
                iconUrl = "http://img.yacol.com/uploads/allimg/121122/59_121122104512_1.jpg",
            },
            {
                id = 4,
                count = 4,
                extra = 4,
                price = "4",
                iconUrl = "http://www.mingxingku.com/Data/Images/Stars/2283/bigphoto.thumb_180_180.jpg",
            },
        }
        LCSceneTransit.getRunningScene():hideLoading()
        self._tableView:reloadData()
    end, .2)
end

_initTableView = function(self)
    local size = self.ccb_tableLayer:getContentSize()
    local t = TableView:create(size, {
        cellHeight = function(index)
            return ShopCell.height()
        end,
        cellWidth = function(index)
            return ShopCell.width()
        end,
        cellAtIndex = function(index, reusedCell) -- index begin from 0
            local data = self._goods[index + 1]
            if reusedCell then
                reusedCell:update(data)
                return reusedCell
            else
                local cell = ShopCell.new()
                cell:update(data)
                return cell
            end
        end,
        numberOfCells = function()
            return #self._goods
        end,
        cellTouched = function(cell, cctouch, cCell)
            local idx = cCell:getIdx() + 1

            local function onIapPay()
                Iappay.pay(LCAppGlobal:getLoginPlayer():getId(), CSDKManager:getInstance():getChannelID(), self._goods[idx].productID)
            end

            local function onIOSPay()
                local gemList = self._shopMgr:getGemsByType(ePayType.appstore)
                table.sort(gemList, function(a, b) return a.money < b.money end) -- 按价格排序
                local gemData = gemList[idx]
                if gemData then
                    LCPaymentMgr:pay(gemData, function() end)
                end
            end

            if self._switchMgr:closeWeixinLogin() then
                onIOSPay()
                return
            end

            onIapPay()

            -- LCToastHUD.new(string.format("购买id : %s", self._goods[idx].id)):showInScene()
            -- Iappay.pay(LCAppGlobal:getLoginPlayer():getId(), CSDKManager:getInstance():getChannelID(), self._goods[idx].id)

                -- pay
            -- LCPaymentMgr:pay(gemData, function ()
            --     return
            -- end)

            --key = {text = "",okText = "", cancelText = "",hideClose = true,okCallback = function  end,cancelCallback = function  end}


            -- local scene = LCSceneTransit:getRunningScene()
            -- LCMessageBox:showSelect(scene,{text = "请选择支付方式",okText = "苹果支付", cancelText = "爱贝支付", okCallback = function() onIOSPay() end,cancelCallback = function() onIapPay() end })
        end,
    })
    t:setScrollDirection(kCCScrollViewDirectionHorizontal)
    self._tableView = t
    self.ccb_tableLayer:addChild(t)
end

return ShopDialog
