#include "../platform/CCFileUtils.h"
#include "CCParticlePool.h"


NS_CC_BEGIN

CCParticleDataPool *ParticleDataPoolInce = NULL;

tCCPositionType transIntToTCCPosT(int posType)
{
	tCCPositionType result = kCCPositionTypeFree;
	if(posType>=kCCPositionTypeFree&&posType<=kCCPositionTypeGrouped){
		result =  (tCCPositionType)posType;
	}
	return result;
}

bool CCParticleData::init(const char *plistFile)
{
	//read json
	m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
	CCDictionary *dictionary = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());
	if(!dictionary)return false;
	maxParticles = dictionary->valueForKey("maxParticles")->intValue();
	// self, not superv

		// angle
		m_fAngle = dictionary->valueForKey("angle")->floatValue();
		m_fAngleVar = dictionary->valueForKey("angleVariance")->floatValue();

		// duration
		m_fDuration = dictionary->valueForKey("duration")->floatValue();

		// blend function 
		m_tBlendFunc.src = dictionary->valueForKey("blendFuncSource")->intValue();
		m_tBlendFunc.dst = dictionary->valueForKey("blendFuncDestination")->intValue();

		// color
		m_tStartColor.r = dictionary->valueForKey("startColorRed")->floatValue();
		m_tStartColor.g = dictionary->valueForKey("startColorGreen")->floatValue();
		m_tStartColor.b = dictionary->valueForKey("startColorBlue")->floatValue();
		m_tStartColor.a = dictionary->valueForKey("startColorAlpha")->floatValue();

		m_tStartColorVar.r = dictionary->valueForKey("startColorVarianceRed")->floatValue();
		m_tStartColorVar.g = dictionary->valueForKey("startColorVarianceGreen")->floatValue();
		m_tStartColorVar.b = dictionary->valueForKey("startColorVarianceBlue")->floatValue();
		m_tStartColorVar.a = dictionary->valueForKey("startColorVarianceAlpha")->floatValue();

		m_tEndColor.r = dictionary->valueForKey("finishColorRed")->floatValue();
		m_tEndColor.g = dictionary->valueForKey("finishColorGreen")->floatValue();
		m_tEndColor.b = dictionary->valueForKey("finishColorBlue")->floatValue();
		m_tEndColor.a = dictionary->valueForKey("finishColorAlpha")->floatValue();

		m_tEndColorVar.r = dictionary->valueForKey("finishColorVarianceRed")->floatValue();
		m_tEndColorVar.g = dictionary->valueForKey("finishColorVarianceGreen")->floatValue();
		m_tEndColorVar.b = dictionary->valueForKey("finishColorVarianceBlue")->floatValue();
		m_tEndColorVar.a = dictionary->valueForKey("finishColorVarianceAlpha")->floatValue();

		// particle size
		m_fStartSize = dictionary->valueForKey("startParticleSize")->floatValue();
		m_fStartSizeVar = dictionary->valueForKey("startParticleSizeVariance")->floatValue();
		m_fEndSize = dictionary->valueForKey("finishParticleSize")->floatValue();
		m_fEndSizeVar = dictionary->valueForKey("finishParticleSizeVariance")->floatValue();

		// position
		x = dictionary->valueForKey("sourcePositionx")->floatValue();
		y = dictionary->valueForKey("sourcePositiony")->floatValue();
		m_tPosVar.x = dictionary->valueForKey("sourcePositionVariancex")->floatValue();
		m_tPosVar.y = dictionary->valueForKey("sourcePositionVariancey")->floatValue();

		// Spinning
		m_fStartSpin = dictionary->valueForKey("rotationStart")->floatValue();
		m_fStartSpinVar = dictionary->valueForKey("rotationStartVariance")->floatValue();
		m_fEndSpin= dictionary->valueForKey("rotationEnd")->floatValue();
		m_fEndSpinVar= dictionary->valueForKey("rotationEndVariance")->floatValue();

		m_nEmitterMode = dictionary->valueForKey("emitterType")->intValue();
		m_ePositionType = transIntToTCCPosT(dictionary->valueForKey("positionType")->intValue());
		// Mode A: Gravity + tangential accel + radial accel
		if( m_nEmitterMode == kCCParticleModeGravity ) 
		{
			// gravity
			modeA.gravity.x = dictionary->valueForKey("gravityx")->floatValue();
			modeA.gravity.y = dictionary->valueForKey("gravityy")->floatValue();

			// speed
			modeA.speed = dictionary->valueForKey("speed")->floatValue();
			modeA.speedVar = dictionary->valueForKey("speedVariance")->floatValue();

			// radial acceleration
			modeA.radialAccel = dictionary->valueForKey("radialAcceleration")->floatValue();
			modeA.radialAccelVar = dictionary->valueForKey("radialAccelVariance")->floatValue();

			// tangential acceleration
			modeA.tangentialAccel = dictionary->valueForKey("tangentialAcceleration")->floatValue();
			modeA.tangentialAccelVar = dictionary->valueForKey("tangentialAccelVariance")->floatValue();

			// rotation is dir
			modeA.rotationIsDir = dictionary->valueForKey("rotationIsDir")->boolValue();
		}

		// or Mode B: radius movement
		else if( m_nEmitterMode == kCCParticleModeRadius ) 
		{
			modeB.startRadius = dictionary->valueForKey("maxRadius")->floatValue();
			modeB.startRadiusVar = dictionary->valueForKey("maxRadiusVariance")->floatValue();
			modeB.endRadius = dictionary->valueForKey("minRadius")->floatValue();
			modeB.endRadiusVar = 0.0f;
			modeB.rotatePerSecond = dictionary->valueForKey("rotatePerSecond")->floatValue();
			modeB.rotatePerSecondVar = dictionary->valueForKey("rotatePerSecondVariance")->floatValue();

		} else {
			CCAssert( false, "Invalid emitterType in config file");
			return false;
		}

		// life span
		m_fLife = dictionary->valueForKey("particleLifespan")->floatValue();
		m_fLifeVar = dictionary->valueForKey("particleLifespanVariance")->floatValue();

		textureName = dictionary->valueForKey("textureFileName")->getCString();

		textureImageData = NULL; // not use 
		CC_SAFE_RELEASE_NULL(dictionary);
		return true;
}


CCParticleDataPool * CCParticleDataPool::shareDataPool()
{
	if(!ParticleDataPoolInce)
	{
		ParticleDataPoolInce = new CCParticleDataPool;
	}
	return ParticleDataPoolInce;
}

CCParticleDataPool::CCParticleDataPool()
{
	
}

CCParticleDataPool::~CCParticleDataPool()
{
	// delete m_pDataPool
	DIC::iterator it = m_dic.begin();

	for(; it != m_dic.end() ; ++it)
	{
		CC_SAFE_DELETE(it->second);
	}
}

CCParticleData * CCParticleDataPool::getData(const string &dir)
{
	DIC::iterator it =  m_dic.find(dir);
	if(it == m_dic.end()) return NULL;
	return it->second;
}

CCParticleData * CCParticleDataPool::addData(const string &key)
{
	CCParticleData *pResult = getData(key);
	if(!pResult)
	{
		pResult = new CCParticleData;
		if(pResult->init(key.c_str()))
		{
			m_dic[key] = pResult;
		}else{
			delete pResult;
			pResult = NULL;
		}
	}
	return pResult;
}



NS_CC_END
