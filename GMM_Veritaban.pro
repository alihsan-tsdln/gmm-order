QT       += core gui sql network axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    firstrapor.cpp \
    main.cpp \
    mainwindow.cpp \
    secondrapor.cpp \
    sorumlu.cpp \
    tablo.cpp \
    turkishlira.cpp

HEADERS += \
    firstrapor.h \
    mainwindow.h \
    secondrapor.h \
    sorumlu.h \
    tablo.h \
    turkishlira.h

FORMS += \
    firstrapor.ui \
    mainwindow.ui \
    secondrapor.ui \
    sorumlu.ui \
    tablo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
