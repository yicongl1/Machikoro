# MachiKoro

> C++ 面向对象课程项目 · 支持 1~N 玩家与 AI 的桌游控制台实现

MachiKoro（街角小吃店）是一款骰子驱动的轻策略桌游。本项目使用 **C++14** 从零实现完整规则：15 种建筑、4 种地标、五色卡牌结算、市场购买与胜负判定，支持人类玩家与 AI 混合对局。

> **团队项目**：陈诺、黄浩、肖文婷、邱奕博等共同完成（2022 秋季学期）。

| 项目信息 | 内容 |
| --- | --- |
| 项目类型 | C++ 课程项目 / 控制台桌游 |
| 技术栈 | C++14 / STL / 面向对象 / UML / Visual Studio 2022 / Git |
| 核心规模 | 15 种建筑 + 4 种地标 + 5 类卡牌 |
| 运行方式 | Windows 控制台 |
| 演示 | [DLC 胜利](#dlc-模式胜利12-秒) · [起源胜利](#起源模式胜利16-秒) · [购买流程](#dlc-模式购买流程59-秒) |

> 详细规则、卡牌数值与命令见 [docs/rules.md](docs/rules.md)；可直接用于简历的文案与面试提纲见 [docs/resume-project.md](docs/resume-project.md)。

---

## 目录

- [项目亮点](#项目亮点)
- [演示视频](#演示视频)
- [技术难点与解决](#技术难点与解决)
- [简历文案](#简历文案)
- [课程笔记与归档](#课程笔记与归档)
- [构建与运行](#构建与运行)
- [目录结构](#目录结构)
- [规则摘要](#规则摘要)
- [代码设计](#代码设计)
- [AI 行为](#ai-行为)
- [本次修复内容](#本次修复内容)
- [验证记录](#验证记录)
- [已知限制与 TODO](#已知限制与-todo)
- [开发环境](#开发环境)

---

## 项目亮点

- **面向对象设计**：`Card` 抽象基类 + 五色卡继承体系，使用纯虚函数 `action()` 实现多态结算，新增卡牌无需修改主流程。
- **设计模式实践**：`Game` 单例、`Deck` 集中创建（简单工厂思想）、`Game::iterator` 自定义迭代器、卡牌多态结算（类似 Strategy）。
- **完整游戏规则**：15 种建筑 + 4 种地标，红/蓝/绿/紫四阶段结算，支持 1~N 名玩家与 AI 混合对局。
- **两种市场模式**：`origin` 使用完整 15 种建筑，`dlc` 随机抽取 10 种；支持查看市场与任意玩家手牌。
- **防御式输入处理**：正则解析 + 单行输入 + 越界/买不起/重复紫卡校验，异常输入不会崩溃。
- **AI 自动决策**：AI 可完成掷骰、选择骰子数量、重掷和购买，不需要人工输入。
- **工程化归档**：UML 设计图、课程笔记（Markdown + 原始 docx）、演示视频、旧版原型，完整保留项目演进过程。

---

## 演示视频

> 以下视频通过 GitHub 附件（user-attachments）内嵌，点击播放即可。仓库内的 `docs/demo/*.mp4` 保留为离线/下载副本。

### DLC 模式胜利（12 秒）

https://github.com/user-attachments/assets/865f7009-6385-49b5-a4d9-4a2aadfd0271

[下载原始 MP4](docs/demo/dlc_win.mp4)

### 起源模式胜利（16 秒）

https://github.com/user-attachments/assets/58274a0f-9507-4ad1-b976-fd4e3be2d2e2

[下载原始 MP4](docs/demo/origin_win.mp4)

### DLC 模式购买流程（59 秒）

https://github.com/user-attachments/assets/3900ce41-a95b-427d-b455-19e44673f438

[下载原始 MP4](docs/demo/dlc_buy.mp4)

> 较长或重复的原始录像（4 分钟起源模式、1 分钟 DLC 备份）放在 [`archive/videos/`](archive/videos/)，作为资料归档。

---

## 技术难点与解决

- **多态结算的上下文设计**：`action(Bank* p1, Bank* p2, Card* c1, Card* c2, int val)` 用两个 `Bank*` 表达“谁付谁”，用 `c1` 非空表示 Shopping Mall 加成，用 `val` 表示倍率数量。
- **购买流程的返回值语义**：早期版本把“金币数 / 卡槽号”混在同一个返回值里，容易白拿卡或越界；重构后使用 `PurchaseCode` 枚举区分 `NO_PURCHASE` / `YELLOW_PURCHASE` / 普通卡槽。
- **卡槽索引失效**：购买后 `erase` 会导致下标变化，因此购买成功后重新打印市场，并基于实际 `slot.size()` 做边界检查。
- **AI 与人类共用回合流程**：通过 `isBot` 分支 + “买得起、非重复紫卡”的合法动作候选集，避免 AI 产生非法输入。
- **输入健壮性**：`normalize` + `parse_int` + `getline` 统一处理空白、EOF、空行、越界和非法命令，避免 `cin >>` 与 `getline` 混用。

---

## 简历文案

可直接复制到简历的项目描述、30 秒介绍和面试口述提纲见 **[docs/resume-project.md](docs/resume-project.md)**。

> 当前按“团队项目”撰写；角色与分工请按实际情况调整。

---

## 课程笔记与归档

- [课程笔记整理（Markdown）](docs/course-notes.md)：按 OOP、C++、UML、设计模式、STL、考点分类整理。
- [原始课程笔记（docx）](archive/course-notes/课程笔记.docx)：完整原始文件，保留不动。
- [UML 设计图](docs/uml/)：`UML.vsdx` / `UML.pdf` / `UML_黑白.pdf`。
- [原始配置截图](archive/original-screenshots/)：未裁剪的 Gitee 配置截图，保留原始版本。
- [旧版原型 v0.1](archive/legacy-v0.1/README.md)：2022-10 的早期实现，仅作版本演进记录。
- [完整演示录像](archive/videos/)：较长的原始录屏，作为归档。

---

## 构建与运行

### 方式一：Visual Studio

1. 打开解决方案：
   ```
   MachiKoro-master/MachiKoro.sln
   ```
2. 选择 `Debug|x64` 或 `Release|x64`。
3. 按 `F5` 或 `Ctrl+F5` 运行。

项目使用预编译头 `stdafx.h`，工具集为 `v143`（VS2022）。如果使用更早的 VS 版本，可能需要在项目属性里调整 `PlatformToolset`。

### 方式二：命令行编译（MinGW / g++）

在 `MachiKoro-master/MachiKoro` 目录下：

```bash
g++ -std=c++14 -I. Card.cpp Bank.cpp BlueCard.cpp GreenCard.cpp \
    RedCard.cpp PurpleCard.cpp YellowCard.cpp Deck.cpp Game.cpp \
    MachiKoro.cpp -o MachiKoro.exe
```

> 代码只依赖标准库和 Windows 控制台的 `system("cls")`，因此在 Linux/macOS 上编译时需要把清屏命令替换掉。

### 从 Gitee 拉取到 Visual Studio

![步骤 1-2：克隆仓库](docs/images/gitee/gitee-clone.png)
![步骤 3-4：提交并推送](docs/images/gitee/gitee-commit.png)
![步骤 5：推送成功](docs/images/gitee/gitee-push.png)

> 以上为按内容裁剪后的版本；原始未裁剪截图保留在 [`archive/original-screenshots/`](archive/original-screenshots/)。

---

## 目录结构

```text
Machikoro/
├─ MachiKoro-master/          # 当前可玩版本
│  ├─ MachiKoro.sln
│  └─ MachiKoro/
│     ├─ MachiKoro.cpp        # 程序入口与主循环
│     ├─ Game.h / Game.cpp    # 游戏流程、结算、购买、AI、输入解析
│     ├─ Card.h / Card.cpp    # 卡牌抽象基类
│     ├─ BlueCard.*           # 蓝色卡（初级产业）
│     ├─ GreenCard.*          # 绿色卡（二级产业）
│     ├─ RedCard.*            # 红色卡（餐饮）
│     ├─ PurpleCard.*         # 紫色卡（大型建筑）
│     ├─ YellowCard.*         # 黄色卡（地标）
│     ├─ Deck.h / Deck.cpp    # 牌堆：集中创建全部卡牌并洗牌
│     ├─ Bank.h / Bank.cpp    # 玩家钱包
│     ├─ ColorAndIcon.h       # 颜色与图标枚举
│     ├─ stdafx.*             # 预编译头
│     └─ targetver.h
├─ docs/
│  ├─ demo/                   # README 展示用短视频
│  │  ├─ origin_win.mp4
│  │  ├─ dlc_win.mp4
│  │  └─ dlc_buy.mp4
│  ├─ images/gitee/           # README 用的裁剪截图
│  │  ├─ gitee-clone.png
│  │  ├─ gitee-commit.png
│  │  └─ gitee-push.png
│  ├─ uml/                    # UML 设计图
│  │  ├─ UML.vsdx
│  │  ├─ UML.pdf
│  │  └─ UML_黑白.pdf
│  ├─ course-notes.md         # 课程笔记整理（Markdown）
│  ├─ rules.md                # 完整规则 / 命令 / 卡牌表
│  └─ resume-project.md       # 简历文案 / 面试提纲
├─ archive/
│  ├─ legacy-v0.1/            # 2022-10 旧版原型（仅源码入库）
│  ├─ course-notes/           # 原始课程笔记 docx
│  ├─ original-screenshots/   # 原始未裁剪截图
│  └─ videos/                 # 完整/备份录像
├─ cardImages/                # BMP 卡面素材（当前控制台版本未使用）
├─ .gitignore                 # 忽略 VS 构建产物
└─ README.md
```

> `archive/legacy-v0.1/` 中保留了旧版原型的完整本地文件，但 `.vs/`、`x64/`、`*.obj`、`*.exe`、`*.pdb` 等构建产物已被 `.gitignore` 排除，不会提交到 GitHub。

---

## 规则摘要

每回合：**掷骰 → 收入结算（红 → 蓝/绿 → 紫）→ 建造 1 张建筑**。率先建齐 4 座地标（火车站、商业中心、游乐园、广播塔）的玩家获胜。

完整的回合流程、颜色说明、命令列表和卡牌数值见 **[docs/rules.md](docs/rules.md)**。

---

## 代码设计

### 类关系

```mermaid
classDiagram
    class Card {
        <<abstract>>
        +action(Bank*, Bank*, Card*, Card*, int)
        +set_card(...)
        +get_name() string
        +get_cost() int
        +get_value() int
    }

    Card <|-- BlueCard
    Card <|-- GreenCard
    Card <|-- RedCard
    Card <|-- PurpleCard
    Card <|-- YellowCard

    BlueCard <|-- WheatField
    BlueCard <|-- Ranch
    BlueCard <|-- Forest
    BlueCard <|-- Mine
    BlueCard <|-- AppleOrchard

    GreenCard <|-- Bakery
    GreenCard <|-- ConvenienceStore
    GreenCard <|-- CheeseFactory
    GreenCard <|-- FurnitureFactory
    GreenCard <|-- FruitAndVegetableMarket

    RedCard <|-- Cafe
    RedCard <|-- FamilyRestaurant

    PurpleCard <|-- Stadium
    PurpleCard <|-- TVStation
    PurpleCard <|-- BusinessCenter

    YellowCard <|-- TrainStation
    YellowCard <|-- ShoppingMall
    YellowCard <|-- AmusementPark
    YellowCard <|-- RadioTower

    class Deck {
        +vector~Card*~ deck
        +shuffle()
    }

    class Bank {
        -int coins
        +deposit(int)
        +withdraw(int) int
        +get_coins() int
    }

    class player {
        +string name
        +bool isBot
        +Bank* bank
        +vector~BlueCard*~ blue_cards
        +vector~GreenCard*~ green_cards
        +vector~RedCard*~ red_cards
        +vector~PurpleCard*~ purple_cards
        +vector~YellowCard*~ yellow_cards
    }

    class Game {
        +turn
        +is_game_over
        +roll_dice()
        +red_card_check()
        +blue_card_check()
        +green_card_check()
        +purple_card_check()
        +buy_propery()
        +end_of_turn()
        +getInstance() Game*
    }

    Game --> Deck
    Game --> player
    player --> Bank
    player --> Card
```

> 更完整的 UML 图（含 Visio 源文件）见 [docs/uml/](docs/uml/)。

### 设计要点

- **多态结算**：`Card::action()` 是纯虚函数，不同颜色/卡牌在子类里实现自己的收付款逻辑。`Game` 只负责判断点数是否命中，然后调用 `action()`。
- **简单工厂**：`Deck` 构造函数集中创建全部卡牌，外部只拿到 `Card*` 列表。
- **单例**：`Game::getInstance()` 保证全局只有一个游戏实例；主循环通过它访问游戏状态。
- **迭代器**：`Game` 内部定义了一个基于 `vector<vector<Card*>>::iterator` 的迭代器，目前主要用于遍历市场；实际视图代码仍大量使用下标访问。
- **玩家钱包**：`Bank` 表示一名玩家的金币，不是中央银行；银行本身不模拟金币总量。
- **输入解析**：`Game::player_input()` 用正则表达式解析命令，`normalize()` 负责压缩空白，`ask_int()` 负责单行整数输入。

---

## AI 行为

AI 的设计目标不是“强”，而是“能自己完整打完一局”：

- 掷骰：拥有火车站时固定选择 2 颗骰子。
- 广播塔：点数 ≤ 4 时自动重掷一次。
- 建造：只会在“买得起、且不是已拥有的紫卡”的选项中随机选择。
- 地标：买得起就会纳入候选。
- 电视台：选择金币最多的其他玩家。
- 商业中心：随机选择目标玩家和双方各一张非紫卡进行交换。

AI 每回合同样只能建造 1 张，和人类玩家一致。

---

## 本次修复内容

本次主要针对“能玩、不崩溃、规则正确”做了一轮集中修复：

### 购买流程

- 修复黄卡购买返回值错误：原来会把“剩余金币数”当作卡槽下标返回，可能白拿卡或越界；现在使用明确的 `YELLOW_PURCHASE` 返回值。
- 修复 `slot[0]` 永远无法购买的问题。
- 修复“先扣钱、后校验”的问题：现在紫卡重复、金币不足都会在扣钱前拦截。
- 保持官方规则：**每回合只能建造 1 张**（普通建筑或地标）。
- 修复卡槽下标越界判断（`>` 改为 `>=`）。
- AI 不再使用固定 `rand() % slotnum`，而是基于实际市场大小选择。

### 卡牌结算

- 紫卡效果现在只在点数为 **6** 时触发。
- Stadium 不再向自己收钱，只向其他玩家收取。
- TV Station / Business Center 改用 `dynamic_cast` 识别，避免字符串名称不一致导致效果永远不触发。
- 修复 Business Center 交换界面里 `for` 循环条件写错变量（`i` / `k`）导致的死循环和越界。
- Shopping Mall 现在使用正确的地标下标（`yellow_cards[1]`），并作用于红卡/绿卡。
- 绿色卡的倍率改为读取 `get_multiplier_icon()`，修复奶酪工厂、家具工厂、果蔬市场此前倍率恒为 0 的问题。
- `Bank::withdraw()` 在余额不足时返回实际扣除的金币数，不再让金币凭空消失。
- 地标可以按任意顺序建造，移除了错误的前置条件。
- 游乐园只在掷出对子时额外获得回合。

### 输入与稳定性

- 修复 `voir table` 会触发 `stoi("table")` 异常的问题；现在 `voir table` / `voir tableau` 都可正常查看市场。
- 支持多位数卡槽编号（不再限制 0~9）。
- 统一使用 `getline` 输入，消除 `cin >>` 与 `getline` 混用导致的空行问题。
- 输入会先 trim + 压缩连续空格，减少“看起来一样但匹配不上”的情况。
- 增加 EOF 处理：管道输入结束时游戏会正常退出，不会无限循环。
- 主循环不再依赖 `system("pause")` 读取按键，改为等待回车，避免输入缓冲区残留换行。

### 代码质量

- `Card` 的成员变量增加默认初始化，避免未初始化值。
- `Card` 增加虚析构函数。
- `Card::set_card()` 声明与定义的 `cost/value` 参数顺序统一。
- `random_shuffle` 替换为 `std::shuffle` + `std::mt19937`。
- 清理了未使用的 `slot_count`、重复洗牌、主函数中的死代码。
- 增加空卡槽防护，避免访问 `slot[i][0]` 时越界。

---

## 验证记录

本次修改后做了以下验证（临时脚本已删除，不污染仓库）：

- 使用 `g++ -std=c++14 -fsyntax-only` 对全部 `.cpp` 做语法检查，通过。
- 使用 `g++ -std=c++14` 完整链接生成可执行文件，通过。
- 用脚本输入跑过 `dlc` 模式的人类对局，验证了市场查看、玩家查看、非法编号、买不起、成功购买、回合结束等流程。
- 用脚本输入跑过 1 人类 + 1 AI 对局，AI 能自动掷骰、重掷和购买。
- 临时单元测试验证了以下数值：
  - `Bank::withdraw()` 余额不足时返回实际扣款。
  - `CheeseFactory`：3 × 牛图标数量。
  - `FruitAndVegetableMarket`：2 × 谷物图标数量。
  - `Bakery`：基础 1 金币。
  - `Stadium`：从其他玩家各拿 2 金币。

---

## 已知限制与 TODO

- 仅支持 Windows 控制台：`system("cls")` 是 Windows 专用。
- 视图和游戏逻辑仍集中在 `Game` 中，后续可以拆成 `Game` + `View` / `Controller`。
- 所有卡牌、玩家、`player` 对象使用 `new` 创建，进程退出时由操作系统回收；若长期运行或做单元测试，建议补 `delete` 或改用智能指针。
- 单例实现不是线程安全的。
- Shopping Mall 目前对**所有**红卡和绿卡加 1；标准规则更细，只对 Restaurant / Bakery 类卡牌生效，后续可以按图标进一步区分。
- AI 只做“买得起就随机买”，没有针对对手卡组、骰子分布做策略优化。
- `cardImages/` 中的 BMP 素材尚未接入，当前是纯文本界面。
- `dlc` 只是“随机 10 种建筑”的市场变体，不是真正的扩展包规则。
- 尚无存档/读档、回放、单元测试工程；本次仅用临时脚本做了编译和冒烟验证。

---

## 开发环境

- 语言：C++（C++14 及以上）
- IDE：Visual Studio 2022（v143）
- 平台：Windows 控制台（Win32 / x64）
- 构建系统：MSBuild（`MachiKoro.sln`）
- 依赖：仅 C++ 标准库

---

## 备注

：这里可以随意留言 XD
