/*!
* @mainpage
* This is program that sorts texts
*/

/*!
* \file main.cpp
*/

#include <cstdio>
#include <cstdlib>
#include "OneginLib.h"

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

int test(int i, FILE* Errors);

int main(int argc, char **argv)
{
    FILE* ErrorFile = fopen( ("errors.txt"), "w" );
    const int NumOfTests = 9;
    for (int i = 1; i < NumOfTests; ++i)
    {
        test(i, ErrorFile);
    }
    return 0;
}

int test(int i, FILE* Errors)
{
    char NowTest[11] = "test .txt";
    NowTest[4] = (char)(i + 48);
    printf("%s\n",NowTest);
    Tmain( NowTest, ("FuncAns.txt"));

    FILE* FuncAns = fopen("FuncAns.txt", "rb");
    char NowAns[11] = "answ .txt";
    NowAns[4] = (char)(i + 48);
    FILE* NiceAns = fopen( NowAns, "rb");
    fseek(Errors, 0L, SEEK_END);

    int CharChange = 0;
    int StrProblem = 0;
    int CharOnProblemStr = 0;
    char tmp1 = {};
    char tmp2 = {};
    char Result[4] = {};
    Result[3] = '\0';

    bool IsAnsIsGood = 1;
    while ( (tmp1 = getc (FuncAns)) != EOF && (tmp2 = getc (NiceAns)) != EOF && IsAnsIsGood )
    {
        ++CharChange;
        ++CharOnProblemStr;
        while(tmp2 == '\r')
        {
            (tmp2 = getc (NiceAns));
        }
        while(tmp1 == '\r')
        {
            (tmp1 = getc (FuncAns));
        }
        if(tmp1 == '\n')
        {
            CharOnProblemStr = 0;
            ++StrProblem;
        }
        if(tmp1 != tmp2)
        {
            IsAnsIsGood = 0;
        }
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

        fprintf(Errors, "Error test%d:\nCharProblem = %d\nStrProblem = %d\nCharOnProblemStr = %d\ntext output:\n\n", i, CharChange, StrProblem, CharOnProblemStr);
        PrintError(FuncAns, Errors);

        fprintf(Errors, "\n\nTest Text:\n\n", i);
        FILE* TestS = fopen(NowTest, "rb");
        PrintError(TestS, Errors);

        fprintf(Errors, "\n\nTest Ans:\n\n", i);
        PrintError(NiceAns, Errors);
        fprintf(Errors, "\n\n");
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
