include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_root_SRC
# 	clientinfo.cpp
# 	document.cpp
# 	fragment.cpp
# 	htmlstreams.cpp
# 	htmlstreamwriter.cpp
	httpservice.cpp
# 	main.cpp
# 	mainwindow.cpp
# 	manuallyshared.cpp
# 	request.cpp
# 	resource.cpp
# 	resourcekeep.cpp
# 	resourcemodel.cpp
	service.cpp
# 	styleattributes.cpp
# 	style.cpp
)

qt4_automoc(${orchid_root_SRC})

add_library(orchid-root SHARED ${orchid_root_SRC})

target_link_libraries(orchid-root
	${QT_LIBRARIES}
)
