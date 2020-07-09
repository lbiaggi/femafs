# Include python

find_package(PythonLibs REQUIRED)

swig_add_library(
    pyFEMClassifier
LANGUAGE
    python
SOURCES
    ../libFEMClassifier.i
)

# linguagem usada pra a geracao de codigo
set_property(SOURCE ../libFEMClassifier.i PROPERTY C ON)

# Nome do modulo a ser exportado pelo swig
set_property(SOURCE ../libFEMClassifier.i PROPERTY SWIG_MODULE_NAME pyFEMClassifier)

# Nao sei direito o porque, mas essa propriedade precisou estar setada pra o projeto compilar direito.
set_property(TARGET pyFEMClassifier PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

# Add swig module
target_link_libraries(
    pyFEMClassifier
    ${PYTHON_LIBRARIES}
)

target_include_directories(pyFEMClassifier
PRIVATE
    ../include
    ${PROJECT_SOURCE_DIR}/libFemClassifier/src
    ${PYTHON_INCLUDE_DIRS}
)


set(PYTHON_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libFEMClassifier.py
        ${CMAKE_CURRENT_BINARY_DIR}/libFEMClassifier.so)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Declare install target for python
#install(TARGETS swig_example
#        COMMAND "${PYTHON_EXECUTABLE} setup.py"
#        COMPONENT swig-python)

# Install target to call setup.py
add_custom_target(install-python
        DEPENDS libFEMClassifier
        COMMAND python ${SETUP_PY_OUT} install)