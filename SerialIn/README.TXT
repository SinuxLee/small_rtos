测试方法:
Recuve任务处理协议：数据包长度为NBYTE，起始字节为STARTBYTE1,STARTBYTE2，最后一个字节为检验和，中间字节不可能出现连续出现STARTBYTE1，STARTBYTE2。

宏EN_OS_Q_POST、EN_OS_Q_POST_FRONT、EN_OS_Q_INT_POST、EN_OS_Q_INT_POST_FRON、 EN_OS_Q_NMsgs的不同组合，程序执行有不同效果(清打开toolbox和serial #1窗口)，请用户自行测试。


注意:
(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,
您可以在Project->Options for Target "Target 1"的output页
点击Select Folder for Objects...和Listing页点击
Select Folder for Listing...改变.
(2)编译时会有3个警告,是禁止任务间变量覆盖分析引起,必须忽略.