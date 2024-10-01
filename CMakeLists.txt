cmake_minimum_required(VERSION 3.23)
project(Fao)

set(CMAKE_CXX_STANDARD 11)

# 定义目标 Fao 并添加 main.cpp 源文件
add_executable(Fao main.cpp
        Window.cpp
        Window.h
        Button.cpp
        Button.h)

# 查找 SDL2 及其相关库
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

# 链接 SDL2 和相关库
target_link_libraries(Fao
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_link_libraries(Fao PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

target_link_libraries(Fao PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
