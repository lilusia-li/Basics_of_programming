/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW
#endif
#endif*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>

using namespace std;

struct MARSH
{
    string startinPoint; //Начальный пункт маршрута
    string destination; //Конечный пункт маршрута
    string routeNumber; //Номер маршрута
};

struct List
{
    MARSH marsh;
    List *next;
};

bool readingFile(List **, List **);
int writingFile(List *);

bool chekAndConversionStr(string &); //Проверка корректности нач и кон пунктов маршрута
bool chekAndConversionNumb(string &); //Проверка корректности номера маршрута

void addEnd(List **startList, MARSH temporaryRoute);
void addStart(List **startList, MARSH temporaryRoute);
bool addIndex(List **startList, MARSH temporaryRoute, const int index);

void deleteList(List **); //Удаление списка полностью
bool deleteEl(List **, MARSH, const int); //Удаление элемента (int - статус удаления)
bool deleteEl(List **, string); //Удаление элемента по уникальному ключу - номеру маршрута (используется в deleteEl предыдущем)

void print_List(List *);

bool findEl(List *, List **, string);
bool findSubstring(const string, const string); //Функция для поиска подстроки в строке (используется в findEl)

void redact(List *, string, string, int); //Конкретно меняет то, что было, на то, что надо
MARSH findMarsh(List *, string); //Поиск маршрута по его номеру (номер проверен на совпадения)
void printMarsh(MARSH); //Вывод конкретного маршрута на экран
void redactEl(List *, string); //Функция редактирования

bool isMoreThan (string, string); //Больше ли str1 чем str2
bool isEquals (string, string); //Сравнивает на равенство 2 строки, лояльна к регистру
bool findEl(MARSH, List *); //Поиск, есть ли подобные маршруты в списке (проверка на уникальность)
int clickIsOk (); //Проверка на правильность введённого int

void sortList(List **, int);

