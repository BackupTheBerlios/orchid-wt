include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(modelplugin_SRC
	mocs.cpp
	modelplugin.cpp
	modelresource.cpp
	xmlmodelresource.cpp
)

qt4_automoc(${modelplugin_SRC})

add_library(modelplugin MODULE ${modelplugin_SRC})

target_link_libraries(modelplugin
	${QT_LIBRARIES}
	orchid-stem
)

install(TARGETS modelplugin DESTINATION ${PLUGIN_INSTALL_DIR})

