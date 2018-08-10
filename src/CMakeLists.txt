#######################SubCmakeList#############################
add_subdirectory(STL-Algorithm)

add_executable(String_Split 字符串分割/String_Split.cpp)
target_link_libraries(String_Split
        ${THIRD_PARTY_LIBS}
        )


add_executable(ReadFile 按行格式化读取文件数据/ReadFile.cpp)
target_link_libraries(ReadFile
        ${THIRD_PARTY_LIBS}
        )

add_executable(FindFile 遍历目录下的所有文件/FindFile.cpp )
target_link_libraries(FindFile
        ${THIRD_PARTY_LIBS}
        )

add_executable(ThreadPoolDemo Boost线程/ThreadPoolDemo.cpp)
target_link_libraries(ThreadPoolDemo
        ${THIRD_PARTY_LIBS}
        )

add_executable(testdemo Boost线程/test.cpp)
target_link_libraries(testdemo
        ${THIRD_PARTY_LIBS}
        )

add_executable(Thread_with_ref 线程执行函数体使用引用传递/Thread_with_ref.cpp)
target_link_libraries(Thread_with_ref
        pthread
        )

add_executable(iomapip_demo 小数格式控制符/mapip.cpp)

#add_executable(OpenCV_yaml_demo OpenCV操作yaml参数文件/opencv_yaml.cpp)
#target_link_libraries(OpenCV_yaml_demo
#        ${THIRD_PARTY_LIBS}
#        )


#add_executable(gnuplot 绘图/gnuplot.cpp) #failed!!!

#add_executable(matplotlib 绘图/matplotlibcpp.h 绘图/matplotlib_in_C++.cpp)
#target_link_libraries(matplotlib python2.7)

###################C++ STL原理####################
add_executable(iterator STL迭代器原理/itertor.cpp)
target_link_libraries(iterator
        ${Boost_LIBRARIES}
        )

####################排序算法#########################
add_executable(quicksort 排序算法/quicksort.cpp) #快速排序算法
add_executable(bubblesort 排序算法/bubblesort.cpp) #冒泡排序
add_executable(selectsort 排序算法/selectsort.cpp) #选择排序
add_executable(heapsort 排序算法/HeapSort.cpp) #堆排序
add_executable(InsertSort 排序算法/InsertSort.cpp) #堆排序


##################环形缓冲区#######################
add_executable(loop_buf 环形缓冲区/Loop_Buffer.cpp)

#################多路IO复用模型#####################
add_executable(socket_epoll 多路IO复用模型/socket_epoll.cpp)
add_executable(Semaphore C++11信号量实现/Semaphore.cpp)
add_executable(Basic_Semaphore C++11信号量实现/Basic_Semaphore.cpp)
target_link_libraries(Basic_Semaphore pthread)

add_executable(ProductorConsumer_sem C++11信号量实现/ProductorConsumer_sem.cpp)
target_link_libraries(ProductorConsumer_sem pthread)

###################数据库连接池#############################
add_executable(mysql_connect_pool C++数据库连接池/mysql_connect.cpp)
target_link_libraries(mysql_connect_pool ${PROJECT_SOURCE_DIR}/lib64/libmysqlcppconn.so)

add_executable(PriorityQueue C++优先级队列/PriorityQueue.cpp)
add_executable(CPP11PriorityQueue C++优先级队列/CPP11Priority_Queue.cpp)

##################实现简单的智能指针############################
add_executable(Smart_ptr 实现智能指针/smart_ptr.cpp)

##################利用无锁队列和Future实现线程池#################
add_executable(CPUThreadPool 线程池原理/ThreadPool.cpp)
target_link_libraries(CPUThreadPool pthread)

add_executable(CPUThreadPool_NoBlock 线程池原理/CPUThreadPool_NoBlock.cpp)
target_link_libraries(CPUThreadPool_NoBlock ${FOLLY_LIBRARIES})


##################实现tail功能############################
add_executable(tail 实现tail的功能/tail.cpp)

add_executable(lower_bound lower_bound&upper_bound原理及应用/lower_bound.cpp)

add_executable(Operator_new new的内部原理/Operator_new.cpp)

add_executable(SephoneExample 两个线程交替打印奇偶数/SephoneExample.cpp)
target_link_libraries(SephoneExample pthread)

add_executable(CircularQueue_list 循环队列/CircularQueue_list.cpp)
add_executable(CircularQueue_array 循环队列/CircularQueue_array.cpp)

add_executable(Ucontext 协程_posix_ucontext用户级线程实现原理/Ucontext.cpp)

add_executable(LRU 实现LRU/LRU.cpp)
add_executable(Unique_ptr 实现unique_ptr/unique_ptr.cpp)
add_executable(Move_return move右值引用与返回值优化/Move_return.cpp)
add_executable(Enable_shared_from_this enable_shared_from_this/Enable_shared_from_this.cpp)
add_executable(Static_Dynamic_cast static_cast&dynamic_cast/Static_Dynamic_cast.cpp)
add_executable(memcpy_memmove memcpy_memmove/memcpy_memmove.cpp)

add_executable(TrieExample 字典树及其应用/Trie.cpp)