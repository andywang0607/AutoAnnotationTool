cmake_minimum_required(VERSION 3.5)

project(AutoAnnotationTool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Core Quick Widgets Concurrent QuickControls2 REQUIRED)
add_subdirectory(thirdparty)

add_executable(AutoAnnotationTool
    QItem/AnnotationManager.cpp
    QItem/LabelCollector.cpp
    QItem/LableDataModel.cpp
    QItem/CvParam.cpp
    CV/CvModule.cpp
    CV/GeometryModule.cpp
    main.cpp
    qml.qrc
)

target_compile_definitions(AutoAnnotationTool
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(AutoAnnotationTool
    PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Concurrent Qt5::QuickControls2)
target_link_libraries(AutoAnnotationTool
    PUBLIC opencv_core opencv_imgproc opencv_highgui)

target_include_directories(AutoAnnotationTool
    PRIVATE 
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_highgui_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
    ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
    ${OPENCV_MODULE_opencv_videoio_LOCATION}/include
)
