#######################################################
# project definition                                  #
#######################################################
#   setting the project name, version and language
project(data_sources     VERSION 0.1.0
    DESCRIPTION "Project description"
    HOMEPAGE_URL "https://github.com/AndreiTurean/measurement_data_solution"
    LANGUAGES CXX)

add_subdirectory(raw_data_generator) 
add_subdirectory(wave_data_generator)