project(Orchid-Resources)

set(QT_USE_QTXML true)
set(QT_USE_QTWEBKIT true)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIRS})
add_definitions(${QT_DEFINITIONS})

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

set(page_SRC
	builder.cpp
	clientinfo.cpp
	document.cpp
	fragment.cpp
	httpservice.cpp
	main.cpp
	mainwindow.cpp
	manuallyshared.cpp
	obuilder.cpp
	resource.cpp
	service.cpp
	styleattributes.cpp
	style.cpp
)

qt4_automoc(${page_SRC})

qt4_wrap_ui(page_SRC mainwindow.ui)

add_executable(page ${page_SRC})

target_link_libraries(page
	${QT_LIBRARIES}
) 
 
