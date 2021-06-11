cmake_minimum_required(VERSION 3.5)
if(WIN32)
    set(VCPKG_ROOT "D:/ThirdParty/vcpkg/" )
else()
    set(VCPKG_ROOT "/home/ubuntu/ThirdParty/vcpkg/" )
endif()
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake)
project(AutoAnnotationTool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Core Quick Widgets Concurrent QuickControls2 REQUIRED)
if(WIN32)
    set( OPENCV3_DIR ${VCPKG_ROOT}installed/x64-windows/share/opencv/)
else()
    set( OPENCV3_DIR ${VCPKG_ROOT}installed/x64-linux/share/opencv/)
endif()
find_package(OpenCV CONFIG REQUIRED)
if( OpenCV_FOUND )
    # Additional Include Directories
    include_directories( ${OpenCV_INCLUDE_DIRS} )

    #    # Additional Library Directories
    link_directories( ${OpenCV_LIBRARY_DIRS} )
endif()


if(ANDROID)
    add_library(AutoAnnotationTool SHARED
        main.cpp
        qml.qrc
        )
else()
    add_executable(AutoAnnotationTool
        CV/cvmodule.cpp
        CV/cvparam.cpp
        annotationmanager.cpp
        geometrymodule.cpp
        labelcollector.cpp
        labeldatamodel.cpp
        opencvtypeconverter.cpp
        main.cpp
        qml.qrc
        )
endif()


target_compile_definitions(AutoAnnotationTool
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(AutoAnnotationTool
    PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Concurrent Qt5::QuickControls2
    opencv_world)
