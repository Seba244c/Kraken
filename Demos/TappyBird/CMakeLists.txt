add_executable(TappyBird src/Tappy.cpp)

# Add Kraken
target_link_libraries(TappyBird LINK_PRIVATE Kraken)

# Copy Assets
add_custom_command(
        TARGET TappyBird POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets)