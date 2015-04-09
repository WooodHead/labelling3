QT += sql

HEADERS += \
    ImageOperate.h \
    CImg.h \
    BasicDataStructure.h \
    interfaces.h \
    PaintTools.h \
    ImageViewer.h \
    imagecompletionui.h \
    searchdata.h \
    classification.h \
    propertynamedlg.h \
    ImageProperties.h \
    Login.h \
    UserAdd.h \
    UserEdit.h \
    UserInfo.h \
    UserManagement.h \
    Global.h \
    MoliProperties.h \
    ImageComparison.h \
    expdlg.h \
    thumbnailwindow.h \
    advancesearchdlg.h \
    Util/def.h


SOURCES += \
    ImageOperate.cpp \
    BasicDataStructure.cpp \
    main.cpp \
    PaintTools.cpp \
    ImageViewer.cpp \
    classification.cpp \
    UserInfo.cpp \
    advancesearchdlg.cpp \
    Global.cpp \
    Login.cpp \
    searchdata.cpp \
    UserAdd.cpp \
    UserEdit.cpp \
    UserManagement.cpp \
    ImageComparison.cpp \
    expdlg.cpp \
    propertynamedlg.cpp \
    thumbnailwindow.cpp \
    imagecompletionui.cpp \
    ImageProperties.cpp \
    MoliProperties.cpp


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
    MoliProperties.ui \
    ImageComparison.ui \
    expdlg.ui \
    thumbnailwindow.ui

RESOURCES += \
    ImageCompletion.qrc

unix:!macx {
    LIBS += `pkg-config opencv --libs`

    INCLUDEPATH += Util
}
win32 {
INCLUDEPATH += D:\opencv\build\include\
               Util \

#INCLUDEPATH += D:\Application\CMake_Qt_OpenCV_MinGW482\install\include\
#            Util \

LIBS += D:\vendor\OpenCV243-mingw\lib\libopencv_core243.dll.a\
    D:\vendor\OpenCV243-mingw\lib\libopencv_highgui243.dll.a\
    D:\vendor\OpenCV243-mingw\lib\libopencv_imgproc243.dll.a\
}

OTHER_FILES += \
    imageLabelling.ini

include(QtAwesome/QtAwesome.pri)

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

