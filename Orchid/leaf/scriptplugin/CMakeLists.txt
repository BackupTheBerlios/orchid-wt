include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(scriptplugin_SRC
	mocs.cpp
	scriptedresource.cpp
	scriptplugin.cpp
)

qt4_automoc(${scriptplugin_SRC})

add_library(scriptplugin MODULE ${scriptplugin_SRC})

target_link_libraries(scriptplugin
	${QT_LIBRARIES}
	orchid-stem
)

install(TARGETS scriptplugin DESTINATION ${PLUGIN_INSTALL_DIR})
