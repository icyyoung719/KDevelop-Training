## 总体说明
支持两种高效的字符串匹配算法：Boyer-Moore (BM) 和 Aho-Corasick (AC)。
项目的主要功能包括：

- 从指定的关键词文件加载关键词。
- 在指定的文本文件中搜索每个关键词的出现次数。
- 输出搜索结果到控制台或文件。
- 提供详细的时间统计，便于性能分析。
- 用户可以通过配置文件或运行时选择使用哪种搜索算法，以及指定输入和输出路径。
- 程序默认会根据关键字数目进行算法的抉择，关键词数较少时，使用 BoyerMoore 算法。；较多时，使用AhoCorasick算法
- 没有采用内存映射文件，因为文件1G，可以直接在载入内存

## 文件说明
- AhoCorasick.cpp / AhoCorasick.h

实现 Aho-Corasick 算法。支持构建多模式自动机并高效搜索所有关键词。

- BoyerMoore.cpp / BoyerMoore.h

实现 Boyer-Moore 算法的基本功能，包括跳跃规则。

- BoyerMooreSearch.cpp / BoyerMooreSearch.h

在 Boyer-Moore 算法的基础上扩展，支持多线程搜索，提升大规模数据处理性能。

- util.cpp / util.h

提供项目中的实用工具函数，如文件读取、关键词加载、配置初始化等。

- main.cpp

主程序入口，负责初始化搜索器、运行搜索任务，并处理用户交互。

- CMakeLists.txt

用于 CMake 的构建脚本，配置项目依赖、目标和编译选项。

- ReadMe.md

本说明文档，提供项目的功能描述、文件说明和使用指南。
## 搜索实现
### BoyerMoore算法
特点：

- 基于跳跃规则（坏字符规则和好后缀规则），快速跳过不可能匹配的区域。
适合少量关键词和较长的文本模式。
实现细节：

- 使用 BoyerMooreSearch 类封装了 BM 搜索逻辑，支持多线程并发搜索，提高了搜索效率。
线程池 的初始化和销毁是通过静态方法完成，避免了手动管理线程的复杂性。

- 搜索完成后自动释放位置，仅记录出现次数，防止占用过大

由于搜索的关键字数目较多，因此每一个关键字不再细分子线程，否则代价过大
### AhoCorasick算法
特点：

- 构建多模式搜索自动机，一次遍历即可完成所有关键词的匹配。
- 适合大量关键词和频繁模式匹配的场景。
- 保存了所有位置信息，没有做释放逻辑

## 开发思路

### 多线程支持
#### BM算法
- 每个keyword开一个线程，因此在keyword特别少如（2.3个）时效率会比较低，但是优于AC算法
- 在keyword特别多时，不如AC算法，因为AC算法可以直接搜索大量关键字
#### AC算法
- 由于算法并非一次搜索一个字符，因此不能在搜索完毕后清理匹配的位置信息
- 由于搜索大量keyword，因此不宜使用多线程搜索，否则判断边界的开销过大
- 在keyword特别多以至于匹配的位置信息过大时，应该选择是否清理该信息，在我的程序中，当keyword多于20000个时，便不会保留位置信息
## 测试
### BM
#### 15个keyword
Time to read keywords: 7.45015 seconds.
Time to initialize thread pool: 0.920548 seconds.
Time for keyword search: 1.6844 seconds.
Keywords num: 15
Average search time: 0.112293 seconds.
Total time: 10.0573 seconds.
#### 35个keyword
Time to read keywords: 4.72336 seconds.
Time to initialize thread pool: 0.964866 seconds.
Time for keyword search: 43.9414 seconds.
Keywords num: 35
Average search time: 1.25547 seconds.
Total time: 49.6315 seconds.
### AC
#### 15个keyword
Time to read keywords: 8.96459 seconds.
Time to initialize thread pool: 0.911866 seconds.
Time for keyword search: 5.8814 seconds.
Keywords num: 15
Average search time: 0.392093 seconds.
Total time: 15.7591 seconds.
#### 35个keyword
Time to read keywords: 3.18187 seconds.
Time to initialize thread pool: 0.906534 seconds.
Time for keyword search: 11.3463 seconds.
Keywords num: 35
Average search time: 0.32418 seconds.
Total time: 15.5237 seconds.
### 结论
- BM 算法在少量关键词下表现优越，搜索时间明显低于 AC。
- AC 算法在关键词数目增多后，随着预处理时间的摊薄，总体效率更高。