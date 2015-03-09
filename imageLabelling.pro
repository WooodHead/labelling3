QT += sql

HEADERS += \
    ImageOperate.h \
    FileOperate.h \
    CImg.h \
    BasicDataStructure.h \
    interfaces.h \
    PaintTools.h \
    PolyLine.h \
    MyPreference.h \
    Linelet.h \
    ImageViewer.h \
    imagecompletionui.h \
    LeftWindow.h \
    searchdata.h \
    classification.h \
    buttom.h \
    advancesearchdlg.h \
    propertynamedlg.h \
    ImageProperties.h \
    Connection.h \
    Login.h \
    UserAdd.h \
    UserEdit.h \
    UserInfo.h \
    UserManagement.h \
    Global.h \
    SimplifiedSearch.h


SOURCES += \
    ImageOperate.cpp \
    BasicDataStructure.cpp \
    main.cpp \
    PaintTools.cpp \
    PolyLine.cpp \
    MyPreference.cpp \
    ImageViewer.cpp \
    LeftWindow.cpp \
    searchdata.cpp \
    classification.cpp \
    buttom.cpp \
    imagecompletionui.cpp \
    advancesearchdlg.cpp \
    propertynamedlg.cpp \
    ImageProperties.cpp \
    Login.cpp \
    UserAdd.cpp \
    UserEdit.cpp \
    UserInfo.cpp \
    UserManagement.cpp \
    Global.cpp \
    SimplifiedSearch.cpp


FORMS += \
    RegionCompetition.ui \
    imagecompletionui.ui \
    SceneCompletionWidget.ui \
    CornerWindow.ui \
    LeftWindow.ui \
    ButtomWindow.ui \
    searchdata.ui \
    classification.ui \
    buttom.ui \
    login.ui \
    usermanagement.ui \
    useradd.ui \
    imageproperties.ui \
    advancesearchdlg.ui \
    useredit.ui \
    propertynamedlg.ui \
    SimplifiedSearch.ui

RESOURCES += \
    ImageCompletion.qrc

unix:!macx {
    LIBS += `pkg-config opencv --libs`
}
win32 {
    INCLUDEPATH += F:\vendor\opencv\build\include\

    LIBS += F:\vendor\opencv\build\x86\vc10\lib\opencv_core242d.lib\
            F:\vendor\opencv\build\x86\vc10\lib\opencv_imgproc242d.lib\
            F:\vendor\opencv\build\x86\vc10\lib\opencv_highgui242d.lib
}

OTHER_FILES += \
    imageLabelling.ini

include(QtAwesome/QtAwesome/QtAwesome.pri)
QMAKE_CXXFLAGS += -std=c++0x
