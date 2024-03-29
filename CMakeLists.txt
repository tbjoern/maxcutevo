cmake_minimum_required (VERSION 2.8)
set(project maxcutevo)
cmake_policy(SET CMP0048 NEW)
project(${project})
set("${project}_VERSION" 0.1)

function(add_format_target target files)
    add_custom_target(${target}-format
        COMMAND clang-format -i ${files}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        VERBATIM)
    add_dependencies(${target} ${target}-format)
endfunction()

function(create_library project name sources includes libraries)
    set(files ${sources} ${includes})
    add_library(${name} ${files})
    target_compile_options(${name} PRIVATE ${DEFAULT_COMPILE_FLAGS})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(${name} PUBLIC ${libraries})
    add_format_target(${name} "${files}")
    install (TARGETS ${name} EXPORT ${project}Targets DESTINATION lib)
    install (DIRECTORY ${CMAKE_CURRENT_LIST_DIR} DESTINATION include/${project} FILES_MATCHING REGEX ".*\\.h(pp)?")
endfunction()

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}
    src/cpp)

function(create_executable project name sources includes libraries)
    set(files ${sources} ${includes})
    add_executable(${name} ${files})
    target_compile_options(${name} PRIVATE ${DEFAULT_COMPILE_FLAGS})
    target_link_libraries(${name} PUBLIC ${libraries})
    add_format_target(${name} "${files}")
    install (TARGETS ${name} EXPORT ${project}Targets DESTINATION bin)
endfunction()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 14)

# Multithreading 
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wfloat-equal -Wold-style-cast -Wno-sign-compare -lstdc++fs")
if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wl,--gc-sections -Wl,--as-needed -ffunction-sections -fdata-sections -flto")
endif()
if (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

add_subdirectory(src)
