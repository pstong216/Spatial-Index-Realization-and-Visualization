set(TARGET_NAME "shape_lib")

add_library(
	${TARGET_NAME}
	"dbfopen.cpp"
	"safileio.cpp"
	"shpopen.cpp"
)
target_link_libraries(
	${TARGET_NAME}
	PRIVATE "freeglut_static"
)

