#include "UserProfileManager.h"

UserProfileManager::UserProfileManager(QObject* parent)
    : QObject(parent)
{
    // Provide a default profile
    UserProfile defaultP;
    defaultP.name = "Default";
    defaultP.basalRate = 1.0;
    defaultP.carbRatio = 10.0;
    defaultP.correctionFactor = 2.0;
    defaultP.targetGlucose = 6.0;

    profiles.push_back(defaultP);
    activeProfile = defaultP;
}

void UserProfileManager::loadProfile(const UserProfile &profile)
{
    activeProfile = profile;
}

UserProfile UserProfileManager::getActiveProfile() const
{
    return activeProfile;
}

const std::vector<UserProfile>& UserProfileManager::getProfiles() const
{
    return profiles;
}

void UserProfileManager::addProfile(const UserProfile &profile)
{
    profiles.push_back(profile);
}

void UserProfileManager::updateProfile(int index, const UserProfile &profile)
{
    if (index >= 0 && index < (int)profiles.size()) {
        profiles[index] = profile;
    }
}

void UserProfileManager::deleteProfile(int index)
{
    if (index >= 0 && index < (int)profiles.size()) {
        profiles.erase(profiles.begin() + index);
    }
}
