project(HACKAGAME LANGUAGES C)
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

# Local dependency: (RayLib) :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/core-include ${PROJECT_SOURCE_DIR}/dpd/include)
link_directories( ${PROJECT_SOURCE_DIR}/dpd )
set(DPD_IHM_LIBS raylib pthread dl rt X11 m)

# Toghap and Toghap-player libs :
set(SRC_FILES
    core-src/hkg-geometry.c
    core-src/hkt-organism.c
    core-src/hks-game.c
)

add_library(hackagames SHARED ${SRC_FILES})
# add_library(hackagames-interface SHARED ${SRC_FILES} core-src/hki-interface.c)

# Draft ... :
add_executable(draft core-src/main-draft.c core-src/hki-interface.c)
target_link_libraries(draft hackagames ${DPD_IHM_LIBS})

# Game NavGraph :
##set(NAVGRAPH_SRC
##    games/navgraph/src/main-navgraph.c)

##add_executable(navgraph ${NAVGRAPH_SRC})
##target_link_libraries(navgraph toghap ${DPD_LIBS})

# Risky-Game :
#set(RISKY_SRCS
#    src/hackagames_interface.c
#    games/risky/src/risky.c)
#add_executable(risky-easy games/risky/src/main-easy.c games/risky/src/risky.c ${RISKY_SRCS})
#target_link_libraries(risky-easy hackagame ${DPD_IHM_LIBS})
