cmake_minimum_required(VERSION 3.15)
project(saiga-process-handler VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "Netcorg Production")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  src/saiga_process.cpp
  src/saiga_process_handler.cpp
  src/saiga_database_manager.cpp
  src/saiga_endpoint_config.cpp
  src/saiga_endpoint.cpp
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# curl library configuration
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_MISC_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_LIBCURL_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ENABLE_CURL_MANUAL OFF CACHE BOOL "" FORCE)
set(PICKY_COMPILER OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(ENABLE_UNICODE ON CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
set(CURL_STATIC_CRT ON CACHE BOOL "" FORCE)
set(USE_NGHTTP2 ON CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(externals/ear)
add_subdirectory(externals/sqlite3)
add_subdirectory(externals/curl)
add_subdirectory(externals/rapidjson)

target_include_directories(${PROJECT_NAME} PUBLIC
  include
  externals/ear/externals/spdlog/include
  externals/ear/include
  externals/sqlite3
  externals/curl/include
  externals/rapidjson/include
)

target_link_directories(${PROJECT_NAME} PUBLIC
  externals/ear/externals/spdlog
  externals/ear/
  externals/sqlite3
  externals/curl/lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC  
  spdlog
  ear
  sqlite3
  libcurl
  User32
  shlwapi
  dwmapi
)
