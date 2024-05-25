add_executable(Sandbox src/Sandbox.cpp)

# Add Kraken
target_link_libraries(Sandbox LINK_PRIVATE Kraken)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)