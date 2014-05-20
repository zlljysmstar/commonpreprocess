cmake_minimum_required(VERSION 2.6)
PROJECT(process_unify)
INCLUDE_DIRECTORIES(/home/zhuliang/common_libs/lib2-64/ullib/include/ 
		/home/zhuliang/common_libs/lib2-64/ccode/include/
        /home/zhuliang/common_libs/third-64/boost/include/)
LINK_DIRECTORIES(/home/zhuliang/common_libs/lib2-64/ullib/lib/ 
		/home/zhuliang/common_libs/lib2-64/ccode/lib/ 
        /home/zhuliang/common_libs/third-64/boost/lib/)
SET(SRC_LIST common_core.cpp run.cpp)
SET(CMAKE_BUILD_TYPE Release)
ADD_EXECUTABLE(commonpreprocess ${SRC_LIST})
TARGET_LINK_LIBRARIES(commonpreprocess libboost_filesystem.a 
    libboost_system.a 
    libulccode.a 
    libullib.a 
    pthread crypto rt)
