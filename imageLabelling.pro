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
    advancesearchdlg.h \
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
    Util/def.h


SOURCES += \
    ImageOperate.cpp \
    BasicDataStructure.cpp \
    main.cpp \
    PaintTools.cpp \
    ImageViewer.cpp \
    classification.cpp \
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
    UserManagement.cpp \
    ImageComparison.cpp


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
    ImageComparison.ui

RESOURCES += \
    ImageCompletion.qrc

unix:!macx {
    LIBS += `pkg-config opencv --libs`

    INCLUDEPATH += Util
}
win32 {
#INCLUDEPATH += D:\opencv\build\include\
#               Util \

INCLUDEPATH += D:\Application\CMake_Qt_OpenCV_MinGW482\install\include\
            Util \

LIBS += D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_calib3d244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_contrib244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_core244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_features2d244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_flann244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_gpu244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_highgui244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_imgproc244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_legacy244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_ml244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_objdetect244.dll.a\
    D:\Application\CMake_Qt_OpenCV_MinGW482\install\lib\libopencv_video244.dll.a
}

OTHER_FILES += \
    imageLabelling.ini

include(QtAwesome/QtAwesome.pri)

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

