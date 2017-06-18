#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <chrono>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstring>
#include <random>
#include <list>
#include <vector>

class Assembler {
public:
    Assembler(std::string file_, int length_, int recq_);

    void run();

private:
    bool join();
    void saveToFile();
    void loadReads();
    void compareAll();
    void compare(unsigned i1, unsigned i2);
    int compareReads(const std::string &read1, const std::string &read2);
    std::string joinReads(const std::string &read1, const std::string &read2, unsigned pos);
    void saveCorrectGenome();
    void throwError(std::string msg);

    int length;
    std::string file;
    int requiredCoverage;
    std::vector<std::string> reads;
    std::unique_ptr<std::list<std::pair<unsigned,unsigned>>[]> table;
};

#endif // ASSEMBLER_H_
