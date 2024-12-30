## 项目设计方案
1. 使用了windows tablet pc sdk，通过识别墨迹来获取用户输入。
2. 使用Qt框架，通过Qt的信号和槽机制，实现了异步的墨迹识别过程。
3. 尝试使用了TSF框架将字符串插入到输入栏中，但遇到了一些问题，最终没有成功实现。
4. 项目中使用了QFuture和QFutureWatcher来实现异步的墨迹识别过程，通过信号和槽机制，将识别结果发送给主窗口，并在主窗口中显示出来。
5. 对于设置页面和识别页面，使用tabwidget，通过切换被隐藏的tab来切换页面。
6. 对于笔画的颜色、粗细等，设置PenAttributes，用以存储，并通过IInkDrawingAttributes体现
## 核心功能代码解释
### 识别功能
``` cpp
if (event->button() == Qt::LeftButton) {
    // 开始异步识别墨迹
    QFuture<QStringList> future = recognizeInkAsync();
    QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>(this);
    connect(watcher, &QFutureWatcher<QStringList>::finished, this, [this, watcher]() {
        QStringList results = watcher->result();
        emit recognitionResults(results);  // 发送识别结果信号
        watcher->deleteLater();
        });
    watcher->setFuture(future);
```
1. 通过调用 recognizeInkAsync 函数启动一个异步的墨迹识别过程。这个函数的结果是一个 QFuture 对象，最终会包含识别结果（一个字符串列表）。
2. 创建一个 QFutureWatcher 对象，并连接它的 finished 信号到一个 lambda 表达式。当识别过程完成时，lambda 表达式会被调用，并从 QFutureWatcher 中获取识别结果，然后通过自定义的 recognitionResults 信号将结果发送出去。
3. 将 QFuture 对象设置到 QFutureWatcher 中，开始监听识别过程的状态。
4. 使用 deleteLater 调度 watcher 对象的删除，以确保它在完成任务后被正确清理，将 QFuture 对象设置给观察器，启动对异步识别过程的监视。
### 具体识别过程
```cpp
// 调用 Recognize 方法
IInkRecognitionResult* recognitionResult = nullptr;
InkRecognitionStatus status;
hr = recognizerContext->Recognize(&status, &recognitionResult);
if (SUCCEEDED(hr) && recognitionResult != nullptr) {
    // 获取识别候选项
    IInkRecognitionAlternates* alternates = nullptr;
    hr = recognitionResult->AlternatesFromSelection(0, -1, 10, &alternates);
    if (SUCCEEDED(hr) && alternates != nullptr) {
        long count = 0;
        alternates->get_Count(&count);

        for (long i = 0; i < count; ++i) {
            IInkRecognitionAlternate* alternate = nullptr;
            alternates->Item(i, &alternate);
            if (alternate) {
                BSTR text;
                alternate->get_String(&text);
```
1. 调用 Recognize 方法进行墨迹识别，并将结果存储在 IInkRecognitionResult 指针 recognitionResult 中。
2. 调用 AlternatesFromSelection 方法获取识别候选项，并将结果存储在 IInkRecognitionAlternates 指针 alternates 中。
3. 遍历每个候选项，获取其字符串表示，并将其添加到结果列表中。
## 亮点介绍
实现了除中文联想和字符串输入（进程通信）外的各个功能
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
### 放弃：tsf框架的不断尝试
由于对tsf框架不了解，因此没能解决光标在点击QT程序后消失的问题，因此无法实现将字符串输出到光标位置，退而求其次，选择将字符输出到一个QT文本框中。