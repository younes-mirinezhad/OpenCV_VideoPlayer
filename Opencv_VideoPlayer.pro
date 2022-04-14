QT += quick quickcontrols2

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        opencv_player_viewport.cpp \
        opencv_videoplayer.cpp

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
    opencv_player_viewport.h \
    opencv_videoplayer.h

DISTFILES +=

# ----- opencv path
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

#INCLUDEPATH += /usr/local/include/opencv4
#LIBS += -L/usr/local/lib \
#     -lopencv_core\
#     -lopencv_imgcodecs \
#     -lopencv_imgproc \
#     -lopencv_photo \
#     -lopencv_videoio \
#     -lopencv_video \


