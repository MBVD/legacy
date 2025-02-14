#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
    int data;
    struct Node* next;
} Node;

typedef struct list{
    Node* head;
    Node* tail;
} list;

Node* create_node(int data, Node* next){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = next;
    return temp;
}

void push_front(int data, list** list){
    if ((*list)->head == NULL){
        (*list)->head = create_node(data, NULL);
        (*list)->tail = (*list)->head;
        return;
    }
    Node* temp = create_node(data, NULL);
    (*list)->head->next = temp;
    (*list)->head = temp; 
}

void push_back(int data, list** list){
    if ((*list)->tail == NULL){
        (*list)->tail = create_node(data, NULL);
        (*list)->head = (*list)->tail;
        return;
    }
    Node* temp = create_node(data, (*list)->tail);
    (*list) -> tail = temp;
}

list* merge(list* l1, list* l2){
    Node* tmp1 = l1->tail;
    Node* tmp2 = (l2)->tail;
    list* ans = (list*)malloc(sizeof(list));
    ans->head = NULL;
    ans->tail = NULL;
    while(tmp1 != NULL && tmp2!= NULL){
        if (tmp1->data > tmp2->data){
            push_front(tmp2->data, &ans);
            tmp2 = tmp2->next;
        }else{
            push_front(tmp1->data, &ans);
            tmp1 = tmp1->next;
        }
    }
    while(tmp1 != NULL){
        push_front(tmp1->data, &ans);
        tmp1 = tmp1->next;
    }
    while(tmp2 != NULL){
        push_front(tmp2->data, &ans);
        tmp2 = tmp2->next;
    }
    return ans;
}


void print(list* list){
    Node* tmp = (list)->tail;
    while(tmp != NULL){
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
}
int main(){
    Node* head = NULL; Node* tail = NULL;
    list* ls = (list*)malloc(sizeof(list));
    ls->head = head;
    ls->tail = tail;
    list* ls2 = (list*)malloc(sizeof(list));
    ls2->head = head;
    ls2->tail = tail;
    push_front(1, &ls);
    push_front(2, &ls);
    push_front(4, &ls);
    push_front(7, &ls);
    push_front(3, &ls2);
    push_front(5, &ls2);
    push_front(6, &ls2);
    print(ls);
    printf("\n");
    print(ls2);
    printf("\n");
    list* ans = merge(ls, ls2);
    print(ans);
}