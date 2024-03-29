# 如何运行作业

- 安装[CMake](https://cmake.org/download/)，版本大于等于3.16
  - 如果使用的 IDE 为 Visual Studio，也可用内置的 CMake
- 对于使用 VS Code 的同学，需要安装 VS Code 的 CMake 插件，并按网上教程配置环境变量、C/C++编译器等
- 直接用 IDE 打开此文件夹，一般会自动加载项目
  - 使用 Visual Studio 的同学，注意在 `项目 / hw6 project的CMake设置` 中点击”编辑JSON“，更改 `generator` 为本机适合的版本（如 "Visual Studio 16 2019 Win64" ），使用默认的 "Ninja" 作为 `generator` 可能有未知问题
  - 使用 Mac OS 或 Linux 的同学请使用 VS Code 或其他 IDE 打开
- 无需安装其他依赖，CMake 构建成功后可运行

# 需要补全的代码

- 标记为 `[[optional]]` 的是扩展要求

## Geometry.cpp

```cpp
double Point::distance(const LineString *line) const;
double Point::distance(const Polygon *polygon) const;

// [[optional]] 实现 Polygon 的内环几何数据存储，并修改 Point 到 Polygon 的欧式距离计算

[[optional]]
double LineString::distance(const LineString *line) const
[[optional]]
double LineString::distance(const Polygon *polygon) const

// [[optional]] 添加 MultiPoint、MultiLineString 和 MultiPolygon 类，同时给出正确性的证明

bool Envelope::contain(const Envelope &envelope) const;
bool Envelope::intersect(const Envelope &envelope) const;
Envelope Envelope::unionEnvelope(const Envelope &envelope) const;
[[optional]]
bool Polygon::intersects(const Envelope &rect) const
```

## QuadTree.cpp

```cpp
void QuadNode::split(size_t capacity);
bool QuadTree::constructTree(const std::vector<Feature> &features);
void QuadNode::rangeQuery(const Envelope &rect,
                          std::vector<Feature> &features);
void QuadTree::rangeQuery(const Envelope &rect,
                          std::vector<Feature> &features);
bool QuadTree::NNQuery(double x, double y,
                       std::vector<Feature> &features);
QuadNode *QuadNode::pointInLeafNode(double x, double y);
QuadNode *QuadTree::pointInLeafNode(double x, double y);

// [[optional]] 添加接口，实现 Spatial Join，同时给出正确性的证明
```

## QuadTreeTest.cpp

- 用于对 `QuadTree` 进行测试和性能分析

```cpp
[[optional]]
void QuadTree::analyse(); // 用于与R树进行比较，选择此扩展项需要同时实现R树的analyse()
```

## RTreeSrc.h

- `RTree` 类是模板类，所以实现写在头文件里
- 编码前，使用 VS 的模板实例化功能载入模板参数（“添加所有的现有实例化” > 选择“M=8”），才能在 VS 中启用代码补全等功能
- 实际使用时，M可以在 `Common.h` 中进行改变，默认为8

```cpp
virtual bool hw6::RTree<M>::constructTree(const std::vector<Feature>& features)
void hw6::RNode<M>::rangeQuery(const Envelope & rect,
                               std::vector<Feature>& features);
virtual bool hw6::RTree<M>::NNQuery(double x, double y,
                                    std::vector<Feature>& features)
RNode<M>* hw6::RNode<M>::pointInLeafNode(double x, double y);

// [[optional]] 添加接口，实现 Spatial Join，同时给出正确性的证明
```

## RTreeTest.h

- 用于对 `RTree` 进行测试和性能分析

```cpp
[[optional]]
static void hw6::RTree<M>::analyse(); // 用于与四叉树进行比较，选择此扩展项需要同时实现四叉树的analyse()
```

## 注意

- 代码中包含中文注释。所有中文注释都是 GB 编码的，需使用支持 GB 编码的编辑器打开。如果你在 Mac 或 Linux 上运行程序，可能需要先进行转码
- 运行时使用 四叉树 还是 R树 由有无定义宏 `USE_RTREE` 决定。`USE_RTREE` 位于 `Common.h` 中，默认为使用四叉树
