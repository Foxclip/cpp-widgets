set(WIDGET_TESTS_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_test.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_basic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_button.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_canvas.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_checkbox.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_container.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_dropdown.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_render.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_post_actions.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_scroll_area.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_size_policy.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_text.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_textbox.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_tree_view.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_toposort.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_widget_link.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/widget_tests/widget_tests_window.h"
)
set(WIDGET_TESTS_SOURCE_FILES
    "src/main.cpp"
    "src/widget_test.cpp"
    "src/widget_tests.cpp"
    "src/widget_tests_application.cpp"
    "src/widget_tests_basic.cpp"
    "src/widget_tests_button.cpp"
    "src/widget_tests_canvas.cpp"
    "src/widget_tests_checkbox.cpp"
    "src/widget_tests_container.cpp"
    "src/widget_tests_dropdown.cpp"
    "src/widget_tests_render.cpp"
    "src/widget_tests_post_actions.cpp"
    "src/widget_tests_scroll_area.cpp"
    "src/widget_tests_size_policy.cpp"
    "src/widget_tests_text.cpp"
    "src/widget_tests_textbox.cpp"
    "src/widget_tests_toposort.cpp"
    "src/widget_tests_tree_view.cpp"
    "src/widget_tests_widget_link.cpp"
    "src/widget_tests_window.cpp"
)
add_executable(widget_tests ${WIDGET_TESTS_HEADER_FILES} ${WIDGET_TESTS_SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${WIDGET_TESTS_HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${WIDGET_TESTS_SOURCE_FILES})
set_property(TARGET widget_tests PROPERTY FOLDER "tests")

if(NOT TARGET test_lib)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_lib/CMakeLists.txt")
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test_lib")
    else()
        message(FATAL_ERROR "test_lib target not found and test_lib not available")
    endif()
endif()

target_include_directories(widget_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_include_directories(widget_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(widget_tests PUBLIC "${CMAKE_SOURCE_DIR}")
target_include_directories(widget_tests PUBLIC "${INCLUDE_DIR}")

target_link_libraries(widget_tests PUBLIC logger)
target_link_libraries(widget_tests PUBLIC test_lib)
target_link_libraries(widget_tests PUBLIC widgets_lib)
set_property(TARGET widget_tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
