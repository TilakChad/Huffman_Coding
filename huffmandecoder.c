#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
// hash table with linear chaining

#define CHAR_LENGTH 15
#define CHAR_TYPES 100

struct hash_entry
{
    char code[CHAR_LENGTH];
    char value;
};

int search_table(struct hash_entry *,const char*);

int main()
{
    // First need to create hash table for quick lookup
    FILE *fp;
    if (!(fp = fopen("dict.dat", "r")))
    {
        printf("Error in opening file... \n Recheck if code is avialable...");
    }
    else
    {
        printf("Passed");
    }

    struct hash_entry table[CHAR_TYPES];
    // Parse the input hardcoded
    char ch;
    int line = 0;
    bool new_char = true;
    char temp[CHAR_LENGTH] = {'\0'};
    int temp_counter = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (new_char)
        {
            new_char = false;
            table[line].value = ch;
          
            strcpy(temp, "\0");
            temp_counter = 0;
            ch = fgetc(fp);
            ch = fgetc(fp);

            if (ch != '-')
                printf("Hypen not found.");
            if (fgetc(fp)!='>')
                printf("Coding error....");
            fgetc(fp);
        }

        assert(line < CHAR_TYPES);
        assert(temp_counter < CHAR_LENGTH);

        if (ch == '\n')
        {
            new_char = true;
            temp[temp_counter] = '\0';
              strcpy(table[line].code, temp);
            table[line].code[temp_counter] = '\0';
            line++;
        }
        else
        {
            if (isdigit(ch))
            {
                temp[temp_counter++] = ch;
            }
        }
    };
    temp[temp_counter] = '\0';
    // strcpy(table[line].code,temp);
    printf("Value of line is : %d.", line);
    for (int i = 0; i <line; ++i)
    {
        printf("\n%c -> %s", table[i].value, table[i].code);
    }
    fclose(fp);

    // decompressing the stream

    if(!(fp=fopen("code.dat","r")))
    {
        printf("No code to decompress...");
    }

    FILE* outfile = fopen("output.dat","w");
    char buffer[CHAR_LENGTH]={'\0'};
    int buffer_len = 0;
    int prev_index = -1;
    printf("\nNow printing the message...\n\n");
    int index;
    while(1)
    {
        ch = fgetc(fp);
        if(ch==EOF)
        {
            index = search_table(table,buffer);
            // putchar(table[prev_index].value);
            fputc(table[prev_index].value,outfile);
            break;
        }
        buffer[buffer_len++] = ch;
        index = search_table(table,buffer);
        if(index == -1)
        {
            if(prev_index==-1)
            {
                // printf("No cure for this stupidity...");
                // printf("Not a single character is read...That's fine...");
            }
            else
            {
                // putchar(table[prev_index].value);
                fputc(table[prev_index].value,outfile);
                buffer_len = 0;
                for(int i = 0; i < CHAR_LENGTH; ++i) buffer[i] = '\0';
                prev_index = -1;
                fseek(fp,-1,SEEK_CUR);
            }
        }
        else
        {
            prev_index = index;
        }  
    }
    fclose(outfile);
    fclose(fp);
    return 0;
}

int search_table(struct hash_entry * table, const char* buffer)
{
    for(int i = 0; i < CHAR_TYPES; ++i)
    {
        if(strcmp(table[i].code,buffer)==0)
            return i;
    }
    return -1;
}