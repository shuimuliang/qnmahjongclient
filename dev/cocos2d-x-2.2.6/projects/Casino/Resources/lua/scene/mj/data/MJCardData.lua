-- ====================== LCMJCardData ============================================================ --

LCMJCardData = class("LCMJCardData")
function LCMJCardData:ctor(num)
	if not num then return nil end
	CC_SYNTHESIZE(self, "num", 			tonumber(num))
	CC_SYNTHESIZE(self, "img", 			nil)
	CC_SYNTHESIZE(self, "imgT", 		nil)
	CC_SYNTHESIZE(self, "imgSmallT", 	nil)
	CC_SYNTHESIZE(self, "imgL", 		nil)
	CC_SYNTHESIZE(self, "imgR", 		nil)

	local prefix,num = LCMJUtil.getImgPrefixAndNumByNum(num)

	self.img  ="mj_"..prefix..num..".png"
	self.imgT ="mj_"..prefix..num.."_t.png"
	self.imgSmallT = "mj_small_"..prefix..num.."_t.png"
	self.imgL ="mj_"..prefix..num.."_l.png"
	self.imgR ="mj_"..prefix..num.."_r.png"

end

return LCMJCardData