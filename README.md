第N周作业涉及的函数定义在 ``sol_weekN.h`` 文件中

每个头文件包含解法函数、相应的测试函数 和 打包好的测试函数 ``test_all_csapp_weekN()``

想要进行测试，以week3为例，在main.c中引入/修改
```c++
 #include "sol_week3.h"
 #define TEST_FUNC test_all_csapp_week3()
```
即可进行代码测试

**测试代码和数据是AI写的~**