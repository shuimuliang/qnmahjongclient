local DEF_MUSIC_FORMAT = ".mp3"
local DEF_SOUND_FORMAT = ".mp3"

function _VIBRATION_ON_OFF(onOff)
	local setting = LCUserContext:getGameSettings()
	setting.vibrationOnOff = onOff or eIsShock.agree
	LCUserContext:saveGameSettings()
end

function _VIBRATION_PLAY()
	if LCUserContext:getGameSettings().vibrationOnOff==eIsShock.never then return end
	if CCEPlugin.playVibration then CCEPlugin:playVibration() end
end

function _MUSIC_PLAY(name, loop)
	if not LCUserContext:getGameSettings().bgMusicOnOff then return end
	LCMusicBox:playMusic(name,loop)
end

function _MUSIC_STOP(needRelease)
	LCMusicBox:stopMusic(needRelease)
end

function _SOUND_PLAY(name, loop)
	if not LCUserContext:getGameSettings().soundEffectOnOff then return end
	return LCMusicBox:playSound(name,loop)
end

local function _MUSIC_SUFFIX(file) return (file or "") .. DEF_MUSIC_FORMAT end
local function _SOUND_SUFFIX(file) return (file or "") .. DEF_SOUND_FORMAT end

LCMusicBox = {}

function LCMusicBox:start()
	CC_SYNTHESIZE(self, "soundList", {} )

	local setting = LCUserContext:getGameSettings()
	 Audio.setMusicVolume(setting.bgMusicVolume or 0.5)
	 Audio.setSoundVolume(setting.soundEffectVolume or 0.5)

end

function LCMusicBox:playMusic (musicName,loop)
	local fileName = DEF_PATH.backmusic.._MUSIC_SUFFIX(musicName)
    Audio.playMusic(fileName, loop)
end
function LCMusicBox:stopMusic ()
	Audio.stopMusic(true)
	Audio.stopAllEffects(true)
end

-- sound related
function LCMusicBox:playSound(soundName,loop,stable)
	if not soundName then return end
	local fileName = DEF_PATH.sound.._SOUND_SUFFIX(soundName)
    m_nSoundId = Audio.playEffect(fileName, loop)
    if not self.soundList[soundName] then self.soundList[soundName] = false end
    if stable then self.soundList[soundName] = stable end
	return m_nSoundId
end

function LCMusicBox:playPeopleChatSound(soundName,loop,stable)
	if DEF_DESGIN_LANGUAGE ~= kLanguageChinese then return end
	self:playPeopleSound(soundName,loop,stable)
end

function LCMusicBox:playPeopleSound(soundName,loop,stable)
	self:playSound(soundName,loop,stable)
end

function LCMusicBox:stopSound(soundID)
	if not soundID then return end
	Audio.stopEffect(soundID)
end
function LCMusicBox:pauseSound(soundID)
	Audio.pauseEffect(soundID)
end
function LCMusicBox:resumeSound(soundID)
	Audio.resumeEffect(soundID)
end
function LCMusicBox:unloadSound(soundName)
	if not soundName then return end
	local fileName = DEF_PATH.sound.._SOUND_SUFFIX(soundName)
	Audio.unloadEffect(fileName)
end

function LCMusicBox:unloadUnusedSounds()
	for soundName,stable in pairs(self.soundList) do
		if not stable then Audio.unloadEffect(soundName) end
	end
end

-------conmon
function LCMusicBox:playControlSounds()
	self:playSound(eResSound.btnClicked, false)
	-- self:playSound("common_btn_clicked", false)
end
function LCMusicBox:playMessSounds()
	self:playSound(eResSound.mess, false)
end
function LCMusicBox:playEnterRoom()
	return self:playSound(eResSound.enterRoom, false)
end

function LCMusicBox:playTimeWarm()
	return self:playSound(eResSound.timeWarm, false)
end
function LCMusicBox:playWin()
	return self:playSound(eResSound.bjWin, false)
end

function LCMusicBox:playToast()
	return self:playSound("common_toast",false)
end

function LCMusicBox:playBuySuccess()
	return self:playSound("common_buy_succ",false)
end

--mj

