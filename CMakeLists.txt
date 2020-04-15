cmake_minimum_required(VERSION 3.5)
project(json_transfer_prj LANGUAGES C)
add_definitions("-Wall -g")
add_library(json_transfer
	STATIC
		src/cJSON.c
		src/cJSON.h
		src/json_transfer.c
		src/json_transfer.h
	)

add_executable(demo1 example/demo1.c)
target_link_libraries(demo1 json_transfer)
target_link_libraries(demo1 m)

add_executable(demo2 example/demo2.c)
target_link_libraries(demo2 json_transfer)
target_link_libraries(demo2 m)