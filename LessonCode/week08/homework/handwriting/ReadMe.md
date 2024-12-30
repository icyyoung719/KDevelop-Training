## ��Ŀ��Ʒ���
1. ʹ����windows tablet pc sdk��ͨ��ʶ��ī������ȡ�û����롣
2. ʹ��Qt��ܣ�ͨ��Qt���źźͲۻ��ƣ�ʵ�����첽��ī��ʶ����̡�
3. ����ʹ����TSF��ܽ��ַ������뵽�������У���������һЩ���⣬����û�гɹ�ʵ�֡�
4. ��Ŀ��ʹ����QFuture��QFutureWatcher��ʵ���첽��ī��ʶ����̣�ͨ���źźͲۻ��ƣ���ʶ�������͸������ڣ���������������ʾ������
5. ��������ҳ���ʶ��ҳ�棬ʹ��tabwidget��ͨ���л������ص�tab���л�ҳ�档
6. ���ڱʻ�����ɫ����ϸ�ȣ�����PenAttributes�����Դ洢����ͨ��IInkDrawingAttributes����
## ���Ĺ��ܴ������
### ʶ����
``` cpp
if (event->button() == Qt::LeftButton) {
    // ��ʼ�첽ʶ��ī��
    QFuture<QStringList> future = recognizeInkAsync();
    QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>(this);
    connect(watcher, &QFutureWatcher<QStringList>::finished, this, [this, watcher]() {
        QStringList results = watcher->result();
        emit recognitionResults(results);  // ����ʶ�����ź�
        watcher->deleteLater();
        });
    watcher->setFuture(future);
```
1. ͨ������ recognizeInkAsync ��������һ���첽��ī��ʶ����̡���������Ľ����һ�� QFuture �������ջ����ʶ������һ���ַ����б���
2. ����һ�� QFutureWatcher ���󣬲��������� finished �źŵ�һ�� lambda ���ʽ����ʶ��������ʱ��lambda ���ʽ�ᱻ���ã����� QFutureWatcher �л�ȡʶ������Ȼ��ͨ���Զ���� recognitionResults �źŽ�������ͳ�ȥ��
3. �� QFuture �������õ� QFutureWatcher �У���ʼ����ʶ����̵�״̬��
4. ʹ�� deleteLater ���� watcher �����ɾ������ȷ����������������ȷ������ QFuture �������ø��۲������������첽ʶ����̵ļ��ӡ�
### ����ʶ�����
```cpp
// ���� Recognize ����
IInkRecognitionResult* recognitionResult = nullptr;
InkRecognitionStatus status;
hr = recognizerContext->Recognize(&status, &recognitionResult);
if (SUCCEEDED(hr) && recognitionResult != nullptr) {
    // ��ȡʶ���ѡ��
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
1. ���� Recognize ��������ī��ʶ�𣬲�������洢�� IInkRecognitionResult ָ�� recognitionResult �С�
2. ���� AlternatesFromSelection ������ȡʶ���ѡ���������洢�� IInkRecognitionAlternates ָ�� alternates �С�
3. ����ÿ����ѡ���ȡ���ַ�����ʾ����������ӵ�����б��С�
## �������
ʵ���˳�����������ַ������루����ͨ�ţ���ĸ�������
## �����д�����е�һЩ�����¼
### ��ʼ������������
1. tablet pc sdk ��ʵ��windows�Դ��ģ��Ѿ��ںϵ�windows sdk���ˣ�����Ҫ�ٰ�װ
### �𲽣�winrt�ĳ���
1. winrt�� auto inkPoints = winrt::single_threaded_vector<winrt::Windows::UI::Input::Inking::InkPoint>(); һֱ�����̴߳���
    - ԭ��winrt::single_threaded_vector���첽�ģ���qtͬ�����Ʋ�̫һ�������˽��Ҳ����
    - �����ʹ��QFutureWatcher������첽���⣬QFuture<QString> future = recognizeInkAsync();�첽ִ��ʶ�����
2. winrtָ�Ӹ���һ��ʶ��������֪����ô��ȡ���ʶ��������˻���msinkaut.h
### ���ף�msinkaut.h�ĳ���
1. msinkaut.h�ĵ�������
    - ���⣺msinkaut.h������windows��.dll�ļ���̬���ӣ��Ҳ�����ԣ��޷��ڵ����м��һЩ�ṹ��
    - �������VC����Ŀ�е���-ѡ��-����������msinkaut.h��pdb�ļ������ǻ����޷����
    - ��Э���ڴ����м���һЩ��������磺qDebug()<<inkDisp->GetStrokes()->Count;���鿴һЩ��Ϣ
2. msinkaut.h��ʹ������
    - ���⣺msinkaut.h��ʹ�ò�̫��������֪����ô��ȡʶ������gptҲ����ʹ
    - ���������IDE����ʾ��ѡ������ܵĺ�������
3. clear���ܵ�ʵ��
4. undo���ܵ�ʵ��
    - ���⣺undo���ܵ�ʵ�֣��ҳ����˺ܶ෽�������Ƕ��޷��ﵽԤ�ڣ�����gpt�����ִ��룬���޷�ʵ��
    - ������������qdebug���֣�undo��ť��Ϊ�����������˻����ϣ���˻��ڵ��ʱ��������Ļ滭�¼�������undo�����޷�ʵ��
    - �����        hr = strokes->Remove(lastStroke);hr = inkDisp->DeleteStroke(lastStroke);���������ȱһ����
### ������tsf��ܵĲ��ϳ���
���ڶ�tsf��ܲ��˽⣬���û�ܽ������ڵ��QT�������ʧ�����⣬����޷�ʵ�ֽ��ַ�����������λ�ã��˶�����Σ�ѡ���ַ������һ��QT�ı����С