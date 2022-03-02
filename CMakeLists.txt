project(HACKAGAMES LANGUAGES C)
cmake_minimum_required(VERSION 3.10)

# Activate C99 standard:
SET(CMAKE_C_COMPILER "gcc" )
SET(CMAKE_C_FLAGS "-std=c99" )
#SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g -fsanitize=address")
SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall")

# RayLib:
#find_package(raylib 3.0 REQUIRED)
#set(raylib_VERBOSE 1)

# RayLib:

# Local dependency: (RayLib) :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/core-include ${PROJECT_SOURCE_DIR}/dpd/include)
link_directories( ${PROJECT_SOURCE_DIR}/dpd )
set(DPD_IHM_LIBS raylib pthread dl rt X11 m)

# HackaGames libs :
set(SRC_FILES
    core-src/hg-geometry.c
    core-src/hg-organism.c
    core-src/hg-game.c
)

add_library(hackagames SHARED ${SRC_FILES})
add_library(hackagames-interface SHARED ${SRC_FILES} core-src/hg-interface.c)

#-----------#
## T E S T ##
#-----------#
if(TEST)

endif()

# hg-draft ... :
add_executable(hg-draft core-src/main-draft.c core-src/hg-interface.c)
target_link_libraries(hg-draft hackagames ${DPD_IHM_LIBS})

# hg-term ... :
add_executable(hg-term core-src/main-term.c)
target_link_libraries(hg-term hackagames)

# hg-play ... :
add_executable(hg-play core-src/main-play.c)
target_link_libraries(hg-play hackagames ${DPD_IHM_LIBS})
