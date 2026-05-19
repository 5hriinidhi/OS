#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

char M[300][4];      // Memory
char IR[4];          // Instruction Register
char R[4];           // General Register
int IC;              // Instruction Counter
bool C;              // Toggle

int SI, PI, TI;
int PTR;             // Page Table Register

bool usedFrame[30];

struct PCB
{
    int jobId;
    int TTL;
    int TLL;
    int TTC;
    int LLC;
};

PCB pcb;

fstream fin, fout;

int allocate()
{
    int frame;

    while (true)
    {
        frame = rand() % 30;

        if (usedFrame[frame] == false)
        {
            usedFrame[frame] = true;
            return frame;
        }
    }
}

void initialize()
{
    memset(M, ' ', sizeof(M));
    memset(IR, ' ', sizeof(IR));
    memset(R, ' ', sizeof(R));
    memset(usedFrame, false, sizeof(usedFrame));

    IC = 0;
    C = false;

    SI = PI = TI = 0;

    pcb.TTC = 0;
    pcb.LLC = 0;
}

int getNumber(char buffer[], int start, int len)
{
    int num = 0;

    for (int i = start; i < start + len; i++)
    {
        if (buffer[i] >= '0' && buffer[i] <= '9')
            num = num * 10 + (buffer[i] - '0');
    }

    return num;
}

bool isOperandValid()
{
    return (IR[2] >= '0' && IR[2] <= '9' &&
            IR[3] >= '0' && IR[3] <= '9');
}

int addressMap(int VA)
{
    if (VA < 0 || VA > 99)
    {
        PI = 2; // Operand error
        return -1;
    }

    int pte = PTR + VA / 10;

    if (M[pte][0] == '*')
    {
        PI = 3; // Page fault
        return -1;
    }

    int frame = (M[pte][2] - '0') * 10 + (M[pte][3] - '0');

    return frame * 10 + VA % 10;
}

void allocatePage(int VA)
{
    int page = VA / 10;
    int frame = allocate();

    M[PTR + page][0] = ' ';
    M[PTR + page][1] = ' ';
    M[PTR + page][2] = frame / 10 + '0';
    M[PTR + page][3] = frame % 10 + '0';
}

void terminate(int em)
{
    fout << "\n\n";

    fout << "Job ID: " << pcb.jobId << endl;

    switch (em)
    {
    case 0:
        fout << "No Error";
        break;

    case 1:
        fout << "Out of Data";
        break;

    case 2:
        fout << "Line Limit Exceeded";
        break;

    case 3:
        fout << "Time Limit Exceeded";
        break;

    case 4:
        fout << "Operation Code Error";
        break;

    case 5:
        fout << "Operand Error";
        break;

    case 6:
        fout << "Invalid Page Fault";
        break;
    }

    fout << endl;
    fout << "IC  = " << IC << endl;
    fout << "TTC = " << pcb.TTC << endl;
    fout << "LLC = " << pcb.LLC << endl;
}

void read(int RA)
{
    char buffer[40];

    if (!fin.getline(buffer, 40))
    {
        terminate(1);
        return;
    }

    if (strncmp(buffer, "$END", 4) == 0)
    {
        terminate(1);
        return;
    }

    int k = 0;
    int len = strlen(buffer);

    for (int i = RA; i < RA + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (k < len)
                M[i][j] = buffer[k++];
            else
                M[i][j] = ' ';
        }
    }
}

void write(int RA)
{
    pcb.LLC++;

    if (pcb.LLC > pcb.TLL)
    {
        terminate(2);
        return;
    }

    for (int i = RA; i < RA + 10; i++)
    {
        for (int j = 0; j < 4; j++)
            fout << M[i][j];
    }

    fout << endl;
}

