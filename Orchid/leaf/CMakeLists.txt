include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_leaf_SRC
	imagecollection.cpp
	imageresource.cpp
	modelresource.cpp
	scriptedresource.cpp
	xmlmodelresource.cpp
)

qt4_automoc(${orchid_leaf_SRC})

add_library(orchid-leaf SHARED ${orchid_leaf_SRC})

target_link_libraries(orchid-leaf
	${QT_LIBRARIES}
	orchid-stem
)


set(imageplugin_SRC
	imageplugin.cpp
)

qt4_automoc(${imageplugin_SRC})

add_library(imageplugin MODULE ${imageplugin_SRC})

target_link_libraries(imageplugin
	${QT_LIBRARIES}
	orchid-leaf
)


set(modelplugin_SRC
	modelplugin.cpp
)

qt4_automoc(${modelplugin_SRC})

add_library(modelplugin MODULE ${modelplugin_SRC})

target_link_libraries(modelplugin
	${QT_LIBRARIES}
	orchid-leaf
)
