cmake_minimum_required(VERSION 3.8)
project(rara VERSION 0.1.0)

add_executable(rara src/main.cpp)

option(BUILD_DOCS "Enable building docs" ON)
if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Use C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)


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


# —————————————————————————
# Collect sources
# —————————————————————————

file(GLOB_RECURSE sources CONFIGURE_DEPENDS  ./include/*.h ./src/*.cpp)

# —————————————————————————
# link libraries
# —————————————————————————

    # Link against LLVM libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs})
   

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
