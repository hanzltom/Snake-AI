#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;

void printMap(vector<vector<int>>& map){
    cout << endl;
    size_t count = 0;
    cout << "x ";
    for(size_t i = 0; i < map[0].size(); i++) {
        cout << count << " ";
        count++;
    }
    cout << endl;
    for(size_t i = 0; i < map[0].size() + 1; i++)
        cout << "--";
    cout << endl;
    count = 0;
    for(size_t i = 0; i < map[0].size(); i++){
        cout << count << "|";
        for(size_t j = 0; j < map[0].size(); j++){
            if( map[i][j] == 2)
                cout << "\033[91m" << "X " << "\033[m";
            else if( map[i][j] == 1 )
                cout << "\033[1;33m" << map[i][j] << " "  << "\033[0m";
            else
                cout << map[i][j] << " ";
        }
        cout << endl;
        count++;
    }

}

vector<vector<int>>  getMap(size_t size){
    vector<vector<int>> res;
    vector<int> tmp;
    for( size_t i = 0; i < size; i++)
        tmp.push_back(0);

    for( size_t i = 0; i < size; i++){
        res.push_back(tmp);
    }

    return res;
}

size_t getSize(){
    size_t tmp;
    cout << "Napis pocet kraloven (N kraloven bude pouzito na mape NxN): " << endl;
    cin >> tmp;
    if( tmp == 0) {
        cout << "NaN" << endl;
        return 0;
    }
    return tmp;
}

size_t getMove(){
    size_t tmp;
    cout << "V jakem rozpeti poli do vsech stran budu hledat souseda?" << endl;
    cin >> tmp;
    if( tmp == 0) {
        cout << "NaN" << endl;
        return 0;
    }
    return tmp;
}

void fillZeros( vector<vector<int>>& map, int x, int y){
    for (size_t i = 0; i < map.size(); i++) {
        if (map[i][x] == 0)
            map[i][x] = 1;
    }

    for (size_t i = 0; i < map[0].size(); i++) {
        if (map[y][i] == 0)
            map[y][i] = 1;
    }

    for (int i = 1; y + i < (int)map.size() && x + i < (int)map[0].size(); i++) {
        if (map[y + i][x + i] == 0)
            map[y + i][x + i] = 1;
        else
            break;
    }

    for (int i = 1; y + i < (int)map.size() && x - i >= 0; i++) {
        if (map[y + i][x - i] == 0)
            map[y + i][x - i] = 1;
        else
            break;
    }

    for (int i = 1; y - i >= 0 && x + i < (int)map[0].size(); i++) {
        if (map[y - i][x + i] == 0)
            map[y - i][x + i] = 1;
        else
            break;
    }

    for (int i = 1; y - i >= 0 && x - i >= 0; i++) {
        if (map[y - i][x - i] == 0)
            map[y - i][x - i] = 1;
        else
            break;
    }
}

pair<int,int> setFirst( vector<vector<int>>& map ){
    srand(time(0));

    size_t x = rand() % map[0].size();
    size_t y = rand() % map[0].size();

    map[y][x] = 2;
    printMap(map);
    fillZeros(map, x, y);
    return make_pair(x,y);
}

size_t getScore( vector<vector<int>>& map ){
    size_t score = 0;
    for(size_t i = 0; i < map[0].size(); i++) {
        for (size_t j = 0; j < map[0].size(); j++) {
            if( map[i][j] == 0)
                score++;
        }
    }
    return score;
}

pair<int,int> selectNeighbour(vector<vector<int>>& map, size_t move, pair<int,int> addedLast ){
    vector<pair<int,int>> vec;
    for (int i = addedLast.first - (int)move; i <= addedLast.first + (int)move; ++i) {
        for (int j = addedLast.second - (int)move; j <= addedLast.second + (int)move; ++j) {
            if (i >= 0 && i < (int)map.size() && j >= 0 && j < (int)map[0].size() && map[j][i] == 0) {
                vec.push_back({i, j});

            }
        }
    }

    for( size_t i = 0; i < vec.size(); i++){
        cout << "<" << vec[i].first << ", " << vec[i].second << "> ";
    }
    cout << endl;

    srand(time(0));
    pair<int,int> selectedPair = vec[rand() % vec.size()];
    cout << "Selected: " << selectedPair.first << ", " << selectedPair.second << endl;
    return selectedPair;
}

void hillClimb( size_t size, size_t move, vector<vector<int>>& map){
    pair<int,int> addedLast;
    addedLast = setFirst(map);
    size_t score = getScore(map);
    printMap(map);
    size_t iter = 0;

    while(true){
        pair<int,int> neighbour = selectNeighbour(map, move, addedLast);
        break;
    }
}

int main(){
    cout << "N QUEENS" << endl;

    size_t size = getSize();
    if( !size ) return 0;
    cout << size << endl;

    size_t move = getMove();
    if( !move || move >= size) return 0;
    cout << move << endl;

    vector<vector<int>> map = getMap( size );
    printMap(map);

    hillClimb(size, move, map);

    return 0;
}