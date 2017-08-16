local LCTestScene = class("LCTestScene",LCSceneBase)

function LCTestScene:ctor(key)
	if not LCTestScene.super.ctor(self, key or {}) then
		return false
	end
end

return LCTestScene
