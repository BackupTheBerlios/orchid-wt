include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DORCHID_BUILD_ROOT)

set(orchid_root_SRC
	httpservice.cpp
	mocs.cpp
	service.cpp
)

qt4_automoc(${orchid_root_SRC})

add_library(orchid-root SHARED ${orchid_root_SRC})

target_link_libraries(orchid-root
	${QT_LIBRARIES}
	${QT_QTNETWORK_LIBRARY}
	orchid-stem
)

install(TARGETS orchid-root
	RUNTIME DESTINATION ${EXE_INSTALL_DIR}
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
