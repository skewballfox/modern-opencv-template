set(sources
    src/fps_utils.cpp
    src/feed.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/myopencvproject/feed.hpp
    include/myopencvproject/fps_utils.hpp
)

set(test_sources
  src/fps_utils_test.cpp
)
