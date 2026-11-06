# 课程笔记整理：C++ / 面向对象 / UML

> 本文是对 [原始课程笔记](../archive/course-notes/课程笔记.docx) 的结构化整理，便于检索和复习；原始 docx 完整保留在 `archive/course-notes/`。
>
> 课程以法语授课、中文辅助，本文保留常见法语术语，方便和原笔记对照。

---

## 0. 课程信息

成绩构成（笔记原文为 105 分制）：

| 项目 | 占比 |
| --- | ---: |
| 期中考试 | 35% |
| 课程项目 | 30% |
| 期末考试 | 40% |

课程主线：

```text
面向对象基础 → C++ 语法 → 类与对象 → 继承与多态
→ UML → 设计模式（Singleton / Iterator）→ STL → 习题与考点
```

---

## 1. 面向对象基础

- **对象（objet）**：现实事物在程序中的抽象，包含属性（attribut）和方法（méthode）。
- **实例（instance）**：由类创建出来的具体对象。
- **抽象（abstraction）**：只保留与问题相关的特征，降低问题复杂度。
- **封装（encapsulation）**：隐藏内部实现，只暴露必要接口。
- **类（classe）**：对象的模板；对象是类的实例。

面向对象的主要优点：

- 稳定性（stabilité）
- 代码复用（réutilisation）
- 可扩展性（extension）

---

## 2. C++ 语言基础

### 2.1 编译流程

```text
Source.cpp
  → 预处理器（préprocesseur）
  → 翻译单元（unité de traduction）
  → 编译（compilation）
  → 目标文件 .o / .obj
  → 链接（link）
  → 可执行文件 .exe
```

### 2.2 数据类型

| 类型 | 典型大小 |
| --- | ---: |
| `int` | 4 byte |
| `char` | 1 byte（-128 ~ 127） |
| `float` | 4 byte |
| `double` | 8 byte |
| `long int` | 8 byte |
| `long double` | 16 byte |

`1 byte = 8 bit`。

### 2.3 变量与初始化

- **声明（déclaration）**：告诉编译器变量/函数存在，并给出名字。
- **定义（définition）**：真正分配存储/给出实现。
- **初始化（initialisation）**：定义变量时同时给出初始值。
- **赋值（affectation）**：变量已经存在后，修改它的值。

C++ 支持列表初始化：

```cpp
int a{0};
int b = {0};
```

`auto` 可以根据初始化表达式自动推导类型。

### 2.4 命名空间与头文件

- `namespace` 用于避免不同模块之间的命名冲突。
- 头文件保护：

```cpp
#ifndef TD1_FILENAME_H
#define TD1_FILENAME_H

// declarations

#endif
```

或更简洁的：

```cpp
#pragma once
```

> 在 `.h` 头文件中直接定义的函数默认是 `inline` 的。

### 2.5 指针、引用与 const

指针：

```cpp
int a = 1;
int* p = &a;   // p 保存 a 的地址
*p = 43;       // 通过指针修改 a
p = nullptr;   // 空指针
if (p) { /* ... */ }
```

`const` 与指针的两种常见组合：

| 写法 | 含义 | 不能做什么 |
| --- | --- | --- |
| `const int* p` | 指向常量的指针 | 不能通过 `*p` 修改值 |
| `int* const p` | 常量指针 | 不能修改 `p` 指向的地址 |

引用：

```cpp
int a = 1;
int& b = a;    // b 是 a 的别名
```

- 普通引用不能绑定表达式，也不能做隐式类型转换。
- `const T&` 可以绑定临时值/表达式，常用于函数参数。

`const` 的位置：

- 放在函数参数前：函数内不能修改该参数。
- 放在成员函数末尾：该成员函数不能修改成员变量。
- 放在返回值前：返回值不能被修改。

### 2.6 类型转换

```cpp
static_cast<int>(x);
static_cast<double>(x);
const_cast<Type*>(ptr);   // 去掉 const 属性
```