int main()
{
    //_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    List *startList = NULL, *startErrorList = NULL;
    List *startFindList = NULL; //Список для функции поиска

    bool flag = false;
    int click = 0, click2 = 0, index;
    MARSH temporaryRoute;
    string firstOrEnd; //Для функции поиска
    bool stop = false;
    while (!stop)
    {
		system("cls");
		cout << "Меню" << endl;
		cout << "1.Загрузить данные из файла" << endl;
		cout << "2.Добавить маршрут" << endl;
		cout << "3.Удалить маршрут" << endl;
		cout << "4.Показать список" << endl;
		cout << "5.Найти маршрут" << endl;
		cout << "6.Отсортировать список" << endl;
		cout << "7.Редактировать маршрут" << endl;
		cout << "8.Удалить весь список" << endl;
		cout << "9.Выгрузить данные в файл" << endl;
		cout << "0.Выход" << endl << endl;
		cout << "Введите номер: ";
		click = clickIsOk ();
        while(click > 9 || click < 0)
        {
            cout << "Вы неверно ввели число (оно должно быть от 1 до 9). Введите еще раз: ";
            click = clickIsOk ();
        }

		switch (click)
		{
            case 1:
                system("cls");
                click2 = 0;
                cout << "Выберите:" << endl;
                cout << "1. Перезаписать данные в список, удалив имеющиеся в списке" << endl;
                cout << "2. Добавить данные к имеющимся в списке" << endl;
                cout << "Ваш выбор: ";

                click2 = clickIsOk ();
                if (click2 > 2 || click2 < 1)
                {
                    cout << "Вы неверно ввели число (оно должно быть от 1 до 2). Возвращайтесь в меню." << endl;
                    break;
                }
                if (click2 == 1)
                    deleteList(&startList);

                if (!readingFile(&startList, &startErrorList))
                {
                    cout << "Ошибка! Файл не открыт!" << endl;
                    return -1;
                }

                if (startList == NULL && startErrorList == NULL) //Если в файле не было ни одной строки (некорретные в том числе)
                {
                    cout << endl << "Файл пуст!" << endl << endl;
                    break;
                }
                cout << endl << endl << "Данные успешно выгружены в список, который выглядит следующим образом:" << endl << endl;
                print_List(startList);

                if (startErrorList != NULL)
                {
                    cout << endl << "Также вывожу строки с ошибками! (Они в работу не взяты):" << endl << endl;
                    print_List(startErrorList);
                    deleteList(&startErrorList);
                }
                else
                    cout << endl << "Ошибочных строк в файле нет" << endl << endl;
                break;
            case 2:
                system("cls");
                click2 = 0;
                cout << "Выберите:" << endl;
                cout << "1. Добавить маршрут в конец списка" << endl;
                cout << "2. Добавить маршрут в начало списка" << endl;
                cout << "3. Добавить маршрут в список по индексу" << endl;
                cout << "Ваш выбор: ";

                click2 = clickIsOk ();
                if (click2 > 3 || click2 < 1)
                {
                    cout << "Вы неверно ввели число (оно должно быть от 1 до 3). Возвращайтесь в меню." << endl;
                    break;
                }
                while (!flag)
                {
                    cout << "Введите начальный пункт маршрута: ";
                    getline(cin, temporaryRoute.startinPoint);
                    while (!chekAndConversionStr(temporaryRoute.startinPoint))
                    {
                        cout << "Данные введены некорректно. Попробуйте еще раз: ";
                        getline(cin, temporaryRoute.startinPoint);
                    }

                    cout << "Введите конечный пункт маршрута: ";
                    getline(cin, temporaryRoute.destination);
                    while (!chekAndConversionStr(temporaryRoute.destination))
                    {
                        cout << "Данные введены некорректно. Попробуйте еще раз: ";
                        getline(cin, temporaryRoute.destination);
                    }

                    cout << "Введите номер маршрута: ";
                    getline(cin, temporaryRoute.routeNumber);
                    while (!chekAndConversionNumb(temporaryRoute.routeNumber))
                    {
                        cout << "Данные введены некорректно. Попробуйте еще раз: ";
                        getline(cin, temporaryRoute.routeNumber);
                    }

                    if (findEl(temporaryRoute, startList)) //Если введённый маршрут не уникален
                        cout << endl << "Маршрут не уникален. Попробуйте еще раз." << endl << endl;
                    else
                        flag = true; //Можно идти дальше, маршрут прошел проверки
                }
                flag = false;
                if (click2 == 1)
                {
                    addEnd(&startList, temporaryRoute);
                    cout << endl << "Данные введены корректно. Маршрут добавлен в список:" << endl;
                }
                if (click2 == 2)
                {
                    addStart(&startList, temporaryRoute);
                    cout << endl << "Данные введены корректно. Маршрут добавлен в список:" << endl;
                }
                if (click2 == 3)
                {
                    cout << "Введите индекс, куда хотите вставить новый маршрут (от 1): ";
                    index = clickIsOk();
                    while(!addIndex(&startList, temporaryRoute, index))
                    {
                        cout << "Невозможно вставить элемент по данному индексу. Попробуйте еще раз. Индекс: ";
                        index = clickIsOk();
                    }
                        cout << endl << "Данные введены корректно. Маршрут добавлен в список:" << endl;

                }
                cout << endl;
                print_List(startList);
                break;
            case 3:
                system("cls");
                click2 = 0;
                cout << "Список выглядит следующим образом:" << endl;
                print_List(startList);
                cout << endl << "Выберите:" << endl;
                cout << "1. Удалить все маршруты по названию начального пункта" << endl;
                cout << "2. Удалить все маршруты по названию конечного пункта" << endl;
                cout << "3. Удалить маршрут по номеру маршрута" << endl << endl;
                cout << "Ваш выбор: ";
                click2 = clickIsOk ();
                while (click2 > 3 || click2 < 1)
                {
                    cout << "Вы неверно ввели число (оно должно быть от 1 до 3). Попробуйте снова." << endl;
                    click2 = clickIsOk ();
                }

                if (click2 == 1)
                {
                    cout << "Введите начальный пункт маршрута: " ;
                    getline(cin, temporaryRoute.startinPoint);
                    while (!chekAndConversionStr(temporaryRoute.startinPoint))
                    {
                        cout << "Данные введены некорректно. Попробуйте снова: ";
                        getline(cin, temporaryRoute.startinPoint);
                    }
                    if (!deleteEl(&startList, temporaryRoute, 1))
                    {
                        cout << "Элементы, удовлетворяющие запросу, не найдены. Возвращайтесь в меню." << endl;
                        break;
                    }
                }
                if (click2 == 2)
                {
                    cout << "Введите конечный пункт маршрута: ";
                    getline(cin, temporaryRoute.destination);
                    while(!chekAndConversionStr(temporaryRoute.destination))
                    {
                        cout << "Данные введены некорректно. Попробуйте снова: ";
                        getline(cin, temporaryRoute.destination);
                    }

                    if (!deleteEl(&startList, temporaryRoute, 2))
                    {
                        cout << "Элементы, удовлетворяющие запросу, не найдены. Возвращайтесь в меню." << endl;
                        break;
                    }
                }

                if (click2 == 3)
                {
                    cout << "Введите номер маршрута: ";
                    getline(cin, temporaryRoute.routeNumber);
                    while(!chekAndConversionNumb(temporaryRoute.routeNumber))
                    {
                        cout << "Данные введены некорректно. Попробуйте снова: ";
                        getline(cin, temporaryRoute.routeNumber);
                    }
                    if (!deleteEl(&startList, temporaryRoute, 3))
                    {
                        cout << "Элементы, удовлетворяющие запросу, не найдены. Возвращайтесь в меню." << endl;
                        break;
                    }
                }
                cout << endl << "Удаление произведено. Список выглядит следующим образом:" << endl;
                print_List(startList);
                break;
            case 4:
                system("cls");
                cout << "Список:" << endl;
                print_List(startList);
                break;
            case 5:
                system("cls");
                cout << "Введите название пункта, по которому нужно осуществить поиск: ";
                while (!flag)
                {
                    getline(cin, firstOrEnd);
                    if (!chekAndConversionStr(firstOrEnd)) //Если строка введена корректно
                        cout << "Данные введены некорректно. Попробуйте еще раз: ";
                    else
                    {
                        if (!findEl(startList, &startFindList, firstOrEnd)) //Если ни одного элемента не найдено
                        {
                            if (startList == NULL)
                                cout << "Ничего не найдено, потому что список пуст!" << endl;
                            else
                                cout << "Маршрутов, удовлетворяющих условиям поиска, в списке нет." << endl;
                            flag = true;
                        }
                        else
                        {
                            cout << endl << "Найденные маршруты:" << endl << endl;
                            print_List(startFindList);
                            flag = true;
                        }
                    }
                }
                flag = false;
                deleteList(&startFindList);
                break;
            case 6:
                system("cls");
                sortList(&startList, 1);
                break;
            case 7:
                system("cls");
                cout << "Введите номер маршрута, который хотите отредактировать: " << endl;
                getline(cin, temporaryRoute.routeNumber);
                while (!chekAndConversionNumb(temporaryRoute.routeNumber)) //Если строка введена корректно
                {
                    cout << "Номер маршрута введен некорректно. Попробуйте еще раз." << endl;
                    getline(cin, temporaryRoute.routeNumber);
                }
                if (findEl(temporaryRoute, startList)) //Удостоверились, что элемент с таким номером есть в нашем списке
                    redactEl(startList, temporaryRoute.routeNumber);
                else
                {
                    cout << "Маршрут с таким номером не найден. Возвращайтесь в меню." << endl;
                    break;
                }
                cout << endl << "Список выглядит следующим образом:" << endl;
                print_List(startList);
                break;
            case 8:
                system("cls");
                if (startList == NULL)
                {
                    cout << "Список пуст, нет данных для удаления!" << endl;
                    break;
                }
                cout << "Вы точно хотите удалить весь список? Введите 1, если да, и 0, если вы передумали." << endl;
                click2 = clickIsOk ();
                while (click2 > 1 || click2 < 0)
                {
                    cout << "Вы неверно ввели число (оно должно быть от 0 до 1). Попробуйте снова." << endl;
                    click2 = clickIsOk ();
                }
                if (click2)
                {
                    cout << "Удаление списка произведено." << endl;
                    deleteList(&startList);
                    print_List(startList);
                }
                else
                    cout << "Удаление не произведено. Возвращаю вас в меню." << endl;

                break;
            case 9:
                system("cls");
                if (!writingFile(startList))
                {
                    cout << "Ошибка! Файл для записи не открыт!" << endl;
                    deleteList(&startList);
                    return -1;
                }
                if (writingFile(startList) == 1)
                    cout << "Данные успешно выведены в файл 'kursovik.txt'." << endl << endl;
                else
                    cout << "Ваш список пуст, в файл ничего не передано, предыдущие данные затерты!" << endl << endl;
                break;
            case 0:
                stop = true;
                deleteList(&startList);
                break;
		}
		system("pause");
    }

    /*_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

    _CrtDumpMemoryLeaks();

    system("pause");*/
    return 0;
}

