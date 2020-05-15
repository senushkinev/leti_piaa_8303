#include <iostream>
#include <vector>
#include <string>
#define DBG

using namespace std;

vector<int> p; // массив для хранения значения префикс функции искомой подстроки

void prefixFunction(string P) // функция считает префикс функцию от всех префиксов строки и записывет значение для всех префиксов в массив p
{
    p[0] = 0;
    for (size_t i = 1; i < P.size() + 1; i++)
    {
        int k = p[i - 1];
        while (k > 0 && P[i] != P[k]) // алгоритм перебирает все строки, которые являются префиксами и суффиксами, строки s[0 .. i - 1]
        {                             // в порядке уменьшения их длинны и проверят равенство элементов s[i] и s[k], если
            k = p[k - 1];             // они равны, значит алгоритм нашел строку, которая является префиксом и суффиксом строки s[0 .. i],
        }                             // если алгоритм дошел до k = 0, значит у строки s[0 .. i] нет строки, которая является префиксом и суффиксом
        if (P[i] == P[k])             // одновременно, алгоритм запоминает значение прификс функции для строки s[0 .. i]
            k++;
        p[i] = k;
    }
}


int stringRotation(const string& A, const string& B){

    p.resize(B.size() + 1);  // нахождением значение префикс функции
    prefixFunction(B);               // для всех префиксов строки B

#ifdef DBG
    cout << "Prefix function of " << B << " : ";
    for (size_t i = 1;i < p.size(); i++)
        cout << p[i] << ' ';
    cout << endl;
#endif

    int k = 0;

    for (size_t i = 0; i < A.size(); i++){ // алгоритм считает значение префикс функции для всех префиксов строки B + A + A,
        while (k > 0 && B[k] != A[i])      // чтобы не использовать дополнительной памяти и не сохранять нигде строку A + A алгоритм два раза проходит по
            k = p[k - 1];                  // строке A. Вычисление префикс функции начинается с начала строки А так как для строки B все уже вычислено
        if (B[k] == A[i])                  // если значение прификс функции для строки A + A стало равно длине строки B значит, алгоритм нашел значением циклического сдвига
            k++;


        if (k == B.size())
            return (static_cast<int>(i) - static_cast<int>(B.size()) + 1);
    }

    for (size_t i = A.size(); i < A.size() + A.size(); i++){
        while (k > 0 && B[k] != A[i - A.size()])
            k = p[k - 1];
        if (B[k] == A[i - A.size()])
            k++;
        if (k == B.size())
            return (static_cast<int>(i) - static_cast<int>(B.size()) + 1);
    }
    return -1;
}


int main(){
    string A;
    string B;

    cin >> A >> B;

    int a = stringRotation(A, B);

    cout << a;
    cout << endl;
    return 0;
}
//s d f a s d f
//a s d f s d d

