// must be compiled with c11 for the use of _Generic keyword
// not thread safe

/*
Gengyuan Huang
gengyuan@ualberta.ca

a Linkedlist ADT using dynamic memory
add on functions allow more flexible usage of the list
    - dynamic length string / string modification
*/

/*
some new ideas:
    - support of multi dimension list
    - support of custom item (with pointer)
    - sort list
        * how to achieve this with low cost
    - list reversal
        * should be done in constance time 
        * can simply reverse the Next and Prev by define an indicator variable
    - string slicing
        * deep copy from the given indexs, return a new list
    - string concatenation
        * can be implemented by combining two list 
        * (connect two linked nodes and free one list)
        * then convert to string
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

// linked list data type
#define LLIST_MIX 0             // havent implemented 
#define LLIST_INT 1
#define LLIST_CHAR 2
#define LLIST_CUSTOM 3          // havent implemented

// add item to list[index]
#define list_add(list, index, item) _Generic    \
((item),                                        \
    int     : list_add_int,                     \
    char    : list_add_char                     \
)(list, index, item)

// get item from list[index]
#define list_get(list, index, item) _Generic    \
((item),                                        \
    int*    : list_get_int,                     \
    char*   : list_get_char                     \
)(list, index, item)

// get item from the node
#define node_get(node, item) _Generic           \
((item),                                        \
    int*    : node_get_int,                     \
    char*   : node_get_char                     \
)(list, index, item)
/*
Type: ListNode
    a pointer of struct LLNode
*/
typedef struct LLNode *ListNode;

/*
Type: List
    a pointer of struct LinkedList
*/
typedef struct LinkedList *List;


/*
Function: list_create
    given a list type, create an empty list

Parameters:
    type    - int   : item type of the list, value is defined in LinkedList.h

Return:
    NULL    - error
    List    - a pointer to List
*/
List list_create(int type);

/*
Function: list_get_type
    get list type info

Parameters:
    alist   - List  : a pointer to a list struct

Return:
    an int representing the list type
*/
int list_get_type(List alist);

/*
Function: list_get_length
    get list length

Parameters:
    alist   - List  : a pointer to a list struct

Return:
    an int of the list length
*/
int list_get_length(List alist);

/*
Function: list_get_char
    get the char of given index

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the item
    p_char  - char* : pointer to a char

Return:
    true    - bool  : succeed, value can be trusted
    false   - bool  : index out of range, failed
*/
bool list_get_char(List alist, int index, char *p_char);

/*
Function: list_get_int
    get the int of given index

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the item
    p_int   - int*  : pointer to an int

Return:
    true    - bool  : succeed, value can be trusted
    false   - bool  : index out of range, failed
*/
bool list_get_int(List alist, int index, int *p_int);

/*
Function: list_add_char
    add a char at given index of the list

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the new char
    achar   - char  : item to be added

Return:
    true    - bool : succeed
    false   - bool : failed
*/
bool list_add_char(List alist, int index, char achar);


/*
Function: list_get_node
    this function can be used to obtain the linked node of given index
    it can be view of an pointer and can be used in other functions to reduce traverse cost
    one usage of using the linked node is traversal

Parameters:
    alist   - List  : a pointer to alist struct
    index   - int   : the index

Return:
    return a ListNode if succeed, if out of index, the returns null
*/
ListNode list_get_node(List alist, int index);

/*
Function: list_add_int
    add an int at given index of the list

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the new char
    aint    - int   : item to be added

Return:
    true    - bool : succeed
    false   - bool : failed
*/
bool list_add_int(List alist, int index, int aint);

/*
Function: list_free
    clear the nodes and free the list

Parameters:
    alist   - List  : a pointer to a list struct to be freed
*/
void list_free(List alist);

/*
Function: list_clear
    empty the list, free all the nodes

Parameters:
    alist   - List  : a pointer to a list struct to be cleared
*/
void list_clear(List alist);

// get next node
ListNode node_next(ListNode anode);

// get prev node
ListNode node_prev(ListNode anode);

// get char from the node
void node_get_char(ListNode anode, char *achar);

// get int from the node
void node_get_int(ListNode anode, int *aint);

/*
Function: node_remove
    give a node that belongs to a list
    remove it from the list and free the memory

Parameter:
    anode   - ListNode : a pointer to list node
*/
void node_remove(ListNode anode);

/*
Function: list_remove_index
    give an index, and remove the node from the list

Parameter:
    alist   - List  : a pointer to list struct
    index   - Int   : the index

Return:
    boolean value, true if succeed. false if index out of range
*/
bool list_remove_index(List alist, int index);


/************************** Add On functions **************************/


/*
Function: list_to_string
    covert a char list to a \0 terminated string
    is using head memory

Parameter:
    alist   - List  : a pointer to a list struct

Return:
    an \0 terminated char array that serve as a string
    if error occurs, then return NULL
*/
char *list_to_string(List alist);

/*
Function: string_to_list
    convert a string to a list

Parameter:
    string  - char* : a \0 terminated char array
    size    - int   : num of char to be copied, if set to -1 then copy until first \0

Return:
    a pointer to the newly created list
    if error occurs, then return NULL
*/
List string_to_list(char *string, int size);

#endif