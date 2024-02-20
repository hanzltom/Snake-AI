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

int main(){
    set<pair<int,int>> seen;
    map<pair<int,int>, pair<int,int>> parent;
    
    queue<pair<int,int>> q;
    q.push(make_pair(0,0));
    seen.insert(make_pair(0,0));

    while(!q.empty()){

        pair<int,int> node = q.front();
        if( node.second == 4) break; // zarazka, mame 4 galony
        q.pop();


        for(int i = 0; i < 6; i++){
            pair<int, int> new_gallons;
            switch(i){
                case 0:
                    //doliti 3
                    new_gallons = make_pair(node.first + 3, node.second);
                    if( node.first == 3) continue; // nadoba uz je plna
                    else if( node.first + 3 > 3) continue; // pretekla by (slo by aby se jen doplnila na max, ale to je jen kombinace vyprazdneni a znova naplneni nadoby)
                    else if( seen.find(new_gallons) != seen.end()) continue; //uz jsme stejny pomer nasli
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                    break;
                case 1:
                    // doliti 5
                    new_gallons = make_pair(node.first, node.second + 5);
                    if( node.second == 5) continue;
                    else if( node.second + 5 > 5) continue;
                    else if( seen.find(new_gallons) != seen.end()) continue;
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                    break;
                case 2:
                    // preliti 3 -> 5
                    if(node.first + node.second <= 5) //obsah nadoby 3 se cely vejde do nadoby 5
                        new_gallons = make_pair(0, node.first + node.second);
                    else{ // obsah se nevejde a v nadobe 3 neco zbyde
                        int empty_room = 5 - node.second;
                        new_gallons = make_pair(node.first - empty_room, 5);
                    }

                    if( seen.find(new_gallons) != seen.end()) continue;
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                        
                    break;
                case 3:
                    //preliti 3 <- 5
                    if(node.first + node.second <= 3)
                        new_gallons = make_pair(node.first + node.second, 0);
                    else{
                        int empty_room = 3 - node.first;
                        new_gallons = make_pair(3, node.second - empty_room);
                    }

                    if( seen.find(new_gallons) != seen.end()) continue;
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                    break;
                case 4:
                    //vyliti 3
                    new_gallons = make_pair(0, node.second);
                    if( seen.find(new_gallons) != seen.end()) continue;
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                    break;
                case 5:
                    // vyliti 5
                    new_gallons = make_pair(node.first, 0);
                    if( seen.find(new_gallons) != seen.end()) continue;
                    else{
                        q.push(new_gallons);
                        seen.insert(new_gallons);
                        parent[new_gallons] = node;
                    }
                    break;
            }
        }
    }

    if( q.front().second != 4)
        cout << "KONEC - 4 litry nelze nalezt" << endl;
    
    deque<pair<int, int>> res;
    res.push_front(q.front());
    while(1){                               //otoceni posloupnosti
        pair<int,int> parent_node = parent[res.front()];
        if( parent_node.first == 0 && parent_node.second == 0){
            res.push_front(parent_node);
            break;
        }
        res.push_front(parent_node);
    }

    cout << "Vysledek: " << endl;
    for( auto i = res.begin(); i != res.end(); i++){
        cout << '(' << i->first << ',' << i->second << ')' << " -> ";
    }
    cout << endl;

    return 0;
}