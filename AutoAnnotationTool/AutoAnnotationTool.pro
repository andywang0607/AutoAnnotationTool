QT += quick
QT += widgets
QT += concurrent

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CV/cvmodule.cpp \
        CV/cvparam.cpp \
        annotationmanager.cpp \
        labelcollector.cpp \
        labeldatamodel.cpp \
        main.cpp \
        opencvtypeconverter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CV/cvmodule.h \
    CV/cvparam.h \
    annotationmanager.h \
    labelcollector.h \
    labeldata.h \
    labeldatamodel.h \
    mouseselectresult.h \
    opencvtypeconverter.h

win32:INCLUDEPATH += $(THIRD_PARTY)/opencv/build/include

CONFIG(debug, debug|release) {
    win32:LIBS += $(THIRD_PARTY)/opencv/build/x64/vc15/lib/opencv_world349d.lib
}
CONFIG(release, debug|release) {
    win32:LIBS += $(THIRD_PARTY)/opencv/build/x64/vc15/lib/opencv_world349.lib
}

unix::INCLUDEPATH += /usr/local/include/opencv \
                     /usr/local/include/opencv2


unix::LIBS += /usr/local/lib/libopencv_highgui.so \
              /usr/local/lib/libopencv_core.so    \
              /usr/local/lib/libopencv_imgproc.so \
              /usr/local/lib/libopencv_imgcodecs.so

DISTFILES += \
    labeldatamodel.qmodel
