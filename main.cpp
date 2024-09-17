#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <string>
#include <set>
#include <vector>

int main(int argc, char* argv[]) {

    if (argc < 3)
    {
        std::cout << "Usage: freq inputfile.txt outputfile.txt\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);

    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(size, ' ');

    if (!file.read(&buffer[0], size)) {
        std::cerr << "Error reading file\n";
        return 1;
    }

    for (auto& c : buffer)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c += 'a' - 'A';
        }
        else if (!(c >= 'a' && c <= 'z'))
        {
            c = ' ';
        }
    }

    std::vector<std::set<std::string>> wordData;
    std::unordered_map<std::string, int> wordCount;
    

    int index = 0;
    std::string word;
    word.reserve(size); //Reserve a lot to avoid reallocations

    while (index < size)
    {
        if (buffer[index] >= 'a' && buffer[index] <= 'z')
        {
            word += buffer[index];
        }
        if ((buffer[index] == ' ' && word.size() > 0) || (index == size-1))
        {
            int wc = ++wordCount[word];

            if (wordData.size() < wc)
            {
                wordData.resize(wc);
            }
            if (wc > 1)
            {
                wordData[wc - 2].erase(word);
            }      
            wordData[wc - 1].insert(word);
            word.resize(0);
        }
        index++;
    }

    std::string& outputBuffer = word; //Reuse memory

    for (int i = 0; i < wordData.size(); i++)
    {
        int index = wordData.size() - i - 1;

        const auto& wordSet = wordData[index];

        for (const auto& word : wordSet)
        {
            int count = index + 1;

            while (count > 0)
            {
                outputBuffer += '0' + count % 10;
                count = count / 10;
            }

            outputBuffer += ' ';

            outputBuffer += word;

            outputBuffer += '\n';
        }
    }

    std::ofstream outputFile(argv[2], std::ios::binary);

    if (!outputFile) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    outputFile.write(outputBuffer.data(), outputBuffer.size());

    if (!outputFile) {
        std::cerr << "Error writing output file\n";
        return 1;
    }

    return 0;

 
}
