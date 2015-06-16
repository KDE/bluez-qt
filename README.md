# BluezQt

Qt wrapper for BlueZ 5 DBus API

## Introduction

BluezQt is a library for communication with BlueZ system and session daemons.

<ul>
  <li>Manager class is used to communicate with system daemon - org.bluez</li>
  <li>ObexManager class is used to communicate with session daemon - org.bluez.obex</li>
</ul>

All method calls are asynchronous using PendingCall that helps tracking
the call progress and handle errors.

## Links

- Home page: <https://projects.kde.org/projects/frameworks/bluez-qt>
- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: \#kde-devel and \#solid on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/bluez-qt/repository>
