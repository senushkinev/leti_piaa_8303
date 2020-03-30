#include <iostream>
#include <stack>

using namespace std;

struct Trio{ // структура, которая хранит координаты поставленного квадрата и его размер
    int x;
    int y;
    int size;
};

struct Figure { // структура, которая хранит все данные о фигуре, которую нужно замостить
    int** rectangle;
    int delivered;
    int N, M;
    stack <Trio> coordinates;
} figure;

stack <pair<int, bool>> sizeSquare; // стек, на котором хранится размеры квадратов и был ли он уже поставлен

stack <Trio> ansCoordinates; // стек, в котором хранится текущие минимальное разбиение фигуры

void print()
{
    for (int i = 0; i < figure.N; i++) {
        for (int j = 0; j < figure.M; j++) {
            cout << figure.rectangle[i][j] << ' ';
        }
        cout << endl;
    }
}


void maxInsert(int x, int y) // функция, которая кладется на стек sizeSquare размер всех квадратов,
{                            // которые можно вставивть в точку x, y
    int size;
    for (size = 1; size <= figure.N - 1; size++)
    {
        if (y + size > figure.N)
            return;

        if (x + size > figure.M)
            return;

        for (int i = y; i < y + size; i++)
            for (int j = x; j < x + size; j++)
            {
                if (figure.rectangle[i][j] != 0)
                    return;
            }
        sizeSquare.push(make_pair(size, false));
    }
}


void clear(Trio a) // функия, которая удаляется квадрат из фигуры
{
    figure.delivered -= a.size * a.size;

    for (int i = a.y; i < a.y + a.size; i++)
        for (int j = a.x; j < a.x + a.size; j++)
            figure.rectangle[i][j] = 0;
}


void insert(int x, int y, int size, int color) // функция, которая вставляет квадрат в фигуру
{
    figure.delivered += size * size;

    for (int i = y; i < y + size; i++)
        for (int j = x; j < x + size; j++)
            figure.rectangle[i][j] = color;
}


pair<int, int> tiling() // функция, которая перебирает все возможные варианты мощения квадрата и ищет среди них минимальное
{
    int x = 0, y = 0;
    int color = 1;
    int numberColorings = 0;
    int numberSquares = figure.N * figure.M + 1;
    bool flag;

    maxInsert(x, y);

    do{ // цикл, который работает пока не будут проверенны все комбинации квадратов
        flag = false;

        for (y = 0; y < figure.N; y++) { // циклы, которые ищут первую, пустую клетку и вставляют в нее квадраты
            for (x = 0; x < figure.M; x++)
            {
                if (figure.rectangle[y][x] == 0)
                {
                    flag = true;

                    if (sizeSquare.top().second)
                    {
                        maxInsert(x, y);
                    }


                    insert(x, y, sizeSquare.top().first, color);
                    figure.coordinates.push(Trio{x, y, sizeSquare.top().first});
                    sizeSquare.top().second = true;

                    color++;

                    //cout << "Insert"<< endl;
                    //print();
                    break;
                }
            }
            if (flag)
                break;
        }
        // если в фигуру вставленно квадратов больше, чем уже в каком-то из известных разбиений, то происходится откат к другим вариантам разбияния
        if (  color - 1 == numberSquares && figure.delivered != figure.M * figure.N)
        {
            //cout << "Откат назад, когда уже было вставленно было квадратов" << endl;
            while ( !sizeSquare.empty() && sizeSquare.top().second)
            {
                sizeSquare.pop();
                clear(figure.coordinates.top());
                figure.coordinates.pop();
                color--;
                /*
                cout << "Dell(color - 1 == numberSquares && figure.delivered != figure.M * figure.N)"<< endl;
                print();
                 */
            }
            //print();
        }
        // если фигура была полность покрыта, тогда проверяется минимальное ли это разбиение, если да, то оно запоминается, если разбиение на столько квадратов уже сущетсвует, то счетчик вариантон разбиение увеличивается
        if ( !sizeSquare.empty() && figure.delivered == figure.M * figure.N)
        {
            //cout << "Откат назад, когда фигура была полностью закрашена" << endl;
            if (numberSquares == color - 1)
            {
                numberColorings++;
            } else{
                ansCoordinates = figure.coordinates;
                numberSquares = color - 1;
                numberColorings = 1;
            }


            while ( !sizeSquare.empty() && sizeSquare.top().second )
            {
                sizeSquare.pop();
                clear(figure.coordinates.top());
                figure.coordinates.pop();
                color--;
                /*
                cout << "Dell(figure.delivered == figure.M * figure.N)"<< endl;
                print();
                 */
            }
            //print();
        }
    }while (!sizeSquare.empty());

    return make_pair(numberSquares, numberColorings);
}


int main() {
    pair <int, int> ans;
    cin >> figure.N >> figure.M;

    if (figure.N > figure.M)
    {
        swap(figure.N, figure.M);
    }

    figure.rectangle = new int * [figure.N];

    for (int i = 0; i < figure.N; i++)
    {
        figure.rectangle[i] = new int[figure.M];
    }

    for (int i = 0; i < figure.N; i++)
        for (int j = 0; j < figure.M; j++)
        {
            figure.rectangle[i][j] = 0;
        }

    ans = tiling();

    cout << "Minimum number of squares: " << ans.first << endl
         << "Number of minimum constellations: " << ans.second << endl;

    

    for (int i = 0; i < ans.first; i++)
    {
        cout << ansCoordinates.top().x << ' ' << ansCoordinates.top().y << ' ' << ansCoordinates.top().size << endl;
        insert(ansCoordinates.top().x, ansCoordinates.top().y, ansCoordinates.top().size, i + 1);
        ansCoordinates.pop();
    }

    //print();


    return 0;
}
