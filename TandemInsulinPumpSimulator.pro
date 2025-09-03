QT       += core gui widgets charts

CONFIG += c++11

SOURCES += \
    AlertDialog.cpp \
    BolusDeliveryWidget.cpp \
    BolusDialog.cpp \
    HistoryManager.cpp \
    HistoryRecord.cpp \
    BolusSafetyManager.cpp \
    CGMGraphWidget.cpp \
    CgmSimulator.cpp \
    HistoryDialog.cpp \
    MainWindow.cpp \
    ProfileDialog.cpp \
    PumpController.cpp \
    UserProfileManager.cpp \
    WarningChecker.cpp \
    main.cpp

HEADERS += \
    AlertDialog.h \
    BolusDeliveryWidget.h \
    BolusDialog.h \
    HistoryManager.h \
    HistoryRecord.h \
    BolusSafetyManager.h \
    CGMGraphWidget.h \
    CgmSimulator.h \
    HistoryDialog.h \
    MainWindow.h \
    ProfileDialog.h \
    PumpController.h \
    UserProfile.h \
    UserProfileManager.h \
    WarningChecker.h

FORMS += \
    MainWindow.ui

RESOURCES += \
    icons.qrc
