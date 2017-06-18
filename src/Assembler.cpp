#include "Assembler.h"

Assembler::Assembler(std::string file_, int length_, int reqc_)
    :length(length_), file(file_), requiredCoverage(reqc_)
{}

void Assembler::loadReads() {
    std::ifstream in(file);

    if(!in.is_open())
        throwError("can't open file " + file);

    std::string buffer;

    while(getline(in, buffer)) {
        if(buffer[0] == '@' || buffer[0] == '+')
            continue;

        reads.push_back(buffer);
    }

    table = std::make_unique<std::list<std::pair<unsigned,unsigned>>[]>(reads.size());
    in.close();
}

void Assembler::saveToFile() {
    std::ofstream out("assembling-result");

    if(!out.is_open())
        throwError("can't open file " + file);

    for(unsigned i=0, j=0; i < reads.size(); i++)
        if(!reads[i].empty()){
            out << "@res_" + std::to_string(j++) << std::endl << reads[i] << std::endl;
        }

    out.close();
}

int Assembler::compareReads(const std::string &read1, const std::string &read2) {

    int comparison1 = 0;
    for(unsigned i = 0; i < read1.size(); i++) {
        for(unsigned j = 0; j < read2.size() - i; j++){
            if( read2[j] != read1[i+j] ){
                comparison1 = 0;
                break;
            }
            comparison1++;
        }
        if(comparison1 != 0)
            break;
    }

    int comparison2 = 0;
    for(unsigned i=0; i < read1.size(); i++) {
        for(unsigned j = 0; j < read2.size() - i; j++){
            if( read1[j] != read2[i+j] ){
                comparison2 = 0;
                break;
            }
            comparison2++;
        }
        if(comparison2 != 0)
            break;
    }

    return comparison1 >= comparison2 ? comparison1 : -comparison2;
}

void Assembler::compare(unsigned i1, unsigned i2) {
    if(i1 == i2)
        return;

    if(reads[i1].empty() || reads[i2].empty())
        return;

    int result = compareReads(reads[i1], reads[i2]);

    if(result == 0 || (result < requiredCoverage && result > -requiredCoverage))
        return;


    if(result > 0 ) {
        table[i1].push_back(std::make_pair(i2, result));
    } else {
        table[i2].push_back(std::make_pair(i1, -result));
    }
}

void Assembler::compareAll() {
    for(unsigned i=0; i<reads.size(); i++) {
        for(unsigned j=i; j<reads.size(); j++) {
            compare(i,j);
        }
        std::cout << i << std::endl;
    }
}

bool Assembler::join() {
    unsigned i1 = 0, i2 = 0, max = 0;

    for(unsigned i = 0; i < reads.size(); i++)
        for(auto j : table[i])
            if(j.second > max)
            {
                max = j.second;
                i1 = i;
                i2 = j.first;
            }

    std::cout << "max: " << max << " i1: " << i1 << " i2: " << i2 <<std::endl;

    if(max == 0)
        return false;

    std::string joined = joinReads(reads[i1], reads[i2], max);

    table[i1].clear();
    table[i2].clear();

    for(unsigned i = 0; i < reads.size(); i++)
        for(auto j = table[i].begin(); j != table[i].end(); j++){
            if((*j).first == i1 || (*j).first == i2){
                table[i].erase(j);
            }
        }


    reads[i1] = joined;
    reads[i2] = "";

    for(unsigned i = 0; i<reads.size(); i++)
        compare(i1, i);

    return true;
}

std::string Assembler::joinReads(const std::string &read1, const std::string &read2, unsigned pos)
{
    return read1 + read2.substr(pos);
}


void Assembler::saveCorrectGenome()
{
    FILE *fp = fopen("whole_correct_genome", "w");

    if(fp == nullptr)
        throwError("Can't open file to save");

    //fwrite(genome.get(), 1, length, fp);
    fclose(fp);
}

void Assembler::run()
{
    using namespace std::chrono;
    typedef high_resolution_clock Time;

    auto start = Time::now();

    loadReads();
    std::cout << "Load genome: " << duration_cast<milliseconds> (Time::now() - start).count()<< "ms"<< std::endl;
    std::cout << "Nr of reads: " << reads.size() << std::endl;
    start = Time::now();

    compareAll();
    std::cout << "Initial compare: " << duration_cast<milliseconds> (Time::now() - start).count()<< "ms"<< std::endl;
    start = Time::now();

    while(join());
    std::cout << "Joining: " << duration_cast<milliseconds> (Time::now() - start).count()<< "ms"<< std::endl;
    start = Time::now();

    saveToFile();
    std::cout << "Saving to file: " << duration_cast<milliseconds> (Time::now() - start).count()<< "ms"<< std::endl;
}

void Assembler::throwError(std::string msg)
{
    throw std::runtime_error(msg);
}
