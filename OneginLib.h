#ifndef _OneginLib_H_
#define _OneginLib_H_

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
    int FileSizeWin(FILE* f);//check
    bool FileRead(char* buff, FILE* fp);
    bool StrDivide(char* buff, char** text, char** rtext, int* CharNum, int* StrNum);
    bool ComparatorStr(const void* str1, const void* str2);
    bool PrintText(char** text, FILE* f, int NumStr);
    bool SpeedStrSort(char** text, int first, int second, bool (*cmp)(const void*, const void*));
    bool ReverseComparatorStr(const void* str1, const void* str2);
    bool ReverseStr(char* str, char** rtext, int StrNow);
    bool NormalizeReverseText(char** text, char* first, int StrNum);
    bool OriginalPrint(char* buff, FILE* f, int StrNum);
    bool ChangeEnterOnEndOfString(char* BuffTmp, int* StrNum, int CharSize); //check

    int Tmain(char FileReadName[50], char FileWriteName[50])
    {
        FILE* f = fopen(FileReadName, "rb");

        int CharSize = FileSizeWin(f);

        char* buffer = (char*)calloc ( (CharSize + 2), sizeof(char));
        ASSERT (buffer != NULL);

        *buffer = '\0';
        ++buffer;

        FileRead(buffer, f);

        int StrNum = 0;

        ChangeEnterOnEndOfString(buffer, &StrNum, CharSize);

        char* text[StrNum];
        char* rtext[StrNum];

        StrDivide(buffer, text, rtext, &CharSize, &StrNum);

        fclose(f);

        SpeedStrSort(text, 0, StrNum-1, ComparatorStr);
        SpeedStrSort(rtext, 0, StrNum-1, ReverseComparatorStr);

        f = fopen(FileWriteName, "w+");

        fprintf(f,"Dictionary of english life:\n");
        PrintText(text, f, StrNum);
        fprintf(f,"\n\nRap Dictionary:\n");
        NormalizeReverseText(rtext, buffer, StrNum);
        PrintText(rtext, f, StrNum);
        fprintf(f,"\n\nOriginal:\n");
        OriginalPrint(buffer, f, StrNum);

        fclose(f);
        free(buffer);

        return 0;
    }

    bool SpeedStrSort(char** text, int first, int second, bool (*cmp)(const void*, const void*))
    {
        if(text == NULL)
        {
            return 0;
        }
        if(cmp == NULL)
        {
            return 0;
        }

        int NowStr = second;
        char* Tmp;
        int i = first;

        while (i < NowStr)
        {
            if( cmp(text[NowStr], text[i]) )
            {
                Tmp = text[NowStr];
                text[NowStr] = text[(NowStr - 1)];
                text[(NowStr - 1)] = Tmp;
                NowStr = NowStr - 1;
                if(i != (NowStr))
                {
                    Tmp = text[i];
                    text[i] = text[(NowStr + 1)];
                    text[(NowStr + 1)] = Tmp;
                }
            } else
            {
                ++i;
            }
        }
        if (first < second)
        {
            SpeedStrSort(text, first, (NowStr - 1), cmp);
            SpeedStrSort(text, (NowStr + 1), second, cmp);
        }

        return 1;
    }

    bool FileRead(char* buff, FILE* fp)
    {
        if (buff == NULL || fp == NULL)
        {
            return 0;
        }

        char* LastChar = buff;
        char tmp = {};
        char prev = {};
        prev = '\n';

        while ( (tmp = getc (fp)) != EOF )
        {
            if(tmp != '\r')
            {
                if (!(tmp == prev && tmp == '\n'))
                {
                    *LastChar = tmp;
                    prev = tmp;
                    ++LastChar;
                }

            }
        }
        if(*(LastChar - 1) == '\0')
        {
            *(LastChar - 1) = '\n';
            *(LastChar) = 'm';
        }  else if(*(LastChar - 1) != '\n')
        {
            *(LastChar) = '\n';
        }

        return 1;
    }

    bool StrDivide(char* buff, char** text, char** rtext, int* CharNum, int* StrNum)
    {
        if(buff == NULL || text == NULL)
        {
            return 0;
        }

        int NowStr = 1;
        text[0] = buff;
        for (int i = 1;i < *CharNum; ++i)
        {
            if( *(buff + i) == '\0' && *StrNum > NowStr)
            {
                text[NowStr] = buff + i + 1;
                ReverseStr((char*)(buff + i - 1), rtext, (NowStr - 1) );
                ++NowStr;
            }
        }
        if (*(buff + *CharNum - 1) != '\0')
        {
            ReverseStr((char*)(buff + *CharNum - 1), rtext, (*StrNum - 1) );
        } else
        {
            ReverseStr((char*)(buff + *CharNum - 2), rtext, (*StrNum - 1) );
        }

        return 1;
    }

    bool ComparatorStr(const void* str1, const void* str2)
    {
        if((char*)str1 == NULL || (char*)str2 == NULL)
        {
            return 0;
        }

        int NowPos1 = 0;
        int NowPos2 = 0;

        while(*((char*)str1 + NowPos1 + 1) == ' ')
        {
            ++NowPos1;
        }
        while(*((char*)str2 + NowPos2 + 1) == ' ')
        {
            ++NowPos2;
        }

        while( ( *((char*)str1 + NowPos1) == *((char*)str2 + NowPos2) ) && (*((char*)str1 + NowPos1 + 1) != '\0' && *((char*)str2 + NowPos2 + 1) != '\0') )
        {
            while(*((char*)str1 + NowPos1 + 1) == ' ')
            {
                ++NowPos1;
            }
            while(*((char*)str2 + NowPos2 + 1) == ' ')
            {
                ++NowPos2;
            }
            ++NowPos1;
            ++NowPos2;
        }

        if ( *((char*)str1 + NowPos1) == *((char*)str2 + NowPos2) )
        {
            if (*((char*)str1 + NowPos1 + 1) == '\0' && *((char*)str2 + NowPos2 + 1) == '\0')
            {
                return NowPos1 < NowPos2;
            }
            if (*((char*)str1 + NowPos1 + 1) == '\0')
            {
                return 1;
            }
            if (*((char*)str2 + NowPos2 + 1) == '\0')
            {
                return 0;
            }
        }

        return *((char*)str1 + NowPos1) < *((char*)str2 + NowPos2);
    }

    bool PrintText(char** text, FILE* f, int NumStr)
    {
        if(text == NULL || f == NULL)
        {
            return 0;
        }
        int tmp = 0;
        int NowStr = 0;

        while (NowStr < NumStr)
        {
            tmp = 0;

            while (*(text[NowStr] + tmp) != '\0')
            {
                fprintf(f,"%c", *(text[NowStr] + tmp));
                ++tmp;
            }
            ++NowStr;
            fprintf(f,"\n");
        }

        return 1;
    }

    bool ReverseComparatorStr(const void* str1, const void* str2)
    {
        if((char*)str1 == NULL || (char*)str2 == NULL)
        {
            return 0;
        }

        int NowPos1 = 0;
        int NowPos2 = 0;

        if(( *((char*)str1 - NowPos1) == *((char*)str2 - NowPos2) ))
        {
            while(*((char*)str1 - (NowPos1 + 1) ) == ' ')
            {
                ++NowPos1;
            }
            while(*((char*)str2 - (NowPos2 + 1) ) == ' ')
            {
                ++NowPos2;
            }
        }

        while( ( *((char*)str1 - NowPos1) == *((char*)str2 - NowPos2) ) && (*((char*)str1 - (NowPos1 + 1) ) != '\0' && *((char*)str2 - (NowPos2 + 1) ) != '\0') )
        {
            while(*((char*)str1 - (NowPos1 + 1) ) == ' ')
            {
                ++NowPos1;
            }
            while(*((char*)str2 - (NowPos2 + 1) ) == ' ')
            {
                ++NowPos2;
            }
            ++NowPos1;
            ++NowPos2;
        }

        if (  *((char*)str1 - NowPos1) == *((char*)str2 - NowPos2) )
        {
            if ( (*((char*)str1 - (NowPos1 + 1) ) == '\0') && *((char*)str2 - (NowPos2 + 1) ) == '\0')
            {
                return NowPos1 < NowPos2;
            }
            if (*((char*)str1 - (NowPos1 + 1) ) == '\0')
            {
                return 1;
            }
            if (*((char*)str2 - (NowPos2 + 1) ) == '\0')
            {
                return 0;
            }
        }

        return *((char*)str1 - NowPos1) < *((char*)str2 - NowPos2);
    }

    bool ReverseStr(char* str, char** rtext, int StrNow)
    {
        if (str == NULL || rtext == NULL)
        {
            return 0;
        }

        rtext[StrNow] = str;

        return 1;
    }

    bool NormalizeReverseText(char** text, char* first, int StrNum)
    {
        if (text == NULL || first == NULL)
        {
            return 0;
        }
        for (int i = 0; i < StrNum; ++i)
        {
            while ( (text[i]) != first && *(text[i] - 1) != '\0')
            {
                --text[i];
            }
        }
        return 1;
    }

    bool OriginalPrint(char* buff, FILE* f, int StrNum)
    {
        if(f != NULL)
        {
            return 0;
        }
        if(buff != NULL)
        {
            return 0;
        }

        char* tmp = buff;
        int i = 0;
        while (i < StrNum)
        {
            fprintf(f, "%c", *tmp);
            ++tmp;

            if(*tmp == '\0' || *tmp == '\n')
            {
                ++i;
                fprintf(f, "\n");
                ++tmp;
            }
        }
        return 1;
    }

    int FileSizeWin(FILE* f)
    {
        if(!(f != NULL))
        {
            return -1;
        }

        char tmp, prev;
        int StrSize = 0;
        tmp = {};
        prev = {};

        tmp = (char)fgetc(f);
        while((tmp != EOF))
        {
            if(tmp != '\r')
            {
                if (!(tmp == prev && tmp == '\n'))
                {
                    ++StrSize;
                    prev = tmp;
                }
            }
            tmp = (char)fgetc(f);
        }
        rewind(f);

        return StrSize;
    }

    bool ChangeEnterOnEndOfString(char* BuffTmp, int* StrNum, int CharSize)
    {
        if(!(StrNum != NULL))
        {
            return 0;
        }
        if(!(BuffTmp != NULL))
        {
            return 0;
        }

        for (int i = 0; i < CharSize + 1; ++i)
        {
            if(*BuffTmp == '\n')
            {
                *BuffTmp = '\0';
                ++(*StrNum);
            }
            ++BuffTmp;
        }

        return 1;
    }
#endif;