`const` 与 `constexpr`：

- `const` 变量的值可以在运行时确定。
- `constexpr` 要求在编译期确定。
- 笔记中的类比：`constexpr` 类似 Dart 的 `final`。

### 2.7 函数

- **函数重载（surcharge）**：同名函数，参数列表不同。
- **内联函数（inline）**：编译期把函数体展开，减少调用开销；用空间换时间。虚函数不建议使用 inline。

### 2.8 常用容器

| 容器 | 特点 |
| --- | --- |
| `vector<T>` | 动态数组，支持随机访问 |
| `list<T>` | 双向链表 |
| `map<K, V>` / `unordered_map` | 键值对 |
| `set<T>` | 集合，默认有序 |

`vector` 常用操作：

```cpp
vector<int> v;
v.push_back(1);
v.pop_back();
v.erase(v.begin() + i);
v.size();
auto it = find(v.begin(), v.end(), value);   // <algorithm>
```

`string` 常用操作：

```cpp
str.c_str();   // 返回 const char*
```

---

## 3. 类与对象

### 3.1 访问控制

- `class` 的成员默认是 `private`；`struct` 的成员默认是 `public`。
- `public`：类外可以访问。
- `private`：只有类内部可以访问。
- `protected`：类内部和子类可以访问，类外对象不能直接访问。

### 3.2 构造函数与初始化

```cpp
class Complexe {
private:
    int re;
    int im;
public:
    Complexe(int r, int i) : re(r), im(i) {}   // 初始化列表
};
```

- **初始化列表**：在对象创建时直接初始化成员。
- **函数体赋值**：先默认构造，再赋值，通常效率更低。
- `explicit`：阻止构造函数的隐式类型转换。

```cpp
explicit Complexe(int r);
```

### 3.3 const、static、friend

`const` 成员函数：

```cpp
double norm() const;   // 不能修改成员变量
```

`static`：

- 静态属性属于整个类，被所有对象共享。
- 静态方法没有 `this` 指针。
- 通过 `ClassName::method()` 调用。

`friend`：

- 友元函数/友元类可以访问另一个类的 `private` 成员。
- `operator<<` 常需要声明为友元，因为 `ostream&` 占据第一个参数位置。

```cpp
friend ostream& operator<<(ostream& os, const Complexe& c);
```

### 3.4 运算符重载

二元运算符（`+`、`-`、`=`、`==` 等）有两种写法：

- 成员函数：只传一个参数，可以使用 `this`。
- 非成员函数：传两个参数；如果需要访问私有成员，声明为 `friend`。

输出/输入流重载：

```cpp
ostream& operator<<(ostream& os, const Complexe& c) {
    os << c.re << " + " << c.im << "i";
    return os;
}
```

### 3.5 异常与断言

```cpp
assert(condition);
assert(condition && "message");

try {
    // ...
}
catch (const Exception& e) {
    // ...
}

void f() noexcept;   // 承诺不抛出异常
```

### 3.6 动态内存

```cpp
int* p = new int[n];
delete[] p;

Complexe* c = new Complexe(1, 2);
delete c;
```

- `new` 必须配 `delete`。
- `new[]` 必须配 `delete[]`。
- 动态数组的空间需要在析构函数中释放。

### 3.7 枚举

```cpp
enum Type { a, b, c };   // 默认 0, 1, 2
```

遍历枚举的常见写法：

```cpp
static const Type All[] = { a, b, c };
for (auto item : All) {
    // ...
}
```

---

## 4. 继承与多态

### 4.1 继承

```cpp
class Child : public Parent {
    // ...
};
```

- `public` 继承：父类的 `public`/`protected` 成员在子类中保持原有可见性。
- `private` 继承：父类的 `public`/`protected` 成员在子类中变成 `private`。
- `protected` 继承：父类的 `public`/`protected` 成员在子类中变成 `protected`。
- 构造函数、拷贝构造函数、`operator=` **不会**被继承；需要时显式调用父类版本：

