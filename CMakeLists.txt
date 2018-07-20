cmake_minimum_required(VERSION 3.9)
project(Code-Tempates)

# 添加c++ 11标准支持
set( CMAKE_CXX_FLAGS "-std=gnu++14" )

# 添加cmake_modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

# 添加库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 添加可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(Boost_USE_MULTITHREADED ON) #开启boost多线程支持


###################dependencies#######################
find_package(folly CONFIG)
if (NOT folly_FOUND)
    # Look for folly using our FindFolly.cmake module.  This may work if the
    # folly was built with its older autoconf build files rather than with CMake.
    find_package(Folly MODULE REQUIRED)
endif()

message(folly-include: ${FOLLY_INCLUDE_DIR})
message(folly-library: ${FOLLY_LIBRARIES})

# 寻找OpenCV库
#find_package( OpenCV 3.3.0 REQUIRED ) #OpenCV
#find_package(Boost COMPONENTS system filesystem thread)
#find_package(MathGL2 REQUIRED)  #MathGL2
#find_package(freenect2 REQUIRED)
#find_package(folly)

#message(folly-include: ${FOLLY_INCLUDES})
#message(folly-library: ${FOLLY_LIBRARIES})
#message(Boost_version: ${Boost_VERSION})
#message(Boost_Lib: ${Boost_LIBRARIES})
#message(OpenCV: ${OpenCV_VERSION})
#message(MathGL2:${MATHGL2_INCLUDE_DIRS})
#message(MathGL2:${MATHGL2_LIBRARIES})
#message(freenect2 include: ${freenect2_INCLUDE_DIR})
#message(freenect2 library: ${freenect2_LIBRARIES})


# 添加头文件
include_directories(
        ${FOLLY_INCLUDE_DIR}
        ${BOOST_INCLUDE_DIR}
        #${OpenCV_INCLUDE_DIRS}
        #${MATHGL2_INCLUDE_DIRS}
        #${freenect2_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/src/C++数据库连接池/
        ${PROJECT_SOURCE_DIR}/include
        #/usr/include/python2.7
)

include_directories(${PROJECT_SOURCE_DIR}/src/Boost线程/threadpool-0_2_5-src/threadpool)

# 添加依赖库
set(THIRD_PARTY_LIBS
        #${Boost_LIBRARIES}
        pthread                 #(-lpthread) Boost::threadpool的依赖库
        #${OpenCV_LIBS}
        #${freenect2_LIBRARIES}
        )

#######################SubCmakeList#############################
add_subdirectory(src)
