include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(galleryplugin_SRC
	gallery.cpp
	gallerydemo.cpp
	galleryplugin.cpp
)

qt4_automoc(${galleryplugin_SRC})

qt4_add_resources(galleryplugin_SRC gallery.qrc)

add_library(galleryplugin MODULE ${galleryplugin_SRC})

target_link_libraries(galleryplugin
	${QT_LIBRARIES}
	orchid-flower
	orchid-stem
)

install(TARGETS galleryplugin DESTINATION ${PLUGIN_INSTALL_DIR})
