#pragma once
#include <inc/types.h>
#include <string>
#include <vector>
#include <map>

class CConfig {
public:
    enum class ESaveType {
        Specific,       // [ID] writes Model + Plate
        GenericModel,   // [ID] writes Model
        GenericNone,    // [ID] writes none
    };

    CConfig() = default;
    static CConfig Read(const std::string& configFile);

    void Write(ESaveType saveType);
    bool Write(const std::string& newName, Hash model, std::string plate, ESaveType saveType);

    // 2.1.0 config or earlier
    // Contains "Models" and not "ModelHashes"/"ModelNames"
    bool Legacy;

    std::string Name;

    Hash ModelHash;
    std::string ModelName;
    std::string Plate;

    // Turbo
    struct {
        // Force-install turbo if not installed yet, on loading.
        bool ForceTurbo = false;

        // RPM where boost may build
        float RPMSpoolStart = 0.2f;

        // RPM where boost may be max
        float RPMSpoolEnd = 0.5f;

        // -1.0f, but -0.8f for pretty
        float MinBoost = -0.8f;

        // 1.0f max
        float MaxBoost = 1.0f;

        // How many % of full boost after 1 second: 0.999f
        float SpoolRate = 0.999f;

        // How many % of no boost after 1 second: 0.97f
        float UnspoolRate = 0.97f;

        // RPM where boost/power starts dropping. Effective if > spool end RPM
        float FalloffRPM = 0.0f;

        // Boost at redline of max boost, if FalloffRPM is active
        float FalloffBoost = 0.0f;

    } Turbo;

    // BoostByGear
    struct {
        bool Enable = false;
        std::map<int, float> Gear;
    } BoostByGear;

    // AntiLag
    struct {
        bool Enable = false;
        float MinRPM = 0.65f;

        // Ptfx and Sfx
        bool Effects = false;

        // Delay = PeriodMs + rand() % RandomMs
        int PeriodMs = 50;
        int RandomMs = 150;

        bool LoudOffThrottle = false;
        int LoudOffThrottleIntervalMs = 500;

        // "Default", "NoSound" or some custom stuff
        std::string SoundSet = "Default";
        float Volume = 0.25f;
    } AntiLag;

    // DashHook
    struct {
        float BoostOffset = 0.0f;
        float BoostScale = 1.0f;

        float VacuumOffset = 0.0f;
        float VacuumScale = 1.0f;

        bool BoostIncludesVacuum = false;
    } Dial;
};
