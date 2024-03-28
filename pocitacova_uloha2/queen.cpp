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

void printMap(vector<vector<int>>& map, size_t score){
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
    cout << "Score: " << score << endl;
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
    cout << "V jakem rozpeti poli do vsech stran budu hledat souseda? ( 2 - N-1)" << endl;
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
    }

    for (int i = 1; y + i < (int)map.size() && x - i >= 0; i++) {
        if (map[y + i][x - i] == 0)
            map[y + i][x - i] = 1;
    }

    for (int i = 1; y - i >= 0 && x + i < (int)map[0].size(); i++) {
        if (map[y - i][x + i] == 0)
            map[y - i][x + i] = 1;
    }

    for (int i = 1; y - i >= 0 && x - i >= 0; i++) {
        if (map[y - i][x - i] == 0)
            map[y - i][x - i] = 1;
    }
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

pair<int,int> setFirst( vector<vector<int>>& map ){
    srand(time(0));

    size_t x = rand() % map[0].size();
    size_t y = rand() % map[0].size();

    map[y][x] = 2;
    printMap(map, getScore(map));
    fillZeros(map, x, y);
    return make_pair(x,y);
}

pair<int,int> selectNeighbour(vector<vector<int>>& map, size_t move, pair<int,int> addedLast ){
    vector<pair<int,int>> neighboursVec;
    for (int i = addedLast.first - (int)move; i <= addedLast.first + (int)move; ++i) {
        for (int j = addedLast.second - (int)move; j <= addedLast.second + (int)move; ++j) {
            if (i >= 0 && i < (int)map.size() && j >= 0 && j < (int)map[0].size() && map[j][i] == 0) {
                neighboursVec.push_back({i, j});

            }
        }
    }

    /*for( size_t i = 0; i < neighboursVec.size(); i++){
        cout << "<" << neighboursVec[i].first << ", " << neighboursVec[i].second << "> ";
    }
    cout << endl;*/

    if( neighboursVec.empty()){
        cout << "Nelze vybrat zadne dalsi pole" << endl;
        return make_pair(-1,-1);
    }

    int best = -1;
    pair<int,int> bestNeigbour;
    for( size_t i = 0; i < neighboursVec.size(); i++){
        vector<vector<int>> tmpMap = map;
        tmpMap[neighboursVec[i].second][neighboursVec[i].first] = 2;
        fillZeros(tmpMap, neighboursVec[i].first, neighboursVec[i].second);
        cout << "Pro: " << neighboursVec[i].first << " " << neighboursVec[i].second << " skore: " << getScore(tmpMap) << endl;
        if( best < (int)getScore(tmpMap)){
            cout << "Best" << endl;
            best = getScore(tmpMap);
            bestNeigbour = neighboursVec[i];
        }
    }

    cout << "Vybran prvek: " << bestNeigbour.first << " " << bestNeigbour.second << endl;
    map[bestNeigbour.second][bestNeigbour.first] = 2;
    fillZeros(map, bestNeigbour.first, bestNeigbour.second);
    return bestNeigbour;
}

void hillClimb( size_t size, size_t move, vector<vector<int>>& map){
    pair<int,int> addedLast;
    addedLast = setFirst(map);
    size_t score = getScore(map);
    printMap(map, score);

    for(size_t i = 0; i < size; i++){
        pair<int,int> neighbour = selectNeighbour(map, move, addedLast);
        if( neighbour.first == -1 && neighbour.second == -1)
            break;
        printMap(map, getScore(map));
        addedLast = neighbour;
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
    printMap(map, getScore(map));

    hillClimb(size, move, map);

    return 0;
}