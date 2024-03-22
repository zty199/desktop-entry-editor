# desktop-entry-editor (forked from [linuxdeepin/desktop-entry-editor](https://github.com/linuxdeepin/desktop-entry-editor))

[Chinese](https://gitee.com/deepin-opensource/desktop-entry-editor) | English

A simple GUI tool to create and edit desktop files, designed for Deepin.

## Dependencies

- qtbase5-dev

- qttools5-dev-tools

- libdtkcore-dev (>= 5.0)

- libdtkgui-dev (>= 5.0)

- libdtkwidget-dev (>= 5.0)

- libqt5xdg-dev

- fakeroot

## Build

```bash
mkdir -p build
qmake desktop-entry-editor.pro -spec linux-g++ CONFIG+=qtquickcompiler -o build
make -C build -j$(nproc)
```

or

```bash
dpkg-buildpackage -Zxz -rfakeroot -b -uc -us -j$(nproc)
```

## Credit

linuxdeepin&emsp;<https://github.com/linuxdeepin/desktop-entry-editor>

linuxdeepin&emsp;<https://linuxdeepin.github.io/dtk/>

deepin-opensource&emsp;<https://gitee.com/deepin-opensource/DtkDemo>
