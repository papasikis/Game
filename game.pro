#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T21:39:37
#
#-------------------------------------------------

QT       += core gui \
            xml

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS    += mainwindow.ui \
    menu.ui

RESOURCES += \
    tiles.qrc

HEADERS += \
    mainwindow.h \
    main/scene.h \
    main/view.h \
    main/map.h \
    main/game.h \
    game_objects/hero.h \
    game_objects/gamecreature.h \
    game_objects/graphicscreature.h \
    game_objects/armor.h \
    game_objects/inventory.h \
    game_objects/item.h \
    game_objects/magic.h \
    game_objects/mob.h \
    game_objects/usable.h \
    game_objects/weapon.h \
    menu.h \
    game_objects/spawn.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    main/scene.cpp \
    main/view.cpp \
    main/map.cpp \
    main/game.cpp \
    game_objects/hero.cpp \
    game_objects/gamecreature.cpp \
    game_objects/graphicscreature.cpp \
    game_objects/armor.cpp \
    game_objects/inventory.cpp \
    game_objects/item.cpp \
    game_objects/magic.cpp \
    game_objects/mob.cpp \
    game_objects/usable.cpp \
    game_objects/weapon.cpp \
    menu.cpp \
    game_objects/spawn.cpp
