include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_flower_SRC
	document.cpp
	fragment.cpp
	htmlstreams.cpp
	htmlstreamwriter.cpp
	styleattributes.cpp
	style.cpp
)

qt4_automoc(${orchid_flower_SRC})

add_library(orchid-flower SHARED ${orchid_flower_SRC})

target_link_libraries(orchid-flower
	${QT_LIBRARIES}
)
