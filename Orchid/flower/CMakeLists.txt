include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DORCHID_BUILD_FLOWER)

set(orchid_flower_SRC
	documenthead.cpp
	documentprocessor.cpp
	documentstreams.cpp
	fragmentbuilder.cpp
	fragment.cpp
	fragmentdom.cpp
	htmlfragmentwriter.cpp
	styleattributes.cpp
	style.cpp
	xhtmlstreamwriter.cpp
	xmlfragmentreader.cpp
	xmlfragmentwriter.cpp
)

qt4_automoc(${orchid_flower_SRC})

add_library(orchid-flower SHARED ${orchid_flower_SRC})

target_link_libraries(orchid-flower
	${QT_LIBRARIES}
)
