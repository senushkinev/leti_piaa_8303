#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <queue>


using namespace std;

struct Cell{
    char name;
    char parent;

    double rough;
};

struct Cmp{
    bool operator()(const Cell& a, const Cell& b)
    {
        if (a.rough == b.rough)
        {
            return a.name < b.name;
        }
        return a.rough > b.rough;
    }

};

map<char, vector<pair<char, double >>> graph;
map<char, bool> closeList;
map<char, pair<char, double>> realWay;
char from, to;
int n;


void readGraph(){
    char start, finish;
    double way;

    cout << "Введите количество вершин снимаемых с очереди за раз: ";
    cin >> n;

    std::cin >> from >> to;

    while (cin >> start)
    {
        if (start == '!')
            break;
        cin >> finish >> way;
        graph[start].push_back(make_pair(finish, way));
    }
}

void printWay(char a){
    if (a == from)
    {
        cout << a;
        return;
    }
    printWay(realWay[a].first);
    cout << a;
}


void aStar()
{
    vector <Cell> cells;
    priority_queue <Cell, vector<Cell>, Cmp> openList;

    openList.push(Cell{from, '\0', 0 + double(to - from)});

    cout << "Intermediate way:\n";

    while(!openList.empty()){
        /*
        for(auto& it : realWay)
        {
            cout << "mW[" << it.first << "]: ";
            printWay(it.first);
            cout << ' ';
        }

        cout << '\n';
         */
        if (openList.top().name == to)
        {
            cout << "A* result: ";
            printWay(to);
            return;
        }

        for (int i = 0 ; i < n && !openList.empty(); i++){
            Cell tmp = openList.top();

            if (tmp.name == to) continue;

            cells.push_back(tmp);
            openList.pop();
        }

        for(int i = 0; i < cells.size(); i++) {

            Cell currCell = cells[i];
            closeList[currCell.name] = true;

            for (int j = 0; j < graph[currCell.name].size(); j++) {
                pair<char, double> newCell = graph[currCell.name][j];


                if (closeList[newCell.first])
                    continue;

                if (realWay[newCell.first].second == 0 || realWay[newCell.first].second > realWay[currCell.name].second + newCell.second)
                {
                    realWay[newCell.first].second = realWay[currCell.name].second + newCell.second;
                    realWay[newCell.first].first = currCell.name;
                }

                openList.push(Cell{newCell.first, currCell.name, realWay[newCell.first].second + double(to - newCell.first)});
            }
        }
        cells.clear();
    }
}


int main(){
    readGraph();

    aStar();

    return 0;
}
