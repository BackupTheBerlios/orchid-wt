include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(page_SRC
# 	clientinfo.cpp
# 	document.cpp
# 	fragment.cpp
# 	htmlstreams.cpp
# 	htmlstreamwriter.cpp
# 	httpservice.cpp
	main.cpp
	mainwindow.cpp
# 	manuallyshared.cpp
# 	request.cpp
# 	resource.cpp
# 	resourcekeep.cpp
# 	resourcemodel.cpp
# 	service.cpp
# 	styleattributes.cpp
# 	style.cpp
)

qt4_automoc(${page_SRC})

qt4_wrap_ui(page_SRC mainwindow.ui)

add_executable(page ${page_SRC})

target_link_libraries(page
	${QT_LIBRARIES}
	orchid-flower
	orchid-root
	orchid-stem
)
 
