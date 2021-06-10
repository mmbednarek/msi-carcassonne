project(msi_evolution)
add_library(msi_evolution DiffEvol.cpp)
target_link_libraries(msi_evolution libmb fmt)
