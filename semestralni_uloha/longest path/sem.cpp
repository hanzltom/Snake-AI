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
#include <cmath>
#include <ctime>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

enum enumMove{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NOWHERE
};

vector<enumMove> moves = {UP, DOWN, LEFT, RIGHT};

class CPos{
public:
    size_t m_x = 0;
    size_t m_y = 0;

    CPos() = default;
    CPos(size_t x, size_t y) : m_x(x), m_y(y) {};
    CPos(const CPos& other) : m_x(other.m_x), m_y(other.m_y) {}

    bool operator==(const CPos& other) const{
        if( m_x == other.m_x && m_y == other.m_y)
            return true;
        return false;
    }

    bool operator<(const CPos& other) const {
        if (m_x < other.m_x) return true;
        if (m_x > other.m_x) return false;
        return m_y < other.m_y;
    }
};

class CField{
public:
    CPos m_pos;
    bool m_wall = false;
    bool m_snake = false;
    bool m_coin = false;
    bool m_opened = false;

    CField()= default;
    CField( size_t x, size_t y, bool wall) :  m_pos(x,y), m_wall(wall) {}
    CField(const CField& other) : m_pos(other.m_pos), m_wall(other.m_wall), m_snake(other.m_snake) {}
};

class CMap{
public:
    vector<vector<CField>> m_map;
    size_t m_height = 0;
    size_t m_width = 0;
    CPos m_coinPos;

    CMap() = default;
    bool isWall(size_t x, size_t y){
        if(m_map[y][x].m_wall)
            return true;
        return false;
    }

    bool isBody(size_t x, size_t y){
        if(m_map[y][x].m_snake)
            return true;
        return false;
    }
};

class CSnake{
public:
    vector<CPos> m_parts;
    CPos m_headPos;
    bool m_behave = false; //false - shortest path, true - longest path
    void checkBehav(CMap& map){
        if( m_parts.size() > ((map.m_height - 2) * (map.m_width - 2 )) / 6 )
            m_behave = true;
    }
    void move(enumMove movement, CMap& map, bool expand){
        CPos nextStep;
        switch( movement ){
            case UP:
                nextStep = CPos(m_parts[0].m_x, m_parts[0].m_y - 1);
                /*if(nextStep.m_y == 0)
                    nextStep.m_y = map.m_height-2;*/
                break;
            case LEFT:
                nextStep = CPos(m_parts[0].m_x - 1, m_parts[0].m_y);
                /*if(nextStep.m_x == 0)
                    nextStep.m_x = map.m_width-2;*/
                break;
            case DOWN:
                nextStep = CPos(m_parts[0].m_x, m_parts[0].m_y + 1);
                /*if(nextStep.m_y == map.m_height-1)
                    nextStep.m_y = 1;*/
                break;
            case RIGHT:
                nextStep = CPos(m_parts[0].m_x + 1, m_parts[0].m_y);
                /*if(nextStep.m_x == map.m_width-1)
                    nextStep.m_x = 1;*/
                break;
            case NOWHERE:
                throw logic_error("Never happens");
        }

        CPos lastChanged = m_parts[0];
        map.m_map[nextStep.m_y][nextStep.m_x].m_snake = true;
        for( size_t i = 0; i < m_parts.size(); i++){

            if( i == 0 )
                m_parts[i] = nextStep;
            else{
                CPos tmp = m_parts[i];
                m_parts[i] = lastChanged;
                lastChanged = tmp;
            }
        }
        if( !expand )
            map.m_map[lastChanged.m_y][lastChanged.m_x].m_snake = false;
        else
            m_parts.push_back(lastChanged);

        m_headPos = nextStep;
    }
};