bool readingFile(List **startList, List **startErrorList)
{
    fstream ioputFile;
    ioputFile.open("kursovik.txt", ios::in | ios::out);
    if (!ioputFile.is_open())
        return false;
    ioputFile.seekg(0, ios::end); //Перешли в конец файла
    if (ioputFile.tellg() > 1)
    {
        ioputFile.seekg(-1, ios::end);
        char c;
        ioputFile.get(c);
        if (c != '\n')
            ioputFile << '\n';
    }
    else
        return true; //Файл прочитан, но список пуст - значит файл пуст
    ioputFile.close();


    ifstream inputFile;
    inputFile.open("kursovik.txt", ifstream::binary);
    if (!inputFile.is_open())
        return false;
    while (!inputFile.eof())
    {
        int cursor;
        string s;
        cursor = inputFile.tellg();
        getline(inputFile, s, '\n');
        if ((int)s[s.length()-1] == 13)
            s.erase(s.length()-1, 1); //Удаляем каретку

        MARSH temporaryErrorRoute;
        temporaryErrorRoute.startinPoint = s;
        int amount = 0;
        for (size_t i = 0; i < s.length(); i++)
        {
            if (s[i] == '|')
                amount++;
        }
        if (amount != 2)
            addEnd(startErrorList, temporaryErrorRoute); //Записываем данную строку в список ошибочных строк
        else
        {
            inputFile.seekg(cursor, ios::beg); //Перемещаем курсор на начало строки
            MARSH temporaryRoute; //Временный маршрут, чтобы не портить оригинал
            getline(inputFile, temporaryRoute.startinPoint, '|');
            getline(inputFile, temporaryRoute.destination, '|');
            getline(inputFile, temporaryRoute.routeNumber);

            if (chekAndConversionStr(temporaryRoute.startinPoint) && chekAndConversionStr(temporaryRoute.destination) && chekAndConversionNumb(temporaryRoute.routeNumber))
            {
                //Если введённый маршрут не уникален (мы смогли его найти в списке) или нач и кон пункты идентичны
                if (findEl(temporaryRoute, *startList) || isEquals(temporaryRoute.startinPoint, temporaryRoute.destination))
                    addEnd(startErrorList, temporaryErrorRoute); //Записываем данную строку в список ошибочных строк
                else
                    addEnd(startList, temporaryRoute);
            }
            else
                addEnd(startErrorList, temporaryErrorRoute);
        }
    }
    inputFile.close();
    List *stErList = *startErrorList;
    if (stErList != NULL)
            if (stErList->next == NULL) //Если в списке с ошибками всего 1 строка
                if (stErList->marsh.startinPoint.length() == 0) //И она пустая
                    deleteList(startErrorList); //Если из всех строк всего одна ошибочная и то пустая, то считаем, что ош строк нет
    return true; //Файл успешно прочитан
}
int writingFile(List *stList)
{
    ofstream outputFile;
    outputFile.open("kursovik.txt");
    if (!outputFile.is_open())
        return 0;

    if (stList == NULL)
        return 2;
    while (stList != NULL)
    {
        outputFile << stList->marsh.startinPoint << " | " << stList->marsh.destination << " | " << stList->marsh.routeNumber;
        if (stList->next != NULL)
            outputFile << endl;
        stList = stList->next;
    }
    outputFile.close();
    return 1;
}


