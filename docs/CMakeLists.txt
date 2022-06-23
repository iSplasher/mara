find_package(Doxygen REQUIRED)

# Find all the public headers
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DOXYGEN_INDEX_DIR ${DOXYGEN_OUTPUT_DIR}/xml)

#This will be the main output of our command
set(DOXYGEN_INDEX_FILE ${DOXYGEN_INDEX_DIR}/index.xml)

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

#Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us

add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${HEADER_FILES}
                   BYPRODUCTS ${DOXYGEN_INDEX_DIR}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating doxygen docs"
                   VERBATIM)

add_custom_target(doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# find Sphinx and breathe-apidoc

include(FindPackageHandleStandardArgs)

# We are likely to find Sphinx near the Python interpreter
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
    get_filename_component(_PYTHON_DIR "${PYTHON_EXECUTABLE}" DIRECTORY)
    set(
        _PYTHON_PATHS
        "${_PYTHON_DIR}"
        "${_PYTHON_DIR}/bin"
        "${_PYTHON_DIR}/Scripts")
endif()

set(
    _PYTHON_ENV_PATHS
    "${PROJECT_SOURCE_DIR}/env"
    "${PROJECT_SOURCE_DIR}/env/bin"
    "${PROJECT_SOURCE_DIR}/env/bin/Scripts"
    "${PROJECT_SOURCE_DIR}/env/Scripts"
    "${PROJECT_SOURCE_DIR}/venv"
    "${PROJECT_SOURCE_DIR}/venv/bin"
    "${PROJECT_SOURCE_DIR}/venv/bin/Scripts"
    "${PROJECT_SOURCE_DIR}/venv/Scripts"
    )

find_program(
    SPHINX_EXECUTABLE
    NAMES sphinx-build sphinx-build.exe
    HINTS ${_PYTHON_PATHS} ${_PYTHON_ENV_PATHS} REQUIRED)
mark_as_advanced(SPHINX_EXECUTABLE)

find_package_handle_standard_args(Sphinx DEFAULT_MSG SPHINX_EXECUTABLE)

find_program(
    BREATHE_APIDOC_EXECUTABLE
    NAMES breathe-apidoc breathe-apidoc.exe
    HINTS ${_PYTHON_PATHS} ${_PYTHON_ENV_PATHS} REQUIRED)
mark_as_advanced(BREATHE_APIDOC_EXECUTABLE)

find_package_handle_standard_args(Sphinx DEFAULT_MSG BREATHE_APIDOC_EXECUTABLE)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BREATHE_DOC_OUTPUT ${SPHINX_SOURCE}/include)
set(SPHINX_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/_build)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
set(SPHINX_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_CONF ${CMAKE_CURRENT_SOURCE_DIR}/conf.py)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                  DEPENDS ${SPHINX_SOURCE}
                  BYPRODUCTS ${SPHINX_BUILD}
                  COMMAND
                  ${BREATHE_APIDOC_EXECUTABLE} -f -o ${BREATHE_DOC_OUTPUT} ${DOXYGEN_INDEX_DIR}  &&
                  ${SPHINX_EXECUTABLE} -b html -c ${SPHINX_CONF_DIR}
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  DEPFILE
                  MAIN_DEPENDENCY ${DOXYGEN_INDEX_FILE} ${SPHINX_CONF}
                  COMMENT "Generating sphinx docs"
                  VERBATIM)

add_custom_target(docs ALL DEPENDS doxygen ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(
    DIRECTORY ${SPHINX_BUILD}
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )