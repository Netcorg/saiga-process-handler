cmake_minimum_required(VERSION 3.15)
project(saiga)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES src/saiga_process.cpp src/saiga_process_handler.cpp src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} include/saiga_process.h include/saiga_process_handler.h)
add_subdirectory(externals/ear)
add_subdirectory(externals/sqlite3)

target_include_directories(${PROJECT_NAME} PUBLIC
  include
  externals/ear/include
  externals/ear/externals/spdlog/include
  externals/sqlite3
)

target_link_directories(${PROJECT_NAME} PUBLIC
  externals/ear/
  externals/ear/externals/spdlog
  externals/sqlite3
)

target_link_libraries(${PROJECT_NAME} PUBLIC  
  spdlog
  ear
  sqlite3
)
