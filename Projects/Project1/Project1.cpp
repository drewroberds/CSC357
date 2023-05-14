#include <unistd.h> //sbrk(), brk()
#include <math.h> //ceil()
#include <stdio.h>
#include <iostream>
#include <limits.h> //UINT_MAX


/*
sbrk() -> increments the program's data space by increment bytes. Calling sbrk(0) can be used to find the 
    current location of the program break. On success, sbrk() returns the previous program break. (If the break 
    was increased, then this value is a pointer to the start of the newly allocated memory). On error, (void *) 
    -1 is returned, and errno is set to ENOMEM.
*/

using namespace std;

typedef unsigned char BYTE;
unsigned int PAGE = 4096;
void *PB;     //pagebreak, used to store start address
int listSize = 0;
unsigned int minDiff = UINT_MAX; //for bestfit
//BYTE *minAddress; //for bestfit
unsigned int success = 0;

struct chunkHead    //24 Byte struct (NODE)
{
    int size;
    int occupied;
    BYTE *next, *prev;
};

chunkHead *head = NULL;
chunkHead *tail = NULL;
chunkHead *minAddress; //for bestfit
chunkHead *temp; //for split
chunkHead *split; //guess what this is for

BYTE *mymalloc(int size)
{
    int rsize = PAGE*(ceil((size + sizeof(chunkHead))/(float)PAGE));
    // cout << "PB address = " << PB << endl;
    // cout << "rsize = " << rsize << endl;
    if(PB == NULL)
    {
        PB = sbrk(0);
    }
    if(head == NULL)
    {
        chunkHead *freshStart = (chunkHead*)(sbrk(rsize));
        // cout << "address pointer (pa*) = " << pa << endl;
        chunkHead *ci =freshStart;
        // cout << "chunkHead info (ci*) = " << ci << endl;
        ci->size = rsize;
        ci->occupied = 1;
        ci->next = 0;
        ci->prev = 0;
        tail = freshStart;
        head = freshStart;
        listSize++;
        // cout << "Adding to list, List Size is: " << listSize << endl;
        //BYTE *returnNumber = ((BYTE*)pa + 24);
        // cout << "Will return: " << pa + 24 << endl;
        return ((BYTE*)PB + sizeof(chunkHead)); ///////////////NOT SURE ABOUT THIS////////////////////
    }
    else
    {
        listSize++;
        // cout << "Adding to list, List Size is: " << listSize << endl;
        for(chunkHead *cur = (chunkHead*)PB; cur != 0 ; cur = (chunkHead*) cur->next)
        {
            // cout << "Current memory address: " << cur << endl;
            // cout << "Current memory size: " << cur->size << endl;
            // cout << "Current rsize: " << rsize << endl;

            if(cur->occupied == 0 && rsize <= cur->size)
            {
               // cout << "rsize: " << rsize << " is <= current size: " << cur->size << endl;
                if(cur->size - rsize < minDiff)
                {
                  //  cout << "minDiff is: " << minDiff << " and the current difference is: " << (cur->size - rsize) << endl;
                    minDiff = cur->size - rsize;
                    minAddress = cur;
                }
                success = 1;

            }
        }
        if(success == 1){
            minAddress->occupied = 1;
            success = 0;
            minDiff = UINT_MAX;

            if(minAddress->size - rsize >= PAGE)
            {
                temp = (chunkHead*)minAddress->next;
                split = (chunkHead*)((BYTE*)minAddress + rsize);
                minAddress->next = (BYTE*)split;
                split->next = (BYTE*)temp;
                split->prev = (BYTE*)minAddress;
                temp->prev = (BYTE*)split;

                split->size = minAddress->size - rsize;
                split->occupied = 0;

                minAddress->size = rsize;
            }

            return ((BYTE*)minAddress + sizeof(chunkHead)); ///////////////NOT SURE ABOUT THIS/////////////////
        }

        BYTE *returnAddress = (BYTE*)sbrk(rsize);
        chunkHead *ci =(chunkHead*)returnAddress;
        ci->size = rsize;
        ci->occupied = 1;
        ci->next = 0;
        ci->prev = (BYTE*)tail;


        tail->next = returnAddress;
        tail = ci;

        return returnAddress + sizeof(chunkHead);
    }
}

