# BluezQt

Qt wrapper for Bluez 5 DBus API

## Introduction

BluezQt is a library for communication with Bluez system and session daemons.

<ul>
  <li>Manager class is used to communicate with system daemon - org.bluez</li>
  <li>ObexManager class is used to communicate with session daemon - org.bluez.obex</li>
</ul>

All method calls are asynchronous using PendingCall that helps tracking
the call progress and handle errors.

## Links

- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: \#kde-devel on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/bluezqt/repository>
