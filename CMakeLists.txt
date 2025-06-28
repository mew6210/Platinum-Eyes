
cmake_minimum_required (VERSION 3.8)

project ("Platinum_Eyes")

set(CMAKE_CXX_STANDARD 17)

set(TESSERACT_TRAINING_ENG ${CMAKE_SOURCE_DIR}/eng_fast.traineddata)
set(TESSERACT_TRAINING_ENG_DESTINATION ${CMAKE_BINARY_DIR}/eng_fast.traineddata)

set(FONT ${CMAKE_SOURCE_DIR}/assets/fonts/Lexend-Regular.ttf)
set(FONT_DESTINATION ${CMAKE_BINARY_DIR}/assets/fonts/Lexend-Regular.ttf)


set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")


find_package(Tesseract CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

find_package(OpenCV REQUIRED)
find_package(liblzma CONFIG REQUIRED)
find_path(TERMCOLOR_INCLUE_DIRS "termcolor/termcolor.hpp")


set(LIBRARIES 
cpr::cpr
sfml-system 
sfml-network 
sfml-graphics 
sfml-window
imgui::imgui
ImGui-SFML::ImGui-SFML
Dwmapi.lib
Tesseract::libtesseract

${OpenCV_LIBS}
liblzma::liblzma
)



add_executable (
    ${PROJECT_NAME} 
        "src/main.cpp" 
        "src/core/core.h" 
        "src/utilities/functions.cpp" 
        "src/utilities/trim.cpp" 
        "src/gui/guiUtilities.cpp" 
        "src/keybindings/keyBindings.cpp" 
        "src/config/configUtilities.cpp" 
        "src/ocr/readingItems.cpp" 
        "src/gui/settingsMenu.cpp"
        "src/ocr/tesseract.cpp" 
        "src/ocr/ocrfunctions.cpp" 
        "src/gui/settingsChanges.cpp" 
        "src/relics/relicDatabase.cpp" 
        "src/ocr/ocr.h" 
        "src/config/config.h" 
        "src/utilities/utilities.h" 
        "src/gui/gui.h" 
        "src/keybindings/keybindings.h" 
        "src/relics/relics.h" "src/core/core.cpp" "src/wfmd/wfmd.cpp")


target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${TERMOCOLOR_INCLUDE_DIRS})



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

