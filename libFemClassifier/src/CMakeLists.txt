include_directories(${PROJECT_SOURCE_DIR}/libFEMClassifier/include)

add_library(
    FEMClassifier SHARED
    FEMDataset.c
    FEMSample.c
    FEMUtils.c
    FEMimages.c
    FEMClassify.c
    FEMBasis.c
    )
