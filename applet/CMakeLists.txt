# Project Needs a name ofcourse
project(superbar-cxx)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

# We add our source code here
set(applet_SRCS
  main.cpp
  launcher_button.cpp
  )

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_superbar-cxx ${applet_SRCS})
target_link_libraries(plasma_applet_superbar-cxx
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS}
                      )

install(TARGETS plasma_applet_superbar-cxx
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-superbar-cxx.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
