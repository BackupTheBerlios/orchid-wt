include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_browser_SRC
	main.cpp
	mainwindow.cpp
	newresourcedialog.cpp
	resourceconfig.cpp
	stringlistedit.cpp
)

qt4_automoc(${orchid_browser_SRC})

qt4_wrap_ui(orchid_browser_SRC
	mainwindow.ui
	newresourcedialog.ui
	resourceconfig.ui
	stringlistedit.ui
)

add_executable(orchid-browser ${orchid_browser_SRC})

target_link_libraries(orchid-browser
	${QT_LIBRARIES}
	orchid-flower
	orchid-root
	orchid-stem
)
 
