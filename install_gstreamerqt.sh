#!/bin/sh
set -ex
wget http://gstreamer.freedesktop.org/src/qt-gstreamer/qt-gstreamer-1.2.0.tar.xz
tar xf qt-gstreamer-1.2.0.tar.xz
cd qt-gstreamer-1.2.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DQT_VERSION=5 -DUSE_GST_PLUGIN_DIR=ON -DQTGSTREAMER_EXAMPLES=OFF
make
sudo make install
