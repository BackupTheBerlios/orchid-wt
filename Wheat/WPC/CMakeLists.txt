include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(orchid_wpc_SRC
	cppfragmentwriter.cpp
	fragmentdom.cpp
	main.cpp
	xmlfragmentreader.cpp
	xmlfragmentwriter.cpp
)

qt4_automoc(${orchid_wpc_SRC})

add_executable(orchid-wpc ${orchid_wpc_SRC})

target_link_libraries(orchid-wpc
	${QT_LIBRARIES}
# 	orchid-flower
# 	orchid-root
# 	orchid-stem
# 	orchid-leaf
)
