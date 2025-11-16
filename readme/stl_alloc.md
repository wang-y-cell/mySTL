# 概览
- 当前 `stl_alloc.h` 已实现“第一级内存配置器”（基于 `malloc/free`）与“第二级内存配置器”（小块内存池/分级自由链表）。
- 组成：`__THROW_BAD_ALLOC` 宏、`malloc_alloc_template<int>`、`default_alloc_template<int>`、别名 `malloc_alloc`/`default_alloc`、以及按元素封装的 `simple_alloc<T, Alloc>`。

# 头文件与宏
- 头文件防卫：`include/stl_alloc.h:1-2`
- 依赖：`<cstddef>`, `<cstdlib>`, `<new>` 引入基础类型与分配函数，`include/stl_alloc.h:4-6`
- 条件宏 `__THROW_BAD_ALLOC` 定义：`include/stl_alloc.h:8-14`
  - 当未自定义 `__THROW_BAD_ALLOC` 时，使用打印错误并 `exit(1)` 的策略：`include/stl_alloc.h:12-13`
  - 若需要抛异常策略，可在外部或切换条件将其改为抛出异常（例如 `throw std::bad_alloc()`）。

# 命名空间
- 全部内容位于 `mystl` 命名空间：`include/stl_alloc.h:16`

# 第一级配置器模板
- 模板声明：`template <int inst> class malloc_alloc_template`，`include/stl_alloc.h:21-27`
  - 私有静态方法声明：`oom_malloc(size_t)` 与 `oom_realloc(void*, size_t)`，用于内存重试逻辑：`include/stl_alloc.h:24-25`
  - 静态函数指针成员 `__malloc_alloc_oom_handler` 保存 OOM 回调：`include/stl_alloc.h:26`
- 公有接口：`include/stl_alloc.h:28-50`
  - `allocate(std::size_t n)`：先尝试 `malloc(n)`，失败时调用 `oom_malloc(n)` 重试，`include/stl_alloc.h:29-33`
    - 参数：`n` 字节数
    - 返回：成功分配的指针，失败走 OOM 回退
    - 用法：`void* p = malloc_alloc::allocate(128);`
  - `deallocate(void* p, std::size_t)`：直接 `std::free(p)`，`include/stl_alloc.h:35-37`
    - 参数：`p` 指针；第二参数为字节数（未使用）
    - 用法：`malloc_alloc::deallocate(p, 128);`
  - `reallocate(void* p, std::size_t, std::size_t new_sz)`：使用 `realloc`，失败时调用 `oom_realloc(p, new_sz)`，`include/stl_alloc.h:39-43`
    - 参数：`p` 旧指针；第二参数旧字节数（未用）；`new_sz` 新字节数
    - 返回：新指针；失败走 OOM 回退（当前未定义将链接失败）
    - 用法：`p = malloc_alloc::reallocate(p, old_sz, new_sz);`
  - `set_malloc_handler(void (*f)())`：设置 OOM 处理器并返回旧处理器，`include/stl_alloc.h:45-50`
    - 参数：`f` 新处理器（`void (*)()`）
    - 返回：旧处理器指针
    - 用法：`auto old = malloc_alloc::set_malloc_handler(&my_oom);`

# 静态成员定义与 OOM 实现
- 静态成员定义：将 `__malloc_alloc_oom_handler` 初始化为空指针，`include/stl_alloc.h:62-63`
- `oom_malloc` 实现：`include/stl_alloc.h:65-76`
  - 参数：`n` 欲分配的字节数
  - 返回：成功分配的非空指针
  - 不断读取当前处理器 `__malloc_alloc_oom_handler`：`include/stl_alloc.h:70`
  - 若处理器不存在，触发 `__THROW_BAD_ALLOC`：`include/stl_alloc.h:71`
  - 调用处理器后再次尝试 `malloc(n)`，成功则返回：`include/stl_alloc.h:72-75`
  - 用法：由 `allocate(n)` 在失败时内部调用，无需直接使用
- `oom_realloc`：已声明但当前文件未定义（`include/stl_alloc.h:25` 与 `include/stl_alloc.h:39-43`），若运行时触发将导致链接错误，需要补全与 `oom_malloc` 类似的重试逻辑。
- 第二级内存配置器：已实现（`default_alloc_template<inst>`），对齐 8 字节、管理 `<=128` 字节的小块（共 `NFREELISTS=16` 条自由链表），大于 128 字节回退到一级；详见 `include/stl_alloc.h:122-231`。

# 默认别名
- 提供 `typedef malloc_alloc_template<0> malloc_alloc`，作为默认实例类型使用：`include/stl_alloc.h:78`
  - 不同 `inst` 专门化拥有独立的静态 OOM 处理器，做到“按实例隔离状态”。

