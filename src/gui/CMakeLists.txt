add_executable(myschedule_gui
    main_gui.cpp
    MainWindow.cpp
    SettingsDialog.cpp
)

target_link_libraries(myschedule_gui
    core
    Qt5::Widgets
    Qt5::Multimedia
)
