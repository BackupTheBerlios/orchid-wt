include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_leaf_SRC
	imagecollection.cpp
	imageresource.cpp
	modelresource.cpp
	xmlmodelresource.cpp
)

qt4_automoc(${orchid_leaf_SRC})

add_library(orchid-leaf SHARED ${orchid_leaf_SRC})

target_link_libraries(orchid-leaf
	${QT_LIBRARIES}
)
 
