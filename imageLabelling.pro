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
    MoliProperties.h


SOURCES += \
    ImageOperate.cpp \
    BasicDataStructure.cpp \
    main.cpp \
    PaintTools.cpp \
    PolyLine.cpp \
    MyPreference.cpp \
    ImageViewer.cpp \
    LeftWindow.cpp \
    classification.cpp \
    buttom.cpp \
    propertynamedlg.cpp \
    UserInfo.cpp \
    advancesearchdlg.cpp \
    Global.cpp \
    imagecompletionui.cpp \
    ImageProperties.cpp \
    Login.cpp \
    MoliProperties.cpp \
    searchdata.cpp \
    UserAdd.cpp \
    UserEdit.cpp \
    UserManagement.cpp


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
    MoliProperties.ui

RESOURCES += \
    ImageCompletion.qrc

unix:!macx {
    LIBS += `pkg-config opencv --libs`
}
win32 {
INCLUDEPATH += D:\opencv\build\include\

LIBS += D:\vendor\OpenCV243-mingw\lib\libopencv_core243.dll.a\
        D:\vendor\OpenCV243-mingw\lib\libopencv_highgui243.dll.a\
        D:\vendor\OpenCV243-mingw\lib\libopencv_imgproc243.dll.a\
}

OTHER_FILES += \
    imageLabelling.ini

include(QtAwesome/QtAwesome.pri)