void MOS()
{
    int operand;
    int RA;

    if (TI == 0)
    {
        if (SI == 1)
        {
            operand = (IR[2] - '0') * 10 + (IR[3] - '0');
            RA = addressMap(operand);

            if (PI == 3)
            {
                allocatePage(operand);
                PI = 0;
                RA = addressMap(operand);
            }

            read(RA);
        }

        else if (SI == 2)
        {
            operand = (IR[2] - '0') * 10 + (IR[3] - '0');
            RA = addressMap(operand);

            if (PI == 3)
            {
                terminate(6);
                return;
            }

            write(RA);
        }

        else if (SI == 3)
        {
            terminate(0);
        }

        else if (PI == 1)
        {
            terminate(4);
        }

        else if (PI == 2)
        {
            terminate(5);
        }

        else if (PI == 3)
        {
            terminate(6);
        }
    }

    else if (TI == 2)
    {
        if (SI == 2)
        {
            operand = (IR[2] - '0') * 10 + (IR[3] - '0');
            RA = addressMap(operand);

            if (PI == 0)
                write(RA);

            terminate(3);
        }

        else if (SI == 3)
        {
            terminate(0);
        }

        else if (PI == 1)
        {
            terminate(4);
            terminate(3);
        }

        else if (PI == 2)
        {
            terminate(5);
            terminate(3);
        }

        else if (PI == 3)
        {
            terminate(3);
        }

        else
        {
            terminate(3);
        }
    }

    SI = 0;
    PI = 0;
}

void executeUserProgram()
{
    while (true)
    {
        int RA = addressMap(IC);

        if (PI != 0)
        {
            MOS();
            break;
        }

        for (int i = 0; i < 4; i++)
            IR[i] = M[RA][i];

        IC++;

        if (IR[0] == 'H')
        {
            pcb.TTC++;

            if (pcb.TTC > pcb.TTL)
                TI = 2;

            SI = 3;
            MOS();
            break;
        }

        if (!isOperandValid())
        {
            PI = 2;
            MOS();
            break;
        }

        int operand = (IR[2] - '0') * 10 + (IR[3] - '0');

        if (IR[0] == 'L' && IR[1] == 'R')
        {
            RA = addressMap(operand);

            if (PI != 0)
            {
                MOS();
                break;
            }

            for (int i = 0; i < 4; i++)
                R[i] = M[RA][i];
        }

        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            RA = addressMap(operand);

            if (PI == 3)
            {
                allocatePage(operand);
                PI = 0;
                RA = addressMap(operand);
            }

            if (PI != 0)
            {
                MOS();
                break;
            }

            for (int i = 0; i < 4; i++)
                M[RA][i] = R[i];
        }

        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            RA = addressMap(operand);

            if (PI != 0)
            {
                MOS();
                break;
            }

            C = true;

            for (int i = 0; i < 4; i++)
            {
                if (R[i] != M[RA][i])
                {
                    C = false;
                    break;
                }
            }
        }

        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            if (C)
                IC = operand;
        }

        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }

        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }

        else
        {
            PI = 1;
            MOS();
            break;
        }

        pcb.TTC++;

        if (pcb.TTC > pcb.TTL)
        {
            TI = 2;
            MOS();
            break;
        }
    }
}

void startExecution()
{
    IC = 0;
    executeUserProgram();
}

void load()
{
    char buffer[40];
    int VA = 0;

    while (fin.getline(buffer, 40))
    {
        if (strncmp(buffer, "$AMJ", 4) == 0)
        {
            initialize();

            pcb.jobId = getNumber(buffer, 4, 4);
            pcb.TTL = getNumber(buffer, 8, 4);
            pcb.TLL = getNumber(buffer, 12, 4);

            int frame = allocate();
            PTR = frame * 10;

            for (int i = 0; i < 10; i++)
            {
                M[PTR + i][0] = '*';
                M[PTR + i][1] = '*';
                M[PTR + i][2] = '*';
                M[PTR + i][3] = '*';
            }

            VA = 0;
        }

        else if (strncmp(buffer, "$DTA", 4) == 0)
        {
            startExecution();
        }

        else if (strncmp(buffer, "$END", 4) == 0)
        {
            continue;
        }

        else
        {
            int len = strlen(buffer);
            int k = 0;

            while (k < len)
            {
                int page = VA / 10;
                int offset = VA % 10;

                if (M[PTR + page][0] == '*')
                {
                    int frame = allocate();

                    M[PTR + page][0] = ' ';
                    M[PTR + page][1] = ' ';
                    M[PTR + page][2] = frame / 10 + '0';
                    M[PTR + page][3] = frame % 10 + '0';
                }

                int frame = (M[PTR + page][2] - '0') * 10 +
                            (M[PTR + page][3] - '0');

                int RA = frame * 10 + offset;

                for (int j = 0; j < 4; j++)
                {
                    if (k < len)
                        M[RA][j] = buffer[k++];
                    else
                        M[RA][j] = ' ';
                }

                VA++;
            }
        }
    }
}

int main()
{
    fin.open("input.txt", ios::in);
    fout.open("output.txt", ios::out);

    load();

    fin.close();
    fout.close();

    return 0;
}