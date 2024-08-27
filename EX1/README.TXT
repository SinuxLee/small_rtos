这是Small RTOS 51 的第一个例子,也是转简单的一个,它有4个任务,除第四个任务外,其它的任务只是不断等待不同时间而已.
在KEIL C51 工程中,我已经在每个任务中设好断点,并且在watch窗口中的watch #1页中可以看到各个任务的剩余嘀嗒数.
注意:(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,您可以在Project->Options for Target "Target 1"的output页点击Select Folder for Objects...和Listing页点击Select Folder for Listing...改变.
    (2)编译时会有一个警告,是因为没有使用一个系统函数OSSendSignal引起,它可以忽略.
    