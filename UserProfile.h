#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QString>

/**
 * @brief Data structure representing a user's basal rate, carb ratio,
 * correction factor, and target glucose for insulin dosing.
 */
struct UserProfile {
    QString name;
    double basalRate;         // U/hour
    double carbRatio;         // grams per 1U
    double correctionFactor;  // mmol/L per 1U
    double targetGlucose;     // mmol/L

    UserProfile()
        : basalRate(0)
        , carbRatio(0)
        , correctionFactor(0)
        , targetGlucose(5)
    {}
};

#endif // USERPROFILE_H
