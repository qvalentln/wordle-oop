
#include <decrypt.h>

#include <algorithm>
#include <random>
#include <ctime>
#include <ostream>
#include <fstream>

/// DictException things
std::ostream& operator<<(std::ostream& os, const DictionaryException& self) {
    os << self.what();
    return os;
}

/// C/D SecureDictionary
SecureDictionary::~SecureDictionary() {
    delete[] rawData;
}

SecureDictionary::SecureDictionary(const SecureDictionary &other) : dataSize(other.dataSize), words(other.words) {
    if (other.rawData) {
        rawData=new char[dataSize];
        std::copy(other.rawData,other.rawData + dataSize,rawData);
    }
    else {
        rawData=nullptr;
    }

}

SecureDictionary::SecureDictionary(SecureDictionary &&other)
noexcept: rawData(other.rawData),dataSize(other.dataSize), words(std::move(other.words)) {
    other.rawData=nullptr;
}


/// Operatori SecureDictionary

SecureDictionary& SecureDictionary::operator=(const SecureDictionary& other) {
    if (this != &other) {
        delete[] rawData;

        // copiere
        dataSize=other.dataSize;
        words=other.words;
        if (other.rawData) {
            rawData=new char[dataSize];
            std::copy(other.rawData,other.rawData + dataSize,rawData);

        }
        else {
            rawData=nullptr;
        }

    }


    return *this;
}

std::ostream& operator<<(std::ostream& os, const SecureDictionary& self) {
    os << "nr cuvinte incarcate: " << self.words.size() << '\n';
    return os;
}
std::istream& operator>>(std::istream& is, SecureDictionary& self) {
    is.seekg(0, std::ios::end);
    size_t dataSize = is.tellg();
    is.seekg(0, std::ios::beg);

    if (dataSize == 0) return is;

    delete[] self.rawData;
    self.dataSize = dataSize;
    self.rawData = new char[dataSize];
    is.read(self.rawData, dataSize);

    self.decryptAndParse();

    return is;
}

std::string SecureDictionary::operator[](size_t index) const {
    if (index >= words.size()) throw DictionaryException("index gresit!");
    return words[index];
}

/// Metode SecureDictionary

void SecureDictionary::decryptAndParse() {
    // Decrypt
    for (size_t i = 0; i < dataSize; ++i) {
        rawData[i] ^= key;
    }
    // Parse
    std::string current;
    for (size_t i = 0; i < dataSize; ++i) {
        char c = rawData[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            current += c;
        } else {
            if (current.size() == 5) {
                words.push_back(current);
            }
            current = "";
        }
    }
    if (current.size() == 5)
        words.push_back(current);

    if (words.empty()) {
        throw DictionaryException("Dictionarul este gol!!!");
    }

}
//  Logica de incarcare si decriptare din words.bin
void SecureDictionary::loadFromBinary(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw DictionaryException("Nu exista fisierul dictionar!!!");
    }
    file >> *this;
    file.close();

    if (this->words.empty()) {
        throw DictionaryException("Dictionarul este gol!!!");
    }
}

//  Check daca input apare in dictionar

bool SecureDictionary::checkWord(const std::string &guess) const{
    return std::ranges::any_of(words, [&](const std::string& word) {
        return word == guess;
    });
}

std::string SecureDictionary::getRandomWord() const {
    if (words.empty()) {
        throw DictionaryException("Dictionarul este gol!!!");
    }

    // shahahahhahahhafffffalfuhhhyyyydlajkryyuuhnfjkalhfjansouyhewghrelqjerkqsadlikyqeuielqh
    // folosesc timpul curent ca seed,
    // ceva algoritm mersenne din std ptr random
    static std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
    // cuv au ac prob
    std::uniform_int_distribution<> distribution(0, words.size()-1);

    return words[distribution(gen)];
}
