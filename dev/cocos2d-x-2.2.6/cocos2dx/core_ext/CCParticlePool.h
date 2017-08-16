#ifndef __CCPARTICLE_POOL_H__
#define __CCPARTICLE_POOL_H__
#include "ccMacros.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCString.h"
#include <map>
using namespace std;
NS_CC_BEGIN
	/** @typedef tCCPositionType
possible types of particle positions
*/
typedef enum {
    /** Living particles are attached to the world and are unaffected by emitter repositioning. */
    kCCPositionTypeFree,

    /** Living particles are attached to the world but will follow the emitter repositioning.
    Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
    */
    kCCPositionTypeRelative,

    /** Living particles are attached to the emitter and are translated along with it. */
    kCCPositionTypeGrouped,
}tCCPositionType;
//* @enum
enum {
    /** The Particle emitter lives forever */
    kCCParticleDurationInfinity = -1,

    /** The starting size of the particle is equal to the ending size */
    kCCParticleStartSizeEqualToEndSize = -1,

    /** The starting radius of the particle is equal to the ending radius */
    kCCParticleStartRadiusEqualToEndRadius = -1,

    // backward compatible
    kParticleStartSizeEqualToEndSize = kCCParticleStartSizeEqualToEndSize,
    kParticleDurationInfinity = kCCParticleDurationInfinity,
};

//* @enum
enum {
    /** Gravity mode (A mode) */
    kCCParticleModeGravity,

    /** Radius mode (B mode) */
    kCCParticleModeRadius,    
};




// backward compatible
enum {
    kPositionTypeFree = kCCPositionTypeFree,
    kPositionTypeGrouped = kCCPositionTypeGrouped,
}; 

// Different modes
//! Mode A:Gravity + Tangential Accel + Radial Accel
struct ParModeA
{
    /** Gravity value. Only available in 'Gravity' mode. */
    CCPoint gravity;
    /** speed of each particle. Only available in 'Gravity' mode.  */
    float speed;
    /** speed variance of each particle. Only available in 'Gravity' mode. */
    float speedVar;
    /** tangential acceleration of each particle. Only available in 'Gravity' mode. */
    float tangentialAccel;
    /** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
    float tangentialAccelVar;
    /** radial acceleration of each particle. Only available in 'Gravity' mode. */
    float radialAccel;
    /** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
    float radialAccelVar;
    /** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
    bool rotationIsDir;
};

struct ParModeB
{
    /** The starting radius of the particles. Only available in 'Radius' mode. */
    float startRadius;
    /** The starting radius variance of the particles. Only available in 'Radius' mode. */
    float startRadiusVar;
    /** The ending radius of the particles. Only available in 'Radius' mode. */
    float endRadius;
    /** The ending radius variance of the particles. Only available in 'Radius' mode. */
    float endRadiusVar;
    /** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
    float rotatePerSecond;
    /** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
    float rotatePerSecondVar;
};

extern tCCPositionType transIntToTCCPosT(int posType);
class CCParticleData;
class CC_DLL CCParticleDataPool
{
public:
	static CCParticleDataPool *shareDataPool();
	CCParticleData *getData(const string &dir);
	CCParticleData *addData(const string &key);

	CCParticleDataPool();
	~CCParticleDataPool();
private:
	typedef map<string , CCParticleData *> DIC;
	DIC m_dic;
};

class CC_DLL CCParticleData
{
public:
    bool init(const char *plistFile);

	std::string m_sPlistFile;
    int maxParticles;
    // angle
    float m_fAngle;
    float m_fAngleVar;

    // duration
    float m_fDuration;

    // blend function
    ccBlendFunc m_tBlendFunc;

    // color
    ccColor4F  m_tStartColor;
    ccColor4F  m_tStartColorVar;
    ccColor4F  m_tEndColor;
    ccColor4F  m_tEndColorVar;

    // particle size
    float m_fStartSize;
    float m_fStartSizeVar;
    float m_fEndSize;
    float m_fEndSizeVar;

    // position
    float x;
    float y;

    CCPoint m_tPosVar;

    // Spinning
    float m_fStartSpin;
    float m_fStartSpinVar;
    float m_fEndSpin;
    float m_fEndSpinVar;

    int m_nEmitterMode;
    tCCPositionType m_ePositionType;
    // Mode A: Gravity + tangential accel + radial accel
    ParModeA modeA;
    ParModeB modeB;

    // life span
    float m_fLife;
    float m_fLifeVar;

    std::string textureName;

	char *textureImageData;
};

NS_CC_END

#endif //__CCPARTICLE_POOL_H__
