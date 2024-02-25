#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <ctime>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;

class CField{
public:
    bool m_door = false;
    size_t m_x = 0;
    size_t m_y = 0;
    bool m_opened = false;
    bool m_final_path = false;

    CField()= default;
    CField(bool door, size_t x, size_t y) : m_door(door), m_x(x), m_y(y), m_opened(false), m_final_path(false) {}
    CField(const CField& other) : m_door(other.m_door), m_x(other.m_x), m_y(other.m_y), m_opened(other.m_opened), m_final_path(other.m_final_path) {}


    bool operator<(const CField& other) const {
        if (m_x < other.m_x)
            return true;
        if (m_x > other.m_x)
            return false;

        return m_y < other.m_y;
    }
};

void read_file(vector<vector<CField>>& map, vector<size_t>& start_end_points){
    std::ifstream ifile;
    std::cout << "Type which map to play:" << std::endl;

    string name;
    cin >> name;

    ifile.open( "dataset/36.txt", std::ios::in );
    while( !ifile || !ifile.is_open() ){
        std::cout << "Could not find the file " << std::endl;
        cin >> name;
        ifile.open( name, std::ios::in );
    }

    std::string text;
    std::vector<CField> tmp;
    size_t start_x, start_y, end_x, end_y;
    char dummy;
    int x, y = 0;
    while( getline(ifile, text, '\n' ) ){
        x = 0;
        tmp.clear();
        if( text[0] == 's'){
            std::istringstream iss(text.substr(6));
            iss >> start_x >> dummy >> start_y;
            start_end_points.push_back(start_x);
            start_end_points.push_back(start_y);
            continue;
        }
        else if( text[0] == 'e'){
            std::istringstream iss(text.substr(4));
            iss >> end_x >> dummy >> end_y;
            start_end_points.push_back(end_x);
            start_end_points.push_back(end_y);
            return;
        }
        for(char& c : text) {
            switch( c ){
                case 'X':
                    tmp.emplace_back(CField(true, x, y) );
                    break;
                case ' ':
                    tmp.emplace_back( CField(false, x, y) );
                    break;
                default:
                    throw std::runtime_error("Error loading the map, not known character");
            }
            x++;
        }
        y++;
        map.push_back( tmp );
    }
}

void print_map(vector<vector<CField>>& map, vector<size_t>& start_end_points){
    for( size_t i = 0; i < map.size(); i++){
        for(size_t j = 0; j < map[i].size(); j++){
            if( j == start_end_points[0] && i == start_end_points[1]){
                cout << 'S';
                continue;
            }
            else if( j == start_end_points[2] && i == start_end_points[3]){
                cout << 'E';
                continue;
            }
            else if(map[i][j].m_final_path){
                cout << 'o';
                continue;
            }
            else if(map[i][j].m_opened){
                cout << '#';
                continue;
            }
            else{
                map[i][j].m_door ? cout << 'X' : cout << ' ';
            }
        }
        cout << endl;
    }
    cout << endl;
}

void reconstruct_path(vector<vector<CField>>& m_map, vector<size_t>& start_end_points, std::map<CField, CField>& prev){
    cout << "Rekonstrukce:" << endl;
    auto &field = prev[m_map[start_end_points[3]][start_end_points[2]]];

    cout << "Initial Field: " << field.m_x << ' ' << field.m_y << endl;

    while (field.m_x != start_end_points[0] || field.m_y != start_end_points[1]) {
        cout << "In Loop: " << field.m_x << ' ' << field.m_y << endl;

        if (field.m_x < m_map[0].size() && field.m_y < m_map.size()) {
            m_map[field.m_y][field.m_x].m_final_path = true;
        }

        field = prev[field];
    }

    cout << "Vysledna trasa:" << endl << endl;
    print_map(m_map, start_end_points);
}

