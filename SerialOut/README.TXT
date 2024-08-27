测试方法:
Serial Windows #1窗口，自行改变源代码，可以看一看用和不用信号量的区别。


注意:
(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,
您可以在Project->Options for Target "Target 1"的output页
点击Select Folder for Objects...和Listing页点击
Select Folder for Listing...改变.
(2)编译时会有4个警告,3个是禁止任务间变量覆盖分析引起,一个是未调用OSSendSignal函数引起，必须忽略。