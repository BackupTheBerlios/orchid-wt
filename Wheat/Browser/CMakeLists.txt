include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_browser_SRC
	main.cpp
	mainwindow.cpp
	mocs.cpp
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

qt4_add_resources(orchid_browser_SRC browser.qrc)

add_executable(orchid-browser ${orchid_browser_SRC})

target_link_libraries(orchid-browser
	${QT_LIBRARIES}
	orchid-flower
	orchid-root
	orchid-stem
)
 
install(TARGETS orchid-browser DESTINATION ${EXE_INSTALL_DIR})
