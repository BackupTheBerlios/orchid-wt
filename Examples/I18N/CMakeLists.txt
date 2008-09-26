include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(i18ndocplugin_SRC
	i18ndocplugin.cpp
	i18ndocsample.cpp
)

qt4_automoc(${i18ndocplugin_SRC})

add_library(i18ndocplugin MODULE ${i18ndocplugin_SRC})

target_link_libraries(i18ndocplugin
	${QT_LIBRARIES}
	orchid-flower
	orchid-stem
)
