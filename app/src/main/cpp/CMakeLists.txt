# 设置最小使用版本
cmake_minimum_required(VERSION 3.18.1)

project("demo")

include_directories(
        include
        logUtil
        render
        glUtil
    )


file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/glUtil/*.cpp
        ${CMAKE_SOURCE_DIR}/render/*.cpp)

# 添加本地so库 native-lib：这个是声明引用so库的名称 SHARED：表示共享so库文件
# 构建so库的源文件
add_library(
        demo
        SHARED
        ${src-files}
)

set(SO_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

# 使用系统ndk 提供的库，如 log库
# log-lib 这个指定的是在NDK库中每个类型的库会存放一个特定的位置，而log库存放
# 在log-lib中
# log 指定使用log库
find_library(
        log-lib
        log
)

message("c_CMAKE_SOURCE_DIR:" ${CMAKE_SOURCE_DIR} )

# 加载avcodec-57库
add_library( avcodec
        SHARED
        IMPORTED)
set_target_properties( avcodec
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libavcodec.so)

add_library( avutil
        SHARED
        IMPORTED)
set_target_properties( avutil
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libavutil.so)

add_library( swresample
        SHARED
        IMPORTED)
set_target_properties( swresample
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libswresample.so)

add_library( avfilter
        SHARED
        IMPORTED)
set_target_properties( avfilter
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libavfilter.so)

add_library( avformat
        SHARED
        IMPORTED)
set_target_properties( avformat
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libavformat.so)

add_library( swscale
        SHARED
        IMPORTED)
set_target_properties( swscale
        PROPERTIES IMPORTED_LOCATION
        ${SO_DIR}/libswscale.so)
#----------------------end-----------------------

# 如果你本地的库（native-lib）想要调用log库的方法，
# 那么就需要配置这个属性，意思是把NDK库关联到本地库。
# 第一个参数表示本地的库 native-lib 要调用到log库的方法，即要被关联的库名称，log-lib 要关联的库名称
target_link_libraries(
        demo
        #ffmpeg------start----------
        avcodec
        avutil
        swresample
        avfilter
        avformat
        swscale

        GLESv3
        EGL
        android
        jnigraphics

        #ffmpeg------end------------
        ${log-lib}
)