include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)


set(i18n_translation_SRC
	i18n_de.ts
	i18n_en.ts
)

qt4_add_translation(i18n_TRANSLATIONS ${i18n_translation_SRC})

add_custom_target(translations ALL DEPENDS ${i18n_TRANSLATIONS})


set(i18ndocplugin_SRC
	i18ndocplugin.cpp
	i18ndocsample.cpp
	mocs.cpp
)

qt4_automoc(${i18ndocplugin_SRC})

add_library(i18ndocplugin MODULE ${i18ndocplugin_SRC})

target_link_libraries(i18ndocplugin
	${QT_LIBRARIES}
	orchid-flower
	orchid-stem
)

install(TARGETS i18ndocplugin DESTINATION ${PLUGIN_INSTALL_DIR})
