build_resources()

set(FILAPP_PUBLIC_HEADERS
        include/FilApp/FilApplication.hpp
        include/FilApp/FilAppView.hpp
        include/FilApp/FilAppWindow.hpp
        include/FilApp/FilAppRenderable.hpp
        include/FilApp/FilAppRenderableCreator.hpp
        )

set(FILAPP_SOURCES
        source/FilAppConversion.hpp
        source/FilApplication.cpp
        source/FilAppView.cpp
        source/FilAppWindow.cpp
        source/NativeWindowHelper.hpp
        source/NativeWindowHelper.cpp
        source/FilAppConversion.cpp
        source/FilAppRenderableBuffers.cpp
        source/FilAppRenderableBuffers.hpp
        source/FilAppRenderableCreator.cpp
        )

add_library(fil_app ${FILAPP_SOURCES} ${FILAPP_PUBLIC_HEADERS})
add_library(filapp::filapp ALIAS fil_app)
target_include_directories(fil_app
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/filapp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/source>
        )
set_target_properties(fil_app
        PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
        DEBUG_POSTFIX d
        LINKER_LANGUAGE CXX
        EXPORT_NAME filapp
        )
target_link_libraries(fil_app
        PUBLIC
        filament
        fil_app_sources
        filapp_interfaces
        sdl2
        math
        utils
        camutils
        )
target_compile_features(fil_app PUBLIC cxx_std_17)

if (MSVC)
    target_compile_definitions(fil_app PRIVATE -DMSVC)
else ()
    target_compile_definitions(fil_app PRIVATE -DLINUX)
endif ()

include(GenerateExportHeader)
generate_export_header(fil_app BASE_NAME FilApp)

install(DIRECTORY "include/FilApp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS
        fil_app
        fil_app_sources
        filapp_interfaces
        filament
        sdl2
        math
        utils
        camutils
        EXPORT FilAppTargetsExportSet
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
        ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

setupExportSetInstall("${PROJECT_NAME}" "FilAppTargetsExportSet")