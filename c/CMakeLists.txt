cmake_minimum_required(VERSION 3.20)
project(csvf LANGUAGES C)

if (MSVC)
    add_compile_options(/W4 /WX /Ox)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
endif()

add_library(csvf SHARED dispcsvf.c csv2csvf.c)

add_executable(dispcsvf dispcsvf-cmd.c)
target_link_libraries(dispcsvf csvf)

add_executable(csv2csvf csv2csvf-cmd.c)
target_link_libraries(csv2csvf csvf)

install(TARGETS csvf dispcsvf csv2csvf
 RUNTIME DESTINATION bin
 LIBRARY DESTINATION lib)
install(FILES libcsvf.h DESTINATION include)
install(PROGRAMS csview DESTINATION bin)
