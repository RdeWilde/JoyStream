#!/bin/bash
# Run this after building on OSX
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @loader_path/../../Frameworks/QtCore.framework/Versions/5/QtCore build/Release/NativeExtension.node
