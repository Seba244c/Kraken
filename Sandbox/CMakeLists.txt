add_executable(Sandbox src/Sandbox.cpp)

# Add Kraken
target_link_libraries(Sandbox LINK_PRIVATE Kraken)
