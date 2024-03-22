# 桌面快捷方式编辑器 (forked from [linuxdeepin/desktop-entry-editor](https://github.com/linuxdeepin/desktop-entry-editor))

中文 | [英文](https://github.com/zty199/desktop-entry-editor)

一款简单的桌面快捷方式图形编辑工具，为 Deepin 设计。

## 依赖

- qtbase5-dev

- qttools5-dev-tools

- libdtkcore-dev (>= 5.0)

- libdtkgui-dev (>= 5.0)

- libdtkwidget-dev (>= 5.0)

- libqt5xdg-dev

- fakeroot

## 构建方式

```bash
mkdir -p build
qmake desktop-entry-editor.pro -spec linux-g++ CONFIG+=qtquickcompiler -o build
make -C build -j$(nproc)
```

或

```bash
dpkg-buildpackage -Zxz -rfakeroot -b -uc -us -j$(nproc)
```

## 感谢

linuxdeepin&emsp;<https://github.com/linuxdeepin/desktop-entry-editor>

linuxdeepin&emsp;<https://linuxdeepin.github.io/dtk/>

deepin-opensource&emsp;<https://gitee.com/deepin-opensource/DtkDemo>

