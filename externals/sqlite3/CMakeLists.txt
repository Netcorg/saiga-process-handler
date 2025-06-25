cmake_minimum_required(VERSION 3.15)
project(sqlite3)
add_library(${PROJECT_NAME} STATIC shell.c sqlite3.c)
