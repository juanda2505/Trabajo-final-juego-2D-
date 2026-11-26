cmake_minimum_required(VERSION 3.19)
project(desafio3 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(desafio3
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    jugador.h
    jugador.cpp
    entidad.h
    entidad.cpp
    entidadgrafica.h
    entidadgrafica.cpp
    proyectil.h
    proyectil.cpp
    proyectilparabolico.h
    proyectilparabolico.cpp
)

target_link_libraries(desafio3
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS desafio3
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET desafio3
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
