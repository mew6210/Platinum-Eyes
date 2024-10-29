# CMakeList.txt: projekt CMake dla elementu warframe_tool, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

# Włącz przeładowywanie kodu XAML na gorąco dla kompilatorów MSVC, jeśli jest to obsługiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("warframe_tool")

set(CMAKE_CXX_STANDARD 17)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 17)





# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (warframe_tool "warframe_tool.cpp" "warframe_tool.h" "functions.cpp")
target_link_libraries(warframe_tool PRIVATE cpr::cpr)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET warframe_tool PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
