set(fakebluez_SRCS
    main.cpp
    fakebluez.cpp
    testinterface.cpp
    objectmanager.cpp
    object.cpp
    agentmanager.cpp
    profilemanager.cpp
    devicemanager.cpp
    adapterinterface.cpp
    deviceinterface.cpp
    inputinterface.cpp
    mediaplayerinterface.cpp
    obexagentmanager.cpp
    obexclient.cpp
)

add_executable(fakebluez ${fakebluez_SRCS})

target_link_libraries(fakebluez
    Qt5::Core
    Qt5::DBus
)
