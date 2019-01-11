# NX Software Center [![Build Status](https://travis-ci.org/Nitrux/nx-software-center.svg?branch=development)](https://travis-ci.org/Nitrux/nx-software-center)
_Software Center for portable applications for [AppImages](https://appimage.org/)._

You can now easily explore and get your favorite applications easily (in some cases directly from their original authors!). No matter which GNU/Linux distribution you are using.

Say goodbye to things like:
 * The applications that I want is not in my distribution repositories.
 * Missing or outdated libraries and package conflicts.
 * Old application version.

**Just download and run!**

# Feeds
The AppImages included are fetched from:
* https://appimage.github.io/
* https://www.appimagehub.com/
* https://www.linux-apps.com/

# Screenshots
 * Search View

![](https://user-images.githubusercontent.com/1138094/39208689-58d3ae84-47c9-11e8-83a9-28d89a6f2cbb.png)

 * Application Details View

![](https://user-images.githubusercontent.com/1138094/39208724-6d128f5a-47c9-11e8-90e3-d41472374406.png)

 * Tasks View

![](https://user-images.githubusercontent.com/1138094/39208748-7978ded4-47c9-11e8-98c4-5dffeaf5ee0d.png)

## Build
### Requirements
 * cmake
 * build-essential
 * xxd
 * libtool
 * automake
 * qt5-default
 * binutils-multiarch (=>2.28)
 * binutils-multiarch-dev (=>2.28)
 * libboost-filesystem (=>1.67)
 * libboost-filesystem-dev (=>1.67)
 * libxml2-dev
 * libssl-dev
 * libboost-program-options (=>1.55)
 * libboost-program-options-dev (=>1.55)
 * qt5declarative-dev
 * libqt5xmlpattern5-dev
 * autotools-dev
 * libcairo2-dev
 * libfuse-dev
 * libappimage-dev
 * libappimageinfo
 * libc6 (=>2.28)
 * desktop-file-utils 

### Instructions
 1. `git clone https://github.com/Nitrux/nx-software-center.git`
 1. `mkdir build`
 1. `cd build`
 1. `cmake .. -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr`
 1. `make`

# Issues
If you find problems with the contents of this repository please create an issue.

©2018 Nitrux Latinoamericana S.C.
