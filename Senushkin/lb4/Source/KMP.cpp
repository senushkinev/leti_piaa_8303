#include <iostream>
#include <vector>
#include <string>
#define DBG



//vector<int> p; // массив для хранения значения префиксфункции искомой подстроки

std::vector<int> prefixFunction(std::string P) // функция считает префикс функцию от всех префиксов строки и записывет значение для всех префиксов в массив p
{
    std::vector<int> p;
    p.resize(P.size() + 1);
    p[0] = 0;
    for (size_t i = 1; i < P.size() + 1; i++)
    {
        int k = p[i - 1];               // получение значение максималной прификс-функции, от строки s[0 ... i - 1]

        while (k > 0 && P[i] != P[k])   // перебираем все строки, которые являются прификсами и суфиксами
        {                               // строки s[0 ... i - 1] и пытаемся расширить их символом s[i]
            k = p[k - 1];
        }
        if (P[i] == P[k])               // если символ s[i] совпал c s[k] значит префикс удалось расширить
            k++;
        p[i] = k;                       // запоминаем длину прифекс-функции для строки s[0 ... i]
    }
    return p;
}




std::vector<int> KMP(const std::string& P, const std::string& T){ // функция ищет все вхождение строки P в строку T и возвращает массив индексов этих вхождений
    std::vector<int> ans;
    std::vector<int> p;


    p = prefixFunction(P);              // для всех префиксов строки P

#ifdef DBG
    std::cout << "Prefix function of " << P << " : ";
    for (size_t i = 0;i < p.size(); i++)
        std::cout << p[i] << ' ';
    std::cout << std::endl;
#endif

    int k = 0;
    for(size_t i = 0; i < T.size(); i++){  // считаем значение префикс-функции для всех префиксов строки P + T
        while (k > 0 && P[k] != T[i])
            k = p[k - 1];
        if (P[k] == T[i])
            k++;

#ifdef DBG
        const std::string green("\033[0;32m");
        const std::string reset("\033[0m");

        std::cout << "Prefix function of \""
             << green << P.substr(0, k) << reset
             << '|' + P.substr(k, P.size()-k) <<
             "\" and \"" + T.substr(0, i-k+1) + '|'
             << green << T.substr(i-k+1, k) << reset
             << "\" = " << k << std::endl;
#endif

        if (k == P.size())                // если длинна прификс-функции совпало с длинной строки P значит в строке Т была найдена строка Р
            ans.push_back(i - P.size() + 1);

    }
    return ans;
}


int main(){
    std::string P;
    std::string T;

    std::cin >> P >> T;

    std::vector<int> a = KMP(P, T);

    if (a.empty()){
        std::cout << -1;
    }else {
        for(size_t i = 0; i < a.size(); i++) {
            std::cout << a[i];
            if (i + 1 != a.size()) std::cout << ',';
        }
    }
    std::cout << std::endl;
    return 0;
}
