## ˵��


1. ÿ�ܵ���ϰ����ҵ������ڶ�Ӧ��ŵ�week0x�ļ����У�����practice��ſ�����ϰ�Ĵ��루�������˵�����ҵ�ο�����homework ��ſκ���ҵ�Ĵ��롣

2.  ������Ŀ���˴������� ClassExamProject �ļ����С�

3. C++��ҵ�ύ��ҵʱֻ��Ҫ�ύ��Ŀ�Ĵ����ļ�.h,.cpp��.sln �ļ���.vcxproj �ļ����ɣ������ύ����������м�����С���뵥Ԫ����

������ϸ������� https://kdocs.cn/l/cdZi2WxolyLn


### �������̳�

1. ��������qt5��صĿ⣬��������ŵ���Develop-TrainingĿ¼ͬ��Ŀ¼�£�����ѹ����ѹ��ʽѡ���ѹ����ǰ�ļ�
   ������Ϻ�, Develop-Train��thirdparty_install������ͬ��λ�á�

    ��������: https://365.kdocs.cn/l/crFHGQuSLP9Q
    

2. ��cmd����,���뵱ǰλ��, ����buildĿ¼�������ִ����������Develop-Train��thirdparty_install��build������ͬ��Ŀ¼��
    ```
    mkdir build
    cd build
    ```
4. ʹ��VS�еı��뻷��,ʵ��������Լ���װ��VS·�����޸�
    ```
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
    ```
5. ִ����ع����������Debug��Release�Լ�ѡ��

-   ����Debug����
    ```
    cmake  -G "Visual Studio 16 2019"  -A x64 -DCMAKE_BUILD_TYPE=Debug  ../Develop-Training

    msbuild /m Develop-Train.sln /p:Platform=x64 /p:Configuration=Debug
    ```


- ����Release����
    ```
    cmake  -G "Visual Studio 16 2019"  -A x64 -DCMAKE_BUILD_TYPE=Release  ../Develop-Training

    msbuild /m Develop-Train.sln /p:Platform=x64 /p:Configuration=Release