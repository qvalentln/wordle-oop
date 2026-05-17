
#ifndef OOP_DECRYPT_H
#define OOP_DECRYPT_H

#include <string>
#include <vector>
#include <exception>


class DictionaryException : public std::exception {
private:
    std::string msg;
public:
    explicit DictionaryException(const std::string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
    friend std::ostream& operator<<(std::ostream& os, const DictionaryException& self);
};

class SecureDictionary {
private:
    char* rawData;
    size_t dataSize;
    std::vector<std::string> words;
    //  XOR key
    static constexpr char key=0x3;

public:
    ~SecureDictionary();
    SecureDictionary() : rawData(nullptr), dataSize(0) {}
    SecureDictionary(const SecureDictionary& other);
    SecureDictionary(SecureDictionary&& other) noexcept;

    SecureDictionary& operator=(const SecureDictionary& other);
    friend std::ostream& operator<<(std::ostream& os, const SecureDictionary& self);
    friend std::istream& operator>>(std::istream& is, SecureDictionary& self);
    std::string operator[](size_t index) const;

    void decryptAndParse();
    void loadFromBinary(const std::string& path);
    //bool checkWord(const std::string &guess) const;
    std::string getRandomWord() const;
};


#endif //OOP_DECRYPT_H