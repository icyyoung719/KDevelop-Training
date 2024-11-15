#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <unordered_map>

class BoyerMoore {
public:
    explicit BoyerMoore(const std::string& pattern);

    // �ڸ����ı��в���ģʽ������ƥ��λ�õ������б�
    std::vector<int> search(const std::string& text);

private:
    std::string pattern;
    std::unordered_map<std::string, int> badCharTable; // �洢���ַ�����ʹ���ַ�����Ϊ����֧��UTF-8
    std::vector<int> goodSuffixTable;

    // Ԥ�����ַ�����
    void preprocessBadCharTable();

    // Ԥ����ú�׺����
    void preprocessGoodSuffixTable();

    // ��������������ַ��Ƿ�ΪUTF-8�Ŀ�ʼ�ֽ�
    bool isUTF8StartByte(unsigned char byte);
};

#endif // BOYERMOORE_H