void myfree(BYTE* removal)
{
    chunkHead *dummyChunkHead = (chunkHead*) removal - 1;
    dummyChunkHead->occupied = 0;
    chunkHead *next = (chunkHead*)dummyChunkHead->next;
    chunkHead *prev = (chunkHead*)dummyChunkHead->prev;
    int nextFree = 0;
    int prevFree = 0;

    if(next != NULL)
    {
        if(next->occupied == 0)
        {
            nextFree = 1;
        }
    }

    if(prev != NULL)
    {
        if(prev->occupied == 0)
        {
            prevFree = 1;
        }
    }

    if(nextFree == 1 && prevFree == 1)
    {
        prev->next = next->next;
        ((chunkHead *)next->next)->prev = (BYTE*)prev;
        prev->size = prev->size + dummyChunkHead->size + next->size;
    }

    if(nextFree == 1 && prevFree != 1){
        dummyChunkHead->next = next->next;
        ((chunkHead *)next->next)->prev = (BYTE*)dummyChunkHead;
        dummyChunkHead->size = dummyChunkHead->size + next->size;
    }

    if(nextFree != 1 && prevFree == 1 && next){
        next->prev = (BYTE*)prev;
        prev->next = (BYTE*)next;
        prev->size = prev->size + dummyChunkHead->size;
    }

    if(next == NULL)
    {
        if((chunkHead*)prev->prev == NULL && prevFree == 1)
        {
            sbrk(-(dummyChunkHead->size + prev->size));
            PB = NULL;
            head = NULL;
            tail = NULL;
            return;
        }
        sbrk(-(dummyChunkHead->size));
        tail = prev;
        tail -> next = NULL;
    }
}

// void *myfree(BYTE* removal)
//     {
//         listSize--;
//         cout << "Subtracting to list, List Size is: " << listSize << endl;
//         chunkHead *dummyChunkHead = (chunkHead*) removal - 1;
//         dummyChunkHead->occupied = 0;
//         chunkHead *top = dummyChunkHead;
//         chunkHead *bottom = dummyChunkHead;
//         int changedLocation = 0;
//         if((chunkHead*)bottom->next != 0 && ((chunkHead*)bottom->next)->occupied == 0)
//         {
//                 bottom = (chunkHead*)bottom->next;
//                 changedLocation = 1;
//         }
//         if((chunkHead*)top->prev != 0 && ((chunkHead*)top->prev)->occupied == 0)
//         {
//                 top = (chunkHead*)top->prev;
//                 changedLocation = 1;
//         }
//         if(changedLocation == 1)
//         {
//             top->size = (((BYTE*)bottom + bottom->size) - (BYTE*)top);
//             top->next = ((BYTE*)bottom + bottom->size);
//             bottom = (chunkHead*)top->next;
//             if(dummyChunkHead->next != NULL)
//             {
//                 bottom->prev = (BYTE*)top;
//             }
//         }
//         if(dummyChunkHead->next == NULL && top->prev != NULL)
//         {
//             ((chunkHead*)top->prev)->next = NULL;
//             sbrk(-(top->size));
//             //PB = NULL;
//         }
//         else
//         {
//             if(bottom->next == NULL && bottom->prev != NULL && bottom->occupied == 0)
//             {
//                 chunkHead *newEnd = (chunkHead*)bottom->prev;
//                 newEnd->next = NULL;
//                 bottom = newEnd;
//                 sbrk(-(top->size));
//                 top = newEnd;
//             }
//             if(bottom->next == NULL && bottom->prev == NULL && bottom->occupied == 0)
//             {
//                 sbrk(-(top->size));
//                 PB = NULL;
//                 head = NULL;
//                 tail = NULL;
//             }
//         }
        

        // if(((chunkHead*)dummyChunkHead->next)->occupied == 0)
        // {
            
        // }
        //if(dummychunkHead)
        //else if()
        //else if()


    //     return 0;
    // }

