#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 256

typedef struct node{
    int data;
    struct node* next;
} node;

node* create_node (data){
    node* temp = (node*)malloc(sizeof(node));
    temp -> data = data;
    temp -> next = NULL;
    return temp;
}

void push_back(node** head, int data){
    if (*head == NULL){
        *head = create_node(data);
        return;
    }
    push_back(&((*head)->next),data);
}

int main(){

}