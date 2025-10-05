cmake_minimum_required(VERSION 3.27.7)
project(project)

#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#配置最终的可执行文件输出的路劲
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#配置头文件的搜索路劲
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/inlcude/server/redis)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
#link_directories(/usr/lib64/mysql)


# --------------------------
# 关键：手动指定MySQL的路径
# --------------------------
# 1. MySQL头文件路径（mysql.h所在目录）
set(MYSQL_INCLUDE_DIR "/usr/local/mysql/include")

# 2. MySQL库文件路径（libmysqlclient.so所在目录）
set(MYSQL_LIB_DIR "/usr/local/mysql/lib")

# --------------------------
# 配置编译和链接
# --------------------------

# 添加头文件路径
# 添加库文件路径
link_directories(${MYSQL_LIB_DIR})

#数据库的头文件搜索路劲
include_directories(${MYSQL_INCLUDE_DIR})

include_directories(/usr/local/mysql/include)

#加载子目录
add_subdirectory(src)