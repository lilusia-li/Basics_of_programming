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
    string startinPoint; //��������� ����� ��������
    string destination; //�������� ����� ��������
    string routeNumber; //����� ��������
};

struct List
{
    MARSH marsh;
    List *next;
};

bool readingFile(List **, List **);
int writingFile(List *);

bool chekAndConversionStr(string &); //�������� ������������ ��� � ��� ������� ��������
bool chekAndConversionNumb(string &); //�������� ������������ ������ ��������

void addEnd(List **startList, MARSH temporaryRoute);
void addStart(List **startList, MARSH temporaryRoute);
bool addIndex(List **startList, MARSH temporaryRoute, const int index);

void deleteList(List **); //�������� ������ ���������
bool deleteEl(List **, MARSH, const int); //�������� �������� (int - ������ ��������)
bool deleteEl(List **, string); //�������� �������� �� ����������� ����� - ������ �������� (������������ � deleteEl ����������)

void print_List(List *);

bool findEl(List *, List **, string);
bool findSubstring(const string, const string); //������� ��� ������ ��������� � ������ (������������ � findEl)

void redact(List *, string, string, int); //��������� ������ ��, ��� ����, �� ��, ��� ����
MARSH findMarsh(List *, string); //����� �������� �� ��� ������ (����� �������� �� ����������)
void printMarsh(MARSH); //����� ����������� �������� �� �����
void redactEl(List *, string); //������� ��������������

bool isMoreThan (string, string); //������ �� str1 ��� str2
bool isEquals (string, string); //���������� �� ��������� 2 ������, ������� � ��������
bool findEl(MARSH, List *); //�����, ���� �� �������� �������� � ������ (�������� �� ������������)
int clickIsOk (); //�������� �� ������������ ��������� int

void sortList(List **, int);

