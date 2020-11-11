/*
written by Gengyuan Huang

Notes:
    - Linked List uses dynamic memory and user must call free_list at the end to free the list
    - Cannot accept mixed data type, maybe add this feature in the future?
    - when adding new supported type, modify the switch statement in list_add, and node_get, also expand the union
*/

#include <stdlib.h>
#include "LinkedList.h"

static void list_append_helper(List alist, ListNode anode);
static void list_push_helper(List alist, ListNode anode);
static void list_insert_helper(List alist, ListNode anode, int index);

/*******************************************************
basics
*******************************************************/

struct LLNode {
    union {
        int i;                  // int
        char c;                 // char
        double d;               // double
        float f;                // float
        void *custom;           // custom pointer
    } content;
    struct LLNode *Next;
    struct LLNode *Prev;
    List owner;
    
};

struct LinkedList {
    llist_t type;
    int length;
    struct LLNode *First;
    struct LLNode *Last;
};

List list_create(llist_t type) {

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

/*******************************************************
list functions
*******************************************************/

llist_t list_get_type(List alist) {
    return alist->type;
}

int list_get_length(List alist) {
    return alist->length;
}

void *list_get(List alist, int index) {
    // get the node
    ListNode anode;
    if ((anode = list_get_node(alist, index)) == NULL) return NULL;
    // extract
    return node_get(anode);
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

bool list_add(List alist, int index, void *item) {
    // create node
    ListNode anode;
    if ((anode = (malloc(sizeof(struct LLNode)))) == NULL) return false;      // error

    // init
    anode->Next = NULL;
    anode->Prev = NULL;
    anode->owner = alist;

    switch (alist->type) {
        case LLIST_CUSTOM:
            anode->content.custom = item;
            break;
        case LLIST_INT:
            anode->content.i = *(int *)item;
            break;
        case LLIST_CHAR:
            anode->content.c = *(char *)item;
            break;
        case LLIST_DOUBLE:
            anode->content.d = *(double *)item;
            break;
        case LLIST_FLOAT:
            anode->content.f = *(float *)item;
            break;
    }

    // do action
    if (index == 0) 
        list_push_helper(alist, anode);
    else if ((index == -1) || (index >= alist->length)) 
        list_append_helper(alist, anode);
    else 
        list_insert_helper(alist, anode, index);


    return true;
}

bool list_remove(List alist, int index) {
    ListNode anode; 
    if ((anode = list_get_node(alist, index)) == NULL) return false;
    node_remove(anode);

    return true;
}

void list_free(List *alist, void (*freefunction) (void*)) {
    list_clear(*alist, freefunction);           // clear all nodes
    free(*alist);                               // free list itself
    *alist = NULL;
}

void list_clear(List alist, void (*freefunction) (void*)) {
    
    
    ListNode anode = alist->First;
    if (freefunction == NULL) {
        // not customer type
        // free all node
        while (anode != NULL) {
            ListNode todelete = anode;
            anode = anode->Next;
            free(todelete);
        }
    } else {
        // customer type
        // free all node
        while (anode != NULL) {
            ListNode todelete = anode;
            anode = anode->Next;
            freefunction((todelete->content.custom));
            free(todelete);
        }
    }





    // restore list info
    alist->First = NULL;
    alist->Last = NULL;
    alist->length = 0;
}

/*******************************************************
node functions
*******************************************************/

void *node_get(ListNode anode) {
    // extract item from the node


    switch (anode->owner->type) {
        case LLIST_CUSTOM:
            return anode->content.custom;
        case LLIST_INT:
            return &(anode->content.i);
        case LLIST_CHAR:
            return &(anode->content.c);
        case LLIST_DOUBLE:
            return &(anode->content.d);
        case LLIST_FLOAT:
            return &(anode->content.f);
    }

    return NULL;
}

ListNode node_next(ListNode anode) {
    return anode->Next;
}

ListNode node_prev(ListNode anode) {
    return anode->Prev;
}

void node_remove(ListNode anode) {
    // check length, then remove acordingly
    // not thread safe
    // user is responsible for clear custom item's memory before calling this function
    List owner = anode->owner;
    if (owner->length == 1) {
        free(anode);
        owner->First = NULL;
        owner->Last = NULL;
        owner->length = 0;
    }

    else {
        if (anode->Next == NULL) {
            // rear end
            owner->Last = anode->Prev;
            owner->Last->Next = NULL;
            free(anode);
            owner->length --;
        }

        else if (anode->Prev == NULL) {
            // front end
            owner->First = anode->Next;
            owner->First->Prev = NULL;
            free(anode);
            owner->length --;
        }

        else {
            // middle
            anode->Next->Prev = anode->Prev;
            anode->Prev->Next = anode->Next;
            free(anode);
            owner-> length --;
        }
    }
}

/*******************************************************
static helper functions
*******************************************************/

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

/*******************************************************
Add On functions
*******************************************************/

bool list_to_string(List alist, char **astring) {
    // special function to convert entire list to a char array with \0 terminator
    // Notes:
    //      - given alist must be consist of all chars
    //      - new char array is created by using dynamic memory, must be manually freed by user
    //      - return NULL if cannot create new array or list if of different type
    

    char *string;
    int length = alist->length;
    if (length == 0) return false;
    if (alist->type != LLIST_CHAR) return false;
    if ((string = malloc(sizeof(char)*(length+1))) == NULL) return false;

    // no error so far, copy all char to string
    ListNode node = alist->First;
    for (int i = 0; i < length; i++) {
        *(string+i) = node->content.c;
        node = node->Next;
    }

    // add terminator
    *(string+length) = '\0';

    *astring = string;

    return true;
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
    List new_list;
    if ((new_list = list_create(LLIST_CHAR)) == NULL) return NULL;
    
    for (int i = 0; i != size; i++) {
        char temp = *(string+i);
        if (temp == '\0') break;
        list_add(new_list, -1, &temp);
    }

    return new_list;
}

bool string_dup(char *string, int size, char **dup) {
    // shortcut for now, will rework if have time
    List temp_list;
    if ((temp_list = string_to_list(string, size)) == NULL) return false;
    // need to free memory


    bool return_val = list_to_string(temp_list, dup);       // if false, no need to free memory
    list_free(&temp_list, NULL);
    return return_val;
}

bool custom_to_array(List alist, void ***items) {
    // get custom array 
    // can be used to manually clear all the custom pointers

    void **customs;
    int length = alist->length;
    if (length == 0) return false;
    if (alist->type != LLIST_CUSTOM) return false;
    if ((customs = malloc(sizeof(void *)*length)) == NULL) return false;

    // copy
    ListNode node = alist->First;
    for (int i = 0; i < length; i++) {
        *(customs+i) = node->content.custom;
        node = node->Next;
    }

    *items = customs;

    return true;
}