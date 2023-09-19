cmake_minimum_required(VERSION 3.14)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIR})

include_directories(../include)
add_executable(rbtree.check rbtree_test.c ../src/rbtree.c)
target_link_libraries(rbtree.check ${CHECK_LIBRARIES} pthread)

include_directories(../include)
add_executable(rbiter.check rbiter_test.c ../src/rbiter.c ../src/rbtree.c)
target_link_libraries(rbiter.check ${CHECK_LIBRARIES} pthread)
