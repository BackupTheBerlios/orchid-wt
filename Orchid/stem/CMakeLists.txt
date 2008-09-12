include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_stem_SRC
	location.cpp
	request.cpp
	resource.cpp
	resourcefactory.cpp
	resourcekeep.cpp
	resourcemodel.cpp
)

qt4_automoc(${orchid_stem_SRC})

add_library(orchid-stem SHARED ${orchid_stem_SRC})

target_link_libraries(orchid-stem
	${QT_LIBRARIES}
)
