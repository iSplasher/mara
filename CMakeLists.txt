cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(VCPKG_FEATURE_FLAGS "versions")

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

# If using clang-cl, find and include system libraries
if(WIN32 AND CMAKE_C_COMPILER MATCHES "clang-cl")
    find_package(PythonInterp)

    # use helper script to get environment variables for clang-cl
    set(VsDevCmdArgs "")

    if(VCPKG_TARGET_TRIPLET MATCHES "x64")
        set(VsDevCmdArgs "x64")
    endif()

    execute_process(COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/VsDevCmd.py" "${VsDevCmdArgs}" OUTPUT_VARIABLE VS_DEV_CMD)

    # see https://renenyffenegger.ch/notes/Windows/development/Visual-Studio/environment-variables/index
    # parse and set environment variables from script
    foreach(X IN LISTS VS_DEV_CMD)
        string(REPLACE "|" ";" X "${X}")
        list(GET X 0 X_KEY)
        list(GET X 1 X_VALUE)

        set("ENV{${X_KEY}}" ${X_VALUE})

        # message(STATUS "${X_KEY}: ${X_VALUE}")
        if(X_KEY MATCHES "^INCLUDE$")
            set(CMAKE_INCLUDE_PATH ${X_VALUE})
        endif()

        if(X_KEY MATCHES "^LIB$")
            set(CMAKE_LIBRARY_PATH ${X_VALUE})
        endif()
    endforeach()

    list(INSERT CMAKE_INCLUDE_PATH 0
        "$ENV{UniversalCRTSdkDir}\\Include\\$ENV{UCRTVersion}\\um"
        "$ENV{UniversalCRTSdkDir}\\Include\\$ENV{UCRTVersion}\\ucrt"
        "$ENV{UniversalCRTSdkDir}\\Include\\$ENV{UCRTVersion}\\winrt"
        "$ENV{UniversalCRTSdkDir}\\Include\\$ENV{UCRTVersion}\\shared"
        "$ENV{UniversalCRTSdkDir}\\Include\\$ENV{UCRTVersion}\\cppwinrt"
    )
    list(INSERT CMAKE_LIBRARY_PATH 0
        "$ENV{UniversalCRTSdkDir}\\Lib\\$ENV{UCRTVersion}\\ucrt\\$ENV{VSCMD_ARG_TGT_ARCH}"
        "$ENV{UniversalCRTSdkDir}\\Lib\\$ENV{UCRTVersion}\\um\\$ENV{VSCMD_ARG_TGT_ARCH}"
    )

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -LIBPATH:\"$ENV{UniversalCRTSdkDir}\\Lib\\$ENV{UCRTVersion}\\um\\$ENV{VSCMD_ARG_TGT_ARCH}\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -LIBPATH:\"$ENV{UniversalCRTSdkDir}\\Lib\\$ENV{UCRTVersion}\\ucrt\\$ENV{VSCMD_ARG_TGT_ARCH}\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -LIBPATH:\"$ENV{VCToolsInstallDir}\\Lib\\$ENV{VSCMD_ARG_TGT_ARCH}\"")

    message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
endif()

message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")

# print out all CMake variables
# dump_cmake_variables(".*")
project(rara VERSION 0.1.0)

# —————————————————————————
# Collect sources
# —————————————————————————
# —————————————————————————
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp)

# remove main.cpp from list of sources so we can reuse the list
list(FILTER sources EXCLUDE REGEX ".*/main\\.cpp$")

# —————————————————————————
# create targets
# —————————————————————————
set(TARGET_TEST "tests")
set(TARGET_LIB "raralib")
set(TARGET "rara")

# add own library to share among executables
add_library(${TARGET_LIB} ${sources})

add_executable(${TARGET} ${PROJECT_SOURCE_DIR}/src/main.cpp)

option(BUILD_TESTS "Enable building tests" ON)

if(BUILD_TESTS)
    file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.h)

    # These tests can use the Catch2-provided main
    add_executable(${TARGET_TEST} ${test_sources})
endif()

option(BUILD_DOCS "Enable building docs" ON)

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Use C++20
target_compile_features(${TARGET_LIB} PUBLIC cxx_std_20)
target_compile_features(${TARGET} PUBLIC cxx_std_20)
target_compile_features(${TARGET_TEST} PUBLIC cxx_std_20)
set_target_properties(${TARGET_LIB} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${TARGET_TEST} PROPERTIES CXX_EXTENSIONS OFF)

# —————————————————————————
# find packages
# —————————————————————————

# all shared includes from packages
set(SHARED_INCLUDE_DIRS "")

find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})
list(APPEND SHARED_INCLUDE_DIRS "${LLVM_INCLUDE_DIRS}")

find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/phmap_utils.h")
list(APPEND SHARED_INCLUDE_DIRS "${PARALLEL_HASHMAP_INCLUDE_DIRS}")

if(BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
endif()

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs Support Core IRReader Target Executionengine Analysis Orcjit x86targetmca x86codegen x86asmparser)

# add include directories to the executables, the library doesn't need them
target_include_directories(${TARGET} PRIVATE ${SHARED_INCLUDE_DIRS})
target_include_directories(${TARGET_TEST} PRIVATE ${SHARED_INCLUDE_DIRS})

# —————————————————————————
# link libraries
# —————————————————————————

# all shared libraries from packages
set(SHARED_LIBRARIES "")

# Link against LLVM libraries
list(APPEND SHARED_LIBRARIES ${llvm_libs})

target_link_libraries(${TARGET_LIB} PRIVATE ${SHARED_LIBRARIES})

if(BUILD_TESTS)
    # These tests can use the Catch2-provided main
    target_link_libraries(${TARGET_TEST} PRIVATE Catch2::Catch2WithMain)

    # link to own library
    target_link_libraries(${TARGET_TEST} PRIVATE ${TARGET_LIB})
endif()

# link to own library
target_link_libraries(${TARGET} PRIVATE ${TARGET_LIB})

# Increase stack size for llvm
# string(REPLACE "/STACK:10000000" "/STACK:1000000000" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
