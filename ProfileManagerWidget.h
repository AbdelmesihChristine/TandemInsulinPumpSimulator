#ifndef PROFILEMANAGERWIDGET_H
#define PROFILEMANAGERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include "UserProfileManager.h"

class ProfileManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileManagerWidget(UserProfileManager* mgr, QWidget *parent = nullptr);

private slots:
    void onAddProfile();
    void onEditProfile();
    void onDeleteProfile();
    void onProfileActivated(QListWidgetItem* item);

private:
    void refreshProfileList();
    bool getProfileInput(UserProfile &profile, const QString &title);

    UserProfileManager* profileManager;  // pointer, not local

    QListWidget*  profileList;
    QPushButton*  addButton;
    QPushButton*  editButton;
    QPushButton*  deleteButton;
};

#endif // PROFILEMANAGERWIDGET_H
