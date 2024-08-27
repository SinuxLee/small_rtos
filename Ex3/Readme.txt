这个例子是例子2的另一种写法（但任务数量不同）,它使用了最高优先级任务作为显示任务,全局变量ShowChar作为参数.函数TaskB是优先级1~10的任务函数,因为没有使用局部变量,这些任务可以用一个函数.
注意:(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,您可以在Project->Options for Target "Target 1"的output页点击Select Folder for Objects...和Listing页点击Select Folder for Listing...改变.
感谢网友John X. Liu,在这个例子中我使用了他编写的仿真dll(Pc51Emu.dll),这个dll原来是为他移植的Ucos-ii for keil c51的例子编写.
    (2)Pc51Emu.dll在KEIL C51 V6.23 V7.00工作不正常.
