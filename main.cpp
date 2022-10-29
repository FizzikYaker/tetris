
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
using namespace sf;

const int M = 20; //Размеры окна
const int N = 10;

int field[M][N] = {0}; //Окно, просто окно
int w = 34;            //Пикселей размера.

struct Point //Структура для фигур
{
    int x, y;
} a[4], b[4];

int figures[7][4] = {
    1,
    3,
    5,
    7,

    2,
    4,
    5,
    7,

    3,
    5,
    4,
    6,

    3,
    5,
    4,
    7,

    2,
    3,
    5,
    7,

    3,
    5,
    7,
    6,

    2,
    3,
    4,
    5,
};
// координаты кубиков для получение фигуры

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;
    }
    return 1;
}

int main()
{
    srand(time(0));
    sf::Music music;                 //создаем объект музыки
    music.openFromFile("music.wav"); //загружаем файл
    music.play();                    //воспроизводим музыку

    RenderWindow window(VideoMode(N * w, M * w), "Tetris ot VlaDIKa"); //Окно приложения

    Texture t;
    t.loadFromFile("C:/project/tiles.png"); //Подключение фигуры
    Sprite tiles(t);                        // Получение спрайта

    int dx = 0, colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3; //Время за которое кубик сдвигается вниз
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds(); //начало работы со временем
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event)) //Управление(события)
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) //Отслежвает нажатие на клавишу
            {
                if (event.key.code == Keyboard::W)
                    rotate = true;
                else if (event.key.code == Keyboard::D)
                    dx = 1;
                else if (event.key.code == Keyboard::A)
                    dx = -1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::S))
            delay = 0.05;

        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check())
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        if (rotate)
        {
            Point p = a[1]; //Зависящая точка вращения фигуры
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y; //Сначало ищет разницу между точкой и кубиком
                int y = a[i].x - p.x;

                a[i].x = p.x - x; //потом складывает и меняет положение кубиков
                a[i].y = p.y + y;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                colorNum = 1 + rand() % 7;
                int n = rand() % 7;         //какая фигура
                for (int i = 0; i < 4; i++) // Заполнение координатами
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }

        int k = M - 1;
        for (int i = M - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
                k--;
        }

        dx = 0; //Сбрасывает значение что бы они не улетели
        rotate = false;
        delay = 0.3;

        window.clear(Color::White);

        for (int i = 0; i < M; i++) // Заполнение координатами
        {
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                    continue;
                tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w)); // Деление прямоугольника на кубики по писелям 2D
                tiles.setPosition(j * w, i * w);
                window.draw(tiles); //вывод фигуры на экран когда поставился
            }
        }

        for (int i = 0; i < 4; i++) // Заполнение кубиками(создание оболочки фигуры)
        {
            tiles.setTextureRect(IntRect(colorNum * w, 0, w, w)); // Деление прямоугольника на кубики по писелям 2D
            tiles.setPosition(a[i].x * w, a[i].y * w);            //ставит координаты кубика в нужное место
            window.draw(tiles);                                   //Вывод во время сдвига кубика
        }
        // window.draw(txt);

        window.display();
    }

    return 0;
}