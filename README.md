# Лабораторные работы по курсу Алгоритмы и структуры данных

## Модуль 1

### A_sum. Задача про сумму

На стандартном потоке ввода задаётся последовательность целых чисел.
Каждое число последовательности не меньше `-200 000 000` и не больше `200 000 000`.
На стандартный поток вывода напечатайте сумму этих чисел.

### B_stack. Задача про стек

Реализуйте стек, используя только массив.  

**Формат входных данных**  
На стандартном потоке ввода задаётся последовательность команд. Пустые строки игнорируются.
Первая строка всегда содержит `set_size N`, где `N` - максимальный размер стека, целое число.
Каждая последующая строка содержит ровно одну команду: `push X`, `pop` или `print`, где `X` - произвольная строка без пробелов.  

**Формат результата**  
Команда `print` выводит содержимое стека (снизу вверх) одной строкой, значения разделяются пробелами. Если стек пуст, то выводится `empty`.  
В случае переполнения стека выводится `overflow`.  
Команда pop выводит элемент или `underflow`, если стек пуст.  
Память под стек должна быть выделена не более одного раза, при вызове команды `set_size`.  
В любой непонятной ситуации результатом работы любой команды будет `error`.  
Результат работы программы выводится в стандартный поток вывода.  

### C_queue. Задача про очередь

Это как задача про стек, только про очередь.  
Реализуйте очередь, используя только массив.  
Ввод и вывод данных осуществляется через файлы. Имена входного и выходного файлов задаются через аргументы командной строки (первый и второй соответственно).  

**Формат входных данных**  
Во входном файле задаётся последовательность команд. Пустые строки игнорируются.  
Первая строка всегда содержит `set_size N`, где `N` - максимальный размер очереди, целое число.  
Каждая последующая строка содержит ровно одну команду: `push X`, `pop` или `print`, где `X` - произвольная строка без пробелов.  

**Формат результата**  
Команда print выводит содержимое очередь (от головы к хвосту) одной строкой, значения разделяются пробелами. Если очередь пуста, то выводится `empty`.  
В случае переполнения очереди выводится `overflow`.  
Команда `pop` выводит элемент или `underflow`, если очередь пуста.  
Память под очередь должна быть выделена не более одного раза, при вызове команды `set_size`.  
В любой непонятной ситуации результатом работы любой команды будет `error`.    

### D_graph. Задача про обход графа

Реализуйте обход графа в ширину и глубину.  
Вершины упорядочены в лексикографическом порядке.  

**Формат входных данных**  
Первая строка стандартного потока ввода данных имеет формат `[graph_type] [start_vertex] [search_type]`,
где  
`[graph_type]` - тип графа, ориентированный (`d`) или неориентированный (`u`);  
`[start_vertex]` - идентификатор вершины, с которой начинать обход графа;  
`[search_type]` - тип обхода, в ширину (`b`) или в глубину (`d`).  
Каждая последующая строка содержит ребро, которая представляет собой идентификаторы начальной и конечной вершины, разделенные пробелом.  

**Формат результата**  
Результат работы программы выводится в стандартный поток вывода.  
Идентификаторы посещенных вершин выводятся по одному в строке в порядке обхода.  

## Модуль 2

### B_splay_tree. Задача про косое дерево

Реализуйте косое дерево (splay tree).  
Реализация самой структуры данных должна быть инкапуслирована, т.е. не зависеть от форматов входных/выходных данных и непосредственно ввода/вывода.  
Тесты предполагают "левостороннюю" реализацию, т.е. если действие можно реализовать двумя симметричными способами, надо делать тот, который больше использует левую сторону.  

**Формат входных данных**  
На стандартном потоке ввода задаётся последовательность команд. Пустые строки игнорируются.
Каждая строка содержит ровно одну команду: `add K V`, `set K V`, `delete K`, `search K`, `min`, `max` или `print`, где `K` - целое число (64 бита вам хватит), ключ, `V` - произвольная строка без пробелов (значение).  