void print_map(CMap& map, CSnake& snake, size_t score = 0, size_t round = 0){
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[3J";
    for (size_t i = 0; i < map.m_height; ++i) {
        for (size_t j = 0; j < map.m_width; ++j) {
            if( map.m_map[i][j].m_wall)
                cout << /*"\033[107m" <<*/ "X" /*<< "\033[m"*/;
            else if( map.m_map[i][j].m_snake )
                if( snake.m_headPos == CPos(j,i) )
                    cout << "\033[31m" << "S" << "\033[m";
                else cout << "\033[32m" << "S" << "\033[m";
            else if( map.m_map[i][j].m_coin )
                cout << "\033[33m" << "O" << "\033[m";
            else
                cout << " ";
        }
        cout << endl;
    }
    cout << "Score: " << score << "   Round: " << round << endl << endl;
}

void getMap(CMap& map){
    size_t width = 0, height = 0;
    cout << "Type width" << endl;
    cin >> width;
    while( std::cin.fail() || width > 20 || width < 5)
    {
        std::cout << "Not a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> width;
    }

    cout << "Type height" << endl;
    cin >> height;
    while( std::cin.fail() || height > 20 || height < 5)
    {
        std::cout << "Not a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> height;
    }

    cout << "Width: " << width << " height: " << height << endl;

    map.m_height = height + 2;
    map.m_width = width + 2;

    map.m_map.resize(map.m_height);
    for (size_t i = 0; i < map.m_height; ++i) {
        map.m_map[i].resize(map.m_width);
    }

    for (size_t i = 0; i < map.m_height; ++i) {
        for (size_t j = 0; j < map.m_width; ++j) {
            if (i == 0 || i == map.m_height - 1 || j == 0 || j == map.m_width - 1) {
                map.m_map[i][j] = CField(j, i, true);  // Set as wall
            } else {
                map.m_map[i][j] = CField(j, i, false);  // Set as empty
            }
        }
    }
}

void setHead( CMap& map, CSnake& snake){
    size_t y = map.m_height / 2;
    size_t x = 3;

    map.m_map[y][x].m_snake = true;
    snake.m_parts.push_back(CPos(x,y));
    snake.m_headPos = CPos(x,y);
}

void genCoin(CMap& map){
    std::uniform_int_distribution<size_t> distribX(0, map.m_width - 1);
    std::uniform_int_distribution<size_t> distribY(0, map.m_height - 1);

    size_t x = distribX(gen);
    size_t y = distribY(gen);

    while(map.isWall(x,y) || map.isBody(x,y)){
        x = distribX(gen);
        y = distribY(gen);
    }

    map.m_map[y][x].m_coin = true;
    map.m_coinPos = CPos(x,y);
}

void closeFields(CMap& map){
    for(size_t i = 0; i < map.m_height; i++)
        for(size_t j = 0; j < map.m_width; j++)
            map.m_map[i][j].m_opened = false;
}

enumMove getDir(CPos start, CPos stop){
    if( start.m_y == stop.m_y + 1)
        return UP;
    else if( start.m_y == stop.m_y - 1)
        return DOWN;
    else if( start.m_x == stop.m_x - 1)
        return RIGHT;
    else if( start.m_x == stop.m_x + 1)
        return LEFT;
    else throw logic_error("Bad direction");
}

double distance(const CPos& start, const CPos& end, size_t length = 0) {
    double x_diff = static_cast<double>(start.m_x) - static_cast<double>(end.m_x);
    double y_diff = static_cast<double>(start.m_y) - static_cast<double>(end.m_y);
    return length + sqrt(pow(x_diff, 2) + pow(y_diff, 2));
}

queue<enumMove> survive(CMap& map, CSnake& snake){
    CPos furthest;
    double dist;
    enumMove resMove = NOWHERE;

    for (size_t i = 0; i < 4; i++) {
        switch (i) {
            case 0: // UP
                if (snake.m_headPos.m_y > 0) {
                    auto &neighbor = map.m_map[snake.m_headPos.m_y - 1][snake.m_headPos.m_x];

                    if (!neighbor.m_wall && !neighbor.m_snake) {
                        if (resMove == NOWHERE || distance(neighbor.m_pos, map.m_coinPos) > dist) {
                            furthest = neighbor.m_pos;
                            dist = distance(neighbor.m_pos, map.m_coinPos);
                            resMove = UP;
                        }

                    }
                }
                break;

            case 1: // DOWN
                if (snake.m_headPos.m_y + 1 < map.m_map.size()) {
                    auto &neighbor = map.m_map[snake.m_headPos.m_y + 1][snake.m_headPos.m_x];

                    if (!neighbor.m_wall && !neighbor.m_snake) {
                        if (resMove == NOWHERE || distance(neighbor.m_pos, map.m_coinPos) > dist) {
                            furthest = neighbor.m_pos;
                            dist = distance(neighbor.m_pos, map.m_coinPos);
                            resMove = DOWN;
                        }
                    }
                }
                break;

            case 2: // RIGHT
                if (snake.m_headPos.m_x + 1 < map.m_map[0].size()) {
                    auto &neighbor = map.m_map[snake.m_headPos.m_y][snake.m_headPos.m_x + 1];

                    if (!neighbor.m_wall && !neighbor.m_snake) {
                        if (resMove == NOWHERE || distance(neighbor.m_pos, map.m_coinPos) > dist) {
                            furthest = neighbor.m_pos;
                            dist = distance(neighbor.m_pos, map.m_coinPos);
                            resMove = RIGHT;
                        }
                    }
                }
                break;

            case 3: // LEFT
                if (snake.m_headPos.m_x > 0) {
                    auto &neighbor = map.m_map[snake.m_headPos.m_y][snake.m_headPos.m_x - 1];

                    if (!neighbor.m_wall && !neighbor.m_snake) {
                        if (resMove == NOWHERE || distance(neighbor.m_pos, map.m_coinPos) > dist) {
                            furthest = neighbor.m_pos;
                            dist = distance(neighbor.m_pos, map.m_coinPos);
                            resMove = LEFT;
                        }
                    }
                }
                break;
        }
    }
    if (resMove == NOWHERE)
        throw logic_error("Cannot get to Coin");

    queue<enumMove> res;
    res.push(resMove);
    return res;
}

queue<enumMove> reconstruct_path(CMap& map, CSnake& snake, std::map<CPos, CPos>& prev){
    vector<CPos> path;
    queue<enumMove> pathRes;

    path.push_back(map.m_coinPos);

    CPos field = prev[map.m_coinPos];

    while ( ! (field == snake.m_headPos)) {
        path.push_back(field);
        field = prev[field];
    }
    path.push_back(snake.m_headPos);

    std::reverse(path.begin(), path.end());

    for( size_t i = 0; i < path.size() - 1; i++)
        pathRes.push(getDir(path[i], path[i+1]));

    return pathRes;
}

queue<enumMove> astar_search(CMap& map, CSnake& snake){

    auto cmp = [&snake](const pair<pair<CPos,size_t>, double>& lhs, const pair<pair<CPos,size_t>, double>& rhs) {
        const double epsilon = 1e-12;
        if( snake.m_behave )
            return lhs.second + epsilon < rhs.second;
        return lhs.second + epsilon > rhs.second;
    };

    priority_queue<pair<pair<CPos,size_t>, double>, vector<pair<pair<CPos,size_t>, double>>, decltype(cmp)> pq(cmp);
    CPos field_end = map.m_coinPos;

    std::map<CPos, CPos> prev;
    pq.emplace(make_pair(make_pair(snake.m_headPos, 0), distance(snake.m_headPos, field_end, 0)));
    map.m_map[snake.m_headPos.m_y][snake.m_headPos.m_x].m_opened = true;

    while( !pq.empty()){
        CPos field = pq.top().first.first;
        size_t length = pq.top().first.second;
        pq.pop();

        if( snake.m_behave ){
            map.m_map[field.m_y][field.m_x].m_opened = true;
        }

        if( field == field_end) {
            return reconstruct_path(map, snake, prev);
        }

        for (size_t i = 0; i < 4; i++) {
            switch (i) {
                case 0: // UP
                    if (field.m_y > 0) {
                        auto &neighbor = map.m_map[field.m_y - 1][field.m_x];

                        if( neighbor.m_pos == field_end && snake.m_behave )
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), 100000));

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
                            if( !snake.m_behave )
                                neighbor.m_opened = true;
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 1: // DOWN
                    if (field.m_y + 1 < map.m_map.size()) {
                        auto &neighbor = map.m_map[field.m_y + 1][field.m_x];

                        if( neighbor.m_pos == field_end && snake.m_behave )
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), 100000));

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
                            if( !snake.m_behave )
                                neighbor.m_opened = true;
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 2: // RIGHT
                    if (field.m_x + 1 < map.m_map[0].size()) {
                        auto &neighbor = map.m_map[field.m_y][field.m_x + 1];

                        if( neighbor.m_pos == field_end && snake.m_behave )
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), 100000));

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
                            if( !snake.m_behave )
                                neighbor.m_opened = true;
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 3: // LEFT
                    if (field.m_x > 0) {
                        auto &neighbor = map.m_map[field.m_y][field.m_x - 1];

                        if( neighbor.m_pos == field_end && snake.m_behave )
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), 100000));

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
                            if( !snake.m_behave )
                                neighbor.m_opened = true;
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;
            }
        }

    }
    return survive(map, snake);
}

