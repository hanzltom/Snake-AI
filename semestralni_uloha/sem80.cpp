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

#define PERCENTAGE 0.7
#define CHANGE_BEHAVIOUR 6

std::random_device rd;
std::mt19937 gen(rd());

/*
 * Enum representing all moves
 */
enum enumMove{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NOWHERE
};

/*
 * vector with all moves
 */
vector<enumMove> moves = {UP, DOWN, LEFT, RIGHT};

/*
 * Class representing a coordinate on the map
 */
class CPos{
public:
    // X coordinate
    size_t m_x = 0;
    // Y coordinate
    size_t m_y = 0;

    /*
     * Constructors
     */
    CPos() = default;
    CPos(size_t x, size_t y) : m_x(x), m_y(y) {};
    CPos(const CPos& other) : m_x(other.m_x), m_y(other.m_y) {}

    /*
     * Operators for easier manipulation
     */
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

/*
 * Class representing a field position on the map
 */
class CField{
public:
    // position variable
    CPos m_pos;
    // boolean if the field is a wall
    bool m_wall = false;
    // boolean if there is a snake on the field currently
    bool m_snake = false;
    // boolean if there is a coin on the field currently
    bool m_coin = false;
    // boolean for astar
    bool m_opened = false;
    // boolean for bfs
    bool m_bfsOpened = false;

    /*
     * Constructors
     */
    CField()= default;
    CField( size_t x, size_t y, bool wall) :  m_pos(x,y), m_wall(wall) {}
    CField(const CField& other) : m_pos(other.m_pos), m_wall(other.m_wall), m_snake(other.m_snake) {}
};

/*
 * Class representing a board map
 */
class CMap{
public:
    // vector with fields
    vector<vector<CField>> m_map;
    // height of the map
    size_t m_height = 0;
    // width of the map
    size_t m_width = 0;
    // position of the coin on the map
    CPos m_coinPos;

    /*
     * Constructor
     */
    CMap() = default;

    /*
     * Method to check if the coordinates are pointing to field of wall
     */
    bool isWall(size_t x, size_t y){
        if(m_map[y][x].m_wall)
            return true;
        return false;
    }

    /*
     * Method to check if the coordinates are pointing to field of snake
     */
    bool isBody(size_t x, size_t y){
        if(m_map[y][x].m_snake)
            return true;
        return false;
    }
};

/*
 * Class representing a snake
 */
class CSnake{
public:
    // vector of positions of snake parts, head is first
    vector<CPos> m_parts;
    // position of the head for easy access
    CPos m_headPos;
    // boolean to check which snake behaviour we use: false - shortest path, true - longest path
    bool m_behave = false;

    /*
     * Method for changing the behaviour
     */
    void checkBehav(CMap& map){
        if( m_parts.size() > ((map.m_height - 2) * (map.m_width - 2 )) / CHANGE_BEHAVIOUR )
            m_behave = true;
    }

    /*
     * Method to move the snake
     */
    void move(enumMove movement, CMap& map, bool expand){
        CPos nextStep;
        // find next position
        switch( movement ){
            case UP:
                nextStep = CPos(m_parts[0].m_x, m_parts[0].m_y - 1);
                break;
            case LEFT:
                nextStep = CPos(m_parts[0].m_x - 1, m_parts[0].m_y);
                break;
            case DOWN:
                nextStep = CPos(m_parts[0].m_x, m_parts[0].m_y + 1);
                break;
            case RIGHT:
                nextStep = CPos(m_parts[0].m_x + 1, m_parts[0].m_y);
                break;
            case NOWHERE:
                throw logic_error("Never happens");
        }

        CPos lastChanged = m_parts[0];
        map.m_map[nextStep.m_y][nextStep.m_x].m_snake = true;
        // change position of each part to position of the part before
        for( size_t i = 0; i < m_parts.size(); i++){

            if( i == 0 )
                m_parts[i] = nextStep;
            else{
                CPos tmp = m_parts[i];
                m_parts[i] = lastChanged;
                lastChanged = tmp;
            }
        }
        if( !expand && !( lastChanged == nextStep ) )
            map.m_map[lastChanged.m_y][lastChanged.m_x].m_snake = false;
        else
            m_parts.push_back(lastChanged);

        m_headPos = nextStep;
    }

