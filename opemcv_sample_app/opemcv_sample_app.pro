QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    opemcv_sample_app_es_CU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OPENCV_DESKTOP_WINDOWS=../OpenCV-MinGW-Build
windows {
    INCLUDEPATH += $$OPENCV_DESKTOP_WINDOWS/include
    LIBS += $$OPENCV_DESKTOP_WINDOWS/x64/mingw/bin/libopencv_core455.dll
    LIBS += $$OPENCV_DESKTOP_WINDOWS/x64/mingw/bin/libopencv_imgproc455.dll
    LIBS += $$OPENCV_DESKTOP_WINDOWS/x64/mingw/bin/libopencv_dnn455.dll
}

OPENCV_ANDROID=../OpenCV-android-sdk
android {
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        isEmpty(OPENCV_ANDROID) {
            error("Let OPENCV_ANDROID point to the opencv-android-sdk, recommended: v4.0")
        }
        INCLUDEPATH += "$$OPENCV_ANDROID/sdk/native/jni/include"
        LIBS += \
            -L"$$OPENCV_ANDROID/sdk/native/libs/armeabi-v7a" \
            -L"$$OPENCV_ANDROID/sdk/native/3rdparty/libs/armeabi-v7a" \
            -lopencv_java4 \

        ANDROID_EXTRA_LIBS = $$OPENCV_ANDROID/sdk/native/libs/armeabi-v7a/libopencv_java4.so
    } else {
        error("Unsupported architecture: $$ANDROID_TARGET_ARCH")
    }

}