bool chekAndConversionStr(string &str) //Корректность дефиса, присутствие ё
{
    string s = "";
    bool flag = true; //Сейчас идут пробелы
    for (size_t i = 0; i < str.length(); i++) //Убираем пробелы и проверяем корректность символов
    {
        if (str[i] == ' ')
        {
            if (flag == false)
                s += ' ';
            flag = true;
        }
        else
        {
            flag = false; //Началась или продолжается запись слова
            if (((int)str[i] < -64 || (int)str[i] > -1) && (int)str[i] != (int)'-' && str[i] != 'ё')
                return false;
            else
            {
                if (str[i] == 'ё')
                    s += 'е';
                else
                    s += str[i];
            }
        }
    }
    if (s[s.length()-1] == ' ')
        s.erase(s.length()-1, 1);
    int k = -1; //Длина текущего слова
    int k1 = 0; //Кол-во '-'
    for (size_t i = 0; i < s.length(); i++) //Проверка на длину слова (больше 2)
    {
        if (s[i] == ' ')
        {
            if (k < 3)
                return false;
            k = 0;
        }
        else
        {
            if (k == -1)
                k = 0;
            if ((int)s[i] >= -64 || (int)s[i] <= -1)
                k++;
            if (s[i] == '-')
            {
                k1++;
                if (k1 > 1 || s[i-1] == ' ' || s[i+1] == ' ')
                    return false;
            }
        }
    }
    if (k < 3)
        return false;
    str = s;
    return true;
}
bool chekAndConversionNumb(string &str) //Корректное число - от 0 до 9999, также недопустимо: 040
{
    string s = "";
    bool flag1 = false; //Запись номера не начата
    bool flag2 = false; //Запись номера не закончена
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            if (flag1) //Если запись номера уже начиналась, значит она закончена
                flag2 = true;
        }
        else
        {
            if (!flag1) //Если запись номера не начиналась, то она начинается сейчас
                {flag1 = true;}
            if (flag1) //Если запись номера начиналась
            {
                if (!flag2) //И не закончилась
                {
                    if (((int)str[i] < 48 || (int)str[i] > 58) && (int)str[i] != 13)
                        return false;
                    else
                    {
                        if ((int)str[i] != 13)
                            s += str[i];
                    }
                }
                else //и закончилась (а значит какие-то еще символы появились
                    return false;
            }
        }
    }
    if (s.length() > 0)
    {
        if (s.length() > 4 || s[0] == '0')
            return false;
    }
    else
        return false;
    str = s;
    return true;
}