# 第二级配置器：default_alloc_template<inst>
- 概览：小块内存池 + 分级自由链表，`ALIGN=8`、`MAX_BYTES=128`、`NFREELISTS=16`，位置 `include/stl_alloc.h:122-131`
- 静态成员：`free_list`（表头数组，`obj* volatile`）、`start_free/end_free`（池边界）、`heap_size`（扩容累计），位置 `include/stl_alloc.h:123, 166-176`
- 工具函数：`ROUND_UP(bytes)` 与 `FREELIST_INDEX(bytes)`，位置 `include/stl_alloc.h:128-129`
- 公有接口：
  - `allocate(std::size_t n)`：小块走自由链表，大块委托一级，位置 `include/stl_alloc.h:133-141`
  - `deallocate(void* p, std::size_t n)`：头插回链，位置 `include/stl_alloc.h:143-148`
  - `reallocate(void* p, std::size_t old_sz, std::size_t new_sz)`：同类块直接返回；否则新块拷贝后释放旧块，位置 `include/stl_alloc.h:149-160`
- 私有逻辑：
  - `refill(size_t n)`：向对应自由链表批量补仓，位置 `include/stl_alloc.h:168-190`
  - `chunk_alloc(size_t size, int& nobjs)`：从池切块；不足时向系统申请并可能回退一级，位置 `include/stl_alloc.h:195-231`
- 别名：`typedef default_alloc_template<0> default_alloc;` 位置 `include/stl_alloc.h:232`
- 用法：`auto* p = simple_alloc<int, default_alloc>::allocate(32); simple_alloc<int, default_alloc>::deallocate(p, 32);`
- 说明：`volatile` 仅抑制优化，非线程安全；`bytes==0` 不适用该映射，元素适配层已避免（`simple_alloc::allocate(n)` 在 `n==0` 返回空指针）。

# simple_alloc 封装
- 模板 `simple_alloc<T, Alloc>`：按元素类型数量进行分配与释放的适配层，`include/stl_alloc.h:80-97`
  - `allocate(std::size_t n)`：分配 `n * sizeof(T)` 字节并返回 `T*`，`include/stl_alloc.h:82-84`
    - 参数：元素个数 `n`
    - 返回：`T*` 指针（`n>0` 时），`n==0` 返回空指针
    - 用法：`auto* arr = simple_alloc<int, malloc_alloc>::allocate(10);`
  - `allocate()`：分配单个元素大小，`include/stl_alloc.h:86-88`
    - 返回：`T*` 指针
  - `deallocate(T* p, std::size_t n)`：按总字节数释放，`include/stl_alloc.h:90-92`
    - 参数：指针 `p` 与元素个数 `n`
    - 用法：`simple_alloc<int, malloc_alloc>::deallocate(arr, 10);`
  - `deallocate(T* p)`：单元素释放，`include/stl_alloc.h:94-96`
    - 参数：指针 `p`

# 使用示例
- 设置 OOM 处理器：
  - `malloc_alloc::set_malloc_handler(&my_oom_handler)`，其中 `my_oom_handler` 为 `void (*)()` 类型
- 分配/释放：
  - 字节级：`void* p = malloc_alloc::allocate(128); malloc_alloc::deallocate(p, 128);`
  - 按元素：`auto* arr = simple_alloc<int, malloc_alloc>::allocate(10); simple_alloc<int, malloc_alloc>::deallocate(arr, 10);`

# 设计要点
- 非类型模板参数 `inst`：用于生成彼此独立的类型实例，使每个实例拥有独立的静态 OOM 处理器，`include/stl_alloc.h:21, 62-63`
- OOM 策略可配置：通过 `set_malloc_handler` 动态切换回调，结合条件宏控制默认行为，`include/stl_alloc.h:45-50, 8-14`
- 简单适配层：`simple_alloc` 为容器或算法提供“按元素”的分配接口，避免直接面向字节大小，`include/stl_alloc.h:80-97`

# 注意事项与建议
- 宏 `__THROW_BAD_ALLOC` 的抛异常写法建议显式使用 `throw std::bad_alloc()`，并包含 `<new>`，避免未限定名或少括号导致编译问题，`include/stl_alloc.h:8-14`
- 统一使用 `nullptr` 替代 `0` 以提升可读性与类型安全（如静态指针初始化与 `simple_alloc` 的空返回），`include/stl_alloc.h:63, 83`
- 补全 `oom_realloc` 的定义，保证 `reallocate` 在失败路径下可正确链接并工作，`include/stl_alloc.h:25, 39-43`
- 第二级内存配置器实现后，建议：
  - 提供对齐策略与分级块管理，降低小对象分配的碎片与系统调用频次
  - 增加统计与诊断接口，便于定位内存热点与泄漏风险
  - 明确线程安全策略（锁粒度或无锁结构）以及与标准分配器接口的适配
- 在头文件中直接 `exit(1)` 会使调用方无法恢复，生产代码更推荐抛异常或将策略委托给可配置的处理器。

# 小结
- 文件实现了一个可配置的、基于 `malloc/free` 的第一级内存配置器，并提供了元素级的适配器接口，便于容器集成与抽象。
- 通过非类型模板参数隔离静态状态、通过 OOM 处理器实现重试策略，整体结构与早期 SGI STL 设计一致；建议按现代 C++风格进行若干小改进以提升健壮性与可维护性。