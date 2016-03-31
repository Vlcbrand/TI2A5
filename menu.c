#include <string.h>

#include "menu.h"
#include "main.h"
#include "display.h"

#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define ERROR -1

MenuNode *currentMenuItem = NULL;
MenuNode *head = NULL;


/* #########################  */
/* ####    Initialization   Functions    ### */
/* ######################### */

MenuNode *Level1Node(char s[17], void * ex) {
	MenuNode *tmp = malloc(sizeof(MenuNode));
    tmp->name = s;
    tmp->parent = NULL;
	tmp->child = NULL;
	tmp->next = NULL;
	tmp->executing = ex;
    return tmp;	
}

MenuNode *ChildNode(char s[17], MenuNode *par, MenuNode *chil, void * ex) {
	MenuNode *tmp = malloc(sizeof(MenuNode));	
    tmp->name = s;
    tmp->parent = par;
	tmp->child = chil;
	tmp->executing = ex;
	tmp->next = NULL;
    return tmp;	
}

void AddL1Node(MenuNode* node)
{
	if (head == NULL) 
	{
		node->prev = NULL;
        head = node;
    }
	else
	{
		MenuNode *items = head;
		while(items->next != NULL){
			items = items->next;
		}
		items->next = node;
		node->prev = items;
	}
}

void AddChildNode(MenuNode* node)
{
	MenuNode *currChildItem = NULL;
	if(node->parent->child == NULL)
	{
		node->parent->child = node;
	} else
	{
		currChildItem = node->parent->child;
		while(currChildItem->next != NULL)
		{
			currChildItem = currChildItem->next;
		}
		currChildItem->next = node;
		node->prev = currChildItem;
	}
}


/* #########################  */
/* #####  Navigation  Functions  ##### */
/* ######################### */

void init_menu(void)
{
//    MenuNode *taalNode = Level1Node("Taal",  NULL); //de laatste is de functiepointer
    MenuNode *tijdNode = Level1Node("Tijd", &time_loop);
    MenuNode *audioNode = Level1Node("Audio", NULL);
	MenuNode *tijdzoneNode = Level1Node("Tijdzone", &timezone_loop);
    MenuNode *alarmNode = Level1Node("Alarm", &alarm_loop);
	MenuNode *resetNode = Level1Node("Reset", &factory_reset_loop);
	MenuNode *weatherNode = Level1Node("Weer", &weather_loop);

	MenuNode *volumeNode = ChildNode("Volume", audioNode, NULL, &volume_loop);
	MenuNode *bassNode = ChildNode("Bass", audioNode, NULL, &bass_loop);
	MenuNode *trebleNode = ChildNode("Treble", audioNode, NULL, &treble_loop);
	
//	AddL1Node(taalNode);
	AddL1Node(tijdNode);
	AddL1Node(tijdzoneNode);
	AddL1Node(alarmNode);
	AddL1Node(audioNode);
	AddL1Node(resetNode);
	AddL1Node(weatherNode);

	AddChildNode(volumeNode);
	AddChildNode(bassNode);
	AddChildNode(trebleNode);
	
	currentMenuItem = head;
	printf("\nMenu Initialized");
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

int prevMenuItem(void)
{
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


/* #########################  */
/* #####   Menu  Functionalities  ##### */
/* ######################### */


int nodeCounter(void) 
{
	MenuNode *temp = NULL;
	int i = 0;
	if(currentMenuItem->parent  != NULL)
	{
		temp = currentMenuItem->parent->child;		
	}
	else
	{
		temp = head;
	}
	
	while (temp != NULL) 
	{
		i++;
        temp = temp->next;
    }
    return i;
}

int nodeIndexFinder(void)
{
	MenuNode *temp = NULL;
	int i = 1;
	if(currentMenuItem->parent  != NULL)
	{
		temp = currentMenuItem->parent->child;		
	}
	else
	{
		temp = head;
	}
    
	while (temp != currentMenuItem) 
	{
		i++;
        temp = temp->next;
    }   
    return i;
}

int showMenuItem(void)
{
		char index[4]; 
		char parentMenuName[11]; 
		char childMenuName[11]; 
		const int i = nodeIndexFinder();
		const int x = nodeCounter();
		sprintf(index, "%d/%d", i, x);
				
		if(currentMenuItem-> parent == NULL )
		{
			sprintf(parentMenuName, "Settings");
			sprintf(childMenuName, "%s", currentMenuItem->name);
		}
		else if(currentMenuItem->child == NULL)
		{
			sprintf(parentMenuName, "%s", currentMenuItem->parent->name);
			sprintf(childMenuName, "%s", currentMenuItem->name);
		}
		else
		{
			sprintf(parentMenuName, "%s", currentMenuItem->parent->name);
			sprintf(childMenuName, "%s", currentMenuItem->child->name);
		}
		
		LcdDDRamStartPos(0, 0);
		LcdStr(index);
		LcdDDRamStartPos(0, 5);
		LcdStr(parentMenuName);
		LcdDDRamStartPos(1, 5);
		LcdStr(childMenuName);		
		return 0;
}

int menuAction()
{
	if(currentMenuItem->child != NULL)
	{
			childMenuItem();
			showMenuItem();
	}
	else
	{
		(*(currentMenuItem->executing))();
	}
		
	return 0;
}
