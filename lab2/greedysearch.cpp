#include <iostream>

#include <map>
#include <vector>
#include <stack>

using namespace std;


map<char, vector<pair<char, double>>> Graph;
map<char, bool> Visited;


char from, to;


void readGraph()
{
    char start, end;
    double distance;

    std::cin >> from >> to;

    while (cin >> start)
    {
        if (start == '!')
            break;
        cin >> end >> distance;

        Graph[start].push_back(make_pair(end,distance));
        Graph[end];
        Visited[start] = false;
        Visited[end] = false;
    }
}


void print(stack<char>& result)
{
    if (result.empty())
        return;

    char tmp = result.top();
    result.pop();
    print(result);
    cout << tmp;
}

void greedySearch()
{

    stack<char> way;
    stack<char> intermediateDataOutput;

    way.push(from);
    char currPeak = way.top();

    //cout << "Intermediate way: \n";
    do
    {
        //intermediateDataOutput = way;
        //print(intermediateDataOutput);
        //cout << "\n";
        bool anyWay = false;
        char nextPeak;
        double minDistance;


        if (Graph[currPeak].empty())
        {
            Visited[currPeak] = true;

            way.pop();
            currPeak = way.top();
            continue;
        }

        for (int i = 0; i < Graph[currPeak].size(); i++)
        {
            if (!Visited[Graph[currPeak][i].first])
            {
                anyWay = true;
                nextPeak = Graph[currPeak][i].first;
                minDistance = Graph[currPeak][i].second;
                break;
            }
        }

        if (!anyWay)
        {
            Visited[currPeak] = true;

            way.pop();
            currPeak = way.top();
            continue;
        }

        for (int i = 0; i < Graph[currPeak].size(); i++)
        {
            if (!Visited[Graph[currPeak][i].first] && minDistance > Graph[currPeak][i].second)
            {
                nextPeak = Graph[currPeak][i].first;
                minDistance = Graph[currPeak][i].second;
            }
        }

        way.push(nextPeak);
        currPeak = way.top();

    }while (currPeak != to);

    cout << "Greedy search: ";
    print(way);

}


int main() {

    readGraph();

    greedySearch();

    return 0;
}
