cmake_minimum_required(VERSION 2.8.7)

project(othello C)
set(PROJECT_VERSION 0.7)

macro (install_path _DIR _DEFAULT)
    if (NOT ${_DIR})
        set (${_DIR} ${_DEFAULT})
    endif ()
endmacro ()

install_path (BINDIR bin)
install_path (MANDIR man)
install_path (MAN1DIR ${MANDIR}/man1)

add_definitions(-Wall -Wshadow)
add_definitions(-std=c89 -D_GNU_SOURCE)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_libraries(${GLUT_LIBRARIES})

find_program (ASCIIDOC asciidoc DOC "Full path to AsciiDoc executable; required for building the manual page")
find_program (XMLTO xmlto DOC "Full path to xmlto executable; required for building the manual page")
find_program (MKDIR mkdir DOC "Full path to mkdir executable; required for building the source distribution")
find_program (CP cp DOC "Full path to cp executable; required for building the source distribution")
find_program (TAR tar DOC "Full path to tar executable; required for building the source distribution")

configure_file(
        ${PROJECT_SOURCE_DIR}/src/version.h.in
        ${PROJECT_BINARY_DIR}/version.h
        @ONLY)
include_directories(${PROJECT_BINARY_DIR})

set(SOURCE_FILES
        src/board.c
        src/board.h
        src/camera.c
        src/camera.h
        src/common.h
        src/comp.c
        src/comp.h
        src/display.c
        src/display.h
        src/flippers.c
        src/flippers.h
        src/global.h
        src/keyboard.c
        src/keyboard.h
        src/main.c
        src/shift.c
        src/shift.h
        src/transform.h
        src/user.c
        src/user.h
        src/version.h.in)

add_executable(othello ${SOURCE_FILES})
install (TARGETS othello DESTINATION "${BINDIR}")

# Manual page
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/othello.1
        DEPENDS ${PROJECT_BINARY_DIR}/othello.1.xml
        COMMAND ${XMLTO}
        ARGS -o ${PROJECT_BINARY_DIR} man ${PROJECT_BINARY_DIR}/othello.1.xml
        COMMENT "Converting othello.1.xml to othello.1 (DocBook to troff)"
)
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/othello.1.xml
        DEPENDS ${PROJECT_SOURCE_DIR}/othello.1.txt
        COMMAND ${ASCIIDOC}
        ARGS -b docbook -d manpage -o ${PROJECT_BINARY_DIR}/othello.1.xml ${PROJECT_SOURCE_DIR}/othello.1.txt
        COMMENT "Converting othello.1.txt to othello.1.xml (AsciiDoc to DocBook)"
)
add_custom_target(man ALL
        DEPENDS ${PROJECT_BINARY_DIR}/othello.1
        )
install (FILES ${PROJECT_BINARY_DIR}/othello.1 DESTINATION "${MAN1DIR}")

# HTML documentation
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/index.html
        DEPENDS ${PROJECT_SOURCE_DIR}/README.adoc
        COMMAND ${ASCIIDOC}
        ARGS -b html -o ${PROJECT_BINARY_DIR}/index.html ${PROJECT_SOURCE_DIR}/README.adoc
        COMMENT "Converting README.adoc to index.html (AsciiDoc to HTML)"
)
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/othello.1.html
        DEPENDS ${PROJECT_SOURCE_DIR}/othello.1.txt
        COMMAND ${ASCIIDOC}
        ARGS -b html -d manpage -o ${PROJECT_BINARY_DIR}/othello.1.html ${PROJECT_SOURCE_DIR}/othello.1.txt
        COMMENT "Converting othello.1.txt to othello.1.html (AsciiDoc to HTML)"
)
add_custom_target(html ALL
        DEPENDS ${PROJECT_BINARY_DIR}/index.html ${PROJECT_BINARY_DIR}/othello.1.html
        )

# Source distribution
add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/othello-${PROJECT_VERSION}.tar.gz
        COMMAND
          ${MKDIR}
        ARGS
          ${PROJECT_BINARY_DIR}/othello-${PROJECT_VERSION}
        COMMAND
          ${CP}
        ARGS
          ${PROJECT_SOURCE_DIR}/LICENSE.txt
          ${PROJECT_SOURCE_DIR}/CMakeLists.txt
          ${PROJECT_SOURCE_DIR}/README.adoc
          ${PROJECT_SOURCE_DIR}/doc/othello.1.txt
          ${SOURCE_FILES}
          ${PROJECT_BINARY_DIR}/othello-${PROJECT_VERSION}/
        COMMAND
          ${TAR}
        ARGS
          czf ${PROJECT_BINARY_DIR}/othello-${PROJECT_VERSION}.tar.gz
          -C ${PROJECT_BINARY_DIR}
          othello-${PROJECT_VERSION}
)
add_custom_target(dist
        DEPENDS ${PROJECT_BINARY_DIR}/othello-${PROJECT_VERSION}.tar.gz
        )