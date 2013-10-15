#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T13:27:54
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CountingWords
TEMPLATE = app


SOURCES += main.cpp\
        CountingWordMWindow.cpp \
    DictionaryModel.cpp \
    DictionaryLoader.cpp

HEADERS  += CountingWordMWindow.h \
    DictionaryModel.h \
    DictionaryLoader.h

FORMS    += CountingWordMWindow.ui