void addEnd(List **startList, MARSH temporaryRoute)
{
    List *stList = *startList;
    if (*startList == NULL)
    {
        *startList = new List;
        List *stList = *startList;
        stList->marsh.startinPoint = temporaryRoute.startinPoint;
        stList->marsh.destination = temporaryRoute.destination;
        stList->marsh.routeNumber = temporaryRoute.routeNumber;
        stList->next = NULL;
    }
    else
    {
        while (stList->next != NULL)
            stList = stList->next;
        stList->next = new List;
        stList->next->marsh.startinPoint = temporaryRoute.startinPoint;
        stList->next->marsh.destination = temporaryRoute.destination;
        stList->next->marsh.routeNumber = temporaryRoute.routeNumber;
        stList->next->next = NULL;
    }
}
void addStart(List **startList, MARSH temporaryRoute)
{
    if (*startList == NULL)
        addEnd(startList, temporaryRoute);
    else
    {
        List *oldStList = *startList; //Запомнили адрес "бывшего" первого элемента
        List *newStList = new List; //Создали новый элемент
        //Заполняем значения нового элемента:
        newStList->marsh.startinPoint = temporaryRoute.startinPoint;
        newStList->marsh.destination = temporaryRoute.destination;
        newStList->marsh.routeNumber = temporaryRoute.routeNumber;
        newStList->next = oldStList;
        //Перемещаем указатель на начало списка на новый первый элемент:
        *startList = newStList;
    }
}
bool addIndex(List **startList, MARSH temporaryRoute, int index)
{
    bool flag = false; //Если false - значит элемент не был записан
    if (index == 1)
    {
        addStart(startList, temporaryRoute);
        flag = true;
    }
    else
    {
        List *stList = *startList;
        if (stList == 0)
            flag = false;
        else
        {
            int k = 1;
            while (stList->next != NULL)
            {
                if (k+1 == index)
                {
                    List *oldNext = stList->next; //Запомнили элемент, следующий за тем, который мы добавим
                    //Выделяем память и записываем добавленный элемент
                    stList->next = new List;
                    stList->next->marsh.startinPoint = temporaryRoute.startinPoint;
                    stList->next->marsh.destination = temporaryRoute.destination;
                    stList->next->marsh.routeNumber = temporaryRoute.routeNumber;
                    stList->next->next = oldNext; //Не потеряли связь с конечной частью списка
                    flag = true; //Элемент записан
                }
                stList = stList->next; //Двигаемся по списку
                k++;
            }
            if (k + 1 == index)
            {
                flag = true; //Элемент записан
                addEnd(startList, temporaryRoute);
            }
        }
    }
    return flag; //Записан элемент, или же индекс неверно указан
}


int lengthList(List **startList)
{
    int length = 0;
    List *stList = *startList;
    if (stList == NULL)
        return 0;
    while (stList != NULL)
    {
        length++;
        stList = stList->next;
    }
    return length;
}
void sortList(List **startList, int status)
{
    int m = lengthList(startList) - 1, length = lengthList(startList);
    if (m > 0)
    {
        cout << "Список до сортировки:" << endl;
        print_List(*startList);
        while (m > 0)
        {
            for (int i = 0; i < length - 1; i++)
            {
                if ((i + m) < length)
                {
                    string a = "", b = "";
                    //this запоминает адрес нужного для замены элемента, before -  адрес элемента до нужного для замены, after - после заменяемого
                    List *before1, *before2, *this1, *after1, *this2, *after2, *stList2 = *startList, *stList3 = *startList;
                    List *before, *after;
                    int j = 0;
                    while (stList2->next != NULL)
                    {
                        if (m != 1)
                        {
                            if (j == 0 && i == 0) //Значит обрабатываемый элемент - первый элемент в списке
                            {
                                before1 = NULL; //Просто пометим, чтобы знать, что нужно переставить ук на начало списка
                                this1 = stList2; //stList2 сейчас указывает на голову списка
                                after1 = stList2->next; //Указатель на 2 эл списка
                                a = stList2->marsh.startinPoint;
                            }
                            else
                            {
                                if (j + 1 == i) //Обрабатываемый элемент - от второго в списке
                                {
                                    before1 = stList2;
                                    this1 = stList2->next;
                                    after1 = stList2->next->next;
                                    a = stList2->next->marsh.startinPoint;
                                }
                            }

                            if (j + 1 == i + m)
                            {
                                before2 = stList2;
                                this2 = stList2->next;
                                after2 = stList2->next->next;
                                b = stList2->next->marsh.startinPoint;
                            }
                        }
                        else
                        {
                            if (j == 0 && i == 0) //Значит обрабатываемый элемент - первый элемент в списке
                            {
                                this1 = stList2; //stList2 сейчас указывает на голову списка
                                this2 = stList2->next; //Указатель на 2 эл списка
                                before = NULL; //Просто пометим, чтобы знать, что нужно переставить ук на начало списка
                                after = this1->next->next;

                                a = stList2->marsh.startinPoint;
                                b = stList2->next->marsh.startinPoint;
                            }
                            else
                            {
                                if (j + 1 == i) //Обрабатываемый элемент - от второго в списке
                                {
                                    this1 = stList2->next;
                                    this2 = stList2->next->next;
                                    before = stList2;
                                    after = stList2->next->next->next;

                                    a = stList2->next->marsh.startinPoint;
                                    b = stList2->next->next->marsh.startinPoint;
                                }
                            }
                        }

                        stList2 = stList2->next;
                        j++;
                    }
                    if (isMoreThan (a, b) && !isEquals (a, b) && a != "" && b != "")
                    {
                        if (m != 1)
                        {
                            List *thisTemp = this2;
                            if (before1 == NULL) //Здесь нужно поменять голову списка
                            {
                                before2->next = this1; //Переставили новый элемент
                                before2->next->next = after2; //Заполнили его next предыдущим after

                                *startList = thisTemp; //Переставили новый элемент
                                stList3 = *startList; //Доп указатель, временный, указывает на новый первый элемент
                                stList3->next = after1; //Заполнили его next предыдущим after
                            }
                            else
                            {
                                before2->next = this1;
                                before2->next->next = after2;

                                before1->next = thisTemp;
                                before1->next->next = after1;
                            }
                        }
                        else
                            if (before == NULL) //Здесь нужно поменять голову списка
                            {
                                *startList = this2;
                                stList3 = *startList;
                                stList3->next = this1;
                                stList3->next->next = after;
                            }
                            else
                            {
                                before->next = this2;
                                before->next->next = this1;
                                before->next->next->next = after;
                            }
                    }
                }
                else
                    break;
            }
            m -= 1;
        }
        cout << endl <<"Сортировка выполнена. Отсортированный список:" << endl;
        print_List(*startList);
    }
    else
        cout << endl << "Список негоден для сортировки. Возвращайтесь в меню." << endl;
}


