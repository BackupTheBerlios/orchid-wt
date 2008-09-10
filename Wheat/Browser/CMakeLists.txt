include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_browser_SRC
	gallery.cpp
	main.cpp
	mainwindow.cpp
)

qt4_automoc(${orchid_browser_SRC})

qt4_wrap_ui(orchid_browser_SRC mainwindow.ui)

qt4_add_resources(orchid_browser_SRC browser.qrc)

add_executable(orchid-browser ${orchid_browser_SRC})

target_link_libraries(orchid-browser
	${QT_LIBRARIES}
	orchid-flower
	orchid-root
	orchid-stem
	orchid-leaf
)
 
