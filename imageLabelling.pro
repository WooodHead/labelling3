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
    connection.h \
    classification.h \
    buttom.h \
    login.h \
    global.h \
    usermanagement.h \
    useradd.h

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
    login.cpp \
    global.cpp \
    usermanagement.cpp \
    useradd.cpp


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
    useradd.ui

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
