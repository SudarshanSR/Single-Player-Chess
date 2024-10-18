add_library(move move.cpp)
target_sources(move PUBLIC move.h)
target_link_libraries(move PRIVATE pieces)