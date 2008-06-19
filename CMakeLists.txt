project(Orchid-Resources)

set(QT_USE_QTXML true)
set(QT_USE_QTWEBKIT true)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIRS})
add_definitions(${QT_DEFINITIONS})

set(page_SRC
	builder.cpp
	dependency.cpp
	document.cpp
	fragment.cpp
	main.cpp
	mainwindow.cpp
	obuilder.cpp
)

qt4_automoc(${page_SRC})

qt4_wrap_ui(page_SRC mainwindow.ui)

add_executable(page ${page_SRC})

target_link_libraries(page
	${QT_LIBRARIES}
) 
 
