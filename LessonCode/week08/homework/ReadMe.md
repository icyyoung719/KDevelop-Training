

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