void print_List(List *stList)
{
    if (stList == NULL)
        cout << "Список пуст!" << endl;
    while (stList != NULL)
    {
        cout << stList->marsh.startinPoint << "   " << stList->marsh.destination << "   " << stList->marsh.routeNumber << endl;
        stList = stList->next;
    }
}


void deleteList(List **startList)//Удалить весь список
{
    List *stList = *startList;
    if (*startList != NULL)
    {
        List *newStList = stList->next;
        while (newStList != NULL)
        {
            delete stList;
            *startList = newStList;
            stList = *startList;
            newStList = stList->next;
        }
        delete *startList;
        *startList = NULL;
    }
}
bool deleteEl(List **startList, string routeNumb)
{
    List *stList = *startList;
    if (stList->marsh.routeNumber == routeNumb) //Значит самый первый элемент - искомый
    {
        *startList = stList->next;
        delete stList;
        return true;
    }
    else
    {
        while (stList->next != NULL) //Пока в списке есть хотя бы два элемента
        {
            if (stList->next->next != NULL) //Проверка на то, что второй по счету элемент от *stList не последний
            {
                if (stList->next->marsh.routeNumber == routeNumb) //Рассматриваем элемент, который находится за тем, на который указывает stList
                {
                    List *delEl = stList->next; //Запоминаем элемент, который нужно удалить
                    stList->next = delEl->next; //Переставили указатель
                    stList = delEl->next; //Перешли на след элемент
                    delete delEl;
                    return true;
                }
            }
            else //В списке осталось 2 элемента, рассматриваем последний
            {
                if (stList->next->marsh.routeNumber == routeNumb) //Рассматриваем элемент, который находится за тем, на который указывает stList
                {
                    List *delEl = stList->next; //Запоминаем элемент, который нужно удалить
                    stList->next = NULL; //Переставили указатель
                    delete delEl;
                    return true;
                }
            }
            stList = stList->next;
        }
    }
    return false;
}
bool deleteEl(List **startList, MARSH temporaryRoute, const int status) //Проверить статус до входа
{
    bool flag = false; //Был ли найден и удален элемент
    List *stList = *startList;
    if (stList == NULL)
        return false; //Список пуст
    else
    {
        if (status == 1)//Значит нужно удалить все элементы по начальному пункту
        {
            while (stList != NULL)
            {
                if (isEquals(stList->marsh.startinPoint, temporaryRoute.startinPoint))
                {
                    if (!deleteEl(startList, stList->marsh.routeNumber))
                        flag = false;
                    else
                        flag = true;
                }
                stList = stList->next;
            }
        }
        if (status == 2)//Значит нужно удалить все элементы по конечному пункту
        {
            while (stList != NULL)
            {
                if (isEquals(stList->marsh.destination, temporaryRoute.destination))
                {
                    if (!deleteEl(startList, stList->marsh.routeNumber))
                        flag = false;
                    else
                        flag = true;
                }
                stList = stList->next;
            }
        }
        if (status == 3)//Значит нужно удалить элемент по номеру, введённому с клавиатуры
        {
            if (!deleteEl(startList, temporaryRoute.routeNumber))
                flag = false;
            else
                flag = true;
        }
    }
    return flag;
}


bool findEl(List *stList, List **startFindList, string firstOrEnd) //Задание на поиск по названию пунктов маршрута
{
    bool flag = false;
    if (stList == NULL)
        return false; //Список пуст
    else
    {
        while (stList != NULL)
        {
            if (findSubstring(stList->marsh.startinPoint, firstOrEnd) || findSubstring(stList->marsh.destination, firstOrEnd))
            {
                addEnd(startFindList, stList->marsh);
                flag = true;
            }
            stList = stList->next;
        }
    }
    return flag;
}
bool findEl(MARSH temporaryRoute, List *stList) //Поиск, есть ли подобные маршруты в списке (проверка на уникальность)
{
    if (stList == NULL)
        return false; //Список пуст, подобных маршрутов нет - ОК
    else
    {
        while (stList != NULL)
        {
            //Проверка на совпадение
            if ((isEquals(stList->marsh.startinPoint, temporaryRoute.startinPoint) && isEquals(stList->marsh.destination, temporaryRoute.destination)) || stList->marsh.routeNumber == temporaryRoute.routeNumber)
                return true;
            stList = stList->next;
        }
    }
    return false;
}
bool findSubstring(const string str, const string subStr)
{
    string temporaryStr;
    if (str.length() < subStr.length())
        return false;
    for (size_t i = 0; i < (str.length() - subStr.length() + 1); i++)
    {
        size_t temp = i;
        for (size_t j = 0; j < subStr.length(); j++)
        {
            temporaryStr += str[temp];
            temp++;
        }
        if (isEquals(temporaryStr, subStr))
            return true;
        temporaryStr = "";
    }
    return false;
}

