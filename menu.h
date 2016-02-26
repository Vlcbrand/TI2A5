#ifndef _Menu_H
#define _Menu_H

#include <stdio.h>

typedef struct MenuNode {
	struct MenuNode *prev;
	struct MenuNode *next;
	struct MenuNode *parent;
	struct MenuNode *child;
	char *name;
	void (*excecuting)(void);
	// TODO Functiepointer toevoegen
} MenuNode;

void init_menu(void);
int nextMenuItem(void);
int prevMenuItem(void);
int childMenuItem(void);
char *getCurrentName(void);
int parentMenuItem(void);


MenuNode *Level1Node(char s[17], MenuNode *par, MenuNode *chil);
MenuNode *ChildNode(char s[17], MenuNode *par, MenuNode *chil);

short menuItemIndex;

#endif /* _Menu_H */
/*  ����  End Of File  �������� �������������������������������������������� */
