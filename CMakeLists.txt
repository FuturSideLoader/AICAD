cmake_minimum_required(VERSION 3.25)

project(AICAD
    VERSION 0.4.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(MPI REQUIRED COMPONENTS C)
find_package(FastFloat REQUIRED CONFIG)
find_package(VTK REQUIRED COMPONENTS
    CommonCore
    RenderingCore
    RenderingFreeType
    FiltersGeneral
    InteractionStyle
    RenderingOpenGL2
    RenderingGL2PSOpenGL2
    IOImage
    ImagingCore
)
find_package(OpenCASCADE REQUIRED)

add_executable(aicad
    src/main.cpp
    
    src/MainWindow.hpp
    src/MainWindow.cpp
    
    src/OccView.hpp
    src/OccView.cpp
    
    src/cad/AiMarker.hpp
    src/cad/AiMarker.cpp

    src/cad/CadDocument.hpp
    src/cad/CadDocument.cpp

    src/cad/CadObject.hpp
    src/cad/CadObject.cpp
)

target_include_directories(aicad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCASCADE_INCLUDE_DIR}
)

target_link_libraries(aicad PRIVATE
    Qt6::Widgets
    ${OpenCASCADE_LIBRARIES}
)

target_compile_options(aicad PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
