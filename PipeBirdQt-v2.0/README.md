# PipeBirdQtPro

Qt Widgets 飞鸟游戏工程，包含：

- 主菜单
- 难度选择
- 音效系统
- PNG 精灵动画
- 计分与最高分保存

## 构建
推荐直接用 Qt Creator 打开该文件夹。

如果用命令行：

```bash
cmake -S . -B build
cmake --build build
```

## 目录
- `src/`：源代码
- `include/`：头文件
- `assets/`：PNG 与 WAV 素材
- `resources.qrc`：Qt 资源文件
