add_library(promotion promotion.cpp)
target_sources(promotion PUBLIC promotion.h)
target_link_libraries(promotion PRIVATE pieces move Qt::Widgets)

add_library(record record.cpp)
target_sources(record PUBLIC record.h)
target_link_libraries(record PRIVATE pieces move Qt::Widgets)

add_library(window window.cpp)
target_sources(window PUBLIC window.h)
target_link_libraries(window PRIVATE promotion record pieces move Qt::Widgets)

add_library(ui INTERFACE)
target_link_libraries(ui INTERFACE window)