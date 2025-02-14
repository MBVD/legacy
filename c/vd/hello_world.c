#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int data;
    struct node* next;
} node;

node* create(int data, node* next)
{
    node* temp = (node*)malloc(sizeof(node));
    temp -> data = data;
    temp -> next = next;
    
    return temp; 
}

node* push_front (int data, node** head) //  head -> x1 -> x2 вставить y1 // int a | &a | int* ptr = a; ptr == &a
{
    if (*head == NULL){
        *head = create(data, NULL);
        return *head;
    }
    node* new_head = create(data, (*head));
    // node* new = create(&((*head)->next), data); // y1.next -> x1
    // *head -> next = &(new); // head -> y1 -> x1 -> x2

    return new_head;
}

void print(node* head)
{
    printf("%d ", head -> data);
    if (head->data == NULL){
        return;
    }
    print(head -> next);
}

int main()
{
    node* head = NULL;
    printf("HELLO \n");
    head = push_front(2, &head);
    head = push_front(3, &head);
    print(head);
}