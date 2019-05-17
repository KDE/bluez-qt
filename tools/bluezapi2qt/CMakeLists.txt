remove_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
)

add_executable(bluezapi2qt
    BluezApiParser.cpp
    Interface.cpp
    Method.cpp
    Methods.cpp
    Properties.cpp
    TypeAnnotation.cpp
    XmlGenerator.cpp
    main.cpp
)

target_link_libraries(bluezapi2qt
    Qt5::Core
)
