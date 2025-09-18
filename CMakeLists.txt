cmake_minimum_required (VERSION 3.8)

project ("Platinum_Eyes")

set(CMAKE_CXX_STANDARD 17)

set(TESSERACT_TRAINING_ENG ${CMAKE_SOURCE_DIR}/eng_fast.traineddata)
set(TESSERACT_TRAINING_ENG_DESTINATION ${CMAKE_BINARY_DIR}/eng_fast.traineddata)

set(FONT ${CMAKE_SOURCE_DIR}/assets/fonts/Lexend-Regular.ttf)
set(FONT_DESTINATION ${CMAKE_BINARY_DIR}/assets/fonts/Lexend-Regular.ttf)


set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")


find_package(Tesseract CONFIG REQUIRED)

if (UNIX AND TARGET Tesseract::libtesseract)
    target_link_libraries(Tesseract::libtesseract INTERFACE
        zstd
        lz4::lz4
        liblzma::liblzma
    )
endif()

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS Window Graphics CONFIG REQUIRED)
find_package(efsw CONFIG REQUIRED)



set(IMGUI_VERSION 1.91.1)
include(FetchContent)
FetchContent_Declare(
  imgui
  URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)
FetchContent_MakeAvailable(imgui)


FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG        v3.0
 
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(imgui-sfml)


FetchContent_Declare(
    wood
    GIT_REPOSITORY https://github.com/mew6210/wood.git
    GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(wood)


find_package(OpenCV REQUIRED)
find_package(liblzma CONFIG REQUIRED)
find_package(lz4 REQUIRED)
find_package(zstd REQUIRED)

set(LIBRARIES 
cpr::cpr
SFML::Graphics 
SFML::Window
ImGui-SFML::ImGui-SFML
Tesseract::libtesseract
${OpenCV_LIBS}
wood
liblzma::liblzma
efsw::efsw
)

if(WIN32)
    list(APPEND LIBRARIES Dwmapi.lib)
endif()


add_executable (
    ${PROJECT_NAME} 
        "src/main.cpp" 
        "src/core/core.h" 
        "src/utilities/functions.cpp" 
         
        "src/gui/guiUtilities.cpp" 
        "src/keybindings/keyBindings.cpp" 
        "src/config/configUtilities.cpp" 
        "src/ocr/readingItems.cpp" 
        "src/gui/settingsMenu.cpp"
        "src/ocr/tesseract.cpp" 
        "src/gui/settingsChanges.cpp" 
        "src/relics/relicDatabase.cpp" 
        "src/ocr/ocr.h" 
        "src/config/config.h" 
        "src/utilities/utilities.h" 
        "src/gui/gui.h"
        "src/keybindings/keybindings.h" 
        "src/relics/relics.h" 
        "src/core/core.cpp" 
        "src/wfmd/wfmd.cpp"
        "src/ocr/native/nativeocr.cpp" 
        "src/keybindings/native/keybindingsnative.cpp" 
        "src/gui/native/guinative.cpp" 
        "src/clipboard/clipboard.cpp" 
        "src/clipboard/native/clipboardnative.cpp" 
        "src/core/native/nativecore.cpp"
        "src/eelogwatcher/eelogwatcher.cpp" 
        "src/config/native/nativeconfig.cpp" 
        "src/config/toolconfig.cpp"
        "src/utilities/timer/timer.cpp" 
        "src/utilities/trim/trim.cpp"  
        "src/relics/relicinfo/relicinfo.cpp" 
        "src/items/itemcache/itemcache.cpp"
)


target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})




add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TESSERACT_TRAINING_ENG} ${TESSERACT_TRAINING_ENG_DESTINATION}
    COMMENT "Copying training data to the output directory"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${FONT} ${FONT_DESTINATION}
    COMMENT "Copying font to the font directory"
)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