    /*
     * Method to check if there is a snake on the position
     */
    bool isSnake(CPos pos){
        for( size_t i = 0; i < m_parts.size(); i++){
            if( m_parts[i] == pos)
                return true;
        }
        return false;
    }
};

/*
 * Function for map printing
 */
void print_map(CMap& map, CSnake& snake, size_t score = 0, size_t round = 0){
    std::cout << "\033[2J\033[1;1H";

    std::cout << "\033[3J";
    for (size_t i = 0; i < map.m_height; ++i) {
        for (size_t j = 0; j < map.m_width; ++j) {
            if( map.m_map[i][j].m_wall)
                cout << /*"\033[107m" <<*/ "X" /*<< "\033[m"*/;
            else if( snake.isSnake( CPos(j,i) ) )
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

/*
 * Function for loading the map
 */
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

/*
 * Function for setting the head at the beginning
 */
void setHead( CMap& map, CSnake& snake){
    size_t y = map.m_height / 2;
    size_t x = 3;

    map.m_map[y][x].m_snake = true;
    snake.m_parts.push_back(CPos(x,y));
    snake.m_headPos = CPos(x,y);
}

/*
 * Function for generating a coin
 */
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

/*
 * Function for setting m_opened variable as false for astar
 */
void closeFields(CMap& map){
    for(size_t i = 0; i < map.m_height; i++)
        for(size_t j = 0; j < map.m_width; j++)
            map.m_map[i][j].m_opened = false;
}

/*
 * Function which says if the next position is on which direction
 */
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

/*
 * Function which outputs the field distance for astar
 */
double distance(const CPos& start, const CPos& end, size_t length = 0) {
    double x_diff = static_cast<double>(start.m_x) - static_cast<double>(end.m_x);
    double y_diff = static_cast<double>(start.m_y) - static_cast<double>(end.m_y);
    return length + sqrt(pow(x_diff, 2) + pow(y_diff, 2));
}

/*
 * Function for survival behaviour, if astar cannot find the path to the coin
 * outputs the most distanced move
 */
enumMove  survive(CMap& map, CSnake& snake){
    cout << "Survival" << endl;
    CPos furthest;
    double dist;
    enumMove resMove = NOWHERE;

    for (size_t i = 0; i < 4; i++) {
        switch (i) {
            case 0: // UP
                if (snake.m_headPos.m_y > 0) {
                    auto &neighbor = map.m_map[snake.m_headPos.m_y - 1][snake.m_headPos.m_x];

                    if (!neighbor.m_wall && !neighbor.m_snake ) {
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

                    if (!neighbor.m_wall && !neighbor.m_snake ) {
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

                    if (!neighbor.m_wall && !neighbor.m_snake ) {
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

                    if (!neighbor.m_wall && !neighbor.m_snake ) {
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

    return resMove;
}

/*
 * Function for path reconstruction after astar algorithm and getting vector of moves
 */
queue<enumMove>  reconstruct_path(CMap& map, CSnake& snake, std::map<CPos, CPos>& prev){
    vector<CPos> path;
    queue<enumMove> pathRes;
    size_t count = 0;
    size_t mapSize = (map.m_height-2) * (map.m_width-2);

    path.push_back(map.m_coinPos);

    CPos field = prev[map.m_coinPos];

    while ( ! (field == snake.m_headPos)) {
        // if stuck - unstuck solution
        if( count > mapSize + 10){
            pathRes.push(survive(map, snake));
            return pathRes;
        }
        path.push_back(field);
        field = prev[field];
        count++;
    }
    path.push_back(snake.m_headPos);

    std::reverse(path.begin(), path.end());

    for( size_t i = 0; i < path.size() - 1; i++)
        pathRes.push(getDir(path[i], path[i+1]));

    return pathRes;
}

/*
 * Function which outputs if on the given position would be a snake
 * snake positions are in the qPositions deque
 */
bool notSnake(deque<CPos>& qPositions, CPos pos){
    for( size_t i = 0; i < qPositions.size(); i++){
        if( qPositions[i] == pos)
            return false;
    }
    return true;
}

/*
 * Functions with BFS algo to check how many fields are accessible
 */
size_t bfs(CPos start, CMap& map, deque<CPos>& qPositions){
    queue<CPos> q;

    map.m_map[start.m_y][start.m_x].m_bfsOpened = true;
    q.push(start);

    size_t accessibleCount = 0;

    while(!q.empty()){
        CPos current = q.front();
        q.pop();

        accessibleCount++;

        for(enumMove move : moves){
            CPos nextpos;

            if(move == UP) nextpos = CPos(current.m_x, current.m_y - 1);
            else if(move == DOWN) nextpos = CPos(current.m_x, current.m_y + 1);
            else if(move == RIGHT) nextpos = CPos(current.m_x + 1, current.m_y);
            else if(move == LEFT) nextpos = CPos(current.m_x - 1, current.m_y);
            else continue;

            CField& neighbor = map.m_map[nextpos.m_y][nextpos.m_x];

            if(!neighbor.m_wall && !neighbor.m_bfsOpened && notSnake(qPositions, nextpos)){
                neighbor.m_bfsOpened = true;
                q.push(nextpos);
            }
        }
    }

    for(size_t i = 0; i < map.m_height; ++i){
        for(size_t j = 0; j < map.m_width; ++j){
            map.m_map[i][j].m_bfsOpened = false;
        }
    }
    return accessibleCount;
}

/*
 * Function to compare the availability of the new field
 * the new field must have access to minimum of PERCENTAGE * all empty fields
 */
bool getAvailability(CMap& map, CSnake& snake, CPos nextpos, deque<CPos>& snakePos){
    size_t countTotal = (map.m_height - 2) * (map.m_width - 2) - snake.m_parts.size();

    size_t accessible = bfs(nextpos, map, snakePos);

    if( PERCENTAGE * (double)countTotal < (double)accessible) {
        //cout << "True: " << 0.7 * (double)countTotal << " < " << (double)accessible << " Size: " << snakePos.size() << endl;
        return true;
    }
    //cout << "False: " <<0.7 * (double)countTotal << " < " << (double)accessible << " Size: " << snakePos.size() << endl;
    return false;
}

/*
 * Function with modified astar algo
 * It finds longer route without setting seen boolean variables so the snake can have a "snake behaviour" and to visit fields next to each other
 * If it cannot find a way to coin, survival behaviour is used
 */
queue<enumMove>  astar_search_long(CMap& map, CSnake& snake){

    auto cmp = [&snake](const pair<pair<pair<CPos,size_t>, double>, deque<CPos>>& lhs, const pair<pair<pair<CPos,size_t>, double>, deque<CPos>>& rhs) {
        const double epsilon = 1e-12;
        return lhs.first.second + epsilon < rhs.first.second;
    };

    priority_queue<pair<pair<pair<CPos,size_t>, double>, deque<CPos>>, vector<pair<pair<pair<CPos,size_t>, double>, deque<CPos>>>, decltype(cmp)> pq(cmp);
    CPos field_end = map.m_coinPos;

    std::map<CPos, CPos> prev;
    deque<CPos> qPos;
    for(size_t i = 0; i < snake.m_parts.size(); i++)
        qPos.push_back(snake.m_parts[i]);

    pq.emplace(make_pair(make_pair(make_pair(snake.m_headPos, 0), distance(snake.m_headPos, field_end, 0)), qPos));
    map.m_map[snake.m_headPos.m_y][snake.m_headPos.m_x].m_opened = true;

    while( !pq.empty()){

        CPos field = pq.top().first.first.first;
        size_t length = pq.top().first.first.second;
        deque<CPos> snakePos = pq.top().second;
        pq.pop();

        map.m_map[field.m_y][field.m_x].m_opened = true;

        if( field == field_end) {
            return reconstruct_path(map, snake, prev);
        }

        for (size_t i = 0; i < 4; i++) {
            switch (i) {
                case 0: // UP
                    if (field.m_y > 0) {
                        auto &neighbor = map.m_map[field.m_y - 1][field.m_x];

                        /*if( neighbor.m_pos == field_end )
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), 100000), deque<CPos>{}));*/
                        deque<CPos> tmp = snakePos;
                        tmp.push_front(neighbor.m_pos);
                        tmp.pop_back();
                        if (!neighbor.m_wall && !neighbor.m_opened && notSnake(snakePos, neighbor.m_pos) && getAvailability(map, snake, neighbor.m_pos, tmp) ) {
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)), tmp));
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 1: // DOWN
                    if (field.m_y + 1 < map.m_map.size()) {
                        auto &neighbor = map.m_map[field.m_y + 1][field.m_x];

                        /*if( neighbor.m_pos == field_end )
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), 100000), deque<CPos>{}));*/
                        deque<CPos> tmp = snakePos;
                        tmp.push_front(neighbor.m_pos);
                        tmp.pop_back();
                        if (!neighbor.m_wall && !neighbor.m_opened && notSnake(snakePos, neighbor.m_pos) && getAvailability(map, snake, neighbor.m_pos, tmp) ) {
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)), tmp));
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 2: // RIGHT
                    if (field.m_x + 1 < map.m_map[0].size()) {
                        auto &neighbor = map.m_map[field.m_y][field.m_x + 1];

                        /*if( neighbor.m_pos == field_end )
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), 100000), deque<CPos>{}));*/
                        deque<CPos> tmp = snakePos;
                        tmp.push_front(neighbor.m_pos);
                        tmp.pop_back();
                        if (!neighbor.m_wall && !neighbor.m_opened && notSnake(snakePos, neighbor.m_pos) && getAvailability(map, snake, neighbor.m_pos, tmp) ) {
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)), tmp));
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;

                case 3: // LEFT
                    if (field.m_x > 0) {
                        auto &neighbor = map.m_map[field.m_y][field.m_x - 1];

                        /*if( neighbor.m_pos == field_end )
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), 100000), deque<CPos>{}));*/
                        deque<CPos> tmp = snakePos;
                        tmp.push_front(neighbor.m_pos);
                        tmp.pop_back();
                        if (!neighbor.m_wall && !neighbor.m_opened && notSnake(snakePos, neighbor.m_pos) && getAvailability(map, snake, neighbor.m_pos, tmp) ) {
                            pq.emplace(make_pair(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)), tmp));
                            prev[neighbor.m_pos] = field;
                        }
                    }
                    break;
            }
        }
    }
    queue<enumMove> res;
    res.push(survive(map, snake));
    return res;
}

/*
 * Function with classical astar algo
 * It does not return a vector of moves, but only the first move, so the snake is looking for the shortest path every round
 * If it cannot find a way to coin, survival behaviour is used
 */
enumMove  astar_search_short(CMap& map, CSnake& snake){

    auto cmp = [&snake](const pair<pair<CPos,size_t>, double>& lhs, const pair<pair<CPos,size_t>, double>& rhs) {
        const double epsilon = 1e-12;
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

        if( field == field_end) {
            return reconstruct_path(map, snake, prev).front();
        }

        for (size_t i = 0; i < 4; i++) {
            switch (i) {
                case 0: // UP
                    if (field.m_y > 0) {
                        auto &neighbor = map.m_map[field.m_y - 1][field.m_x];

                        if( neighbor.m_pos == field_end && snake.m_behave )
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), 100000));

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake ) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
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

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake ) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
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

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake ) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
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

                        if (!neighbor.m_wall && !neighbor.m_opened && !neighbor.m_snake ) {
                            pq.emplace(make_pair(make_pair(neighbor.m_pos, length+1), distance(neighbor.m_pos, field_end, length+1)));
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

/*
 * Function which selects the next step of snake
 */
enumMove nextStep(CMap& map, CSnake& snake, queue<enumMove>& foundPath){
    closeFields(map);
    if( !snake.m_behave )
        return astar_search_short(map, snake);
    else if( foundPath.empty() || snake.m_headPos == map.m_coinPos ){
        foundPath = astar_search_long(map, snake);
    }
    enumMove move = foundPath.front();
    foundPath.pop();
    return move;
}

/*
 * Function with main play cycle
 */
void play( CMap& map, CSnake& snake){
    queue<enumMove> foundPath;
    size_t score = 0;
    size_t round = 0;
    bool expand = false;
    genCoin(map);

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        enumMove step = nextStep(map, snake, foundPath);
        snake.move(step, map, expand);
        expand = false;

        if( snake.m_headPos == map.m_coinPos){
            map.m_map[map.m_coinPos.m_y][map.m_coinPos.m_x].m_coin = false;
            score++;
            closeFields(map);
            if( !snake.m_behave ) snake.checkBehav(map);
            genCoin(map);
            expand = true;
        }
        print_map(map, snake, score, round);
        round++;
    }
}

/*
 * Main for setup and game play
 */
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
    try{
        play(map, snake);
    }
    catch( const exception& e){
        cout << "Cannot continue, ending." << endl;
        return 0;
    }

    return 0;
}