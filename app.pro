QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    answerrouteinformation.cpp \
    answerroutesearch.cpp \
    dashboardscreen.cpp \
    database.cpp \
    main.cpp \
    routeinformation.cpp \
    routesearch.cpp \

HEADERS += \
    answerrouteinformation.h \
    answerroutesearch.h \
    dashboardscreen.h \
    database.h \
    routeinformation.h \
    routesearch.h \

FORMS += \
    answerrouteinformation.ui \
    answerroutesearch.ui \
    dashboardscreen.ui \
    routeinformation.ui \
    routesearch.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    app.pro.user \
    app.pro.user.65319cc
