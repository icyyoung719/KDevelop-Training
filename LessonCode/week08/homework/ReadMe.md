

## 代码编写过程中的一些问题记录
### 开始：环境的配置
1. tablet pc sdk 其实是windows自带的，已经融合到windows sdk中了，不需要再安装
### 起步：winrt的尝试
1. winrt下 auto inkPoints = winrt::single_threaded_vector<winrt::Windows::UI::Input::Inking::InkPoint>(); 一直出现线程错误
    - 原因：winrt::single_threaded_vector是异步的，与qt同步机制不太一样，我了解的也不多
    - 解决：使用QFutureWatcher来解决异步问题，QFuture<QString> future = recognizeInkAsync();异步执行识别操作
2. winrt指挥给出一个识别结果，不知道怎么获取多个识别结果，因此换用msinkaut.h
### 进阶：msinkaut.h的尝试
1. msinkaut.h的调试问题
    - 问题：msinkaut.h的利用windows的.dll文件动态链接，我不会调试，无法在调试中监测一些结构体
    - 解决：在VC的项目中调试-选项-符号中载入msinkaut.h的pdb文件，但是还是无法监测
    - 妥协：在代码中加入一些输出，例如：qDebug()<<inkDisp->GetStrokes()->Count;来查看一些信息
2. msinkaut.h的使用问题
    - 问题：msinkaut.h的使用不太熟练，不知道怎么获取识别结果，gpt也不好使
    - 解决：利用IDE的提示，选择最可能的函数尝试
3. clear功能的实现
4. undo功能的实现
    - 问题：undo功能的实现，我尝试了很多方法，但是都无法达到预期，各种gpt，各种代码，都无法实现
    - 解决：最后利用qdebug发现，undo按钮因为被我设置在了画板上，因此会在点击时触发画板的绘画事件，导致undo功能无法实现
    - 解决：        hr = strokes->Remove(lastStroke);hr = inkDisp->DeleteStroke(lastStroke);这两条语句缺一不可
