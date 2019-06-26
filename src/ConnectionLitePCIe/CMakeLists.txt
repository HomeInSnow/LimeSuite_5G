########################################################################
## Support for ConnectionLitePCIe connection
########################################################################
set(THIS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionLitePCIe)

set(CONNECTION_LITE_PCIE_SOURCES
    ${THIS_SOURCE_DIR}/ConnectionLitePCIeEntry.cpp
    ${THIS_SOURCE_DIR}/ConnectionLitePCIe.cpp
)

########################################################################
## Feature registration
########################################################################
include(FeatureSummary)
include(CMakeDependentOption)
cmake_dependent_option(ENABLE_LITE_PCIE "Enable Lite PCIe" ON "ENABLE_LIBRARY" OFF)
add_feature_info(ConnectionLitePCIe ENABLE_LITE_PCIE "Lite PCIe Connection support")
if (NOT ENABLE_LITE_PCIE)
    return()
endif()

########################################################################
## Add to library
########################################################################
target_sources(LimeSuite PRIVATE ${CONNECTION_LITE_PCIE_SOURCES})
target_include_directories(LimeSuite PUBLIC ${THIS_SOURCE_DIR}/kernel/)

