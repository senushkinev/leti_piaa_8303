#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <set>

using namespace std;

struct Node{ // структура для хранения прямого и дополнительно ребра в остаточной сети
    int f1;
    int f2;

    Node() : f1(0), f2(0) {}
    Node(int f1, int f2) : f1(f1), f2(f2){}
};



vector<vector<Node>> network(128, vector<Node>(128)); // остаточная сеть
vector<pair<int, int>> graph; // исходный граф
vector<bool> mark; // посещенные вершины
int source, sink; // исток и сток


int dfs(int v, int delta){ // функция для поиска дополняющего пути
    //cout << static_cast<char>(v);
    if (mark[v]) // если вершина уже была посещена выходим из нее
        return 0;
    mark[v] = true; // помечаем вершину как посещенную

    if (v == sink) // если текущая вершина является конечная, то мы нашли дополняющий путь, возвращаемся обратно и по пути применяем найденную минимальну пропускную способность
        return delta;

    set<pair<int, int>, greater<>> q; // сет для сортировки вершин по пропускной способности

    for (size_t u = 0; u < network[v].size(); u++)
    {
        if (!mark[u] && network[v][u].f2 > 0)
            q.insert(make_pair(network[v][u].f2, u));
        if (!mark[u] && network[v][u].f1 > 0)
            q.insert((make_pair(network[v][u].f1, u)));
    }

    for(auto u : q){ // обходим все смежные вершины в порядке приоретета
        if (network[v][u.second].f2 > 0){
            int newDelta = dfs(u.second, min(delta, network[v][u.second].f2));
            if (newDelta > 0){
                network[u.second][v].f1 += newDelta; //применяем минимальную пропускную способность
                network[v][u.second].f2 -= newDelta;
                return newDelta;
            }
        }
        if (network[v][u.second].f1 > 0){
            int newDelta = dfs(u.second, min(delta, network[v][u.second].f1));
            if (newDelta > 0) {
                network[u.second][v].f2 += newDelta; //применяем минимальную пропускную способность
                network[v][u.second].f1 -= newDelta;
                return newDelta;
            }
        }

    }
    return 0;
}


void print(){
    sort(graph.begin(), graph.end());
    for(size_t i = 0; i < graph.size(); i++){ // выводим ребра графа и то сколько по ним было пущено потока
        cout << static_cast<char>(graph[i].first) << ' ' << static_cast<char>(graph[i].second) << ' ' << network[graph[i].second][graph[i].first].f2 << endl;
    }

}

void readGraph(){
    int N;
    char u, v;
    int c; // capacity

    cin >> N;

    cin >> u >> v;
    source = static_cast<int>(u);
    sink = static_cast<int>(v);

    mark.resize(128);

    for(size_t _ = 0; _ < N; _++){
        cin >> u >> v >> c;
        int i = static_cast<int>(u); //u
        int j = static_cast<int>(v); //v

        graph.emplace_back(i, j);
        network[i][j].f1 = c;
    }
}


void FFA(){
    int flow = 0;
    int ans = 0;
    while (true){
        fill(mark.begin(), mark.end(), false);
        flow = dfs(source, INT_MAX);
        cout << endl;

        if (flow == 0 || flow == INT_MAX) { // если функция вернула поток равный нулю или INT_MAX, значит не было найденно дополняющего пути и был найден максимальный поток
            //cout << "Path not found " << endl;
            break;
        }else {
            //cout << "A new path increased flow by: " << flow << endl;
        }
        ans +=flow;
    }
    //cout << endl;
   // cout << "Max flow: ";
    cout <<  ans << endl;
    print();
}


int main(){
    readGraph();
    //cout << endl;
    FFA();
    return 0;
}
