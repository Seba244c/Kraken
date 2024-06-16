# Root Project Cmake
# Includes Library Kraken
# Includes Application Sandbox
cmake_minimum_required (VERSION 3.28)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# No minmax
add_definitions(-DNOMINMAX)

# Projects
project ("Kraken")
add_subdirectory(libs)
add_subdirectory(Sandbox)
add_subdirectory(Demos/TappyBird)
add_subdirectory(Kraken)