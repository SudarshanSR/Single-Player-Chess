add_library(pieces pieces.cpp)
target_sources(pieces PUBLIC pieces.h)
target_link_libraries(pieces PRIVATE move)