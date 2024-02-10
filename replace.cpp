#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <vector> 

class ProperNounCounter {
private:
    std::string input_;
    std::unordered_map<std::string, int> nameCount_;
    std::vector<std::string> databaseWords_;

public:
    ProperNounCounter(const std::string& input, const std::string& database) : input_(input) {
        std::ifstream readdatabase(database);
        std::string word;
        while (std::getline(readdatabase, word, ',')) {
            word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
            word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
            if (!word.empty()) {
                databaseWords_.push_back(word);
            }  
        }
    }

    void countProperNouns() {
        std::istringstream iss(input_);
        std::string word;
        while (std::getline(iss, word, ' ')) {
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            if (!word.empty() && (std::find(databaseWords_.begin(), databaseWords_.end(), word) != databaseWords_.end())) {
                nameCount_[word]++;
                replaceWordWithX(input_, word);
            }
        }
    }

    void replaceWordWithX(std::string& text, const std::string& word) {
        size_t pos = text.find(word);
        while (pos != std::string::npos) {
            text.replace(pos, word.length(), "X");
            pos = text.find(word, pos + 1);
        }
    }

    void writeJsonOutput(const std::string& filename) {
        std::ofstream ofs(filename);
        ofs << "{\n";
        ofs << "  \"Text\": \"" << input_ << "\",\n"; 
        ofs << "  \"ProperNouns\": {\n";
        for (auto it = nameCount_.begin(); it != nameCount_.end(); ++it) {
            ofs << "    \"" << it->first << "\": " << it->second;
            if (std::next(it) != nameCount_.end()) {
                ofs << ",\n";
            } else {
                ofs << "\n";
            }
        }
        ofs << "  }\n";
        ofs << "}\n";
    }
};

int main() {
    std::string input;
    std::ifstream readinput("input.txt");
    std::getline(readinput, input);
    readinput.close(); 
    
    ProperNounCounter counter(input, "database.txt");
    counter.countProperNouns();
    counter.writeJsonOutput("output.json");

    std::ifstream readdatabase("database.txt");
    readdatabase.close(); 

    return 0;
}