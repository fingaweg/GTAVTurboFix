#include "TurboFix.h"

#include "Script.hpp"
#include "Util/Math.hpp"
#include <algorithm>

bool TF_Active() {
    if (!TurboFix::GetScript())
        return false;
    auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
    return activeConfig;
}

float TF_GetNormalizedBoost() {
    if (!TurboFix::GetScript())
        return 0.0f;
    auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
    if (activeConfig) {
        return map(TurboFix::GetScript()->GetCurrentBoost(),
            activeConfig->Turbo.MinBoost, activeConfig->Turbo.MaxBoost,
            -1.0f, 1.0f);
    }
    return 0.0f;
}

float TF_GetAbsoluteBoost() {
    if (!TurboFix::GetScript())
        return 0.0f;
    auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
    if (activeConfig) {
        return TurboFix::GetScript()->GetCurrentBoost();
    }
    return 0.0f;
}

float TF_GetAbsoluteBoostMin() {
    if (!TurboFix::GetScript())
        return 0.0f;
    auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
    if (activeConfig) {
        return activeConfig->Turbo.MinBoost;
    }
    return 0.0f;
}

float TF_GetAbsoluteBoostMax() {
    if (!TurboFix::GetScript())
        return 0.0f;
    auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
    if (activeConfig) {
        return activeConfig->Turbo.MaxBoost;
    }
    return 0.0f;
}

TF_API const char* TF_GetActiveConfigName() {
    if (TurboFix::GetScript()) {
        auto* activeConfig = TurboFix::GetScript()->ActiveConfig();
        if (activeConfig) {
            return activeConfig->Name.c_str();
        }
    }
    return nullptr;
}

float TF_GetAbsoluteBoostConfig(const char* configName, float rpm) {
    if (!TurboFix::GetScript())
        return 0.0f;

    const auto& configs = TurboFix::GetConfigs();
    auto configIt = std::find_if(configs.begin(), configs.end(),
        [&](const auto& config) {
            return config.Name == configName;
        });

    if (configIt == configs.end())
        return 0.0f;

    if (rpm < 0.0f) {
        return configIt->Turbo.MaxBoost;
    }

    float boostWOT = map(rpm,
        configIt->Turbo.RPMSpoolStart, configIt->Turbo.RPMSpoolEnd,
        0.0f, configIt->Turbo.MaxBoost);

    if (configIt->Turbo.FalloffRPM > configIt->Turbo.RPMSpoolEnd &&
        rpm >= configIt->Turbo.FalloffRPM) {

        float falloffBoost = map(rpm,
            configIt->Turbo.FalloffRPM, 1.0f,
            configIt->Turbo.MaxBoost, configIt->Turbo.FalloffBoost);

        if (boostWOT > falloffBoost)
            boostWOT = falloffBoost;
    }

    return std::clamp(boostWOT,
        configIt->Turbo.MinBoost,
        configIt->Turbo.MaxBoost);
}