**Формат результата**  
Команда `add` добавляет значение `V` в дерево по ключу `K`, `set` - изменяет данные по ключу, команда `delete` удаляет данные.  
Команда `search` выводит либо `1 V`, либо `0`, где `V` - значение для найденного ключа.  
Команды `min` и `max` выводят `K V`, где `K` - минимальный или максимальный ключ дерева соответственно, `V` - значение по этому ключу.  
Команда `print` выводит все дерево целиком. Она не изменяет дерево.  
Дерево выводится строго по уровням, слева направо, 1 строка - 1 уровень. Первая строка содержит только корень дерева в формате `[K V]` или `_`, если дерево пустое.  
Каждая последующая строка содержит один уровень дерева. Вершины выводятся в формате `[K V P]`, где `P` - ключ родительской вершины. Если вершина отсутствует, ставится `_`. Вершины разделены пробелом.  
В любой непонятной ситуации результатом работы любой команды будет `error`.  
Результат работы программы выводится в стандартный поток вывода.  

### C_min_heap. Непростая куча

Реализуйте двоичную min-кучу. Модифицируйте ее таким образом, чтобы внутреннее ее строение было таким же, но при этом доступ по ключу к любому элементу осуществлялся в среднем за константное время.  
Реализация самой структуры данных должна быть инкапуслирована, т.е. не зависеть от форматов входных/выходных данных и непосредственно ввода/вывода.  

**Формат входных данных**  
На стандартном потоке ввода задаётся последовательность команд. Пустые строки игнорируются.
Каждая строка содержит ровно одну команду: `add K V`, `set K V`, `delete K`, `search K`, `min`, `max`, `extract` или `print`, где `K` - целое число (64 бита вам хватит), ключ, `V` - произвольная строка без пробелов (значение).  

**Формат результата**  
Команда `add` добавляет значение `V` в кучу по ключу `K`, `set` - изменяет данные по ключу, команда `delete` удаляет данные.  
Команда `search` выводит либо `1 I V`, либо `0`, где `I` - индекс, `V` - значение для найденного ключа.  
Команды `min` и `max` выводят `K I V`, где `K` - минимальный или максимальный ключ кучи соответственно, `I` - индекс, `V` - значение по этому ключу.  
Команда `extract` извлекает корень кучи и выводит `K V`, где `K`, `V` - ключ и значение извлеченного элемента.  
Команда `print` выводит всю кучу целиком.  
Куча выводится строго по уровням, слева направо, 1 строка - 1 уровень. Первая строка содержит только корень кучи в формате `[K V]` или `_`, если куча пустая.  
Каждая последующая строка содержит один уровень кучи. Вершины выводятся в формате `[K V P]`, где `P` - ключ родительской вершины. Если вершина отсутствует, ставится `_`. Вершины разделены пробелом.  
В любой непонятной ситуации результатом работы любой команды будет `error`.  
Результат работы программы выводится в стандартный поток вывода.  

### D_trie. Автокоррекция

Реализуйте программу, которая предлагает варианты замены слова, в котором допущена одна ошибка.  
Для решения этой задачи реализуйте сжатое префиксное дерево.  
Регистр букв для программы коррекции не имеет значения (слова в словаре хранятся в нижнем регистре).  
Варианты ошибок - как в алгоритме Дамерау-Левенштейна: вставка лишнего символа, удаление символа, замена символа или транспозиция соседних символов.  
Реализация алгоритма должна быть инкапсулирована. В комментариях напишите сложность ключевых алгоритмов с пояснением.  

**Формат входных данных**  
Данные подаются на стандартный поток ввода. Пустые строки игнорируются.  
Первая строка содержит число `N` - количество слов в словаре.  
Последующие `N` строк содержат слова из словаря, по одному в строке.  
Остальные строки - слова, которые надо проверять.  

**Формат результата**  
Каждая строка выхода содержит предложение для исправления слова. Слова обрабатываются в порядке их появления.  
Если слово не содержит ошибок, то выводится `%слово% - ok`.  
Если слово содержит одну ошибку, то выводится `%слово% -> %слово_в_словаре%`. Если вариантов несколько, то они сортируются лексикографически и разделяются запятой с пробелом.  
Если слово содержит более одной ошибки, то выводится `%слово% -?`  
Результат работы программы выводится в стандартный поток вывода.  
