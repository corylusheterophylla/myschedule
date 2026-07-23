add_library(core STATIC
    TaskManager.cpp
    UserManager.cpp
    Storage.cpp
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(core OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json)
