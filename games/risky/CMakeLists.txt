project(HACKAGAMES_RISKY LANGUAGES C)
cmake_minimum_required(VERSION 3.10)

# Activate C99 standard:
SET(CMAKE_C_COMPILER "gcc" )
SET(CMAKE_C_FLAGS "-std=c99" )
#SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g -fsanitize=address")
SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall")

# Local dependency:
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/dpd/include)
link_directories( ${PROJECT_SOURCE_DIR}/dpd )
set(DPD_IHM_LIBS raylib pthread dl rt X11 m)

# Risky-Game :
set(RISKY_SRCS
    src/main-risky.c
    src/risky.c)
add_executable(hg-risky ${RISKY_SRCS})
target_link_libraries(hg-risky hackagames-interface ${DPD_IHM_LIBS})
