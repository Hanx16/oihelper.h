请务必使用 C++14 及以上版本的编译器。

使用 `#include "helper.hpp"` 来调用这个库。

## array

- `vector<T> genArray(int n, T L, T R)`

   返回一个长度为 `n` 的 `vector<T>`，值域为 $[L,R]$。`L, R` 可以为整型或 `double`。

- `vector<T> genArray_d(int n, T L, T R)`

   返回一个长度为 `n` 的 `vector<T>`，满足序列中的所有元素两两不同，值域为 $[L,R]$ 的整数。存在 `assert(R - L + 1 >= n)`。

- `vector<pair<T, T>> genPairs(int n, T L, T R)`

   返回一个长度为 $n$ 的 `vector<pair<T, T>>`，值域为 $[L,R]$。`L, R` 可以为整型或 `double`。

- `vector<pair<T, T>> genPairs_d(int n, T L, T R)`

   返回一个长度为 $n$ 的 `vector<pair<T, T>>`，值域为 $[L,R]$ 的整数，且内部所有二元组两两不同。存在 `assert((R - L + 1) * (R - L + 1) >= n)`。

- `vector<int> genPerm(int n)`

   返回一个长度为 $n$ 的 `vector<int>`，其中元素为一个排列，值域 $[1,n]$。

## graph

生成的图 / 树均为 0-index。

- 类 `Tree<class T = int>`：

   你可以使用 `Tree<int> T` 来声明一棵边权在 `int` 范围内的树。

   * `Tree(int n = 0, int maxDeg = 0, bool opt = false, int root = 0)`

      构建一棵有 $n$ 个节点，根节点为 `root`，每个节点最多有 `maxDeg` 个儿子的树。

      `opt = false` 时保证每个点的父亲节点都小于自己的编号，`opt = true` 时将随机打乱所有点的编号。

      注意，当 `root` 非 $0$ 时将把 `opt` 强制改为 `true`（即无视传入的 `opt` 参数）。

      当 `maxDeg = 0` 时将不限制每个节点的儿子数量。

      你可以不传入任何参数来新建一棵空树。

      边权集合默认为空。

   * `void buildTree(int n = 0, int maxDeg = 0, bool opt = false, int root = 0)`

      清空整棵树，重新建立一棵有 $n$ 个节点，根节点为 `root`，每个节点最多有 `maxDeg` 个儿子的树。

      参数含义同构造函数。

   * `void initEdge(vector<T> w = vector<T>())`

      初始化边权集，将树的边权集合设定为 `w`。保证边权集合中的每个元素只会出现在一条边的边权上。

      若 `w.size() < n - 1`，则用 `0` 补全剩余的位置；若 `w.size() > n - 1`，则仅会使用 `w` 中的前 `n - 1` 个元素。

      请注意 `buildTree` 清空整棵树的时候不会清空边权集。

   * `vector<tuple<int, int, T>> getEdge()`

      返回一个长度为 $n-1$ 的 `vector<tuple<int, int, T>>`。`tuple` 中的前两位表示一条边的两个端点，第三位表示这条边的边权。

   * `vector<int> getFather()`

      返回一个长度为 $n$ 的 `vector<int>`。令 `fa` 为这个函数的返回值，那么 `fa[i]` 表示 `i` 节点的祖先是 `fa[i]`。特别的，如果 `i` 为整棵树的根节点，则 `fa[i] = -1`。

