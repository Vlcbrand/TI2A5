#include <string.h>

#include "menu.h"
#include "main.h"

#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define ERROR -1

MenuNode *currentMenuItem = NULL;
MenuNode *currChildItem = NULL;
MenuNode *head = NULL;

MenuNode *Level1Node(char s[17], MenuNode *par, MenuNode *chil, void * ex) {
	MenuNode *tmp = malloc(sizeof(MenuNode));
    tmp->name = s;
    tmp->parent = par;
	tmp->next = NULL;
	tmp->executing = ex;
	
	if(chil == NULL){
		tmp->child = NULL;
	} else{
		tmp->child = chil;
	}

	
	if (head == NULL) 
	{
		tmp->prev = NULL;
        head = tmp;
        return tmp;
    }
	else
	{
		MenuNode *items = head;
		while(items->next != NULL){
			items = items->next;
		}
		items->next = tmp;
		tmp->prev = items;
	}
    return tmp;	
}

MenuNode *ChildNode(char s[17], MenuNode *par, MenuNode *chil, void * ex) {
	MenuNode *tmp = malloc(sizeof(MenuNode));
	
    tmp->name = s;
    tmp->parent = par;
	tmp->child = chil;
	tmp->executing = ex;
	tmp->next = NULL;
	
	if(par->child == NULL){
		par->child = tmp;
	} else{
		currChildItem = par->child;
		while(currChildItem->next != NULL){
			currChildItem = currChildItem->next;
		}
		currChildItem->next = tmp;
		tmp->prev = currChildItem;
	}
    return tmp;	
}

char* getCurrentName()
{
	return currentMenuItem->name;
}


void init_menu()
{
	menuItemIndex = 1;

    MenuNode *taalNode = Level1Node("Taal", NULL, NULL, NULL); //de laatste is de functiepointer
    MenuNode *tijdNode = Level1Node("Tijd", NULL, NULL, &time_loop);
    MenuNode *alarmNode = Level1Node("Alarm", NULL, NULL, NULL);
    MenuNode *netwerkNode = Level1Node("Netwerk", NULL, NULL, NULL);
	
	
	MenuNode *alarm1Node = ChildNode("Alarm1", alarmNode, NULL, NULL);
	MenuNode *alarm2Node = ChildNode("Alarm2", alarmNode, NULL, NULL);
	MenuNode *alarm3Node = ChildNode("Alarm3", alarmNode, NULL, NULL);
	
	MenuNode *ntpNode = ChildNode("NTP", netwerkNode, NULL, NULL);
	MenuNode *dhcpNode = ChildNode("DHCP", netwerkNode, NULL, NULL);
}


int nextMenuItem(void)
{
	if(currentMenuItem == NULL){
		currentMenuItem = head;
		return SUCCESS;
	} else if (currentMenuItem->next != NULL) {
		currentMenuItem = currentMenuItem->next;
		return SUCCESS;
	} else {
		return ERROR;
	}
}

int prevMenuItem(void){
	if(currentMenuItem == NULL){
		currentMenuItem = head;
		return SUCCESS;
	} else if (currentMenuItem->prev != NULL) {
		currentMenuItem = currentMenuItem->prev;
		return SUCCESS;
	} else {
		return ERROR;
	}
}

int childMenuItem(void)
{
	if(currentMenuItem == NULL){
		currentMenuItem = head;
		return ERROR;
	}
	if (currentMenuItem->child != NULL) {
		currentMenuItem = currentMenuItem->child;
		return SUCCESS;
	} else {
		return ERROR;
	}
}

int parentMenuItem(void)
{
	if(currentMenuItem == NULL){
		currentMenuItem = head;
		return ERROR;
	}
	if(currentMenuItem->parent != NULL)
	{
		currentMenuItem = currentMenuItem->parent;
		return SUCCESS;
	}
	else{
		return ERROR;
	}
	
}

int menuAction()
{
	if(currentMenuItem->child != NULL)
	{
			childMenuItem();
			LcdStr(currentMenuItem->name);	
	}
	else
	{
		(*(currentMenuItem->executing))();
	}
		
	return 0;
}
