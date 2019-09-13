/*!
* @mainpage
* This is program that sorts texts
*/

/*!
* \file main.cpp
*/

#include <cstdio>
#include <cstdlib>
#include "..\OneginGood\OneginLib.h"

#ifdef _DEBUG
    #define ASSERT( cond )                                                                                                 \
    {                                                                                                                      \
        if (!(cond))                                                                                                       \
        {                                                                                                                  \
            printf("Ass failed: %s, file: %s,\nline: %d, function %s:\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            abort();                                                                                                       \
        }                                                                                                                  \
    }

    #define DPRINT printf
#else
    #define ASSERT( cond ) ;
    #define DPRINT if(0) printf
#endif

bool PrintError(FILE* fr, FILE* fw);

int test(int i);

int main(int argc, char **argv)
{
    const int NumOfTests = 8;
    for (int i = 1; i < NumOfTests; ++i)
    {
        test(i);
    }
    return 0;
}

int test(int i)
{
    char NowTest[11] = "test .txt";
    NowTest[4] = (char)(i + 48);
    printf("%s\n",NowTest);
    Tmain( NowTest, ("FuncAns.txt"));

    FILE* FuncAns = fopen("FuncAns.txt", "rb");
    char NowAns[11] = "answ .txt";
    NowAns[4] = (char)(i + 48);
    FILE* NiceAns = fopen( NowAns, "rb");
    FILE* Errors = fopen( ("errors.txt"), "w+" );
    fseek(Errors, 0L, SEEK_END);

    int CharChange = 0;
    char tmp1 = {};
    char tmp2 = {};
    char Result[4] = {};
    Result[3] = '\0';

    bool IsAnsIsGood = 1;
    while ( (tmp1 = getc (FuncAns)) != EOF && (tmp2 = getc (NiceAns)) != EOF && IsAnsIsGood )
    {
        ++CharChange;

        if(tmp2 == '\r')
        {
            (tmp2 = getc (NiceAns));
        }
        if(tmp1 == '\r')
        {
            (tmp1 = getc (FuncAns));
        }
        if(tmp1 != tmp2)
        {
            IsAnsIsGood = 0;
        }
    }

    if(tmp1 != tmp2)
    {
        IsAnsIsGood = 0;
    }

    if (IsAnsIsGood)
    {
        Result[0] = 'O';
        Result[1] = 'K';
        Result[2] = ' ';
    } else
    {
        rewind(FuncAns);
        rewind(NiceAns);

        fprintf(Errors, "Error test%d:\nCharProblem = %d\ntext output:\n\n", i, CharChange);
        PrintError(FuncAns, Errors);

        fprintf(Errors, "\n\nTest Text:\n\n", i);
        FILE* TestS = fopen(NowTest, "rb");
        PrintError(TestS, Errors);

        fprintf(Errors, "\n\nTest Ans:\n\n", i);
        PrintError(NiceAns, Errors);
        Result[0] = 'B';
        Result[1] = 'A';
        Result[2] = 'D';
    }
    printf("Test number %d , result is %s\n\n", i, Result);
}

bool PrintError(FILE* fr, FILE* fw)
{
    ASSERT(fr != NULL);
    ASSERT(fw != NULL);

    char tmp = {};
    while ( (tmp = getc (fr)) != EOF)
    {
        fprintf(fw, "%c", tmp);
    }
    return 0;
}