void redact(List *stList, string routeNumb, string replaceStr, int status)
{
    while (stList != NULL)
    {
        //Проверка на совпадение
        if (stList->marsh.routeNumber == routeNumb)
        {
            if (status == 1)
            {
                stList->marsh.startinPoint = replaceStr;
            }
            if (status == 2)
            {
                stList->marsh.destination = replaceStr;
            }
            if (status == 3)
            {
                stList->marsh.routeNumber = replaceStr;
            }
            break;
        }
        stList = stList->next;
    }
}
MARSH findMarsh(List *stList, string routeNumb) //Поиск маршрута по его номеру (номер проверен на совпадения)
{
    MARSH temporaryRoute;
    while (stList != NULL)
    {
        //Выводим выбранный маршрут, чтобы показать, как он выглядит
        if (stList->marsh.routeNumber == routeNumb)
        {
            temporaryRoute.startinPoint = stList->marsh.startinPoint;
            temporaryRoute.destination = stList->marsh.destination;
            temporaryRoute.routeNumber = stList->marsh.routeNumber;
            return temporaryRoute;
        }
        stList = stList->next;
    }
    return temporaryRoute;
}
void printMarsh(MARSH temporaryRoute)//Вывод конкретного маршрута на экран
{
    cout << "Маршрут выглядит следующим образом:" << endl;
    cout << temporaryRoute.startinPoint << "  " << temporaryRoute.destination << "  " << temporaryRoute.routeNumber << endl;
}
void redactEl(List *stList, string routeNumb) //Функция редактирования
{
    bool flag1 = false; //Маячок о том, есть ли ошибка в веденных данных
    MARSH temporaryRouteOrig;
    temporaryRouteOrig.routeNumber = routeNumb;
    int click2 = 0;
    while (click2 != 4)
    {
        MARSH temporaryRouteTemp;
        temporaryRouteOrig = findMarsh(stList, temporaryRouteOrig.routeNumber);
        printMarsh(temporaryRouteOrig);
        cout << "Выберите:" << endl;
        cout << "1. Редактировать начальный пункт" << endl;
        cout << "2. Редактировать конечный пункт" << endl;
        cout << "3. Редактировать номер" << endl;
        cout << "4. Закончить редактирование" << endl << endl;
        cout << "Ваш выбор: ";
        click2 = clickIsOk ();
        while (click2 > 4 || click2 < 1)
        {
            cout << "Вы неверно ввели число (оно должно быть от 1 до 4). Попробуйте снова: ";
            click2 = clickIsOk ();
        }

        if (click2 == 1)
        {
            cout << "Введите начальный пункт маршрута, на который хотите заменить действующий: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.startinPoint);
                if (!chekAndConversionStr(temporaryRouteTemp.startinPoint))
                    cout << "Данные введены некорректно. Попробуйте снова: ";
                else
                    if (isEquals (temporaryRouteOrig.startinPoint, temporaryRouteTemp.startinPoint))
                        cout << "Вы ввели то же, что и было. Редактирование не произведено. Попробуйте снова: ";
                    else
                        if (isEquals (temporaryRouteOrig.destination, temporaryRouteTemp.startinPoint))
                            cout << "Редактирование не произведено, ведь иначе начальный и конечный пункты будут совпадать. Попробуйте снова: ";
                        else
                        {
                            string number = temporaryRouteOrig.routeNumber; //Запоминаем текущий номер маршрута
                            temporaryRouteOrig.startinPoint = temporaryRouteTemp.startinPoint;
                            temporaryRouteOrig.routeNumber = -1; //Делаем это для проверки на уникальность
                            if (findEl(temporaryRouteOrig, stList)) //Если после изменений наш маршрут станет неуникальным, то не позволяем редактирование
                            {
                                temporaryRouteOrig.routeNumber = number; //Возвращаем корректный номер
                                cout << "Данное редактирование невозможно, маршрут станет неуникальным. Попробуйте снова: ";
                            }
                            else
                            {
                                temporaryRouteOrig.routeNumber = number; //Возвращаем корректный номер
                                redact(stList, temporaryRouteOrig.routeNumber, temporaryRouteTemp.startinPoint, 1);
                                flag1 = true;
                            }
                        }
            }
        }
        if (click2 == 2)
        {
            cout << "Введите конечный пункт маршрута, на который хотите заменить действующий: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.destination);
                if (!chekAndConversionStr(temporaryRouteTemp.destination))
                    cout << "Данные введены некорректно. Попробуйте снова: ";
                else
                    if (isEquals (temporaryRouteOrig.destination, temporaryRouteTemp.destination))
                        cout << "Вы ввели то же, что и было. Редактирование не произведено. Попробуйте снова: ";
                    else
                        if (isEquals (temporaryRouteOrig.startinPoint, temporaryRouteTemp.destination))
                            cout << "Редактирование не произведено, ведь иначе начальный и конечный пункты будут совпадать. Попробуйте снова: ";
                        else
                        {
                            string number = temporaryRouteOrig.routeNumber; //Запоминаем текущий номер маршрута
                            temporaryRouteOrig.destination = temporaryRouteTemp.destination;
                            temporaryRouteOrig.routeNumber = -1; //Делаем это для проверки на уникальность
                            if (findEl(temporaryRouteOrig, stList)) //Если после изменений наш маршрут станет неуникальным, то не позволяем редактирование
                            {
                                temporaryRouteOrig.routeNumber = number; //Возвращаем корректный номер
                                cout << "Данное редактирование невозможно, маршрут станет неуникальным. Попробуйте снова: ";
                            }
                            else
                            {
                                temporaryRouteOrig.routeNumber = number; //Возвращаем корректный номер
                                redact(stList, temporaryRouteOrig.routeNumber, temporaryRouteTemp.destination, 2);
                                flag1 = true;
                            }
                        }
            }
        }
        if (click2 == 3)
        {
            cout << "Введите номер маршрута, на который хотите заменить действующий: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.routeNumber);
                if (!chekAndConversionNumb(temporaryRouteTemp.routeNumber))
                    cout << "Данные введены некорректно. Попробуйте снова: ";
                else
                    if (temporaryRouteOrig.routeNumber == temporaryRouteTemp.routeNumber)
                        cout << "Вы ввели то же, что и было. Редактирование не произведено. Попробуйте снова: ";
                    else
                    {
                        string number = temporaryRouteOrig.routeNumber; //Запоминаем текущий номер маршрута
                        string stPoint = temporaryRouteOrig.startinPoint; //Запомнили нач пункт, потому что мы его будем менять
                        temporaryRouteOrig.startinPoint = "-1"; //Для проверки на уникальность
                        temporaryRouteOrig.routeNumber = temporaryRouteTemp.routeNumber;
                        if (findEl(temporaryRouteOrig, stList)) //Если после изменений наш маршрут станет неуникальным, то не позволяем редактирование
                        {
                            //Если редактирование невозможно, то возвращаем номер маршрута
                            temporaryRouteOrig.routeNumber = number;
                            cout << "Данное редактирование невозможно, маршрут станет неуникальным. Попробуйте снова: ";
                        }
                        else
                        {
                            redact(stList, number, temporaryRouteTemp.routeNumber, 3);
                            flag1 = true;
                        }
                    }
            }
        }
        if (click2 == 4)
            break;
        if (flag1)
        {
            cout << endl << "Редактирование произведено." << endl;
            temporaryRouteOrig = findMarsh(stList, temporaryRouteOrig.routeNumber);
            printMarsh(temporaryRouteOrig);
        }
        flag1 = false;
        cout << endl;
        system("pause");
        system("cls");
    }
    cout << "Редактирование завершено." << endl;
}

