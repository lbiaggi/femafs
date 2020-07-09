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
