#ifndef _Menu_H
#define _Menu_H

#include <stdio.h>

typedef struct MenuNode {
	struct MenuNode *prev;
	struct MenuNode *next;
	struct MenuNode *parent;
	struct MenuNode *child;
	char *name;
	void (*executing)(void);
} MenuNode;

/* #########################  */
/* #####    Structure   Functions  ##### */
/* ######################### */

//  Allocate new Nodes
MenuNode *Level1Node(char s[17],void * ex);
MenuNode *ChildNode(char s[17], MenuNode *par, MenuNode *chil, void * ex);

// Add Nodes after last known Node
void AddL1Node(MenuNode*);
void AddChildNode(MenuNode*);


/* #########################  */
/* #####  Navigation  Functions  ##### */
/* ######################### */

void init_menu(void);
int nextMenuItem(void);
int prevMenuItem(void);
int childMenuItem(void);
int parentMenuItem(void);


/* #########################  */
/* #####   Menu  Functionalities  ##### */
/* ######################### */

int menuAction(void);
int showMenuItem(void);

#endif /* _Menu_H */
/*  ����  End Of File  �������� �������������������������������������������� */
