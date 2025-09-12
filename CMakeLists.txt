cmake_minimum_required(VERSION 3.10)
project(cpp-search-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить все предупреждения компилятора
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Добавляем исполняемый файл
add_executable(search_server
    search-server/document.cpp
    search-server/main.cpp
    search-server/read_input_functions.cpp
    search-server/request_queue.cpp
    search-server/search_server.cpp
    search-server/string_processing.cpp
)

# Устанавливаем зависимости для компиляции
target_include_directories(search_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Для отладочной сборки добавляем символы отладки
set(CMAKE_BUILD_TYPE Debug)

# Опционально: добавляем тесты, если они появятся в будущем
#if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
#    enable_testing()
#    add_subdirectory(test)
#endif()