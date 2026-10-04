if(WIN32)
    add_executable(widgets_sandbox WIN32)
    set_target_properties(widgets_sandbox PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
else()
    add_executable(widgets_sandbox)
endif()
target_sources(widgets_sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp
)
target_include_directories(widgets_sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(widgets_sandbox PRIVATE widgets_lib)
