--Encapsulate SimpleAudioEngine to Audio,Play music and sound effects.
Audio = {}
local sharedEngine = SimpleAudioEngine:sharedEngine()

function Audio.stopAllEffects()
    sharedEngine:stopAllEffects()
end

function Audio.getMusicVolume()
    return sharedEngine:getBackgroundMusicVolume()
end

function Audio.isMusicPlaying()
    return sharedEngine:isBackgroundMusicPlaying()
end

function Audio.getSoundVolume()
    return sharedEngine:getEffectsVolume()
end

function Audio.setMusicVolume(volume)
    volume = volume or 1
    volume = math.max(volume,0)
    volume = math.min(volume,1)
    sharedEngine:setBackgroundMusicVolume(volume)
    local setting = LCUserContext:getGameSettings()
    setting.bgMusicVolume = volume
    LCUserContext:saveGameSettings()
end

function Audio.stopEffect(handle)
    sharedEngine:stopEffect(handle)
end

function Audio.stopMusic(isReleaseData)
    local releaseDataValue = false
    if nil ~= isReleaseData then
        releaseDataValue = isReleaseData
    end
    sharedEngine:stopBackgroundMusic(releaseDataValue)
end

function Audio.playMusic(filename, isLoop)
    if not LCUserContext:getGameSettings().bgMusicOnOff then return end
    local loopValue = false
    if nil ~= isLoop then
        loopValue = isLoop
    end
    sharedEngine:playBackgroundMusic(filename, loopValue)
end

function Audio.pauseAllEffects()
    sharedEngine:pauseAllEffects()
end

function Audio.preloadMusic(filename)
    sharedEngine:preloadBackgroundMusic(filename)
end

function Audio.resumeMusic()
    sharedEngine:resumeBackgroundMusic()
end

function Audio.playEffect(filename, isLoop)
    if not LCUserContext:getGameSettings().soundEffectOnOff then return end
    local loopValue = false
    if nil ~= isLoop then
        loopValue = isLoop
    end
    return sharedEngine:playEffect(filename, loopValue)
end

function Audio.rewindMusic()
    sharedEngine:rewindBackgroundMusic()
end

function Audio.willPlayMusic()
    return sharedEngine:willPlayBackgroundMusic()
end

function Audio.unloadEffect(filename)
    sharedEngine:unloadEffect(filename)
end

function Audio.preloadEffect(filename)
    sharedEngine:preloadEffect(filename)
end

function Audio.setSoundVolume(volume)
    volume = volume or 1
    volume = math.max(volume, 0)
    volume = math.min(volume, 1)
    sharedEngine:setEffectsVolume(volume)
    local setting = LCUserContext:getGameSettings()
    setting.soundEffectVolume = volume
    LCUserContext:saveGameSettings()
end

function Audio.pauseEffect(handle)
    sharedEngine:pauseEffect(handle)
end

function Audio.resumeAllEffects(handle)
    sharedEngine:resumeAllEffects()
end

function Audio.pauseMusic()
    sharedEngine:pauseBackgroundMusic()
end

function Audio.resumeEffect(handle)
    sharedEngine:resumeEffect(handle)
end