```cpp
B& B::operator=(const B& b) {
    A::operator=(b);
    // ...
    return *this;
}
```

### 4.2 多重继承

```cpp
class C : public A, public B {
    // ...
};
```

如果 A、B 有同名成员，需要消歧：

```cpp
obj.A::f();
obj.B::f();

using A::f;   // 明确使用 A 的版本
```

### 4.3 向上转型与向下转型

- 子类指针可以赋给父类指针（向上转型，安全）。
- 父类指针不能隐式赋给子类指针（向下转型，需要显式转换并确保对象确实是子类）。
- `static_cast` 可用于有继承关系的类型转换。

### 4.4 虚函数与多态

```cpp
class Card {
public:
    virtual void action() = 0;   // 纯虚函数
};

class WheatField : public Card {
public:
    void action() override { /* ... */ }
};
```

- **静态多态**：函数重载，编译期决定调用哪个函数。
- **动态多态**：父类指针/引用 + 虚函数，运行期决定调用哪个子类实现。
- 只有重写**虚函数**才有多态；重写非虚函数不会有多态。
- 虚函数不建议声明为 `inline`。
- 父类析构函数应为虚函数（本项目 `Card` 已加虚析构，避免通过基类指针释放派生对象时行为不正确）。

---

## 5. UML

### 5.1 可见性符号

| 符号 | 含义 |
| --- | --- |
| `-` | private |
| `+` | public |
| `#` | protected |
| `$` | static |

### 5.2 类间关系

| 关系 | 图示 | 含义 |
| --- | --- | --- |
| 关联（association） | 直线/箭头 | 一个类使用另一个类，关系较弱 |
| 聚合（agrégation） | 空心菱形 | 整体包含部分，但不负责部分的生命周期 |
| 组合（composition） | 实心菱形 | 整体负责部分的创建/销毁，生命周期强绑定 |
| 继承（héritage） | 实线 + 空心三角 | 子类指向父类 |
| 依赖/实例 | 虚线 + 箭头 | 临时使用/实例关系 |

笔记中的例子：

- `Jeu` **组合** `Carte`：由 `Jeu` 管理 `Carte` 的生命周期。
- `Pioche` **聚合** `Carte`：`Pioche` 包含 `Carte`，但不负责其生命周期。
- `Plateau` **组合** `Carte`。

### 5.3 其他约定

- 多重性：`1`、`0..1`、`M..N`。
- `enum` 类型也要画进 UML。
- UML 类图中通常不画构造函数和析构函数。

---

## 6. 设计模式

### 6.1 Singleton（单例）

目标：保证一个类只有一个实例，并提供全局访问点。

核心写法：

```cpp
class Singleton {
private:
    static Singleton* instance;
    Singleton();
    Singleton(const Singleton&) = delete;
public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
};
```

要点：

- 构造函数、拷贝构造函数私有化/禁用。
- 实例由类自己管理。
- 如果只允许某个类创建实例，可以把那个类声明为 `friend class`。
- 笔记提到单例有“三种方式”，本质都是控制实例的创建和生命周期。

### 6.2 Iterator（迭代器）

目标：不暴露容器内部结构，用统一方式遍历元素。

```cpp
for (auto it = v.begin(); it != v.end(); ++it) {
    // *it
}
```

一个自定义迭代器通常需要：

- `begin()` / `end()`
- `currentItem()`
- `isDone()`
- `next()`
- `operator++`
- `operator!=`

`iterator` 与 `const_iterator`：

- `iterator` 可以读/改所指元素。
- `const_iterator` 只能读。
- `cbegin()` / `cend()` 返回 `const_iterator`。

---

## 7. 期中/期末考点整理

### 7.1 高频概念

