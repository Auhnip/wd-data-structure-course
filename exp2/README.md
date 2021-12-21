# 实验二 栈和队列的应用

## 停车场模拟

利用栈实现单道停车场，也就是该停车场先进来的车只能最后出，如果某辆车想出车道，必须先把其前面的车先退出栈，等该车开走，再将之前的车压入栈。输入文件如附件文件中`data.txt`所示，其中格式如：`COOLONE arrives`，表示`COOLONE`的车到达；输出如文件`output.txt`所示，其中格式为：`COOLONE was moved 0 times while it was here`，表示该车在离开之前没有移动过
