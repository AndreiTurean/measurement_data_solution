#######################################################
# project definition                                  #
#######################################################
#   setting the project name, version and language
project(data_visualizations     VERSION 0.1.0
    DESCRIPTION "Project description"
    HOMEPAGE_URL "https://github.com/AndreiTurean/measurement_data_solution"
    LANGUAGES CXX)

add_subdirectory(raw_processor)
add_subdirectory(wave_visualizer)