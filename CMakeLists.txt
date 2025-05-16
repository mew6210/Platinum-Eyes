
cmake_minimum_required (VERSION 3.8)

project ("Platinum_Eyes")

set(CMAKE_CXX_STANDARD 17)

set(TESSERACT_TRAINING_ENG ${CMAKE_SOURCE_DIR}/eng_fast.traineddata)
set(TESSERACT_TRAINING_ENG_DESTINATION ${CMAKE_BINARY_DIR}/eng_fast.traineddata)

set(FONT ${CMAKE_SOURCE_DIR}/fonts/Lexend-Regular.ttf)
set(FONT_DESTINATION ${CMAKE_BINARY_DIR}/fonts/Lexend-Regular.ttf)


set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")


find_package(Tesseract CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(liblzma CONFIG REQUIRED)


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
lodepng
${OpenCV_LIBS}
liblzma::liblzma
)



add_executable (
    ${PROJECT_NAME} 
        "main.cpp" 
        "platinumEyes.h" 
        "modules/utilities/functions.cpp" 
        "modules/utilities/trim.cpp" 
        "modules/gui/guiUtilities.cpp" 
        "modules/keybindings/keyBindings.cpp" 
        "modules/config/configUtilities.cpp" 
        "modules/ocr/readingItems.cpp" 
        "modules/gui/settingsMenu.cpp"
        "modules/ocr/tesseract.cpp"
         
        "modules/ocr/ocrfunctions.cpp" 
        "modules/gui/settingsChanges.cpp" 
        "modules/gui/settingsChanged.h" 
        "modules/relics/relicDatabase.cpp" 
        "modules/ocr/ocr.h" 
        "modules/config/config.h" 
        "modules/utilities/utilities.h" 
        "modules/gui/gui.h" 
        "modules/keybindings/keybindings.h" 
        "modules/relics/relics.h")


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

