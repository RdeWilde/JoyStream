# Node Native Extension Proof of Concept

## Build joystream libraries

First Make sure to build the joystream libraries and their dependencies before building the node or electron module:

## Building For NodeJs

Install dependencies and automatically build node module

```
$ npm install
```

All subsequent builds only need

```
$ npm run build-node
```

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

Install dependencies (This will also automatically build the nodejs module)

```
$ npm install
```

Reconfigure and build electron module:

```
$ npm run build-electron
```

test by running the electron app

```
$ electron examples/electron_app/
```
