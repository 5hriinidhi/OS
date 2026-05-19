#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char M[100][4];     // Memory: 100 words, 4 characters each
char IR[4];         // Instruction Register
char R[4];          // General Register
bool C = false;     // Toggle / condition flag
int IC = 0;         // Instruction Counter
int SI = 0;         // Service Interrupt

string buffer;

ifstream fin("input.txt");
ofstream fout("output.txt");

void MOS();
void init();
void load();
void execute();
void read();
void write();
void terminate();

void init()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }

    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }

    C = false;
    IC = 0;
    SI = 0;
}

void load()
{
    int m = 0;

    while (getline(fin, buffer))
    {
        if (buffer.substr(0, 4) == "$AMJ")
        {
            init();
            m = 0;
        }

        else if (buffer.substr(0, 4) == "$DTA")
        {
            execute();
        }

        else if (buffer.substr(0, 4) == "$END")
        {
            fout << endl << endl;
        }

        else
        {
            for (int i = 0; i < buffer.length(); i++)
            {
                M[m][i % 4] = buffer[i];

                if (i % 4 == 3)
                    m++;
            }

            if (buffer.length() % 4 != 0)
                m++;
        }
    }
}

void execute()
{
    while (true)
    {
        for (int i = 0; i < 4; i++)
            IR[i] = M[IC][i];

        IC++;

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }

        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }

        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            int loc = (IR[2] - '0') * 10 + (IR[3] - '0');

            for (int i = 0; i < 4; i++)
                R[i] = M[loc][i];
        }

        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int loc = (IR[2] - '0') * 10 + (IR[3] - '0');

            for (int i = 0; i < 4; i++)
                M[loc][i] = R[i];
        }

        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int loc = (IR[2] - '0') * 10 + (IR[3] - '0');

            C = true;

            for (int i = 0; i < 4; i++)
            {
                if (R[i] != M[loc][i])
                {
                    C = false;
                    break;
                }
            }
        }

        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            int loc = (IR[2] - '0') * 10 + (IR[3] - '0');

            if (C == true)
                IC = loc;
        }

        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            break;
        }
    }
}

void MOS()
{
    if (SI == 1)
        read();

    else if (SI == 2)
        write();

    else if (SI == 3)
        terminate();

    SI = 0;
}

void read()
{
    getline(fin, buffer);

    int loc = (IR[2] - '0') * 10 + (IR[3] - '0');
    int k = 0;

    for (int i = loc; i < loc + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (k < buffer.length())
                M[i][j] = buffer[k++];
            else
                M[i][j] = ' ';
        }
    }
}

void write()
{
    int loc = (IR[2] - '0') * 10 + (IR[3] - '0');

    for (int i = loc; i < loc + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fout << M[i][j];
        }
    }

    fout << endl;
}

void terminate()
{
    fout << endl << endl;
}

int main()
{
    load();

    fin.close();
    fout.close();

    return 0;
}