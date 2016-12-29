# Node Native Extension Proof of Concept

The module can be built for node or electron

## Install Prerequisits

```
$ npm -g install node-gyp
```

## Build joystream libraries

Build the joystream libraries and their dependencies before building the node module:

## Building For NodeJs

To compile the extension run

```
$ npm run configure-node
$ npm run build-node
```

All subsequent builds only need `npm run build-node`

You can confirm everything built correctly by running:

```
$ npm test
```

or run the basic app

```
$ node examples/nodejs_app/basic.js
```

## Building For Electron

Make sure to install electron 1.4.11

```
$ npm -g install electron@1.4.11
```

Build and configure:

```
$ npm run configure-electron
$ npm run build-electron
```

test by running the electron app

```
$ electron examples/electron_app/
```
