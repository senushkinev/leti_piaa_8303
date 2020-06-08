#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>


struct Node{
    Node(Node* parent, char value, int h){
        this->parent = parent;
        this->value = value;
        this->h = h;

        suffixLink = nullptr;
        terminal = false;
        p = -1;
    }

    Node* parent; // Ссылка на родителя
    Node* suffixLink; // Ссылка на вершину, которая является максимальныым суффиксом текущей строки
    std::map<char, Node*> children = {{'A', nullptr}, {'C', nullptr}, {'G', nullptr}, {'T', nullptr}, {'N', nullptr}}; // Ссылки на детей

    char value; // Символ, который хранится в вершине
    bool terminal; // Является ли вершина терминальной
    int p; // Если вершина является терминальной, то в p хранится индекс строки в словаре

    std::vector<std::pair<int, std::vector<int>>> terminalSuffixes; // Массив в котором хранятся все терминальные суффиксы, для текущейк строки
    std::vector<int> arr;

    int h; // Высота вершин
};


Node* buildTrie(std::vector<std::string>& P, std::vector<int>& l) {
    Node* root = new Node(nullptr, 'R', 0);
    for(int i = 0; i < (int)P.size(); i++) {
        Node* currNode = root;
        for(int j = 0; j < (int)P[i].size(); j++) {
            if(currNode->children[P[i][j]] != nullptr) {
                currNode = currNode->children[P[i][j]];
            } else {
                std::cout << "New node: parrent " << currNode->value << " symbol " << P[i][j] << " word length " << currNode->h + 1 << std::endl;
                currNode->children[P[i][j]] = new Node(currNode, P[i][j], currNode->h + 1);
                currNode = currNode->children[P[i][j]];
            }
        }
        currNode->terminal = true;
        currNode->p = i + 1;
        currNode->arr.emplace_back(l[i]);
        std::cout << "Node " << currNode->value << " is terminal. Word " << P[i] << std::endl;
    }
    return root;
}


void createSuffixLink(Node* root) {
    std::queue<Node*> q;
    std::map<char, Node*>::iterator it;

    for(it = root->children.begin(); it != root->children.end(); it++) {
        if (it->second != nullptr)
            q.push(it->second);
    }

    while (!q.empty()) {
        Node* v = q.front();
        q.pop();
        for(it = v->children.begin(); it != v->children.end(); it++) {
            if (it->second != nullptr)
                q.push(it->second);
        }

        char x = v->value;
        Node* p = v->parent;
        p = p->suffixLink;
        while (p != nullptr && p->children[x] == nullptr)
            p = p->suffixLink;

        if (p == nullptr) {
            v->suffixLink = root;
            std::cout << "Suffix link for the node " << v->value << " references to root"  << std::endl;
            if (v->terminal) {
                v->terminalSuffixes.emplace_back(v->h, v->arr);
            }
        }
        else {
            v->suffixLink = p->children[x];
            v->terminalSuffixes = p->children[x]->terminalSuffixes;
            std::cout << "Suffix link for the node " << v->value << " references to " << v->suffixLink->value << std::endl;
            if (v->terminal) {
                v->terminalSuffixes.emplace_back(v->h, v->arr);
            }
        }
    }
}


std::vector<int> findInText(Node* root, const std::string& T, int patternSize, std::vector<std::string>& P) {
    std::vector<int> C(T.size() + 1, 0);

    Node* currNode = root;
    for(int i = 0; i < (int)T.size(); i++) {
        std::cout << "Curr symbol is " << T[i] << std::endl;
        if (currNode->children[T[i]] != nullptr) {
            std::cout << "Transition is possible" << std::endl;
            currNode = currNode->children[T[i]];
            std::vector<std::pair<int, std::vector<int>>> terSuff = currNode->terminalSuffixes;
            for(int j = 0; j < (int)terSuff.size(); j++) {
                std::cout << "Find pattern. Index pattern in text " << i - currNode->terminalSuffixes[j].first + 2 << " Patternt: " << P[currNode->p - 1] << std::endl;
               for (int k = 0; k < (int)terSuff[j].second.size(); k++) {
                   int curr_i = i - (int)terSuff[j].second[k] + 2 - (int)terSuff[j].first;
                   if (curr_i >= 0 && curr_i + patternSize <= T.size())
                   {
                       C[curr_i]++;
                   }
               }

            }
        } else {
            std::cout << "Transition is not possible, use suffix link" << std::endl;
            while (currNode->children[T[i]] == nullptr && currNode != root) {
                currNode = currNode->suffixLink;
            }
            if (currNode->children[T[i]] != nullptr) {
                i--;
            }
        }
    }
    return C;
}



int main() {
    std::string T; // Текст
    std::string pattern; // Шаблон
    char joker;

    std::cin >> T;
    std::cin >> pattern;
    std::cin >> joker;

    std::vector<std::string> Q; // Словарь подстрок разделенных масками
    std::vector<int> l; // стартовые позиции подстрок в шаблоне

    int j = 0;
    bool flag = false;
    std::string tmp;
    // нахождение всех подстрок и их стартовых позиций в шаблоне
    for(int i = 0; i < (int)pattern.size(); i++) {

        if (pattern[i] == joker && flag) {
            flag = false;
            j++;
            Q.emplace_back(tmp);
            tmp.clear();
        }
        if (!flag && pattern[i] != joker) {
            l.push_back(i + 1);
            flag = true;
        }
        if (flag) {
            tmp += pattern[i];
        }
    }
    if (!tmp.empty())
        Q.emplace_back(tmp);


    // Построение бора
    std::cout << "Build trie." << std::endl;
    Node* root = buildTrie(Q, l);
    std::cout << std::endl;

    // Построение суффиксных ссылок
    std::cout << "Create suffix link." << std::endl;
    createSuffixLink(root);
    std::cout << std::endl;

    std::cout << "Find patterns in the text: "  << T << std::endl;
    std::vector<int> C= findInText(root, T, pattern.size(), Q);
    std::cout << std::endl;

    std::cout << "Ans to stepik" << std::endl;
    int counter = pattern.size();

    for (int i = 0; i < C.size() - l.back() - Q.back().size() + 1; i++) {
        if (C[i] == Q.size() && counter >= pattern.size()){
            std::cout << i + 1 << std::endl;
            counter = 0;
        }
        counter++;
    }

    return 0;
}

