cmake_minimum_required(VERSION 3.8)

message(STATUS "WINDOWS KITS: ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

project(rara VERSION 0.1.0)

set(VCPKG_FEATURE_FLAGS "versions")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# —————————————————————————
# Collect sources
# —————————————————————————
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp)

# remove main.cpp from list of sources so we can reuse the list
list(FILTER sources EXCLUDE REGEX ".*/main\\.cpp$")

set(TARGET_LIB "raralib")
set(TARGET_TEST "tests")
set(TARGET "rara")

add_library(${TARGET_LIB} STATIC ${sources})

add_executable(${TARGET} ${PROJECT_SOURCE_DIR}/src/main.cpp)

option(BUILD_DOCS "Enable building docs" ON)
option(BUILD_TESTS "Enable building tests" ON)

# Use C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# function to print out CMake variables
function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list(SORT _variableNames)

    foreach(_variableName ${_variableNames})
        if(ARGV0)
            unset(MATCHED)

            # case sensitive match
            # string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            #
            # case insenstitive match
            string(TOLOWER "${ARGV0}" ARGV0_lower)
            string(TOLOWER "${_variableName}" _variableName_lower)
            string(REGEX MATCH ${ARGV0_lower} MATCHED ${_variableName_lower})

            if(NOT MATCHED)
                continue()
            endif()
        endif()

        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

# —————————————————————————
# find packages
# —————————————————————————
find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})

target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs Support Core IRReader Target Executionengine Analysis Orcjit x86targetmca x86codegen x86asmparser)

if(BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
endif()

# —————————————————————————
# create target for library
# —————————————————————————
if(BUILD_TESTS)
    file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.h)

    # These tests can use the Catch2-provided main
    add_executable(${TARGET_TEST} ${test_sources})
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# print out all CMake variables
# dump_cmake_variables("catch")

# —————————————————————————
# link libraries
# —————————————————————————

# Link own library
target_link_libraries(${TARGET} PRIVATE ${TARGET_LIB})

# Link against LLVM libraries
target_link_libraries(${TARGET_LIB} PRIVATE ${llvm_libs})

if(BUILD_TESTS)
    # Link own library
    target_link_libraries(${TARGET_TEST} PRIVATE ${TARGET_LIB})

    # These tests can use the Catch2-provided main
    target_link_libraries(${TARGET_TEST} PRIVATE Catch2::Catch2WithMain)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)