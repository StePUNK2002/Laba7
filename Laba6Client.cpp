// Laba6Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Laba6DLL.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    HANDLE turnBufE = CreateEvent(NULL, FALSE, TRUE, L"turnBufE");
    HANDLE turnBufF = CreateEvent(NULL, FALSE, FALSE, L"turnBufF");
    HANDLE stopTurnThread = CreateEvent(NULL, TRUE, FALSE, L"stopTurnThread");
    Turn turn(0);
    turn.PushToBuf(1); 
    turn.PushToBuf(10);
    WaitForSingleObject(turnBufE, INFINITE);
    SetEvent(turnBufF); 
    SetEvent(stopTurnThread);
    ResetEvent(turnBufF);
    WaitForSingleObject(turnBufF, INFINITE);
    Turn turn1 = turn1.ReadFromFile();
    printf("Очередь, в файле:\n");
    for (int i = 0; i < 2; i++)
    {
        printf("%d итерация чтения ", i+1);
        for (int j = 0; j <= i; j++) {
            printf(" %d", turn.GetFirstEl());
            turn.EjectionElemet();
        }
        printf("\n");
    }
    printf("\n");
    system("Pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
