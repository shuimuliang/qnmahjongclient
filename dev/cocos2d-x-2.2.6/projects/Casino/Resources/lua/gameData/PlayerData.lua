local EventProtocol = require "lua/scene/sceneUtils/EventProtocol"

LCPlayerData = class("LCPlayerData")

LCPlayerData.EVENT_UPDATE_COINS_AFTER = "EVENT_UPDATE_COINS_AFTER"
LCPlayerData.EVENT_UPDATE_CARDS_AFTER = "EVENT_UPDATE_CARDS_AFTER"

function LCPlayerData:ctor(data)
	EventProtocol.extend(self)

	data = data or {}
	CC_SYNTHESIZE(self, "id", data.id)
	CC_SYNTHESIZE(self, "nickname", data.nickname)
	CC_SYNTHESIZE(self, "avatar", data.avatar)
	CC_SYNTHESIZE(self, "gender", data.gender)
	CC_SYNTHESIZE(self, "ip", data.ip)
	self._coins = data.coins
	self._cards = data.cards
end

function LCPlayerData:updateAll(data)
	self.id 	  = data.id
	self.nickname = data.nickname
	self.avatar   = data.avatar
	self.gender   = data.gender
	self.ip 	  = data.ip
	self:setCoins(data.coins)
	self:setCards(data.cards)

	if self.gender == ePlayerGender.unknown then
        self.gender = ePlayerGender.male
    end
end

function LCPlayerData:setCoins(count)
	self._coins = count or self._coins
	self:dispatchEvent{name = LCPlayerData.EVENT_UPDATE_COINS_AFTER, coins = self._coins}
end

function LCPlayerData:getCoins()
	return self._coins
end

function LCPlayerData:setCards(count)
	self._cards = count or self._cards
	self:dispatchEvent{name = LCPlayerData.EVENT_UPDATE_CARDS_AFTER, cards = self._cards}
end

function LCPlayerData:getCards()
	return self._cards
end
