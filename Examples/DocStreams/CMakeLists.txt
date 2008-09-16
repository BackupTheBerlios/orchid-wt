include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(docstreamsplugin_SRC
	docstreamsplugin.cpp
	docstreamssample.cpp
)

qt4_automoc(${docstreamsplugin_SRC})

add_library(docstreamsplugin MODULE ${docstreamsplugin_SRC})

target_link_libraries(docstreamsplugin
	${QT_LIBRARIES}
	orchid-flower
	orchid-stem
)
