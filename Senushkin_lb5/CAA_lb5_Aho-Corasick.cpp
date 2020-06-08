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

    std::vector<std::pair<int, int>> terminalSuffixes; // Массив в котором хранятся все терминальные суффиксы, для текущейк строки

    int h; // Высота вершин
};


Node* buildTrie(std::vector<std::string> P) {
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
                v->terminalSuffixes.emplace_back(v->h, v->p);
            }
        }
        else {
            v->suffixLink = p->children[x];
            v->terminalSuffixes = p->children[x]->terminalSuffixes;
            std::cout << "Suffix link for the node " << v->value << " references to " << v->suffixLink->value << std::endl;
            if (v->terminal) {
                v->terminalSuffixes.emplace_back(v->h, v->p);
            }
        }
    }
}


std::vector<std::pair<int, int>> findInText(Node* root, const std::string& T, std::vector<std::string>& P) {
    std::vector<std::pair<int, int>> foundString;

    Node* currNode = root;
    for(int i = 0; i < (int)T.size(); i++) {
        std::cout << "Curr symbol is " << T[i] << std::endl;
        if (currNode->children[T[i]] != nullptr) {
            std::cout << "Transition is possible" << std::endl;
            currNode = currNode->children[T[i]];
            for(int j = 0; j < (int)currNode->terminalSuffixes.size(); j++) {
                std::cout << "Find pattern. Index pattern in text " << i - currNode->terminalSuffixes[j].first + 2 << " Patternt: " << P[ currNode->terminalSuffixes[j].second - 1] << std::endl;
                foundString.emplace_back(i - currNode->terminalSuffixes[j].first + 2, currNode->terminalSuffixes[j].second);
                currNode = root;
                break;
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
    return foundString;
}



int main() {
    std::string T; // Текст
    int n; // Количество слов в словаре
    std::vector<std::string> P; // Словарь

    std::cin >> T;
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::string tmp;
        std::cin >> tmp;
        P.push_back(tmp);
    }

    // Построение бора
    std::cout << "Build trie." << std::endl;
    Node* root = buildTrie(P);
    std::cout << std::endl;

    std::cout << "Create suffix link." << std::endl;
    // Построение суффиксных ссылок
    createSuffixLink(root);
    std::cout << std::endl;

    std::cout << "Find patterns in the text: "  << T << std::endl;
    std::vector<std::pair<int, int>> ans = findInText(root, T, P);
    std::cout << std::endl;

    std::cout << "Ans to stepik" << std::endl;
    std::sort(ans.begin(), ans.end());
    for (int i = 0; i < (int)ans.size(); i++) {
        std::cout << ans[i].first << ' ' << ans[i].second << std::endl;
    }

    return 0;
}

