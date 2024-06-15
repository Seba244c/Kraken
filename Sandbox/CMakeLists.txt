add_executable(Sandbox src/Sandbox.cpp "src/Sandbox2D.h" "src/Sandbox2D.cpp"  )
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

# Add Kraken
target_link_libraries(Sandbox LINK_PRIVATE Kraken)

# Copy Assets
add_custom_command(
        TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets)