# Electron app

Joystream app in electron.

## Start

```
npm start
```

## TODO

- npm install local : `npm install --save ../../`


## Notes

### Where do we start joystream ?

Joystream library could be added in the main process or the rendering process. For now it is located in the rendering process.


### levelDB for electron

If using joystream lib in electron you need to recompile leveldown. for electron.
It supposed you are using the latest version of electron (1.4.15)
```
HOME=~/.electron-gyp node-gyp rebuild --target=1.4.15 --arch=x64 --dist-url=https://atom.io/download/electron
```