void analyze()
{
    printf("\n--------------------------------------------------------------\n");
    if(!PB)
        {
            printf("no heap, program break on address: %p\n", sbrk(0));
            return;
        }
        chunkHead* ch = (chunkHead*)PB;
        for (int no=0; ch; ch = (chunkHead*)ch->next,no++)
        {
            printf("%d | current addr: %p |", no, ch);
            printf("size: %d | ", ch->size);
            printf("info: %d | ", ch->occupied);
            printf("next: %p | ", ch->next);
            printf("prev: %p", ch->prev);
            printf(" \n");
        }
        printf("program break on address: %p\n",sbrk(0));
}

int main(int argc, char const *argv[])
{
    // analyze();
    // BYTE *a[100];
    // clock_t ca, cb;
    // for(int i=0;i<100;i++)
    // {
    //     a[i]= mymalloc(1000);
    // }
    // for(int i=0;i<90;i++)
    // {
    //     myfree(a[i]);
    // }
    //     myfree(a[95]);
    //     a[95] = mymalloc(1000);
    // for(int i=90;i<100;i++)
    // {
    //     myfree(a[i]);
    // }
    //     cb = clock();
    // printf("\nduration: % f\n", (double)(cb - ca));
    // BYTE *a[100];
    // analyze();//50% points
    // for(int i=0;i<100;i++)
    //     {
    //         a[i]= mymalloc(1000);
    //     }
    // for(int i=0;i<90;i++)
    // {
    //     myfree(a[i]);
    //     analyze(); //50% of points if this is correct
    //     myfree(a[95]);
    //     a[95] = mymalloc(1000);
    //     analyze();//25% points, this new chunkHead should fill the smaller free one
    //     //(best fit)
    // }
    // for(int i=90;i<100;i++)
    // {
    //     myfree(a[i]);
    //     analyze();// 25% should be an empty heap now with the start address
    //     //from the program start
    // }
    //PB = sbrk(0);
    // BYTE *a = mymalloc(4000);
    // BYTE *b = mymalloc(1000);
    // analyze();
    // myfree(a);
    // analyze();
    // a = mymalloc(4000);
    // analyze();
    // myfree(b);
    // analyze();
    // myfree(a);
    // analyze();

    // BYTE *a = mymalloc(4000);
    // BYTE *b = mymalloc(4000);
    // BYTE *c = mymalloc(4000);
    // BYTE *d = mymalloc(4000);
    // BYTE *e = mymalloc(4000);
    // BYTE *f = mymalloc(4000);
    // BYTE *g = mymalloc(4000);
    // BYTE *h = mymalloc(4000);
    // analyze();
    // myfree(a);
    // analyze();
    // myfree(b);
    // analyze();
    // myfree(c);
    // analyze();
    // myfree(d);
    // analyze();
    // myfree(e);
    // analyze();
    // myfree(f);
    // analyze();
    // myfree(b);
    // analyze();
    // myfree(a);
    // analyze();

















    BYTE* a[100];
    analyze();//50% points
    for (int i = 0; i < 100; i++)
    a[i] = mymalloc(1000);
    for (int i = 0; i < 90; i++)
    myfree(a[i]);
    analyze(); //50% of points if this is correct
    myfree(a[95]);
    a[95] = mymalloc(1000);
    analyze();//25% points, this new chunk should fill the smaller free one
    //(best fit)
    for (int i = 90; i < 100; i++)
    myfree(a[i]);
    analyze();// 25% should be an empty heap now with the start address
    //from the program start
    //if best fit doestn work, -25%
    //init_progbrk = (BYTE)sbrk(0);
    int page = 4096;
    a[0] = mymalloc(page * 4);
    a[1] = mymalloc(100);
    a[2] = mymalloc(page * 3);
    a[3] = mymalloc(100);
    a[4] = mymalloc(page * 2 - 10);
    a[5] = mymalloc(100);
    a[6] = mymalloc(100);
    a[7] = mymalloc(100);
    analyze();
    myfree(a[0]);
    myfree(a[2]);
    myfree(a[4]);
    myfree(a[6]);
    analyze();
    a[0] = mymalloc(100);
    analyze();

    //if splitting does not work, -15%
    a[2] = mymalloc(100);
    analyze();
    
    return 0;
}
