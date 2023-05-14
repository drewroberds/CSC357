#include <stdio.h>
#include <iostream>
#include <string.h>
using std::cout;
using std::cin;
using std::endl;
int size = 0;

typedef struct Node{
struct Node *next, *prev;
char text[1000];
}Node;

Node *head = NULL;

void insert(char *text){
    Node *prev = (Node*) malloc(sizeof(Node));
    strcpy(prev->text, text);

    if(head == NULL){  //if no list exists, create a new head
        head = prev;
        prev->prev = NULL;
        prev->next = NULL;
        size += 1;
        return;
       //newNode->
    }

    Node *cur = head;
    for( ; cur->next != 0; cur = cur->next){}

    cur->next = prev;
    prev->prev = cur;
    prev->next = NULL;
    size += 1;
};




int main(){
    
    char entry;

    while(entry != 52){
        cout << "\nMenu:\n1 push string\n2 print list\n3 delete item\n4 end program\n\nPlease enter a digit to select an action: ";
        cin >> entry;
        if(entry == 49){
            cin.ignore(999, '\n');
            char buffer[1000];
            cout << "\nPlease enter text to insert to end of list: ";
            if(cin >> buffer){
                insert(buffer);
            }
        }
        else if(entry == 50){
            Node *cur = head;
            printf("\n");
            while(cur != NULL){
                printf("%s\n", cur->text);
                cur = cur->next;
            }
            printf("\n");
        }
        else if(entry == 51){
            int selection;
            Node *cur;
            cout << "\nPlease enter item to delete from list: ";
            if(cin >> selection){
                if(selection > size){
                    printf("\nItem does not exist, please keep selection within %d.\nPlease try again:\n\n", size);
                }else{
                    cur = head;
                    if (selection == 1){
                        if(cur->next != NULL){
                        cur = cur->next;
                        head = cur;
                        cur->prev = NULL;
                        size -= 1;
                        selection -= 1;
                        }else{
                            cur = NULL;
                            head = NULL;
                            size -= 1;
                            selection -= 1;
                        }
                    }else{
                        if (selection > 1){
                            selection -= 1;
                        }
                        while(selection != 0){
                            if (cur->next != NULL){
                                cur = cur->next;
                                selection -= 1;
                            }else {
                                cur->next = NULL;
                                cur->prev = NULL;
                                cur = NULL;
                                head = NULL;
                            }
                        }
                        if(cur != NULL && cur->prev != NULL){
                        cur->prev->next = cur->next;
                        }
                        if(cur != NULL && cur->next != NULL){
                        cur->next->prev = cur->prev;
                        }
                        if(cur != NULL){cur->prev = NULL;
                        cur->next = NULL;
                        free(cur);
                        }
                        size--;
                    }
                }
            }
        }
        else if (entry != 52){
            cout << "\n\nERROR: Please select an entry from the list and try again.\n" << endl;
        }
    }

    cout << "\nEND PROGRAM\n\n\n\n";

    return 0;
}