function LCMusicBox:mjPlayDiscard(player,num)
	if not player or not num then return end
	local gender = player.gender
	local res = "mj_man"..num
	if gender == ePlayerGender.female then res = "mj_woman"..num end
	return self:playSound(res,false)
end

function LCMusicBox:mjPlayPeng(player)
	if not player then return end
	local gender = player.gender
	local num = math.random(0,2)
	local res = "mj_man_peng"..num
	if gender == ePlayerGender.female then res = "mj_woman_peng"..num end	
	return self:playSound(res,false)
end

function LCMusicBox:mjPlayChi(player)
	if not player then return end
	local gender = player.gender
	local num = math.random(0,2)
	local res = "mj_man_chi"..num
	if gender == ePlayerGender.female then res = "mj_woman_chi"..num end		
	return self:playSound(res,false)
end

function LCMusicBox:mjPlayHu(player)
	if not player then return end	
	local gender = player.gender
	local num = math.random(0,2)
	local res = "mj_man_hu"..num
	if gender == ePlayerGender.female then res = "mj_man_hu"..num end			
	return self:playSound(res,false)
end

function LCMusicBox:mjPlayMingGang(player)
	if not player then return end	
	local gender = player.gender	
	local num = math.random(0,1)
	local res = "mj_man_gang"..num
	if gender == ePlayerGender.female then res = "mj_man_gang"..num end	
	return self:playSound(res,false)
end

function LCMusicBox:mjPlayAnGang(player)
	if not player then return end	
	local gender = player.gender		
	local res = "mj_man_gang_AG"
	if gender == ePlayerGender.female then res = "mj_woman_gang_AG"..num end	
	return self:playSound(res,false)
end

--zjh
function LCMusicBox:playZJHHumanSound(gender,operation)
	if not gender or not operation then return end
	local sound = ""
	if operation==eZJHOperationType.look then
		sound = (gender == ePlayerGender.male) and eResSound.zjhwokanpai_m or eResSound.zjhwokanpai_w
	elseif operation==eZJHOperationType.call then
		sound = (gender == ePlayerGender.male) and eResSound.zjhgenzhu_m or eResSound.zjhgenzhu_w
	elseif operation==eZJHOperationType.fold then
		sound = (gender == ePlayerGender.male) and eResSound.zjhfangqi_m or eResSound.zjhfangqi_w
	elseif operation==eZJHOperationType.betAll then
		sound = (gender == ePlayerGender.male) and eResSound.zjhquanya or eResSound.zjhquanya
	elseif operation==eZJHOperationType.pk then
		sound = (gender == ePlayerGender.male) and eResSound.zjhbipai_m or eResSound.zjh_bipai_w
	elseif operation==eZJHOperationType.raise then
		sound = (gender == ePlayerGender.male) and eResSound.zjhjiazhu_m or eResSound.zjhjiazhu_w
	end

	GDebug("play zhajinhua sound   %s    %s",sound,operation)

	return self:playPeopleSound(sound, false)
end

function LCMusicBox:playZJHcountDown()
	return self:playSound(eResSound.zjhcountdown, false)
end
function LCMusicBox:playZJHfoldOrLook()
	return self:playSound(eResSound.zjhfangqi_kanpai, false)
end
function LCMusicBox:playZJHsendCard()
	return self:playSound(eResSound.zjhfapai, false)
end
function LCMusicBox:playZJHoverChipMove()
	return self:playSound(eResSound.zjhover_coin_move, false)
end
function LCMusicBox:playZJHpkSound()
	return self:playSound(eResSound.zjhpk, false)
end
function LCMusicBox:playZJHpush_coin()
	return self:playSound(eResSound.zjhpush_coin, false)
end
function LCMusicBox:playZJHbetAll()
	return self:playSound(eResSound.zjhquanya, false)
end
function LCMusicBox:playZJHlookCard()
	return self:playSound(eResSound.zjhwokanpai, false)
end
function LCMusicBox:playZJHpkLight()
	return self:playSound(eResSound.zjhPKLight, false)
end
function LCMusicBox:playZJHheadLight()
	return self:playSound(eResSound.zjhHeadLight, true)
end
