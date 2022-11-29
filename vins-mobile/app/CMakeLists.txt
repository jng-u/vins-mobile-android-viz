# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GL3STUB_SRC gl3stub.c)
    set(OPENGL_LIB GLESv2)
else ()
    set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

set(LIB_NAME NativeLib)
set(
        ${LIB_NAME}_SRCS
        src/main/cpp/jni/nativelib.cpp
)

add_subdirectory(libs/VINS-Mobile-master)

add_subdirectory(src/main/cpp/VINS_Android)
#add_subdirectory(src/main/cpp/visualize)


# Gradle automatically packages shared libraries with your APK.
add_library( 
    ${LIB_NAME}
    SHARED
    ${${LIB_NAME}_SRCS}
    
   src/main/cpp/visualize/application.cpp
   src/main/cpp/visualize/utils.cpp
   src/main/cpp/visualize/vertex.cpp
)

# add_library( # Sets the name of the library.
#     VisualizeLib

#     # Sets the library as a shared library.
#     SHARED

#     # Provides a relative path to your source file(s).
#     src/main/cpp/visualize/native-lib.cpp
#     src/main/cpp/visualize/application.cpp
#     src/main/cpp/visualize/utils.cpp
#     src/main/cpp/visualize/vertex.cpp
# )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( 
    # Sets the name of the path variable.
    log-lib
    
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    log 
)

#add_dependencies(nativelib VINS_Android)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( 
    # Specifies the target library.
    NativeLib
    
    # VINSMobile
    VINS_Android
    
    ${OPENGL_LIB}
    android
    EGL
    
    log
    # Links the target library to the log library
    # included in the NDK.
    ${log-lib}
)

# target_link_libraries( # Specifies the target library.
#     VisualizeLib

#     ${OPENGL_LIB}
#     android
#     EGL
#     # Links the target library to the log library
#     # included in the NDK.
#     ${log-lib}
# )