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
// abc ab a

void printWay(Node* node) {
    if (node == nullptr)
        return;
    printWay(node->parent);
    std::cout << node->value;
}

Node* buildTrie(std::vector<std::string> P) {
    // создание корня
    Node* root = new Node(nullptr, 'R', 0);
    for(int i = 0; i < (int)P.size(); i++) {
        // какждый новый шаблон добавляется в бор начиная с корня
        Node* currNode = root;
        for(int j = 0; j < (int)P[i].size(); j++) {
            // если такой переход уже есть, то просто переходим в следующие вергину
            if(currNode->children[P[i][j]] != nullptr) {
                currNode = currNode->children[P[i][j]];
            } else {
                // если такого перехода нет, то создаем нового ребенка
                std::cout << "New node: " << currNode->value << '(';
                printWay(currNode);
                std::cout << ')' << std::endl;
                currNode->children[P[i][j]] = new Node(currNode, P[i][j], currNode->h + 1);
                currNode = currNode->children[P[i][j]];
            }
        }
        // помечаем вершины, в которых кончается шаблон
        currNode->terminal = true;
        currNode->p = i + 1;
        std::cout << "Node " << currNode->value << '(';
        printWay(currNode);
        std::cout << ") is terminal. Word " << P[i] << std::endl;
    }

    return root;
}


void createSuffixLink(Node* root) {
    std::queue<Node*> q;
    std::map<char, Node*>::iterator it;
    // кладем в очередь все вершины в которые можно попасть из корня
    for(it = root->children.begin(); it != root->children.end(); it++) {
        if (it->second != nullptr)
            q.push(it->second);
    }

    while (!q.empty()) {
        // считыавем верхнию вершину из очереди
        Node* v = q.front();
        q.pop();
        // кладем в очередь все вершины, которые можно достичь из текущей вершины
        for(it = v->children.begin(); it != v->children.end(); it++) {
            if (it->second != nullptr)
                q.push(it->second);
        }

        // переходим по суффиксным ссылкам пока, в какой-нибудь из них не будет перехода
        // по символу текущей вершины или пока не будет достигнут корень
        char x = v->value;
        Node* p = v->parent;
        p = p->suffixLink;
        std::cout << "Parent for current node "<< v->value << '(';
        printWay(v);
        std::cout << ") is ";
        std::cout << v->parent->value << '(';
        printWay(v->parent);
        std::cout << ')' << std::endl;

        if (p != nullptr) {
            std::cout << "Transition the suffix link for this node. ";
            std::cout << "Current node is " << p->value << '(';
            printWay(p);
            std::cout << ')' << std::endl;
        }



        while (p != nullptr && p->children[x] == nullptr){
            std::cout << "Transition the suffix link for this node. ";
            p = p->suffixLink;
            if (p != nullptr) {
                std::cout << "Current node is " << p->value << '(';
                printWay(p);
                std::cout << ')' << std::endl;
            }
        }


        // если суффиксная ссылка указыает на ноль, значит суффиксная ссылка для текущей вершины, должна указывать на корень
        if (p == nullptr) {
            v->suffixLink = root;
            std::cout << "Suffix link for the node " << v->value << '(';
            printWay(v);
            std::cout << ") references to root"  << std::endl;
            if (v->terminal) {
                v->terminalSuffixes.emplace_back(v->h, v->p);
            }
        }
        // если мы нашли не нулевой суффикс, то суффиксная ссылка должна указывать на эту вершину
        else {
            std::cout << "Managed to find non-zero suffix ";
            printWay(p->children[x]);
            std::cout << std::endl;
            v->suffixLink = p->children[x];
            v->terminalSuffixes = p->children[x]->terminalSuffixes;
            std::cout << "Suffix link for the node " << v->value << '(';
            printWay(v);
            std::cout << ") references to " << v->suffixLink->value << std::endl;
            if (v->terminal) {
                v->terminalSuffixes.emplace_back(v->h, v->p);
            }
        }
        std::cout << std::endl;
    }
}


std::vector<std::pair<int, int>> findInText(Node* root, const std::string& T, std::vector<std::string>& P) {
    std::vector<std::pair<int, int>> foundString;

    Node* currNode = root;
    for(int i = 0; i < (int)T.size(); i++) {
        std::cout << "Curr symbol is " << T[i] << '(' << i << ')' << std::endl;
        // если мы можем перейти считанному символу, то переходим
        if (currNode->children[T[i]] != nullptr) {
            std::cout << "Transition is possible. Curr node is " << currNode->children[T[i]]->value << '(';
            printWay(currNode->children[T[i]]);
            std::cout << ')' << std::endl;
            currNode = currNode->children[T[i]];
            // если вершина терминальная, то запоминает индекс вхождения шаблона в текст и индекс шаблона в массиве шаблонов
            for(int j = 0; j < (int)currNode->terminalSuffixes.size(); j++) {
                std::cout << "Find pattern. Index pattern in text " << i - currNode->terminalSuffixes[j].first + 2 << " Patternt: " << P[ currNode->terminalSuffixes[j].second - 1] << std::endl;
                foundString.emplace_back(i - currNode->terminalSuffixes[j].first + 2, currNode->terminalSuffixes[j].second);
                currNode = root;
                break;
            }
            // если переход невозможен, то переходим по суффиксной ссылки
        } else {
            while (currNode->children[T[i]] == nullptr && currNode != root) {
                currNode = currNode->suffixLink;
                std::cout << "Transition is not possible, use suffix link" << std::endl;
                std::cout << "Curr node is " << currNode->value << '(';
                printWay(currNode);
                std::cout << ')' << std::endl;
            }
            if (currNode->children[T[i]] != nullptr) {
                i--;
                std::cout << "No null suffix was found" << std::endl;
            } else {
                std::cout << "The suffix was not found, we are at the root" << std::endl;
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

    std::cout << "Create suffix link. R - mean root" << std::endl;
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