- 类 `Graph<class T = int>`

   你可以使用 `Graph<T>` 来声明一个边权类型为 `T` 的图。

   * `void buildNormalGraph(int N, int M, bool RptEdge, bool SelfCircle, bool IsConnected, bool IsDirected)`

      建立一个 $n$ 个节点 $m$ 条边的图。

      `RptEdge` 表示是否允许重边，`SelfCircle` 表示是否允许自环，`IsConnected` 表示是否要求联通（有向图为弱联通），`IsDireceted` 表示图是否有向。

      比如你可以使用 `buildNormalGraph(10, 15, false, false, true, true)` 来生成一个 $10$ 个节点 $15$ 条边的无重边无自环的弱联通有向图。
   
   * `void buildDAG(int N, int M, bool RptEdge)`

      建立一个 $n$ 个节点 $m$ 条边的有向无环图。

      `RptEdge` 表示是否允许重边。

      比如你可以使用 `buildDAG(10, 15, false)` 来生成一个 $10$ 个节点 $15$ 条边的无重边的有向无环图。
   
   * `void initEdge(vector<T> w = vector<T>())`

      初始化边权集，将图的边权集合设定为 `w`。保证边权集合中的每个元素只会出现在一条边的边权上。
   
      若 `w.size() < m`，则用 `0` 补全剩余的位置；若 `w.size() > m`，则仅会使用 `w` 中的前 `m` 个元素。
   
      请注意重建图的时候不会清空边权集。
   
   * `vector<tuple<int, int, T>> getEdge()`
   
      返回一个长度为 $m$ 的 `vector<tuple<int, int, T>>`。`tuple` 中的前两位表示一条边的两个端点 $x,y$，第三位表示这条边的边权。当图为有向图的时候边的方向是 $x\to y$。

## Check

对拍，所有文件请使用标准输入输出流，必要时请不要解除流同步。

- `int diffFile(string file1, string file2)`

   传入文件名 `file1` 和 `file2`，比较两个文件内容（忽略换行符，多个空格按照单个空格算）。

   如果无差异返回 `0`，有差异返回 `1`，如果文件不存在返回 `-1`，并在标准错误流输出 `Cant find such file!`。

- `bool checkAnswer_ll(string file, int T, Data data, Bf bf)`

   传入要对拍的程序名 `file`（函数内使用 `system` 命令调用，因此需要保证你的程序名可以被命令行调用：如 linux 下填入 `./filename`），对拍组数 `T`。传入两个 lambda / 仿函数 `data, bf`，分别为数据生成器和暴力。

   数据生成器和暴力可以正常使用输入输出，但是请不要解除流同步。

   如果你的答案正确，函数会在对拍完成后在标准错误流输出 `Accepted!`，否则，将会输出 `Wrong answer on testcase i.`，其中 `i` 为出错的数据组数。此时你的文件目录下的 `input.txt, output.txt, answer.txt` 即为出错的数据。

- `bool checkAnswer_ls(string file, int T, Data data, string bf)`

   除 `bf` 外含义同 `checkAnswer_ll`。`bf` 传入暴力文件的文件名，要求同 `file`。`data` 请不要解除流同步。

- `bool checkAnswer_sl(string file, int T, string data, Bf bf)`

   传入 `data` 的文件名以及一个 lambda `bf`，`bf` 请不要解除流同步。

- `bool checkAnswer_ss(string file, int T, string data, string bf)`

   传入 `data` 和 `bf` 的文件名。

上述所有 `checkAnswer` 相关函数都是错误返回 `1`，正确返回 `0`。

## sampleChecker.cpp

一个辅助小程序，自动测试样例。

运行后会扫描当前目录下的所有 `.exe` 文件，选择一个作为测试程序。然后将会扫描目录下所有 `.in`，`.out`，`.ans` 文件，将文件名相同的一组 `.in` 和 `.out` / `.ans` 作为一组测试数据，并进行测试。如果一个 `.in` 文件同时存在 `.out` 和 `.ans` 文件与之对应，则优先使用 `.ans` 文件作为答案文件。

请使用 C++17 及以上进行编译，只支持 Windows。附带了一个编译好的文件 `sampleChecker.exe`，不清楚在其它电脑上是否可以正常运行。

使用方法：将 `sampleChecker.exe` 文件放到文件夹下，或者添加进环境变量。然后在命令行中运行 `sampleChecker.exe`。

对于一组测试点，将会返回 `Accepted`，`Wrong Answer`，`Runtime Error`，`Time Limit Exceed` 四种结果。默认超时时间为 `5000` 毫秒，可以通过修改 `sampleChecker.cpp` 开头的 `MaxTime` 来修改超时时间。

如果程序超时将会调用 `taskkill` 强制杀死进程，会误伤其他名字相同的进程。

对于你的 `.exe` 文件，请使用标准输入输出。

## diff.cpp

一个命令行工具，在命令行使用 `diff -H` 查看帮助。
