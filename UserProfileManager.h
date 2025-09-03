#ifndef USERPROFILEMANAGER_H
#define USERPROFILEMANAGER_H

#include <QObject>
#include <vector>
#include "UserProfile.h"

/**
 * @brief Manages a collection of UserProfiles and tracks the currently active one.
 */
class UserProfileManager : public QObject
{
    Q_OBJECT
public:
    explicit UserProfileManager(QObject* parent = nullptr);

    void loadProfile(const UserProfile& profile);
    UserProfile getActiveProfile() const;

    const std::vector<UserProfile>& getProfiles() const;
    void addProfile(const UserProfile& profile);
    void updateProfile(int index, const UserProfile& profile);
    void deleteProfile(int index);

private:
    std::vector<UserProfile> profiles;
    UserProfile activeProfile;
};

#endif // USERPROFILEMANAGER_H
