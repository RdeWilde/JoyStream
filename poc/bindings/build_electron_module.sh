#!/bin/bash
HOME=~/.electron-gyp node-gyp configure --target=1.4.11 --dist-url=https://atom.io/download/electron &&
HOME=~/.electron-gyp node-gyp rebuild --target=1.4.11 --dist-url=https://atom.io/download/electron
