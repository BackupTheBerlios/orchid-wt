include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DORCHID_BUILD_STEM)

set(orchid_stem_SRC
	configurationloader.cpp
	configurationwriter.cpp
	extensionmanager.cpp
	extensionplugin.cpp
	location.cpp
	mocs.cpp
	request.cpp
	resource.cpp
	resourcebase.cpp
	resourcefactory.cpp
	resourcekeep.cpp
	resourcemodel.cpp
	resourceobject.cpp
)

qt4_automoc(${orchid_stem_SRC})

add_library(orchid-stem SHARED ${orchid_stem_SRC})

target_link_libraries(orchid-stem
	${QT_LIBRARIES}
)

install(TARGETS orchid-stem
	RUNTIME DESTINATION ${EXE_INSTALL_DIR}
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
