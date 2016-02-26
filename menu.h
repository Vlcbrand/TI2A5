#include <stdio.h>

// Hoofdmenu Item
typedef struct menu_node {
    char *name;
    struct menu_node *parent;
    struct menu_node *child;
} MENU_NODE;

// Submenu Item
typedef struct Node {
    MENU_NODE *menu_item;
    struct Node *next;
    struct Node *prev;
} NODE;

// Laatste menu item
NODE *head;

MENU_NODE *create_menu_node(char[], MENU_NODE *, MENU_NODE *) ;

void llist_show();
void llist_add(MENU_NODE *);
void llist_remove(char *);
void llist_clear();
void llist_nrItems();
void llist_exists(char *);
