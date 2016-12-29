#!/bin/bash

node-gyp configure && node-gyp rebuild && sh post_build.sh
