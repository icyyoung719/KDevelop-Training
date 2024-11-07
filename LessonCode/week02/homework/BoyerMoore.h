#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <unordered_map>

class BoyerMoore {
public:
    // ���캯����ʼ��ģʽ
    explicit BoyerMoore(const std::string& pattern);

    // �ڸ����ı��в���ģʽ������ƥ��λ�õ������б�
    std::vector<int> search(const std::string& text);

private:
    std::string pattern;
    std::unordered_map<char, int> badCharTable;
    std::vector<int> goodSuffixTable;

    // Ԥ�����ַ�����
    void preprocessBadCharTable();

    // Ԥ����ú�׺����
    void preprocessGoodSuffixTable();
};

#endif // BOYERMOORE_H
