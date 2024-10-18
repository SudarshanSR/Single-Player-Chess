add_library(ui ui.cpp)
target_sources(ui PUBLIC ui.h)
target_link_libraries(ui PRIVATE pieces move Qt::Widgets)