bool isMoreThan (string str1, string str2) //Больше ли str1 чем str2
{
    size_t len = min(str1.length(), str2.length());
    char c1, c2;
    for (size_t i = 0; i < len; i++)
    {
        if ((int)str1[i] >= -64 && (int)str1[i] <= -33)
            c1 = str1[i] + 32;
        else
            c1 = str1[i];

        if ((int)str2[i] >= -64 && (int)str2[i] <= -33)
            c2 = str2[i] + 32;
        else
            c2 = str2[i];

        if (c1 != c2)
        {
            if (c2 == '-')
            {
                if (c1 == ' ')
                    return true;
                else
                    return false;
            }
            else
            {
                if (c1 == '-')
                {
                    if (c2 == ' ')
                        return false;
                    else
                        return true;
                }
                else
                {
                    if (c1 > c2)
                        return true;
                    else
                        return false;
                }
            }
        }
    }
    //Если мы оказались тут, то одна из строк является подстрокой другой строки
    if (str1.length() > str2.length())
        return true;
    else
        return false;
}
bool isEquals (string str1, string str2) //Функция сравнения строк на равенство, лояльная к регистру
{
    if (str1.length() != str2.length())
        return false;
    for (size_t i = 0; i < str1.length(); i++)
    {
        if ((int)str1[i] >= -64 && (int)str1[i] <= -33)
            str1[i] = str1[i] + 32;
        if ((int)str2[i] >= -64 && (int)str2[i] <= -33)
            str2[i] = str2[i] + 32;
    }
    if (str1 == str2)
        return true;
    else
        return false;
}
int clickIsOk ()
{
    int result;
	cin >> result;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Что-то пошло не так! Введите число правильно: ";
		cin >> result;
	}
	cin.ignore(INT_MAX, '\n');
	return (result);
}
