
-- ====================== LCMJCardsSet ============================================================ --

LCMJCardsSet = class("LCMJCardsSet")
function LCMJCardsSet:ctor(cardsInfo,hunCard)
	CC_SYNTHESIZE(self, "pos", 				cardsInfo.pos)
	CC_SYNTHESIZE(self, "pengCards", 		cardsInfo.pengCards)
	CC_SYNTHESIZE(self, "chiCards", 		cardsInfo.chiCards)
	CC_SYNTHESIZE(self, "anGangCards", 		cardsInfo.anGangCards)
	CC_SYNTHESIZE(self, "mingGangCards", 	cardsInfo.mingGangCards)
	CC_SYNTHESIZE(self, "discardCards", 	cardsInfo.discardCards)
	CC_SYNTHESIZE(self, "leftCards", 		cardsInfo.leftCards)
	CC_SYNTHESIZE(self, "drawCard", 		cardsInfo.DrawCard)

	if self.leftCards then
		self.leftCards = LCMJUtil.sortCardNumList(self.leftCards,hunCard)
	end

	if self.drawCard == -1 then
		self.drawCard = nil
	end
end

return LCMJCardsSet
