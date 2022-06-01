#/**************************************************************************
# *  This file is part of QXmlEdit                                         *
# *  Copyright (C) 2011-2020 by Luca Bellonda and individual contributors  *
# *    as indicated in the AUTHORS file                                    *
# *  lbellonda _at_ gmail.com                                              *
# *                                                                        *
# * This library is free software; you can redistribute it and/or          *
# * modify it under the terms of the GNU Library General Public            *
# * License as published by the Free Software Foundation; either           *
# * version 2 of the License, or (at your option) any later version.       *
# *                                                                        *
# * This library is distributed in the hope that it will be useful,        *
# * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
# * Library General Public License for more details.                       *
# *                                                                        *
# * You should have received a copy of the GNU Library General Public      *
# * License along with this library; if not, write to the                  *
# * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
# * Boston, MA  02110-1301  USA                                            *
# **************************************************************************/
#
# Project created by QtCreator 2011-08-15T09:23:52
#
#-------------------------------------------------

include("../src/version.pri")

############################ BEGIN INSTALLATION FOLDERS DECLARATION ###########################################

include("../src/cconfig.pri")

############################ END INSTALLATION FOLDERS DECLARATION #############################################


#default value for the unix/Linux target name
TARGET_NAME_UNIXSTYLE_DEFAULT=""

unix:!macx: {
   TARGET_NAME_UNIXSTYLE_DEFAULT="1"
}

equals(TARGET_NAME_UNIXSTYLE, "") {
    TARGET_NAME_UNIXSTYLE = $$TARGET_NAME_UNIXSTYLE_DEFAULT
}

# This is necessary to build the executable as an app
DEFINES += LIBQXMLEDIT_LIBRARY_STATIC
DEFINES += QXMLEDITSESSIONS_LIBRARY_STATIC

QT       += gui xml xmlpatterns svg network sql

macx: {
    QT       += macextras
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += printsupport widgets core
}

isEqual(ENABLE_SCXML, "Y") {
    QT       += scxml
}

greaterThan(QT_MAJOR_VERSION, 4) {
win32 {
    QT += winextras
    DEFINES += "_NO_W32_PSEUDO_MODIFIERS"
    DEFINES += "NOGDI"
}
}


include(../src/coptions.pri)

TARGET = QXmlEdit

TEMPLATE = app

DESTDIR = ../build

equals(INST_AVOID_PRECOMP_HEADERS, "") {
 CONFIG += precompile_header
 PRECOMPILED_HEADER  = ../src/precompiled_app.h
}

HEADERS += ../src/precompiled_app.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

# object files
OBJECTS_DIR = ../build/singleapp/obj
MOC_DIR = ../build/singleapp/moc
UI_DIR = ../build/singleapp/ui
UI_HEADERS_DIR = ../build/singleapp/ui/include
UI_SOURCES_DIR = ../build/singleapp/ui/src
RCC_DIR = ../build/singleapp/rcc

INCLUDEPATH += ../src
INCLUDEPATH += ../sessions/include
INCLUDEPATH += ../framework/include
INCLUDEPATH += ../globals/includes
INCLUDEPATH += ../src/sessions

RESOURCES += ../src/sessions/sessions.qrc

ICON = ../src/images/qxmledit.icns
win32:RC_FILE = ../src/win.rc
os2:RC_FILE = ../src/os2.rc

DEFINES += TRANLASTION_DIR= translations

macx:QMAKE_INFO_PLIST    = ../src/resources/QXmlEdit.plist

TRANSLATIONS = \
   ../src/languages_app/QXmlEdit_cs.ts \
    ../src/languages_lib/QXmlEditlib_cs.ts \
    ../src/sessions/languages/QXmlEditSessions_cs.ts
TRANSL = \
   ../src/languages_app/QXmlEdit_cs.qm \
    ../src/languages_lib/QXmlEditlib_cs.qm \
    ../src/sessions/languages/QXmlEditSessions_cs.qm


target.path = $$INST_DIR
translationsfiles.path = $$INST_TRANSLATIONS_DIR
translationsfiles.files = $$TRANSL


INSTALLS += target translationsfiles

#-----------------------------------------------------------
styles.path = $$INST_DATA_DIR
styles.files = data/sample.style
snippets.path = $$INST_DATA_DIR/snippets
snippets.files = data/snippets/{6d11b8d1-e285-46b2-8375-79e17cab9862}.xml \
    data/snippets/{b3303cb3-8537-46d0-943b-cf898cc93b3f}.xml \
    data/snippets/{bd4984f5-107e-48ca-9780-c263ff1aa58d}.xml \
    data/snippets/{d78f4e09-4003-4178-adec-134a7b996d6e}.xml \
    data/snippets/{e40792fb-16de-4418-9320-4feaa92c28a3}.xml \
    data/snippets/{edc60671-0ae0-401b-989d-5d0b82976bce}.xml \
    data/snippets/{f6d7ebdd-c505-4a6a-8a16-f7c99581a995}.xml

manual.path = $$INST_DOC_DIR
manual.files = ../doc/QXmlEdit_manual.pdf

iconInfo.path = $$INST_ICON_DIR
iconInfo.files = ../install_scripts/environment/icon/qxmledit.png

desktopInfo.path = $$INST_DESKTOP_DIR
desktopInfo.files = ../install_scripts/environment/desktop/QXmlEdit.desktop

metaInfo.path = $$INST_APPDATA_DIR
metaInfo.files = ../install_scripts/environment/desktop/QXmlEdit.appdata.xml

#-----------------------------------------------------------

# resources are defined in paths.h
unix:!macx:DEFINES += UNIX_RESOURCES

DEFINES += UNIX_RESOURCE_PATH=$$INST_DATA_DIR
DEFINES += UNIX_DOC_PATH=$$INST_DOC_DIR

unix:INSTALLS += snippets
unix:INSTALLS += styles
unix:INSTALLS += manual
unix:INSTALLS += desktopInfo

equals(INSTALL_METAINFO_ENABLED, "Y") {
    unix:INSTALLS += metaInfo
}

include("../src/allsources.pri")

equals(USE_QWTPLOT, "Y") {

 greaterThan(QT_MAJOR_VERSION, 4) {

    QT += opengl
    INCLUDEPATH += ../external/qwtplot3d/include
    DEPENDPATH += ../external/qwtplot3d/include
    DEFINES += QWT_PLOT3D

    LIBS += -L../build/ -lqwtplot3d

    unix:!macx{
        LIBS += -lGL -lGLU
    }

    win32:{
        LIBS += -lopengl32  -lglu32
    }
  }
}
