#include <stdio.h>
#include <string.h>
#include "system.h"
#include "menustruct.h"
#include "menu.h"

typedef struct Node{
    Menu_struct *menuitem;
    struct Node *next;
    struct Node *prev;
}NODE;

struct Node *head;


void llist_show()
{
    NODE *temp = head;
    printf("Printing all values in list\n");
    while (temp != NULL) {
        printf("Value: %s\n", temp->menuitem->name);
        temp = temp->next;
    }
}

void list_add(Menu_struct *str){
	NODE *temp = head;
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->string= str;
    if (head == NULL) {
        head = newNode;
        return;
    }
    while(temp->next != NULL) temp = temp->next;
    temp->next=newNode;
    newNode->prev = temp;
}

Menu_struct* create_struct(char[17] name, Menu_struct *hoofdmenu, Menu_struct *submenu){	
	Menu_struct *tmp = malloc(sizeof(Menu_struct));
	tmp->name = name;
	tmp->hoofdmenuItem = hoofdmenu;
	tmp->submenu = submenu;
	return tmp;
}

void initMenu(){
//	menuItems[0] = Menu_struct{"Tijdsetings\0", NULL, NULL, "x"}";
//	menuItems[1] = Menu_struct{"Datumsettings\0", NULL, NULL, "y"};
//	menuItems[2] = Menu_struct{"Alarm\0", Insert(Menu_struct{"Add alarm\0", NULL, menuItems[2], "q"}), NULL, NULL};
	head = malloc(sizeof(Menu_struct)*50);
	
	list_add(create_struct("Time",NULL,NULL));
	list_add(create_struct("Date",NULL,NULL));
	list_add(create_struct("Alarm",NULL,NULL));
	
}

void main(){
	initMenu();
	llist_show();
}