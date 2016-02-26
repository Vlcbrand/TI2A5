#ifndef _Menu_H
#define _Menu_H

#include <stdio.h>

typedef struct MenuNode {
	struct MenuNode *prev;
	struct MenuNode *next;
	struct MenuNode *parent;
	struct MenuNode *child;
	char *name;
	// TODO Functiepointer toevoegen
} MenuNode;

void init_menu(void);
int nextMenuItem(void);
int childMenuItem(void);
char *getCurrentName(void);
int prevMenuItem(void);

short menuItemIndex;

#endif /* _Menu_H */
/*  ����  End Of File  �������� �������������������������������������������� */
