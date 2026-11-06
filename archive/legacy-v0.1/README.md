# Legacy v0.1 原型（2022-10）

这是项目早期的一份 C++ 原型，和仓库根目录 `MachiKoro-master/` 中的当前版本不是同一套实现。

## 特点

- 固定 3 名玩家，使用全局 `Player player1/player2/player3`。
- 每名玩家用 `list<Card>` 和一组 `wheatFields`、`ranches`、`bakeries` 等计数字段保存资产。
- 没有当前版本的多态卡牌体系、市场、`Game` 单例和 AI。
- 代码中保留了早期问题，例如 `player.cpp` 中把比较写成了赋值（`if (roll1 = roll2 ...)`）。

## 保留目的

作为课程项目的**版本演进记录**和资料归档：

- v0.1：3 人固定、面向过程风格较重的原型；
- 当前版本：N 名玩家、五色卡继承体系、多态结算、市场、AI、输入校验。

本目录不再维护，也不建议把这里的代码合并进当前版本。

> 本目录下的 `.vs/`、`x64/`、`*.obj`、`*.exe`、`*.pdb`、`*.ilk`、`*.tlog`、`*.vcxproj.user` 等构建产物已被根目录 `.gitignore` 排除，不会提交到 GitHub。保留源码和工程文件即可在本地重新打开。

当前可玩版本请回到仓库根目录的 [`MachiKoro-master/`](../../MachiKoro-master/)。
