include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(imageplugin_SRC
	imagecollection.cpp
	imageresource.cpp
	imageplugin.cpp
	mocs.cpp
)

qt4_automoc(${imageplugin_SRC})

add_library(imageplugin MODULE ${imageplugin_SRC})

target_link_libraries(imageplugin
	${QT_LIBRARIES}
	orchid-stem
)

install(TARGETS imageplugin DESTINATION ${PLUGIN_INSTALL_DIR})

