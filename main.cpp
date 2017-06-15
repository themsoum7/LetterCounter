#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <map>

using namespace std;
vector<string> read_file(string nameOFfile) {
    vector<string> words;
    ifstream myfile(nameOFfile);
    string line;
    while (getline(myfile, line, ' ')) {
            words.push_back(line);
        }
    return words;
    }

map<char, int> counted_letters(vector<string> words){
    map<char, int> map1;
    for(int i = 0; i < words.size();++i) {
        for (int j = 0; j < words[i].size(); j++) {
            if (map1.find(words[i][j]) == map1.end()) {
                char a = words[i][j];
                map1.insert(make_pair(a, 1));
            } else {
                map1[words[i][j]] += 1;
            }
        }
    }
    return map1;
}
void work_for_thread(int start,int finish,vector<string> &words,map<char,int> &global_map,mutex &m){
    vector<string> local_vector(words.begin() + start,words.begin() + finish + 1);
    map<char, int> local_map = counted_letters(local_vector);
    m.lock();
    for(map<char, int>::iterator it = local_map.begin(); it != local_map.end(); it++){
        if (local_map.find(it->first) == global_map.end()){
            global_map.insert(make_pair(it->first,it->second));
        }else{
            global_map[it->first] += it->second;
        }
    }
    m.unlock();
}


void Join(vector<thread> &threads){
    for(int i = 0; i < threads.size(); i++){
        threads[i].join();
    }
}
map<char,int> threading(int n,mutex &m,vector<string> &words){
    int start = 0;
    map<char,int> global_map;
    vector<thread> vector_threads;
    for(int i = 0; i < n; i++){
        if(i != n-1) {
            vector_threads.push_back(thread(work_for_thread, start, start + words.size() / n, ref(words), ref(global_map),ref(m)));
            start += words.size() / n + 1;
        }else{
            vector_threads.push_back(thread(work_for_thread, start, words.size() - 1, ref(words), ref(global_map),ref(m)));
        }
    }
    Join(vector_threads);
    return global_map;
}

void print_map(map<char,int> &global_map){
    for(auto i = global_map.cbegin(); i != global_map.cend(); ++i){
        cout <<"\"" << i->first << "\": " << i->second << endl;
    }
}
int main() {
    vector<string> a = read_file("fl.txt");
    mutex m;
    map<char,int> result = threading(5,ref(m),a);
    print_map(result);
    return 0;
}