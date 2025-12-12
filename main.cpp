#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>
#include <stack>
#include <cmath>
#include <map>

struct MadeReader{
    std::ifstream reader;
    int _len;
};

struct FlightNumber{
    std::string airport;
    int id;
};

struct DepartureTime
{
    int hh;
    int mm;
};

struct Record{
    FlightNumber flight_number;
    DepartureTime time;
    int cost;
    std::vector<std::string> departure_days;
};

std::string slice(std::string data, int first_ind, int second_ind){
    std::string result="";
    if (second_ind > data.size()){
        std::cout<<"Second ind error";
        return data;
    }
    if (first_ind < 0){
        std::cout<<"First ind error";
    }
    for (int ind = first_ind; ind < second_ind; ind ++){
        result += data[ind];
    }
    return result;
}

std::vector<std::string> split(std::string line, char delim){
    std::vector<std::string> to_return;
    std::stringstream ss(line);
    std::string part_line;
    while (std::getline(ss, part_line, delim)){
        to_return.push_back(part_line);
    }
    return to_return;
}

std::string make_string_from_Record(Record data){
    std::string result;
    result += data.flight_number.airport;
    result += std::to_string(data.flight_number.id) + " ";
    result += std::to_string(data.time.hh) +":"\
    + std::to_string(data.time.mm) + " ";
    result += std::to_string(data.cost) + " ";
    for (std::string elem: data.departure_days){
        result += elem + " ";
    }
    return result;
}

MadeReader make_reader(std::string root_to_input_file){
    MadeReader to_return;
    std::ifstream reader(root_to_input_file);
    std::string line_n;
    int n=0;
    if (std::getline(reader, line_n)){
        n = std::stoi(line_n);
    }else{
        throw std::invalid_argument( "Wrong file!" );
    }
    to_return.reader = std::move(reader);
    to_return._len = n;
    return to_return;
}

Record make_record(std::string line){
    Record to_return;
    auto to_rec = split(line, ',');
    FlightNumber now_FN;
    DepartureTime now_DP;
    now_FN.airport = slice(to_rec[0], 0, 2);
    now_FN.id = std::stoi(slice(to_rec[0], 2, 5));
    auto to_DP = split(to_rec[1], ':');
    now_DP.hh = std::stoi(to_DP[0]);
    now_DP.mm = std::stoi(to_DP[1]);
    to_return.flight_number = now_FN;
    to_return.time = now_DP;
    to_return.cost = std::stoi(to_rec[2]);
    if (to_rec.size() == 4){
        to_return.departure_days  = split(to_rec[3], ' ');
    }else{
        to_return.departure_days = {};
    }
    return to_return;
}

class HashTable{
private:
    std::map<std::string, Record> data;

    std::string get_key(Record rec){
        std::string result;
        result += rec.flight_number.airport;
        result += std::to_string(rec.flight_number.id);
        result += std::to_string(rec.time.hh);
        result += std::to_string(rec.time.mm);
    }
public:
    ~HashTable(){
        data.clear();
    }

    bool add_elem(Record elem){
        std::string key = get_key(elem);
        data[key] = elem;
        return true;
    }

    bool del_elem(Record elem) {
        std::string key = get_key(elem);
        data.erase(key);
        return true;
    }

    void print(){
        for (auto iter = data.begin(); iter != data.end() ; iter++){
            std::cout<<"Hash key:"<<iter->first<<" Val:"<<
            make_string_from_Record(iter->second)<< std::endl;
        }

    }
};

int main(){

    return 0;
}