int main()
{
    //_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    List *startList = NULL, *startErrorList = NULL;
    List *startFindList = NULL; //������ ��� ������� ������

    bool flag = false;
    int click = 0, click2 = 0, index;
    MARSH temporaryRoute;
    string firstOrEnd; //��� ������� ������
    bool stop = false;
    while (!stop)
    {
		system("cls");
		cout << "����" << endl;
		cout << "1.��������� ������ �� �����" << endl;
		cout << "2.�������� �������" << endl;
		cout << "3.������� �������" << endl;
		cout << "4.�������� ������" << endl;
		cout << "5.����� �������" << endl;
		cout << "6.������������� ������" << endl;
		cout << "7.������������� �������" << endl;
		cout << "8.������� ���� ������" << endl;
		cout << "9.��������� ������ � ����" << endl;
		cout << "0.�����" << endl << endl;
		cout << "������� �����: ";
		click = clickIsOk ();
        while(click > 9 || click < 0)
        {
            cout << "�� ������� ����� ����� (��� ������ ���� �� 1 �� 9). ������� ��� ���: ";
            click = clickIsOk ();
        }

		switch (click)
		{
            case 1:
                system("cls");
                click2 = 0;
                cout << "��������:" << endl;
                cout << "1. ������������ ������ � ������, ������ ��������� � ������" << endl;
                cout << "2. �������� ������ � ��������� � ������" << endl;
                cout << "��� �����: ";

                click2 = clickIsOk ();
                if (click2 > 2 || click2 < 1)
                {
                    cout << "�� ������� ����� ����� (��� ������ ���� �� 1 �� 2). ������������� � ����." << endl;
                    break;
                }
                if (click2 == 1)
                    deleteList(&startList);

                if (!readingFile(&startList, &startErrorList))
                {
                    cout << "������! ���� �� ������!" << endl;
                    return -1;
                }

                if (startList == NULL && startErrorList == NULL) //���� � ����� �� ���� �� ����� ������ (����������� � ��� �����)
                {
                    cout << endl << "���� ����!" << endl << endl;
                    break;
                }
                cout << endl << endl << "������ ������� ��������� � ������, ������� �������� ��������� �������:" << endl << endl;
                print_List(startList);

                if (startErrorList != NULL)
                {
                    cout << endl << "����� ������ ������ � ��������! (��� � ������ �� �����):" << endl << endl;
                    print_List(startErrorList);
                    deleteList(&startErrorList);
                }
                else
                    cout << endl << "��������� ����� � ����� ���" << endl << endl;
                break;
            case 2:
                system("cls");
                click2 = 0;
                cout << "��������:" << endl;
                cout << "1. �������� ������� � ����� ������" << endl;
                cout << "2. �������� ������� � ������ ������" << endl;
                cout << "3. �������� ������� � ������ �� �������" << endl;
                cout << "��� �����: ";

                click2 = clickIsOk ();
                if (click2 > 3 || click2 < 1)
                {
                    cout << "�� ������� ����� ����� (��� ������ ���� �� 1 �� 3). ������������� � ����." << endl;
                    break;
                }
                while (!flag)
                {
                    cout << "������� ��������� ����� ��������: ";
                    getline(cin, temporaryRoute.startinPoint);
                    while (!chekAndConversionStr(temporaryRoute.startinPoint))
                    {
                        cout << "������ ������� �����������. ���������� ��� ���: ";
                        getline(cin, temporaryRoute.startinPoint);
                    }

                    cout << "������� �������� ����� ��������: ";
                    getline(cin, temporaryRoute.destination);
                    while (!chekAndConversionStr(temporaryRoute.destination))
                    {
                        cout << "������ ������� �����������. ���������� ��� ���: ";
                        getline(cin, temporaryRoute.destination);
                    }

                    cout << "������� ����� ��������: ";
                    getline(cin, temporaryRoute.routeNumber);
                    while (!chekAndConversionNumb(temporaryRoute.routeNumber))
                    {
                        cout << "������ ������� �����������. ���������� ��� ���: ";
                        getline(cin, temporaryRoute.routeNumber);
                    }

                    if (findEl(temporaryRoute, startList)) //���� �������� ������� �� ��������
                        cout << endl << "������� �� ��������. ���������� ��� ���." << endl << endl;
                    else
                        flag = true; //����� ���� ������, ������� ������ ��������
                }
                flag = false;
                if (click2 == 1)
                {
                    addEnd(&startList, temporaryRoute);
                    cout << endl << "������ ������� ���������. ������� �������� � ������:" << endl;
                }
                if (click2 == 2)
                {
                    addStart(&startList, temporaryRoute);
                    cout << endl << "������ ������� ���������. ������� �������� � ������:" << endl;
                }
                if (click2 == 3)
                {
                    cout << "������� ������, ���� ������ �������� ����� ������� (�� 1): ";
                    index = clickIsOk();
                    while(!addIndex(&startList, temporaryRoute, index))
                    {
                        cout << "���������� �������� ������� �� ������� �������. ���������� ��� ���. ������: ";
                        index = clickIsOk();
                    }
                        cout << endl << "������ ������� ���������. ������� �������� � ������:" << endl;

                }
                cout << endl;
                print_List(startList);
                break;
            case 3:
                system("cls");
                click2 = 0;
                cout << "������ �������� ��������� �������:" << endl;
                print_List(startList);
                cout << endl << "��������:" << endl;
                cout << "1. ������� ��� �������� �� �������� ���������� ������" << endl;
                cout << "2. ������� ��� �������� �� �������� ��������� ������" << endl;
                cout << "3. ������� ������� �� ������ ��������" << endl << endl;
                cout << "��� �����: ";
                click2 = clickIsOk ();
                while (click2 > 3 || click2 < 1)
                {
                    cout << "�� ������� ����� ����� (��� ������ ���� �� 1 �� 3). ���������� �����." << endl;
                    click2 = clickIsOk ();
                }

                if (click2 == 1)
                {
                    cout << "������� ��������� ����� ��������: " ;
                    getline(cin, temporaryRoute.startinPoint);
                    while (!chekAndConversionStr(temporaryRoute.startinPoint))
                    {
                        cout << "������ ������� �����������. ���������� �����: ";
                        getline(cin, temporaryRoute.startinPoint);
                    }
                    if (!deleteEl(&startList, temporaryRoute, 1))
                    {
                        cout << "��������, ��������������� �������, �� �������. ������������� � ����." << endl;
                        break;
                    }
                }
                if (click2 == 2)
                {
                    cout << "������� �������� ����� ��������: ";
                    getline(cin, temporaryRoute.destination);
                    while(!chekAndConversionStr(temporaryRoute.destination))
                    {
                        cout << "������ ������� �����������. ���������� �����: ";
                        getline(cin, temporaryRoute.destination);
                    }

                    if (!deleteEl(&startList, temporaryRoute, 2))
                    {
                        cout << "��������, ��������������� �������, �� �������. ������������� � ����." << endl;
                        break;
                    }
                }

                if (click2 == 3)
                {
                    cout << "������� ����� ��������: ";
                    getline(cin, temporaryRoute.routeNumber);
                    while(!chekAndConversionNumb(temporaryRoute.routeNumber))
                    {
                        cout << "������ ������� �����������. ���������� �����: ";
                        getline(cin, temporaryRoute.routeNumber);
                    }
                    if (!deleteEl(&startList, temporaryRoute, 3))
                    {
                        cout << "��������, ��������������� �������, �� �������. ������������� � ����." << endl;
                        break;
                    }
                }
                cout << endl << "�������� �����������. ������ �������� ��������� �������:" << endl;
                print_List(startList);
                break;
            case 4:
                system("cls");
                cout << "������:" << endl;
                print_List(startList);
                break;
            case 5:
                system("cls");
                cout << "������� �������� ������, �� �������� ����� ����������� �����: ";
                while (!flag)
                {
                    getline(cin, firstOrEnd);
                    if (!chekAndConversionStr(firstOrEnd)) //���� ������ ������� ���������
                        cout << "������ ������� �����������. ���������� ��� ���: ";
                    else
                    {
                        if (!findEl(startList, &startFindList, firstOrEnd)) //���� �� ������ �������� �� �������
                        {
                            if (startList == NULL)
                                cout << "������ �� �������, ������ ��� ������ ����!" << endl;
                            else
                                cout << "���������, ��������������� �������� ������, � ������ ���." << endl;
                            flag = true;
                        }
                        else
                        {
                            cout << endl << "��������� ��������:" << endl << endl;
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
                cout << "������� ����� ��������, ������� ������ ���������������: " << endl;
                getline(cin, temporaryRoute.routeNumber);
                while (!chekAndConversionNumb(temporaryRoute.routeNumber)) //���� ������ ������� ���������
                {
                    cout << "����� �������� ������ �����������. ���������� ��� ���." << endl;
                    getline(cin, temporaryRoute.routeNumber);
                }
                if (findEl(temporaryRoute, startList)) //��������������, ��� ������� � ����� ������� ���� � ����� ������
                    redactEl(startList, temporaryRoute.routeNumber);
                else
                {
                    cout << "������� � ����� ������� �� ������. ������������� � ����." << endl;
                    break;
                }
                cout << endl << "������ �������� ��������� �������:" << endl;
                print_List(startList);
                break;
            case 8:
                system("cls");
                if (startList == NULL)
                {
                    cout << "������ ����, ��� ������ ��� ��������!" << endl;
                    break;
                }
                cout << "�� ����� ������ ������� ���� ������? ������� 1, ���� ��, � 0, ���� �� ����������." << endl;
                click2 = clickIsOk ();
                while (click2 > 1 || click2 < 0)
                {
                    cout << "�� ������� ����� ����� (��� ������ ���� �� 0 �� 1). ���������� �����." << endl;
                    click2 = clickIsOk ();
                }
                if (click2)
                {
                    cout << "�������� ������ �����������." << endl;
                    deleteList(&startList);
                    print_List(startList);
                }
                else
                    cout << "�������� �� �����������. ��������� ��� � ����." << endl;

                break;
            case 9:
                system("cls");
                if (!writingFile(startList))
                {
                    cout << "������! ���� ��� ������ �� ������!" << endl;
                    deleteList(&startList);
                    return -1;
                }
                if (writingFile(startList) == 1)
                    cout << "������ ������� �������� � ���� 'kursovik.txt'." << endl << endl;
                else
                    cout << "��� ������ ����, � ���� ������ �� ��������, ���������� ������ �������!" << endl << endl;
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
    ioputFile.seekg(0, ios::end); //������� � ����� �����
    if (ioputFile.tellg() > 1)
    {
        ioputFile.seekg(-1, ios::end);
        char c;
        ioputFile.get(c);
        if (c != '\n')
            ioputFile << '\n';
    }
    else
        return true; //���� ��������, �� ������ ���� - ������ ���� ����
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
            s.erase(s.length()-1, 1); //������� �������

        MARSH temporaryErrorRoute;
        temporaryErrorRoute.startinPoint = s;
        int amount = 0;
        for (size_t i = 0; i < s.length(); i++)
        {
            if (s[i] == '|')
                amount++;
        }
        if (amount != 2)
            addEnd(startErrorList, temporaryErrorRoute); //���������� ������ ������ � ������ ��������� �����
        else
        {
            inputFile.seekg(cursor, ios::beg); //���������� ������ �� ������ ������
            MARSH temporaryRoute; //��������� �������, ����� �� ������� ��������
            getline(inputFile, temporaryRoute.startinPoint, '|');
            getline(inputFile, temporaryRoute.destination, '|');
            getline(inputFile, temporaryRoute.routeNumber);

            if (chekAndConversionStr(temporaryRoute.startinPoint) && chekAndConversionStr(temporaryRoute.destination) && chekAndConversionNumb(temporaryRoute.routeNumber))
            {
                //���� �������� ������� �� �������� (�� ������ ��� ����� � ������) ��� ��� � ��� ������ ���������
                if (findEl(temporaryRoute, *startList) || isEquals(temporaryRoute.startinPoint, temporaryRoute.destination))
                    addEnd(startErrorList, temporaryErrorRoute); //���������� ������ ������ � ������ ��������� �����
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
            if (stErList->next == NULL) //���� � ������ � �������� ����� 1 ������
                if (stErList->marsh.startinPoint.length() == 0) //� ��� ������
                    deleteList(startErrorList); //���� �� ���� ����� ����� ���� ��������� � �� ������, �� �������, ��� �� ����� ���
    return true; //���� ������� ��������
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


bool chekAndConversionStr(string &str) //������������ ������, ����������� �
{
    string s = "";
    bool flag = true; //������ ���� �������
    for (size_t i = 0; i < str.length(); i++) //������� ������� � ��������� ������������ ��������
    {
        if (str[i] == ' ')
        {
            if (flag == false)
                s += ' ';
            flag = true;
        }
        else
        {
            flag = false; //�������� ��� ������������ ������ �����
            if (((int)str[i] < -64 || (int)str[i] > -1) && (int)str[i] != (int)'-' && str[i] != '�')
                return false;
            else
            {
                if (str[i] == '�')
                    s += '�';
                else
                    s += str[i];
            }
        }
    }
    if (s[s.length()-1] == ' ')
        s.erase(s.length()-1, 1);
    int k = -1; //����� �������� �����
    int k1 = 0; //���-�� '-'
    for (size_t i = 0; i < s.length(); i++) //�������� �� ����� ����� (������ 2)
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
bool chekAndConversionNumb(string &str) //���������� ����� - �� 0 �� 9999, ����� �����������: 040
{
    string s = "";
    bool flag1 = false; //������ ������ �� ������
    bool flag2 = false; //������ ������ �� ���������
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            if (flag1) //���� ������ ������ ��� ����������, ������ ��� ���������
                flag2 = true;
        }
        else
        {
            if (!flag1) //���� ������ ������ �� ����������, �� ��� ���������� ������
                {flag1 = true;}
            if (flag1) //���� ������ ������ ����������
            {
                if (!flag2) //� �� �����������
                {
                    if (((int)str[i] < 48 || (int)str[i] > 58) && (int)str[i] != 13)
                        return false;
                    else
                    {
                        if ((int)str[i] != 13)
                            s += str[i];
                    }
                }
                else //� ����������� (� ������ �����-�� ��� ������� ���������
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
        List *oldStList = *startList; //��������� ����� "�������" ������� ��������
        List *newStList = new List; //������� ����� �������
        //��������� �������� ������ ��������:
        newStList->marsh.startinPoint = temporaryRoute.startinPoint;
        newStList->marsh.destination = temporaryRoute.destination;
        newStList->marsh.routeNumber = temporaryRoute.routeNumber;
        newStList->next = oldStList;
        //���������� ��������� �� ������ ������ �� ����� ������ �������:
        *startList = newStList;
    }
}
bool addIndex(List **startList, MARSH temporaryRoute, int index)
{
    bool flag = false; //���� false - ������ ������� �� ��� �������
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
                    List *oldNext = stList->next; //��������� �������, ��������� �� ���, ������� �� �������
                    //�������� ������ � ���������� ����������� �������
                    stList->next = new List;
                    stList->next->marsh.startinPoint = temporaryRoute.startinPoint;
                    stList->next->marsh.destination = temporaryRoute.destination;
                    stList->next->marsh.routeNumber = temporaryRoute.routeNumber;
                    stList->next->next = oldNext; //�� �������� ����� � �������� ������ ������
                    flag = true; //������� �������
                }
                stList = stList->next; //��������� �� ������
                k++;
            }
            if (k + 1 == index)
            {
                flag = true; //������� �������
                addEnd(startList, temporaryRoute);
            }
        }
    }
    return flag; //������� �������, ��� �� ������ ������� ������
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
        cout << "������ �� ����������:" << endl;
        print_List(*startList);
        while (m > 0)
        {
            for (int i = 0; i < length - 1; i++)
            {
                if ((i + m) < length)
                {
                    string a = "", b = "";
                    //this ���������� ����� ������� ��� ������ ��������, before -  ����� �������� �� ������� ��� ������, after - ����� �����������
                    List *before1, *before2, *this1, *after1, *this2, *after2, *stList2 = *startList, *stList3 = *startList;
                    List *before, *after;
                    int j = 0;
                    while (stList2->next != NULL)
                    {
                        if (m != 1)
                        {
                            if (j == 0 && i == 0) //������ �������������� ������� - ������ ������� � ������
                            {
                                before1 = NULL; //������ �������, ����� �����, ��� ����� ����������� �� �� ������ ������
                                this1 = stList2; //stList2 ������ ��������� �� ������ ������
                                after1 = stList2->next; //��������� �� 2 �� ������
                                a = stList2->marsh.startinPoint;
                            }
                            else
                            {
                                if (j + 1 == i) //�������������� ������� - �� ������� � ������
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
                            if (j == 0 && i == 0) //������ �������������� ������� - ������ ������� � ������
                            {
                                this1 = stList2; //stList2 ������ ��������� �� ������ ������
                                this2 = stList2->next; //��������� �� 2 �� ������
                                before = NULL; //������ �������, ����� �����, ��� ����� ����������� �� �� ������ ������
                                after = this1->next->next;

                                a = stList2->marsh.startinPoint;
                                b = stList2->next->marsh.startinPoint;
                            }
                            else
                            {
                                if (j + 1 == i) //�������������� ������� - �� ������� � ������
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
                            if (before1 == NULL) //����� ����� �������� ������ ������
                            {
                                before2->next = this1; //����������� ����� �������
                                before2->next->next = after2; //��������� ��� next ���������� after

                                *startList = thisTemp; //����������� ����� �������
                                stList3 = *startList; //��� ���������, ���������, ��������� �� ����� ������ �������
                                stList3->next = after1; //��������� ��� next ���������� after
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
                            if (before == NULL) //����� ����� �������� ������ ������
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
        cout << endl <<"���������� ���������. ��������������� ������:" << endl;
        print_List(*startList);
    }
    else
        cout << endl << "������ ������� ��� ����������. ������������� � ����." << endl;
}


void print_List(List *stList)
{
    if (stList == NULL)
        cout << "������ ����!" << endl;
    while (stList != NULL)
    {
        cout << stList->marsh.startinPoint << "   " << stList->marsh.destination << "   " << stList->marsh.routeNumber << endl;
        stList = stList->next;
    }
}


void deleteList(List **startList)//������� ���� ������
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
    if (stList->marsh.routeNumber == routeNumb) //������ ����� ������ ������� - �������
    {
        *startList = stList->next;
        delete stList;
        return true;
    }
    else
    {
        while (stList->next != NULL) //���� � ������ ���� ���� �� ��� ��������
        {
            if (stList->next->next != NULL) //�������� �� ��, ��� ������ �� ����� ������� �� *stList �� ���������
            {
                if (stList->next->marsh.routeNumber == routeNumb) //������������� �������, ������� ��������� �� ���, �� ������� ��������� stList
                {
                    List *delEl = stList->next; //���������� �������, ������� ����� �������
                    stList->next = delEl->next; //����������� ���������
                    stList = delEl->next; //������� �� ���� �������
                    delete delEl;
                    return true;
                }
            }
            else //� ������ �������� 2 ��������, ������������� ���������
            {
                if (stList->next->marsh.routeNumber == routeNumb) //������������� �������, ������� ��������� �� ���, �� ������� ��������� stList
                {
                    List *delEl = stList->next; //���������� �������, ������� ����� �������
                    stList->next = NULL; //����������� ���������
                    delete delEl;
                    return true;
                }
            }
            stList = stList->next;
        }
    }
    return false;
}
bool deleteEl(List **startList, MARSH temporaryRoute, const int status) //��������� ������ �� �����
{
    bool flag = false; //��� �� ������ � ������ �������
    List *stList = *startList;
    if (stList == NULL)
        return false; //������ ����
    else
    {
        if (status == 1)//������ ����� ������� ��� �������� �� ���������� ������
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
        if (status == 2)//������ ����� ������� ��� �������� �� ��������� ������
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
        if (status == 3)//������ ����� ������� ������� �� ������, ��������� � ����������
        {
            if (!deleteEl(startList, temporaryRoute.routeNumber))
                flag = false;
            else
                flag = true;
        }
    }
    return flag;
}


bool findEl(List *stList, List **startFindList, string firstOrEnd) //������� �� ����� �� �������� ������� ��������
{
    bool flag = false;
    if (stList == NULL)
        return false; //������ ����
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
bool findEl(MARSH temporaryRoute, List *stList) //�����, ���� �� �������� �������� � ������ (�������� �� ������������)
{
    if (stList == NULL)
        return false; //������ ����, �������� ��������� ��� - ��
    else
    {
        while (stList != NULL)
        {
            //�������� �� ����������
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
        //�������� �� ����������
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
MARSH findMarsh(List *stList, string routeNumb) //����� �������� �� ��� ������ (����� �������� �� ����������)
{
    MARSH temporaryRoute;
    while (stList != NULL)
    {
        //������� ��������� �������, ����� ��������, ��� �� ��������
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
void printMarsh(MARSH temporaryRoute)//����� ����������� �������� �� �����
{
    cout << "������� �������� ��������� �������:" << endl;
    cout << temporaryRoute.startinPoint << "  " << temporaryRoute.destination << "  " << temporaryRoute.routeNumber << endl;
}
void redactEl(List *stList, string routeNumb) //������� ��������������
{
    bool flag1 = false; //������ � ���, ���� �� ������ � �������� ������
    MARSH temporaryRouteOrig;
    temporaryRouteOrig.routeNumber = routeNumb;
    int click2 = 0;
    while (click2 != 4)
    {
        MARSH temporaryRouteTemp;
        temporaryRouteOrig = findMarsh(stList, temporaryRouteOrig.routeNumber);
        printMarsh(temporaryRouteOrig);
        cout << "��������:" << endl;
        cout << "1. ������������� ��������� �����" << endl;
        cout << "2. ������������� �������� �����" << endl;
        cout << "3. ������������� �����" << endl;
        cout << "4. ��������� ��������������" << endl << endl;
        cout << "��� �����: ";
        click2 = clickIsOk ();
        while (click2 > 4 || click2 < 1)
        {
            cout << "�� ������� ����� ����� (��� ������ ���� �� 1 �� 4). ���������� �����: ";
            click2 = clickIsOk ();
        }

        if (click2 == 1)
        {
            cout << "������� ��������� ����� ��������, �� ������� ������ �������� �����������: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.startinPoint);
                if (!chekAndConversionStr(temporaryRouteTemp.startinPoint))
                    cout << "������ ������� �����������. ���������� �����: ";
                else
                    if (isEquals (temporaryRouteOrig.startinPoint, temporaryRouteTemp.startinPoint))
                        cout << "�� ����� �� ��, ��� � ����. �������������� �� �����������. ���������� �����: ";
                    else
                        if (isEquals (temporaryRouteOrig.destination, temporaryRouteTemp.startinPoint))
                            cout << "�������������� �� �����������, ���� ����� ��������� � �������� ������ ����� ���������. ���������� �����: ";
                        else
                        {
                            string number = temporaryRouteOrig.routeNumber; //���������� ������� ����� ��������
                            temporaryRouteOrig.startinPoint = temporaryRouteTemp.startinPoint;
                            temporaryRouteOrig.routeNumber = -1; //������ ��� ��� �������� �� ������������
                            if (findEl(temporaryRouteOrig, stList)) //���� ����� ��������� ��� ������� ������ ������������, �� �� ��������� ��������������
                            {
                                temporaryRouteOrig.routeNumber = number; //���������� ���������� �����
                                cout << "������ �������������� ����������, ������� ������ ������������. ���������� �����: ";
                            }
                            else
                            {
                                temporaryRouteOrig.routeNumber = number; //���������� ���������� �����
                                redact(stList, temporaryRouteOrig.routeNumber, temporaryRouteTemp.startinPoint, 1);
                                flag1 = true;
                            }
                        }
            }
        }
        if (click2 == 2)
        {
            cout << "������� �������� ����� ��������, �� ������� ������ �������� �����������: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.destination);
                if (!chekAndConversionStr(temporaryRouteTemp.destination))
                    cout << "������ ������� �����������. ���������� �����: ";
                else
                    if (isEquals (temporaryRouteOrig.destination, temporaryRouteTemp.destination))
                        cout << "�� ����� �� ��, ��� � ����. �������������� �� �����������. ���������� �����: ";
                    else
                        if (isEquals (temporaryRouteOrig.startinPoint, temporaryRouteTemp.destination))
                            cout << "�������������� �� �����������, ���� ����� ��������� � �������� ������ ����� ���������. ���������� �����: ";
                        else
                        {
                            string number = temporaryRouteOrig.routeNumber; //���������� ������� ����� ��������
                            temporaryRouteOrig.destination = temporaryRouteTemp.destination;
                            temporaryRouteOrig.routeNumber = -1; //������ ��� ��� �������� �� ������������
                            if (findEl(temporaryRouteOrig, stList)) //���� ����� ��������� ��� ������� ������ ������������, �� �� ��������� ��������������
                            {
                                temporaryRouteOrig.routeNumber = number; //���������� ���������� �����
                                cout << "������ �������������� ����������, ������� ������ ������������. ���������� �����: ";
                            }
                            else
                            {
                                temporaryRouteOrig.routeNumber = number; //���������� ���������� �����
                                redact(stList, temporaryRouteOrig.routeNumber, temporaryRouteTemp.destination, 2);
                                flag1 = true;
                            }
                        }
            }
        }
        if (click2 == 3)
        {
            cout << "������� ����� ��������, �� ������� ������ �������� �����������: ";
            while (!flag1)
            {
                getline(cin, temporaryRouteTemp.routeNumber);
                if (!chekAndConversionNumb(temporaryRouteTemp.routeNumber))
                    cout << "������ ������� �����������. ���������� �����: ";
                else
                    if (temporaryRouteOrig.routeNumber == temporaryRouteTemp.routeNumber)
                        cout << "�� ����� �� ��, ��� � ����. �������������� �� �����������. ���������� �����: ";
                    else
                    {
                        string number = temporaryRouteOrig.routeNumber; //���������� ������� ����� ��������
                        string stPoint = temporaryRouteOrig.startinPoint; //��������� ��� �����, ������ ��� �� ��� ����� ������
                        temporaryRouteOrig.startinPoint = "-1"; //��� �������� �� ������������
                        temporaryRouteOrig.routeNumber = temporaryRouteTemp.routeNumber;
                        if (findEl(temporaryRouteOrig, stList)) //���� ����� ��������� ��� ������� ������ ������������, �� �� ��������� ��������������
                        {
                            //���� �������������� ����������, �� ���������� ����� ��������
                            temporaryRouteOrig.routeNumber = number;
                            cout << "������ �������������� ����������, ������� ������ ������������. ���������� �����: ";
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
            cout << endl << "�������������� �����������." << endl;
            temporaryRouteOrig = findMarsh(stList, temporaryRouteOrig.routeNumber);
            printMarsh(temporaryRouteOrig);
        }
        flag1 = false;
        cout << endl;
        system("pause");
        system("cls");
    }
    cout << "�������������� ���������." << endl;
}

bool isMoreThan (string str1, string str2) //������ �� str1 ��� str2
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
    //���� �� ��������� ���, �� ���� �� ����� �������� ���������� ������ ������
    if (str1.length() > str2.length())
        return true;
    else
        return false;
}
bool isEquals (string str1, string str2) //������� ��������� ����� �� ���������, �������� � ��������
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
		cout << "���-�� ����� �� ���! ������� ����� ���������: ";
		cin >> result;
	}
	cin.ignore(INT_MAX, '\n');
	return (result);
}
