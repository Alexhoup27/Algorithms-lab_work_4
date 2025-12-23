#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>
#include <cmath>

struct MadeReader {
    std::ifstream reader;
    int _len;
};

struct FlightNumber {
    std::string airport;
    int id;
};

struct DepartureTime {
    int hh;
    int mm;
};

struct Record {
    FlightNumber flight_number;
    DepartureTime time;
    int cost;
    std::vector<std::string> departure_days;

    bool operator== (Record& to_comp){
        if (flight_number.airport == to_comp.flight_number.airport &&
        flight_number.id == to_comp.flight_number.id &&
        time.hh == to_comp.time.hh &&
        time.mm == to_comp.time.mm &&
        cost == to_comp.cost){
            bool key = true;
            if (departure_days.size() == to_comp.departure_days.size()) {
                if (departure_days.empty()){
                    return true;
                }
                for (auto elem: departure_days) {
                    if (departure_days != to_comp.departure_days) {
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    }
    bool operator!= (Record& to_comp){
        if (flight_number.airport != to_comp.flight_number.airport &&
            flight_number.id != to_comp.flight_number.id &&
            time.hh != to_comp.time.hh &&
            time.mm != to_comp.time.mm &&
            cost != to_comp.cost){
            bool key = true;
            for (auto elem:departure_days){
                if (departure_days == to_comp.departure_days){
                    return false;
                }
                return true;
            }
        }
        return false;
    }
};

std::string slice(std::string data, int first_ind, int second_ind) {
    std::string result = "";
    if (second_ind > data.size()) {
        std::cout << "Second ind error";
        return data;
    }
    if (first_ind < 0) {
        std::cout << "First ind error";
    }
    for (int ind = first_ind; ind < second_ind; ind++) {
        result += data[ind];
    }
    return result;
}

std::vector<std::string> split(std::string line, char delim) {
    std::vector<std::string> to_return;
    std::stringstream ss(line);
    std::string part_line;
    while (std::getline(ss, part_line, delim)) {
        to_return.push_back(part_line);
    }
    return to_return;
}

std::string make_string_from_Record(Record data) {
    std::string result;
    result += data.flight_number.airport;
    result += std::to_string(data.flight_number.id) + " ";
    result += std::to_string(data.time.hh) + ":"\
 + std::to_string(data.time.mm) + " ";
    result += std::to_string(data.cost) + " ";
    for (std::string elem: data.departure_days) {
        result += elem + " ";
    }
    return result;
}

MadeReader make_reader(std::string root_to_input_file) {
    MadeReader to_return;
    std::ifstream reader(root_to_input_file);
    std::string line_n;
    int n = 0;
    if (std::getline(reader, line_n)) {
        n = std::stoi(line_n);
    } else {
        throw std::invalid_argument("Wrong file!");
    }
    to_return.reader = std::move(reader);
    to_return._len = n;
    return to_return;
}

Record make_record(std::string line) {
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
    if (to_rec.size() == 4) {
        to_return.departure_days = split(to_rec[3], ' ');
    } else {
        to_return.departure_days = {};
    }
    return to_return;
}

class NewHashTable {
private:
    Record *data;
    bool *statuses;
    int length;
    int count_of_elements;

    int GetFirstKey(Record rec) {
        int key = rec.flight_number.airport[0] + rec.flight_number.airport[1];
        key += rec.flight_number.id;
        key += rec.time.hh;
        key += rec.time.mm;
        key %= length;
        return key;
    }

    int GetSecondKey(int key, int iter_num){
        key += iter_num * iter_num;
        key %= length;
        return key;
    }

    std::pair<bool, int> FindFree(int key, Record elem){
        std::pair<bool, int> to_return;
        int iter_num = 1;
        while (statuses[key] && iter_num <= length * 0.7) {
            key = GetSecondKey(key, iter_num);
            if (data[key] == elem){
                return {false, -1};
            }
            iter_num++;;
        }
        if (statuses[key] == false && (data[key] == elem)==false){
            to_return.first = true;
            to_return.second = key;
        }else{
            to_return.first = false;
            to_return.second = -1;
        }
        return to_return;
    }

    void resize(bool flag){
        int old_length = length;
        if (flag){
            length *= 2;
        }else{
            length /= 2;
        }
        Record* old_data = data;
        data = new Record[length];
        auto old_statuses = statuses;
        statuses = new bool[length];
        for (int i=0; i< length; i++){
            statuses[i] = false;
        }
        for (int i = 0; i< old_length; i++){
            if (old_statuses[i]){
                add(old_data[i]);
            }
        }
    }

public:

    bool add(Record rec) {
        if (count_of_elements + 1 >= length*0.8){
            resize(true);
        }
        int now_key = GetFirstKey(rec);
        if (statuses[now_key] == false) {
            data[now_key] = rec;
            statuses[now_key] = true;
            count_of_elements++;
            return true;
        } else {
           auto new_key = FindFree(now_key, rec);
           if (new_key.first){
               data[new_key.second] = rec;
               statuses[new_key.second] = true;
               count_of_elements++;
               return true;
           }else{
               resize(1);
               return add(rec);
           }
        }
    }

    bool dispose(Record rec){
        auto new_key = find_elem(rec);
        if (new_key.first == false){
            return false;
        }else{
            if (new_key.first){//use resize
                if (count_of_elements - 1 <= length*0.2 && length >2) {
                    count_of_elements--;
                    statuses[new_key.second] = false;
                    Record *old_data = data;
                    data = new Record[length];
                    auto old_statuses = statuses;
                    statuses = new bool[length];
                    for (int i = 0; i < length; i++) {
                        statuses[i] = false;
                    }
                    for (int i = 0; i < length; i++) {
                        if (old_statuses[i]) {
                            return add(old_data[i]);
                        }
                    }
                }else{
                    count_of_elements--;
                    statuses[new_key.second] = false;
                    resize(0);
                    return true;
                }
            }
        }
        return false;
    }

    std::pair<bool, int> find_elem(Record rec){
        int key = GetFirstKey(rec);
        if (data[key] == rec){
            return {true, key};
        }else {
            int iter_num = 1;
            bool flag = false;
            while (iter_num < length * 0.7 && statuses[key]) {
                key = GetSecondKey(key, iter_num);
                iter_num++;
                std::cout<<data[key].cost<<std::endl;
                std::cout<<make_string_from_Record(data[key])<<std::endl;
                std::cout<<(data[key] == rec)<<std::endl;
                if (data[key] == rec) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                return {true, key};
            }
        }
        return {false, -1};
    }

    void print(){
        for (int i=0; i< length; i++){
            if (statuses[i]){
                std::cout<<"Key: "<<i<<", Record: "<< make_string_from_Record(data[i])<<std::endl;
            }
        }

    }


    NewHashTable(int _len) {
        data = new Record[_len];
        length = _len;
        statuses = new bool[_len];
        count_of_elements = 0;
        for (int i=0; i< length; i++){
            statuses[i] = false;
        }
    }

    ~NewHashTable() {
        delete[] data;
        delete[] statuses;
        data = nullptr;
        statuses = nullptr;
    }
};


int main() {
    Record r;
    r.flight_number.airport="ab";
    r.flight_number.id = 111;
    r.time.hh = 1;
    r.time.mm = 10;
    r.cost = 1000;
    NewHashTable h(10);
    for (int i=1;i<10; i++){
//        std::cout<<i<<std::endl;
        h.add(r);
        r.cost+=1;
        r.time.hh += 1;
        r.time.mm -= 1;
    }
    r.flight_number.airport="ab";
    r.flight_number.id = 111;
    r.time.hh = 1;
    r.time.mm = 10;
    r.cost = 1000;
    for (int i = 1; i< 10; i++){
        if (i % 2 == 1){
            if(h.dispose(r)){
                std::cout<<"Good"<<std::endl;
            }else{
                std::cout<<"Bad"<<std::endl;
            }
        }
        r.cost+=1;
        r.time.hh += 1;
        r.time.mm -= 1;
    }
    std::cout<<"here!"<<std::endl;
    h.print();
    std::cout<<"end"<<std::endl;

    return 0;
}