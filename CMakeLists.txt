cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp src/tile.cpp src/worldMap.cpp src/task.cpp src/person.cpp src/resource.cpp src/util.cpp src/gui.cpp src/battle.cpp src/scenes.cpp src/components.h)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)


set(FONTS
    "${CMAKE_SOURCE_DIR}/Assets/Fonts/Arial.ttf"
    "${CMAKE_SOURCE_DIR}/Assets/Fonts/BigShouldersStencil.ttf"
)

set(ICONS

    "${CMAKE_SOURCE_DIR}/Assets/Icons/build.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/endTurn.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/mountain.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/move.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/no-image.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/ore.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/shovel.png"
    "${CMAKE_SOURCE_DIR}/Assets/Icons/play.png"

)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:main>/Assets/Fonts
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:main>/Assets/Icons
)

foreach(FONT ${FONTS})
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${FONT} $<TARGET_FILE_DIR:main>/Assets/Fonts
    )
endforeach()

foreach(ICON ${ICONS})
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${ICON} $<TARGET_FILE_DIR:main>/Assets/Icons
    )
endforeach()