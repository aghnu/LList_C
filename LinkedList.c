/*
written by Gengyuan Huang

Notes:
    - Linked List uses dynamic memory and user must call free_list at the end to free the list
    - Cannot accept mixed data type, maybe add this feature in the future?
*/

#include <stdlib.h>
#include "LinkedList.h"

static void list_append_helper(List alist, ListNode anode);
static void list_push_helper(List alist, ListNode anode);
static void list_insert_helper(List alist, ListNode anode, int index);


struct LLNode {
    union {
        int i;                  // int
        char c;                 // char
    } content;
    struct LLNode *Next;
    struct LLNode *Prev;
    List nodelist;
};


struct LinkedList {
    int type;
    int length;
    struct LLNode *First;
    struct LLNode *Last;
};


List list_create(int type) {

    // create LList
    List new_list;
    if ((new_list = malloc(sizeof(struct LinkedList))) == NULL) return NULL;

    // init
    new_list->First = NULL;
    new_list->Last = NULL;
    new_list->length = 0;
    new_list->type = type;

    return new_list;
}


static void list_append_helper(List alist, ListNode anode) {
    // append anode to list (rear)
    // if list is empty
    if (alist->length == 0) {
        alist->First = anode;
        alist->Last = anode;
        alist->length ++;
    }
    // if list is not empty
    else {
    anode->Prev = alist->Last; 
    alist->Last->Next = anode;
    alist->Last = anode;
    alist->length ++;
    }
}

static void list_push_helper(List alist, ListNode anode) {
    // push anode to list (front)
    // if list is empty
    if (alist->length == 0) {
        alist->First = anode;
        alist->Last = anode;
        alist->length ++;
    }
    // if list is not empty
    else {
    anode->Next = alist->First; 
    alist->First->Prev = anode;
    alist->First = anode;
    alist->length ++;
    }
}

static void list_insert_helper(List alist, ListNode anode, int index) {
    // assumptions:
    //      - list is not empty
    //      - 0 < index < length (inserted node cannot be the first node nor the last)
    // since this helper function is private to this file
    // these assumptions are written for functions in this file

    ListNode target_node = list_get_node(alist, index);
    ListNode target_node_prev = target_node->Prev;

    // insert
    target_node->Prev = anode;
    target_node_prev->Next = anode;
    anode->Prev = target_node_prev;
    anode->Next = target_node;
    alist->length ++;
}

ListNode list_get_node(List alist, int index) {
    // return the pointer to the node with given index
    // return NULL if out of index

    int length = alist->length;                             // get length
    if (index < 0) index = length + index;                  // convert negative index
    if (index >= length) return NULL;                       // out of index

    ListNode anode = alist->First;                          // first node, index 0
    for (int i = 0; i < index; i++) anode = anode->Next;    // from front to rear, find the requested node

    return anode;
}

bool list_add_char(List alist, int index, char achar) {
    // create node
    ListNode anode;
    if ((anode = (malloc(sizeof(struct LLNode)))) == NULL) return false;      // error

    // init
    anode->Next = NULL;
    anode->Prev = NULL;
    anode->nodelist = alist;
    anode->content.c = achar;

    // do action
    if (index == 0) 
        list_push_helper(alist, anode);
    else if ((index == -1) || (index >= alist->length)) 
        list_append_helper(alist, anode);
    else 
        list_insert_helper(alist, anode, index);
    
    return true;
}

bool list_add_int(List alist, int index, int aint) {
    // create node
    ListNode anode;
    if ((anode = (malloc(sizeof(struct LLNode)))) == NULL) return false;      // error

    // init
    anode->Next = NULL;
    anode->Prev = NULL;
    anode->nodelist = alist;
    anode->content.i = aint;

    // do action
    if (index == 0) 
        list_push_helper(alist, anode);
    else if ((index == -1) || (index >= alist->length)) 
        list_append_helper(alist, anode);
    else 
        list_insert_helper(alist, anode, index);

    return true;
}

void list_clear(List alist) {
    // clear all nodes in a list
    ListNode anode = alist->First;

    // free all node
    while (anode != NULL) {
        ListNode todelete = anode;
        anode = anode->Next;
        free(todelete);
    }

    // restore list info
    alist->First = NULL;
    alist->Last = NULL;
    alist->length = 0;
}

void list_free(List alist) {
    list_clear(alist);          // clear all nodes
    free(alist);                // free list itself
}

int list_get_type(List alist) {
    return alist->type;
}

int list_get_length(List alist) {
    return alist->length;
}

bool list_get_char(List alist, int index, char *p_char) {
    ListNode node;
    if ((node = list_get_node(alist, index)) == NULL) return false;
    *p_char = node->content.c;
    return true;
}

bool list_get_int(List alist, int index, int *p_int) {
    ListNode node;
    if ((node = list_get_node(alist, index)) == NULL) return false;
    *p_int = node->content.i;
    return true;
}

ListNode node_next(ListNode anode) {
    return anode->Next;
}

ListNode node_prev(ListNode anode) {
    return anode->Prev;
}

void node_get_char(ListNode anode, char *achar) {
    *achar = anode->content.c;
}

void node_get_int(ListNode anode, int *aint) {
    *aint = anode->content.i;
}

void node_remove(ListNode anode) {
    // check length, then remove acordingly
    // pass
}

/************************** Add On functions **************************/

char *list_to_string(List alist) {
    // special function to convert entire list to a char array with \0 terminator
    // Notes:
    //      - given alist must be consist of all chars
    //      - new char array is created by using dynamic memory, must be manually freed by user
    //      - return NULL if cannot create new array or list if of different type
    
    char *string;
    int length = alist->length;
    if (length == 0) return NULL;
    if (alist->type != LLIST_CHAR) return NULL;
    if ((string = malloc(sizeof(char)*(length+1))) == NULL) return NULL;

    // no error so far, copy all char to string
    ListNode node = alist->First;
    for (int i = 0; i < length; i++) {
        *(string+i) = node->content.c;
        node = node->Next;
    }

    // add terminator
    *(string+length) = '\0';

    return string;
}

List string_to_list(char *string, int size) {
    // create a List and copy chars from given string to list
    // Notes:
    //      - if size set to -1
    //          copy will stop before first null (undefined behavior if has no null terminator)
    //      - if size set to 0
    //          function will return 0
    //      - else
    //          copy will stop before first null or at the given size whichever come first
    // return:
    //      NULL - error
    //      List - a list of chars

    // handles size 0
    if ((size == 0)||(*string == '\0')) return NULL;

    // else
    List new_list = list_create(LLIST_CHAR);
    
    for (int i = 0; i != size; i++) {
        char temp = *(string+i);
        if (temp == '\0') break;
        list_add_char(new_list, -1, temp);
    }

    return new_list;
}

