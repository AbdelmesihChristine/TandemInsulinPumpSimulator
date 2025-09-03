#include "ProfileDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "UserProfile.h"

ProfileDialog::ProfileDialog(UserProfileManager* mgr, QWidget *parent)
    : QDialog(parent)
    , profileManager(mgr)
{
    setWindowTitle("Profile Manager");
    resize(400, 300);

    profileList  = new QListWidget(this);
    addButton    = new QPushButton("Add", this);
    editButton   = new QPushButton("Edit", this);
    deleteButton = new QPushButton("Delete", this);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(profileList);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // Hook up button signals
    connect(addButton,    &QPushButton::clicked, this, &ProfileDialog::onAddProfile);
    connect(editButton,   &QPushButton::clicked, this, &ProfileDialog::onEditProfile);
    connect(deleteButton, &QPushButton::clicked, this, &ProfileDialog::onDeleteProfile);
    connect(profileList,  &QListWidget::itemDoubleClicked, this, &ProfileDialog::onProfileActivated);

    // Load initial list from the manager
    refreshProfileList();
}

/**
 * @brief refreshProfileList reloads the ListWidget with the manager's profiles.
 */
void ProfileDialog::refreshProfileList()
{
    profileList->clear();
    const auto &profiles = profileManager->getProfiles();
    for (auto &p : profiles) {
        profileList->addItem(p.name);
    }
}

/**
 * @brief getProfileInput uses QInputDialog to collect profile data from the user.
 */
bool ProfileDialog::getProfileInput(UserProfile &profile, const QString &title)
{
    bool ok = false;
    QString name = QInputDialog::getText(this, title, "Profile Name:",
                                         QLineEdit::Normal, profile.name, &ok);
    if (!ok || name.isEmpty()) return false;

    double basal = QInputDialog::getDouble(this, title, "Basal Rate (U/hr):",
                                           profile.basalRate, 0, 100, 1, &ok);
    if (!ok) return false;

    double carbRatio = QInputDialog::getDouble(this, title, "Carb Ratio (g/U):",
                                               profile.carbRatio, 0, 1000, 1, &ok);
    if (!ok) return false;

    double cf = QInputDialog::getDouble(this, title, "Correction Factor (mmol/L per U):",
                                        profile.correctionFactor, 0, 50, 1, &ok);
    if (!ok) return false;

    double tgt = QInputDialog::getDouble(this, title, "Target BG (mmol/L):",
                                         profile.targetGlucose, 3, 30, 1, &ok);
    if (!ok) return false;

    // Assign results
    profile.name = name;
    profile.basalRate = basal;
    profile.carbRatio = carbRatio;
    profile.correctionFactor = cf;
    profile.targetGlucose = tgt;

    return true;
}

/**
 * @brief Adds a new profile by collecting user input.
 */
void ProfileDialog::onAddProfile()
{
    UserProfile newProfile;
    // Provide some default example values
    newProfile.basalRate       = 1.0;
    newProfile.carbRatio       = 10.0;
    newProfile.correctionFactor= 2.0;
    newProfile.targetGlucose   = 6.0;

    if (getProfileInput(newProfile, "Add Profile")) {
        profileManager->addProfile(newProfile);
        refreshProfileList();
    }
}

/**
 * @brief Edits an existing profile from the list.
 */
void ProfileDialog::onEditProfile()
{
    int index = profileList->currentRow();
    if (index < 0) return;

    auto profiles = profileManager->getProfiles();
    if (index >= (int)profiles.size()) return;

    UserProfile editable = profiles[index];
    if (getProfileInput(editable, "Edit Profile")) {
        profileManager->updateProfile(index, editable);
        refreshProfileList();
    }
}

/**
 * @brief Deletes an existing profile from the list.
 */
void ProfileDialog::onDeleteProfile()
{
    int index = profileList->currentRow();
    if (index < 0) return;

    auto profiles = profileManager->getProfiles();
    if (index >= (int)profiles.size()) return;

    profileManager->deleteProfile(index);
    refreshProfileList();
}

/**
 * @brief onProfileActivated is triggered when the user double-clicks a profile.
 * It sets that profile as active in the manager.
 */
void ProfileDialog::onProfileActivated(QListWidgetItem* item)
{
    if (!item) return;
    int index = profileList->row(item);
    auto profiles = profileManager->getProfiles();
    if (index < 0 || index >= (int)profiles.size()) return;

    profileManager->loadProfile(profiles[index]);
    QMessageBox::information(this, "Profile Activated",
                             QString("Profile '%1' now active.")
                             .arg(profiles[index].name));
}
