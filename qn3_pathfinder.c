#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int r, c;
    struct node* next;
} node;

typedef struct retval {
    int len;
    node *start;
} ret;

void clear(node *a) {
    node* temp;
    while(a != NULL) {
        temp = a;
        a = a->next;
        free(temp);
    }
}

int found(node* lst, int a, int b) {
    int val = 0;
    node *ptr = lst->next;
    while(ptr!=NULL) {
        if(ptr->r == a && ptr->c == b) {
            val = 1;
            break;
        }
        ptr = ptr->next;
    }
    return val;
}

//0. north      -1,+0
//1. south      +1,+0
//2. east       +0,+1
//3. west       +0,-1
//4. northeast  -1,+1
//5. northwest  -1,-1
//6. southeast  +1,+1
//7. southwest  +1,-1

//recursive function to search the path
ret* search(int m, int n, int arr[][n], int a, int b, node* head) {
    ret *retvals[8], *cur;
    node *ptr = head, *temp;

    //found the end
    if(a == m-1 && b == n-1) {
        printf("Found: %d, %d\n", a, b);
        cur = (ret*) malloc(sizeof(ret));
        temp = (node*) malloc(sizeof(node));
        temp->r = a;
        temp->c = b;
        temp->next = NULL;
        cur->len = 1;
        cur->start = temp;
        return cur;
    }

    //returned back to previous path
    else if(found(head, a, b)) {
        printf("Again: %d, %d\n", a, b);
        return NULL;
    }

    else {
        //initialising list to null
        for(int i = 0; i<8; i++) {
            retvals[i] = NULL;
        }
        printf("Searching: %d, %d   m: %d, n: %d\n", a, b, m, n);

        //add current position to head
        while(ptr->next != NULL) { 
            ptr = ptr->next;
        }
        ptr->next = (node*) malloc(sizeof(node));
        ptr = ptr->next;
        ptr->r = a;
        ptr->c = b;
        ptr->next = NULL;
        
        //search in each direction
        if(a+1!=m && b+1!=n && arr[a+1][b+1]==0)                //SE
            retvals[6] = search(m, n, arr, a+1, b+1, head);

        if(b+1!=n && arr[a][b+1]==0)                            //E
            retvals[2] = search(m, n, arr, a, b+1, head);
        
        if(a+1!=m && arr[a+1][b]==0)                            //S
            retvals[1] = search(m, n, arr, a+1, b, head);
        
        if(a!=0 && b+1!=n && arr[a-1][b+1]==0)                  //NE
            retvals[4] = search(m, n, arr, a-1, b+1, head);
        
        if(a+1!=m && b!=0 && arr[a+1][b-1]==0)                  //SW
            retvals[7] = search(m, n, arr, a+1, b-1, head);
        
        if(a!=0 && arr[a-1][b]==0)                              //N
            retvals[0] = search(m, n, arr, a-1, b, head);
        
        if(b!=0 && arr[a][b-1]==0)                              //W
            retvals[3] = search(m, n, arr, a, b-1, head);
        
        if(a!=0 && b!=0 && arr[a-1][b-1]==0)                    //NW
            retvals[5] = search(m, n, arr, a+1, b+1, head);
        //remove current position from head
        temp = ptr;
        ptr = head;
        while(ptr->next!=temp) {
            ptr = ptr->next;
        }
        ptr->next = NULL;
        free(temp);

        //find shortest from all returned paths
        cur = NULL;
        for(int i = 0; i<8; i++) {
            if(retvals[i]!=NULL && (cur == NULL || retvals[i]->len < cur->len)) {   //To find the shortest path
                cur = retvals[i];
            }
        }
        
        for(int i = 0; i<8; i++) {
            if(retvals[i]!=cur) {   //To free memory from unwanted paths
                ptr = retvals[i]->start;
                while(ptr!=NULL) {
                    temp = ptr;
                    ptr = ptr->next;
                    free(temp);
                }
                free(retvals[i]);
            }
        }
        
        //return length and list after inserting
        //current position in list
        if(cur != NULL) {
            cur->len++;
            temp = (node*) malloc(sizeof(node));
            temp->r = a;
            temp->c = b;
            temp->next = cur->start;
            cur->start = temp;
        }
        return cur;
    }
}

void display(node* path) {
    node* ptr = path;
    while(ptr!=NULL) {
        printf(" %d , %d \n", ptr->r, ptr->c);
    }
}

void main() {
    int arr[6][6] = {
        {0,0,1,0,1,0},
        {0,1,0,0,1,0},
        {1,1,0,1,0,0},
        {1,0,0,1,1,1},
        {0,0,1,0,1,0},
        {0,0,1,0,0,0}
    };
    node *head = (node*) malloc(sizeof(node));
    head->next = NULL;
    ret *path = search(6, 6, arr, 0, 0, head);
    printf("True");
    head->next = path->start;
    display(head->next);
}