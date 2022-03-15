project(HACKAGAMES_RISKY LANGUAGES C)
cmake_minimum_required(VERSION 3.10)

# Activate C99 standard:
SET(CMAKE_C_COMPILER "gcc" )
SET(CMAKE_C_FLAGS "-std=c99" )
#SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g -fsanitize=address")
SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall")

# Local dependency:
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/../dpd/include
    ${PROJECT_SOURCE_DIR}/../hackagames/src)
link_directories(
    ${PROJECT_SOURCE_DIR}/../dpd
    ${PROJECT_SOURCE_DIR}/../hackagames )
set(DPD_IHM_LIBS raylib pthread dl rt X11)

# Risky-Game :
set(RISKY_SRCS
    src/risky.c)

add_executable(hg-risky ${RISKY_SRCS} src/main-risky.c)
target_link_libraries(hg-risky hackagames-interface m ${DPD_IHM_LIBS})

add_executable(hg-risky-hidden ${RISKY_SRCS} src/main-hidden.c)
target_link_libraries(hg-risky-hidden hackagames m)
