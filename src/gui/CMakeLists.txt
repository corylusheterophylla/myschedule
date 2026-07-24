add_executable(myschedule_gui
    main_gui.cpp
    MainWindow.cpp
)

# 链接核心库和 Qt 组件
target_link_libraries(myschedule_gui
    core
    Qt5::Widgets
)

# 自动包含 core 头文件路径（从 core 目标继承）
