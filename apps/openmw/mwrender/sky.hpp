#ifndef OPENMW_MWRENDER_SKY_H
#define OPENMW_MWRENDER_SKY_H

#include <string>
#include <memory>

#include <osg/ref_ptr>
#include <osg/Vec4f>
#include <osg/PositionAttitudeTransform>

namespace osg
{
    class Group;
    class Node;
    class Material;
}

namespace osgParticle
{
    class ParticleSystem;
}

namespace Resource
{
    class SceneManager;
}

namespace MWRender
{
    class AtmosphereUpdater;
    class AtmosphereNightUpdater;
    class CloudUpdater;
    class Sun;
    class Moon;
    class RainShooter;
    class RainFader;
    class AlphaFader;
    class UnderwaterSwitchCallback;

    struct WeatherResult
    {
        std::string mCloudTexture;
        std::string mNextCloudTexture;
        float mCloudBlendFactor;

        osg::Vec4f mFogColor;

        osg::Vec4f mAmbientColor;

        osg::Vec4f mSkyColor;

        // sun light color
        osg::Vec4f mSunColor;

        // alpha is the sun transparency
        osg::Vec4f mSunDiscColor;

        float mFogDepth;

        float mWindSpeed;

        float mCloudSpeed;

        float mGlareView;

        bool mNight; // use night skybox
        float mNightFade; // fading factor for night skybox

        bool mIsStorm;

        std::string mAmbientLoopSoundID;
        float mAmbientSoundVolume;

        std::string mParticleEffect;
        std::string mRainEffect;
        float mEffectFade;

        float mRainSpeed;
        float mRainFrequency;
    };

    struct MoonState
    {
        enum Phase
        {
            Phase_Full = 0,
            Phase_WaningGibbous,
            Phase_ThirdQuarter,
            Phase_WaningCrescent,
            Phase_New,
            Phase_WaxingCrescent,
            Phase_FirstQuarter,
            Phase_WaxingGibbous,
            Phase_Unspecified
        };

        float mRotationFromHorizon;
        float mRotationFromNorth;
        Phase mPhase;
        float mShadowBlend;
        float mMoonAlpha;
    };

    ///@brief The SkyManager handles rendering of the sky domes, celestial bodies as well as other objects that need to be rendered
    /// relative to the camera (e.g. weather particle effects)
    class SkyManager
    {
    public:
        SkyManager(osg::Group* parentNode, Resource::SceneManager* sceneManager);
        ~SkyManager();

        void update(float duration);

        void setEnabled(bool enabled);

        void setHour (double hour);
        ///< will be called even when sky is disabled.

        void setDate (int day, int month);
        ///< will be called even when sky is disabled.

        int getMasserPhase() const;
        ///< 0 new moon, 1 waxing or waning cresecent, 2 waxing or waning half,
        /// 3 waxing or waning gibbous, 4 full moon

        int getSecundaPhase() const;
        ///< 0 new moon, 1 waxing or waning cresecent, 2 waxing or waning half,
        /// 3 waxing or waning gibbous, 4 full moon

        void setMoonColour (bool red);
        ///< change Secunda colour to red

        void setWeather(const WeatherResult& weather);

        void sunEnable();

        void sunDisable();

        void setRainSpeed(float speed);

        void setStormDirection(const osg::Vec3f& direction);

        void setSunDirection(const osg::Vec3f& direction);

        void setMasserState(const MoonState& state);
        void setSecundaState(const MoonState& state);

        void setGlareTimeOfDayFade(float val);

        /// Enable or disable the water plane (used to remove underwater weather particles)
        void setWaterEnabled(bool enabled);

        /// Set height of water plane (used to remove underwater weather particles)
        void setWaterHeight(float height);

        void listAssetsToPreload(std::vector<std::string>& models, std::vector<std::string>& textures);

    private:
        void create();
        ///< no need to call this, automatically done on first enable()

        void createRain();
        void destroyRain();
        void updateRainParameters();

        Resource::SceneManager* mSceneManager;

        osg::ref_ptr<osg::Group> mRootNode;
        osg::ref_ptr<osg::Group> mEarlyRenderBinRoot;

        osg::ref_ptr<osg::PositionAttitudeTransform> mParticleNode;
        osg::ref_ptr<osg::Node> mParticleEffect;
        std::vector<osg::ref_ptr<AlphaFader> > mParticleFaders;
        osg::ref_ptr<UnderwaterSwitchCallback> mUnderwaterSwitch;

        osg::ref_ptr<osg::PositionAttitudeTransform> mCloudNode;

        osg::ref_ptr<CloudUpdater> mCloudUpdater;
        osg::ref_ptr<CloudUpdater> mCloudUpdater2;
        osg::ref_ptr<osg::Node> mCloudMesh;
        osg::ref_ptr<osg::Node> mCloudMesh2;

        osg::ref_ptr<osg::Node> mAtmosphereDay;

        osg::ref_ptr<osg::PositionAttitudeTransform> mAtmosphereNightNode;
        float mAtmosphereNightRoll;
        osg::ref_ptr<AtmosphereNightUpdater> mAtmosphereNightUpdater;

        osg::ref_ptr<AtmosphereUpdater> mAtmosphereUpdater;

        std::auto_ptr<Sun> mSun;
        std::auto_ptr<Moon> mMasser;
        std::auto_ptr<Moon> mSecunda;

        osg::ref_ptr<osg::Group> mRainNode;
        osg::ref_ptr<osgParticle::ParticleSystem> mRainParticleSystem;
        osg::ref_ptr<RainShooter> mRainShooter;
        osg::ref_ptr<RainFader> mRainFader;

        bool mCreated;

        bool mIsStorm;

        int mDay;
        int mMonth;

        float mCloudAnimationTimer;

        float mRainTimer;

        osg::Vec3f mStormDirection;

        // remember some settings so we don't have to apply them again if they didnt change
        std::string mClouds;
        std::string mNextClouds;
        float mCloudBlendFactor;
        float mCloudSpeed;
        float mStarsOpacity;
        osg::Vec4f mCloudColour;
        osg::Vec4f mSkyColour;
        osg::Vec4f mFogColour;

        std::string mCurrentParticleEffect;

        float mRemainingTransitionTime;

        bool mRainEnabled;
        std::string mRainEffect;
        float mRainSpeed;
        float mRainFrequency;
        float mWindSpeed;

        bool mEnabled;
        bool mSunEnabled;

        osg::Vec4f mMoonScriptColor;
    };
}

#endif // GAME_RENDER_SKY_H
