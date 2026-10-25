if(WIN32)
    add_executable(widget_sandbox WIN32)
    set_target_properties(widget_sandbox PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
else()
    add_executable(widget_sandbox)
endif()
target_sources(widget_sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp
)
target_include_directories(widget_sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(widget_sandbox PRIVATE widgets_lib)
