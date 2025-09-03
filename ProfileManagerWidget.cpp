#include "ProfileManagerWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "UserProfile.h"

ProfileManagerWidget::ProfileManagerWidget(UserProfileManager* mgr, QWidget *parent)
    : QWidget(parent)
    , profileManager(mgr)
{
    profileList  = new QListWidget(this);
    addButton    = new QPushButton("Add", this);
    editButton   = new QPushButton("Edit", this);
    deleteButton = new QPushButton("Delete", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(profileList);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(addButton,    &QPushButton::clicked, this, &ProfileManagerWidget::onAddProfile);
    connect(editButton,   &QPushButton::clicked, this, &ProfileManagerWidget::onEditProfile);
    connect(deleteButton, &QPushButton::clicked, this, &ProfileManagerWidget::onDeleteProfile);
    connect(profileList,  &QListWidget::itemDoubleClicked, this, &ProfileManagerWidget::onProfileActivated);

    refreshProfileList();
}

void ProfileManagerWidget::refreshProfileList()
{
    profileList->clear();
    const auto &profiles = profileManager->getProfiles();
    for (const auto &p : profiles) {
        profileList->addItem(p.name);
    }
}

bool ProfileManagerWidget::getProfileInput(UserProfile &profile, const QString &title)
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

    int targetBG = QInputDialog::getInt(this, title, "Target BG (mmol/L):",
                                        (int)profile.targetGlucose, 3, 30, 1, &ok);
    if (!ok) return false;

    profile.name            = name;
    profile.basalRate       = basal;
    profile.carbRatio       = carbRatio;
    profile.correctionFactor= cf;
    profile.targetGlucose   = targetBG;

    return true;
}

void ProfileManagerWidget::onAddProfile()
{
    UserProfile profile;
    // default or last known?
    profile.basalRate = 1.0;
    profile.carbRatio = 10.0;
    profile.correctionFactor = 2.0;
    profile.targetGlucose    = 6; // mmol/L

    if (getProfileInput(profile, "Add Profile")) {
        profileManager->addProfile(profile);
        refreshProfileList();
    }
}

void ProfileManagerWidget::onEditProfile()
{
    int index = profileList->currentRow();
    if (index < 0) return;

    auto profiles = profileManager->getProfiles(); // returns const ref
    if (index >= (int)profiles.size()) return;

    UserProfile editable = profiles[index];
    if (getProfileInput(editable, "Edit Profile")) {
        profileManager->updateProfile(index, editable);
        refreshProfileList();
    }
}

void ProfileManagerWidget::onDeleteProfile()
{
    int index = profileList->currentRow();
    if (index < 0) return;

    auto profiles = profileManager->getProfiles();
    if (index >= (int)profiles.size()) return;

    profileManager->deleteProfile(index);
    refreshProfileList();
}

void ProfileManagerWidget::onProfileActivated(QListWidgetItem* item)
{
    // Double-clicking an item in the list
    if (!item) return;
    int index = profileList->row(item);
    if (index < 0) return;

    auto profiles = profileManager->getProfiles();
    if (index >= (int)profiles.size()) return;

    profileManager->loadProfile(profiles[index]);
    QMessageBox::information(this, "Profile Activated",
        QString("Profile '%1' is now active.").arg(profiles[index].name));
}
