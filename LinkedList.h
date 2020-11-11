// not thread safe
// if a list of custom type is created, user is responsible for freeing custom item's memory before remove any node 

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

rev1:
    - remove overloading, replace with void pointer 
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

// linked list data type
#define LLIST_CUSTOM 0              // address to a custom object
#define LLIST_INT 1                 // int
#define LLIST_CHAR 2                // char
#define LLIST_DOUBLE 3              // double
#define LLIST_FLOAT 4               // float

/*
Type: llist_t
    list type
*/
typedef int llist_t;             // linked list type

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
    type    - llist_t   : item type of the list, value is defined in LinkedList.h

Return:
    NULL    - error
    List    - a pointer to List
*/
List list_create(llist_t type);

/*******************************************************
list functions
*******************************************************/

/*
Function: list_get_type
    get list type info

Parameters:
    alist   - List  : a pointer to a list struct

Return:
    an int representing the list type
*/
llist_t list_get_type(List alist);

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
Function: list_get
    get the item with the given index from the list
    *(type *)list_get(...) to get non-custom items
    (custom_type *)list_get(...) to get custom items

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the item

Return:
    NULL if out of index, else, return the item at the index
    if its not custom item, then an pointer to the value is returned
    if custom item, then the custom item's address is returned
*/
void *list_get(List alist, int index);

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
Function: list_add
    add an item at given index to the list

Parameters:
    alist   - List  : a pointer to a list struct
    index   - int   : index of the new char
    item    - void *  : address of the item

Return:
    true    - bool : succeed
    false   - bool : failed
*/
bool list_add(List alist, int index, void *item);

/*
Function: list_remove
    give an index, and remove the node from the list

Parameter:
    alist   - List  : a pointer to list struct
    index   - Int   : the index

Return:
    boolean value, true if succeed. false if index out of range
*/
bool list_remove(List alist, int index);

/*
Function: list_free
    clear the nodes and free the list
    if freefunction is not NULL then freefunction will be used to free custom content

Parameters:
    alist   - List  : a pointer to a list pointer to be freed
*/
void list_free(List *alist, void (*freefunction) (void*));

/*
Function: list_clear
    empty the list, free all the nodes, if freefunction is not NULL then
    free function will be used for freeing custom content

Parameters:
    alist   - List  : a pointer to a list struct to be cleared
*/
void list_clear(List alist, void (*freefunction) (void*));

/*******************************************************
node functions
*******************************************************/

// extract item from the given node
void *node_get(ListNode anode);

// get next node
ListNode node_next(ListNode anode);

// get prev node
ListNode node_prev(ListNode anode);

/*
Function: node_remove
    give a node that belongs to a list
    remove it from the list and free the memory
    NOTE:
        if using LLIST_CUSTOM type, free the item manually before calling this function

Parameter:
    anode   - ListNode : a pointer to list node
*/
void node_remove(ListNode anode);

/*******************************************************
Add On functions
*******************************************************/

/*
Function: list_to_string
    covert a char list to a \0 terminated string
    is using head memory

Parameter:
    alist   - List  : a pointer to a list struct
    astring - char* : address of the string (on heap memory)

Return:
    boolean value, false if failed, true if succeed.
*/
bool list_to_string(List alist, char **astring);

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

/*
Function: string_dup
    duplicate a string, and store the new string in heap

Parameter:
    string  - char* : target string
    dup     - char**: double pointer of the new string

Return:
    false if failed, true if succeed.

*/
bool string_dup(char *string, int size, char **dup);

/*
Function: custom_to_array
    this function gives all the custom pointer in a list of type LLIST_CUSTOM
    user can then traverse all the pointer and free them if needed

Parameters:
    alist   - List  : a pointer to alist struct
    items   - void **   : array of void * 

Return:
    boolean value, false if failed, true if succeed
*/
bool custom_to_array(List alist, void ***items);

#endif