enumMove nextStep(CMap& map, CSnake& snake, queue<enumMove>& path){
    if( path.empty() ||  snake.m_headPos == map.m_coinPos){
        if( !path.empty() &&  snake.m_headPos == map.m_coinPos )
            throw logic_error("Path q not empty");

        closeFields(map);
        path = astar_search(map, snake);
    }

    enumMove move = path.front();
    path.pop();
    return move;
}

void play( CMap& map, CSnake& snake){
    queue<enumMove> path;
    size_t score = 0;
    size_t round = 0;
    bool expand = false;
    genCoin(map);

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        enumMove step = nextStep(map, snake, path);
        snake.move(step, map, expand);
        expand = false;

        if( snake.m_headPos == map.m_coinPos){
            map.m_map[map.m_coinPos.m_y][map.m_coinPos.m_x].m_coin = false;
            score++;
            if( !snake.m_behave ) snake.checkBehav(map);
            genCoin(map);
            expand = true;
        }
        print_map(map, snake, score, round);
        round++;
    }
}


int main(){
    cout << "Snake" << endl;
    CMap map;
    CSnake snake;
    try{
        getMap(map);
    }
    catch (const exception& e) {
        cout << "Exception " << e.what() << endl;
    }
    print_map(map, snake);
    setHead(map, snake);
    print_map(map, snake);

    play(map, snake);

    return 0;
}