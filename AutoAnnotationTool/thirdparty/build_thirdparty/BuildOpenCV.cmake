include(FetchContent)
set(_EXTERNAL_PROJECT_NAME "OpenCV")

FetchContent_Declare(
    ${_EXTERNAL_PROJECT_NAME}
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../opencv
    )

# Check if population has already been performed
FetchContent_GetProperties(${_EXTERNAL_PROJECT_NAME})
string(TOLOWER "${_EXTERNAL_PROJECT_NAME}" _EXTERNAL_PROJECT_LOWER_CASE_NAME)
if(NOT ${_EXTERNAL_PROJECT_LOWER_CASE_NAME}_POPULATED)
    message("== fetch ${_EXTERNAL_PROJECT_NAME} in project ${PROJECT_NAME} ==")

    # Fetch the content using previously declared details
    FetchContent_Populate(${_EXTERNAL_PROJECT_NAME})

    # Set custom variables, policies, etc.
    set(BUILD_TESTS OFF)
    set(BUILD_PERF_TESTS OFF)
    set(BUILD_EXAMPLES OFF)
    set(BUILD_DOCS OFF)
    set(BUILD_opencv_apps OFF)
    set(BUILD_opencv_video OFF)
    set(BUILD_opencv_videoio OFF)
    set(BUILD_opencv_java_bindings_generator OFF)
    set(BUILD_opencv_python_bindings_generator OFF)
    set(BUILD_opencv_dnn OFF)
    set(BUILD_opencv_features2d OFF)
    set(BUILD_opencv_flann OFF)
    set(BUILD_opencv_ml OFF)

    set(BUILD_LIST core,highgui,imgproc)

    add_subdirectory(${${_EXTERNAL_PROJECT_LOWER_CASE_NAME}_SOURCE_DIR} ${${_EXTERNAL_PROJECT_LOWER_CASE_NAME}_BINARY_DIR})
endif()
