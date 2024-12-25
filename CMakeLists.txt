# CMakeList.txt: projekt CMake dla elementu warframe_tool, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

# Włącz przeładowywanie kodu XAML na gorąco dla kompilatorów MSVC, jeśli jest to obsługiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Platinum_Eyes")

set(CMAKE_CXX_STANDARD 17)

set(SERVER_SCRIPT ${CMAKE_SOURCE_DIR}/ocrserver.py)
set(SERVER_SCRIPT_DESTINATION ${CMAKE_BINARY_DIR}/ocrserver.py)
set(TESSERACT_TRAINING_ENG ${CMAKE_SOURCE_DIR}/eng.traineddata)
set(TESSERACT_TRAINING_ENG_DESTINATION ${CMAKE_BINARY_DIR}/eng.traineddata)


set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")


find_package(Tesseract CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

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
)



add_executable (
    ${PROJECT_NAME} 
        "main.cpp" 
        "platinumEyes.h" 
        "utilities/functions.cpp" 
        "utilities/trim.cpp" 
        "gui/guiUtilities.cpp" 
        "keybindings/keyBindings.cpp" 
        "config/configUtilities.cpp" 
        "ocr/readingItems.cpp" 
        "gui/settingsMenu.cpp"
        "ocr/tesseract.cpp"
        "ocr/easyocr.cpp" 
        "ocr/ocrfunctions.cpp" 
        "gui/settingsChanges.cpp" 
        "gui/settingsChanged.h" 
        "relics/relicDatabase.cpp" 
         "ocr/ocr.h" "config/config.h" "utilities/utilities.h" "gui/gui.h" "keybindings/keybindings.h" "relics/relics.h")


target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})


add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SERVER_SCRIPT} ${SERVER_SCRIPT_DESTINATION}
    COMMENT "Copying server.py to the output directory"
)


add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TESSERACT_TRAINING_ENG} ${TESSERACT_TRAINING_ENG_DESTINATION}
    COMMENT "Copying training data to the output directory"
)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