- 封装的意义：隐藏实现、降低耦合、保护数据。
- Singleton 的意义：全局唯一实例、受控访问。
- Iterator 的意义：统一遍历方式、隔离容器实现。
- `explicit` 的作用：避免构造函数的隐式类型转换。
- `const` / `static` / `constexpr` 的区别。
- 初始化（initialisation）与赋值（affectation）的区别。
- 浅拷贝与深拷贝的区别。
- 为什么需要自定义析构/拷贝构造/赋值运算符（Rule of Three）。
- 头文件保护的作用。
- UML 可见性符号与类间关系。

### 7.2 易错点

- `if (a = b)` 是赋值，不是比较；应使用 `==`。
- 父类指针不能隐式转换为子类指针。
- 重写非虚函数不会产生多态。
- 默认拷贝构造是浅拷贝；类中有指针（动态内存）时要写深拷贝。
- 静态方法不能使用 `this`。
- `const` 成员函数不能修改成员变量。
- `new[]` 要配 `delete[]`，不能配 `delete`。
- 虚函数不建议 `inline`。
- 动态数组/指针成员要在析构函数中释放。

### 7.3 常见简答题

- `l'intérêt de l'encapsulation`：封装的意义。
- `l'intérêt du design pattern Singleton`：单例的意义。
- `l'intérêt du design pattern Iterator`：迭代器的意义。
- `Pourquoi utiliser explicit ?`：避免隐式转换。
- `Pourquoi utiliser const ?`：防止意外修改。
- 初始化列表与函数体赋值的区别。
- 浅拷贝与深拷贝的区别。

---

## 8. 和本项目的对应关系

这部分是把课程知识映射到本仓库，方便在简历/答辩时说明“我用了哪些知识点”。

| 课程知识点 | 项目中的位置 |
| --- | --- |
| 类与对象、封装 | `Card`、`Bank`、`player`、`Game` |
| 继承 | `BlueCard` / `GreenCard` / `RedCard` / `PurpleCard` / `YellowCard` 继承 `Card` |
| 动态多态 | `Card::action()` 纯虚函数 + 各卡牌子类重写 |
| 组合/聚合 | `Game` 持有 `Deck*` 和 `vector<player*>`；`player` 持有 `Bank*` 和卡牌指针 |
| Singleton | `Game::getInstance()` |
| Iterator | `Game::iterator`、`vector` 的 `begin()/end()` 遍历 |
| 集中创建（简单工厂思想） | `Deck` 构造函数集中创建全部卡牌 |
| 头文件保护 | 所有头文件使用 `#pragma once` |
| 枚举 | `Color`、`Icon`、`player::YellowSlot` |
| UML | 根目录 `UML.vsdx` / `UML.pdf` |
| 异常/输入校验 | `parse_int()` 的 `try/catch`、购买与命令校验 |
| const/引用 | getter、`const` 方法、`const string&` 参数 |
| 运算符重载 | `Deck.cpp` 中 `Color` / `Icon` 的 `operator<<` |
| 动态内存 | 卡牌、玩家、牌堆使用 `new` 创建 |

---

## 9. 法语术语速查

| 法语 | 中文 |
| --- | --- |
| objet | 对象 |
| classe | 类 |
| instance | 实例 |
| attribut | 属性 |
| méthode | 方法 |
| constructeur / destructeur | 构造函数 / 析构函数 |
| héritage | 继承 |
| polymorphisme | 多态 |
| encapsulation | 封装 |
| pointeur | 指针 |
| référence | 引用 |
| surcharge | 重载 |
| exception | 异常 |
| assertion | 断言 |
| association | 关联 |
| agrégation | 聚合 |
| composition | 组合 |
| visibilité | 可见性 |
| initialisation / affectation | 初始化 / 赋值 |
| copie superficielle / profonde | 浅拷贝 / 深拷贝 |
| fonction virtuelle | 虚函数 |
| classe abstraite | 抽象类 |

---

## 10. 原始笔记

- [课程笔记.docx](../archive/course-notes/课程笔记.docx)
- 原始笔记共 1219 行，本文是结构化摘要，不替代原文；细节和课堂例子请以原文为准。
