include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(page_SRC
	main.cpp
	mainwindow.cpp
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
 
