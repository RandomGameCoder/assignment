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

int count = 0;

int found(node* lst, int a, int b) {
    int val = 0;
    node *ptr = lst->next;
    ptr = lst->next;
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
ret* search(int m, int n, int **arr, int a, int b, node* head) {
    ret *cur;
    node *ptr = head, *temp;

    //found the end
    if(a == m-1 && b == n-1) {
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
        return NULL;
    }

    else {
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
        cur = NULL;
        if(a+1<m && b+1<n && arr[a+1][b+1]==0)                              //SE
            cur = search(m, n, arr, a+1, b+1, head);
        
        if(b+1<n && arr[a][b+1]==0 && cur==NULL)                            //E
            cur = search(m, n, arr, a, b+1, head);
        
        if(a+1<m && arr[a+1][b]==0 && cur==NULL)                            //S
            cur = search(m, n, arr, a+1, b, head);
        
        if(a>0 && b+1<n && arr[a-1][b+1]==0 && cur==NULL)                  //NE
            cur = search(m, n, arr, a-1, b+1, head);
        
        if(a+1<m && b>0 && arr[a+1][b-1]==0 && cur==NULL)                  //SW
            cur = search(m, n, arr, a+1, b-1, head);
        
        if(a>0 && arr[a-1][b]==0 && cur==NULL)                              //N
            cur = search(m, n, arr, a-1, b, head);
        
        if(b>0 && arr[a][b-1]==0 && cur==NULL)                              //W
            cur = search(m, n, arr, a, b-1, head);
        
        if(a>0 && b>0 && arr[a-1][b-1]==0 && cur==NULL)                    //NW
            cur = search(m, n, arr, a-1, b-1, head);
        
        //remove current position from head
        temp = ptr;
        ptr = head;
        while(ptr->next!=temp) {
            ptr = ptr->next;
        }
        ptr->next = NULL;
        free(temp);

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
        ptr = ptr->next;
    }
}

void main() {
    int **arr, m, n, t, *ptr;
    ret *path;
    node *head;
    printf("Enter the dimensions of the maze: ");
    scanf("%d%d", &m, &n);
    printf("0->open, 1->closed, maze should have a path\n");
    printf("Enter a valid maze:\n");
    arr = (int**) malloc(m*sizeof(int*) + m*n*sizeof(int));
    ptr = (int *)(arr + m);
    for(int i = 0; i<m; i++) {
        arr[i] = ptr +n*i;
    }
    for(int i = 0; i<m; i++) {
        for(int j = 0; j<n; j++) {
            scanf("%d", &arr[i][j]);
        }
    }
    head = (node*) malloc(sizeof(node));
    head->next = NULL;
    path = search(m, n, arr, 0, 0, head);
    if(path!=NULL) {
        display(path->start);
    }
    else {
        printf("No path found");
    }
}