#include <iostream>
#include <algorithm>
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

stack <pair<int, bool>> sizeSquare; // стек на котором хранятся размеры квадратов,которые могут быть вставлены в точку(координаты точки получаются из структуры Figure) и был ли он уже поставлены в фигуру.

stack <Trio> ansCoordinates; // стек, в котором хранится текущие минимальное разбиение фигуры

void print()
{
    for (int i = 0; i < figure.N; i++) {
        for (int j = 0; j < figure.M; j++) {
            cout << figure.rectangle[i][j] << ' ';
        }
        cout << endl;
    }
    //getchar();
}


void maxInsert(int x, int y)// функции передается точка
{                           // функия ищет размеры всех квадратов, которые
    int size;		    // можно вставить в точку и при этом не
					    // этом не перекрыть другие квадраты, которые 						    //уже стоят в фигуре
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
                        maxInsert(x, y);

                    insert(x, y, sizeSquare.top().first, color);
                    figure.coordinates.push(Trio{x, y, sizeSquare.top().first});
                    sizeSquare.top().second = true;

                    color++;

                    cout << "Insert a new square into the Figure"<< endl;
                    print();

                    break;
                }
            }
            if (flag)
                break;
        }
        // если в фигуру вставленно квадратов больше, чем уже в каком-то из известных разбиений, то происходится откат к другим вариантам разбияния
        if (  color - 1 == numberSquares && figure.delivered != figure.M * figure.N)
        {
            cout << "More squares than the minimum known partition.Delete squares\n";
            while ( !sizeSquare.empty() && sizeSquare.top().second)
            {
                sizeSquare.pop();
                clear(figure.coordinates.top());
                figure.coordinates.pop();
                color--;

            }
            print();
        }
        // если фигура была полность покрыта, тогда проверяется минимальное ли это разбиение, если да, то оно запоминается, если разбиение на столько квадратов уже сущетсвует, то счетчик вариантон разбиение увеличивается
        if ( !sizeSquare.empty() && figure.delivered == figure.M * figure.N)
        {
            cout << "The figure is tiled with the number of squares less than or equal to the current split. Remember the number of squares and delete.\n";
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
            }
            print();

        }
    }while (!sizeSquare.empty());

    return make_pair(numberSquares, numberColorings);
}


int main() {
    pair <int, int> ans;
    cout << "Enter the size of the rectangle:\n";
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
    cout << "The coordinates of the inserted squares and their size:\n";
    for (int i = 0; i < ans.first; i++)
    {
        cout << ansCoordinates.top().x << ' ' << ansCoordinates.top().y << ' ' << ansCoordinates.top().size << endl;
        ansCoordinates.pop();
    }

    return 0;
}
