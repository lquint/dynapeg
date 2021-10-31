#include <stdlib.h>

typedef struct InputBuffer InputBuffer;

struct InputBuffer
{
    int position;
    char *text;
};

InputBuffer *initInputBuffer(InputBuffer *ib,char *text)
{
    ib->position= 0;
    ib->text= text;
    return ib;
}

InputBuffer *mkInputBuffer(char *text)
{
    return initInputBuffer(calloc(1,sizeof(InputBuffer*)),text); //sizeof du type pointeur
}

void advance(InputBuffer *ib, int add)
{
    ib->position+= add;
}

int atEnd(InputBuffer *ib) {
    return ib->text[ib->position] == 0;
    }

void setPosition(InputBuffer *ib, int position)
{
    ib->position= position;
}

int getPosition(InputBuffer *ib) {
    return ib->position;
}

int currentChar(InputBuffer *ib)
{
    return ib->text[ib->position];
}

char *currentText(InputBuffer *ib)
{
    return ib->text + ib->position;
}

#ifdef DO_TEST
#include <stdio.h>
int main(int argc, char **argv){

    InputBuffer *ib = mkInputBuffer("hello world\n");
    while(!atEnd(ib)){
        putchar(currentChar(ib));
        advance(ib,1);
    }
    return 0;
}

#endif