void random_search(vector<vector<CField>>& m_map, vector<size_t>& start_end_points){
    std::map<CField, CField> prev;

    vector<CField> vec;
    vec.push_back(m_map[start_end_points[1]][start_end_points[0]]);
    m_map[start_end_points[1]][start_end_points[0]].m_opened = true;

    srand(static_cast<unsigned>(std::time(0)));

    while( !vec.empty()){
        print_map(m_map, start_end_points);
        size_t pos = rand() % vec.size();
        CField field = vec[pos];
        cout << "Nova iterace, vybran prvek" << field.m_x << ' '<< field.m_y << endl;
        vec.erase( vec.begin() + pos);

        if( field.m_x == start_end_points[2] && field.m_y == start_end_points[3]){
            reconstruct_path(m_map, start_end_points, prev);
            return;
        }

        for (size_t i = 0; i < 4; i++) {
            switch (i) {
                case 0: // UP
                    if (field.m_y > 0) {
                        auto &neighbor = m_map[field.m_y - 1][field.m_x];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            vec.emplace_back(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 1: // DOWN
                    if (field.m_y + 1 < m_map.size()) {
                        auto &neighbor = m_map[field.m_y + 1][field.m_x];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            vec.emplace_back(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 2: // RIGHT
                    if (field.m_x + 1 < m_map[0].size()) {
                        auto &neighbor = m_map[field.m_y][field.m_x + 1];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            vec.emplace_back(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 3: // LEFT
                    if (field.m_x > 0) {
                        auto &neighbor = m_map[field.m_y][field.m_x - 1];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            vec.emplace_back(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;
            }
        }

    }
    cout << "Could not find a way to end, sorry" << endl;
}

void bfs(vector<vector<CField>>& m_map, vector<size_t>& start_end_points){
    std::map<CField, CField> prev;

    queue<CField> q;
    q.push(m_map[start_end_points[1]][start_end_points[0]]);
    m_map[start_end_points[1]][start_end_points[0]].m_opened = true;

    while( !q.empty()){
        print_map(m_map, start_end_points);
        CField field = q.front();
        q.pop();

        if( field.m_x == start_end_points[2] && field.m_y == start_end_points[3]){
            reconstruct_path(m_map, start_end_points, prev);
            return;
        }

        for (size_t i = 0; i < 4; i++) {
            switch (i) {
                case 0: // UP
                    if (field.m_y > 0) {
                        auto &neighbor = m_map[field.m_y - 1][field.m_x];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            q.push(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 1: // DOWN
                    if (field.m_y + 1 < m_map.size()) {
                        auto &neighbor = m_map[field.m_y + 1][field.m_x];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            q.push(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 2: // RIGHT
                    if (field.m_x + 1 < m_map[0].size()) {
                        auto &neighbor = m_map[field.m_y][field.m_x + 1];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            q.push(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;

                case 3: // LEFT
                    if (field.m_x > 0) {
                        auto &neighbor = m_map[field.m_y][field.m_x - 1];

                        if (!neighbor.m_door && !neighbor.m_opened) {
                            q.push(neighbor);
                            neighbor.m_opened = true;
                            prev[neighbor] = field;
                        }
                    }
                    break;
            }
        }

    }
    cout << "Could not find a way to end, sorry" << endl;
}

int dfs_godeep(vector<vector<CField>> m_map, vector<size_t>& start_end_points, map<CField, CField> prev, size_t x, size_t y){

    if( x == start_end_points[2] && y == start_end_points[3]){
        reconstruct_path(m_map, start_end_points, prev);
        return 1;
    }

    m_map[y][x].m_opened = true;
    print_map(m_map, start_end_points);

    for (size_t i = 0; i < 4; i++) {
        switch (i) {
            case 0: // UP
                if (y > 0) {
                    if (!m_map[y - 1][x].m_door && !m_map[y - 1][x].m_opened) {
                        prev[m_map[y - 1][x]] = m_map[y][x];
                        if(dfs_godeep(m_map, start_end_points, prev, x, y - 1))
                            return 1;
                    }
                }
                break;

            case 1: // DOWN
                if (y + 1 < m_map.size()) {
                    if (!m_map[y + 1][x].m_door && !m_map[y + 1][x].m_opened) {
                        prev[m_map[y + 1][x]] = m_map[y][x];
                        if(dfs_godeep(m_map, start_end_points, prev, x, y + 1))
                            return 1;
                    }
                }
                break;

            case 2: // RIGHT
                if (x + 1 < m_map[0].size()) {
                    if (!m_map[y][x + 1].m_door && !m_map[y][x + 1].m_opened) {
                        prev[m_map[y][x + 1]] = m_map[y][x];
                        if(dfs_godeep(m_map, start_end_points, prev, x + 1, y))
                            return 1;
                    }
                }
                break;

            case 3: // LEFT
                if (x > 0) {
                    if (!m_map[y][x - 1].m_door && !m_map[y][x - 1].m_opened) {
                        prev[m_map[y][x - 1]] = m_map[y][x];
                        if(dfs_godeep(m_map, start_end_points, prev, x - 1, y))
                            return 1;
                    }
                }
                break;
        }
    }
    return 0;
}

void dfs(vector<vector<CField>>& m_map, vector<size_t>& start_end_points){
    std::map<CField, CField> prev;

    CField field = m_map[start_end_points[1]][start_end_points[0]];
    m_map[start_end_points[1]][start_end_points[0]].m_opened = true;

    for (size_t i = 0; i < 4; i++) {
        switch (i) {
            case 0: // UP
                if (field.m_y > 0) {
                    auto &neighbor = m_map[field.m_y - 1][field.m_x];

                    if (!neighbor.m_door && !neighbor.m_opened) {
                        prev[neighbor] = field;
                        if(dfs_godeep(m_map, start_end_points, prev, field.m_x, field.m_y - 1))
                            return;
                    }
                }
                break;

            case 1: // DOWN
                if (field.m_y + 1 < m_map.size()) {
                    auto &neighbor = m_map[field.m_y + 1][field.m_x];

                    if (!neighbor.m_door && !neighbor.m_opened) {
                        prev[neighbor] = field;
                        if(dfs_godeep(m_map, start_end_points, prev,field.m_x, field.m_y + 1))
                            return;
                    }
                }
                break;

            case 2: // RIGHT
                if (field.m_x + 1 < m_map[0].size()) {
                    auto &neighbor = m_map[field.m_y][field.m_x + 1];

                    if (!neighbor.m_door && !neighbor.m_opened) {
                        prev[neighbor] = field;
                        if(dfs_godeep(m_map, start_end_points, prev,field.m_x + 1, field.m_y))
                            return;
                    }
                }
                break;

            case 3: // LEFT
                if (field.m_x > 0) {
                    auto &neighbor = m_map[field.m_y][field.m_x - 1];

                    if (!neighbor.m_door && !neighbor.m_opened) {
                        prev[neighbor] = field;
                        if(dfs_godeep(m_map, start_end_points, prev,field.m_x - 1, field.m_y))
                            return;
                    }
                }
                break;
        }
    }

}

void greedy_search(vector<vector<CField>>& m_map, vector<size_t>& start_end_points){

}

void astar_search(vector<vector<CField>>& m_map, vector<size_t>& start_end_points){

}

void choose_algo(vector<vector<CField>>& map, vector<size_t>& start_end_points){
    cout << "Which algorithm to choose? Type number:" << endl;
    int num_algo;
    cout << "[1] Random search" << endl << "[2] BFS" << endl << "[3] DFS" << endl << "[4] Greedy Search" << endl << "[5] A*" << endl;
    cin >> num_algo;
    while( num_algo != 1 && num_algo != 2 && num_algo != 3 && num_algo != 4 && num_algo != 5){
        cout << "Not valid number, type again:" << endl;
        cin >> num_algo;
    }
    switch(num_algo){
        case 1:
            random_search(map, start_end_points);
            break;
        case 2:
            bfs(map, start_end_points);
            break;
        case 3:
            dfs(map, start_end_points);
            break;
        case 4:
            greedy_search(map, start_end_points);
            break;
        case 5:
            astar_search(map, start_end_points);
            break;
    }
}

int main(){
    vector<vector<CField>> m_map;
    vector<size_t> start_end_points;
    try{
        read_file(m_map, start_end_points);
    }
    catch (const exception& e) {
        cout << "Exception " << e.what() << endl;
    }

    cout << "Map size:" << m_map.size() << ' ' << m_map[0].size() << endl;
    print_map(m_map, start_end_points);

    cout << "Start: " << start_end_points[0] << ' ' << start_end_points[1] << endl;
    cout << "End: " << start_end_points[2] << ' ' << start_end_points[3] << endl << endl;
    choose_algo(m_map, start_end_points);
    return 0;
}