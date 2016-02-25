#include <stdio.h>
#include <string.h>
#include "system.h"
#include "menustruct.h"
#include "menu.h"

void Insert(Menu_struct *x) {

    struct Menu_struct *temp = (struct Menu_struct*)malloc(sizeof(struct Menu_struct));
    temp->stuff = x;
    temp->submenu = NULL;
    if(head != NULL) { temp->submenu = head;}
    head = temp;
}

void Remove(Menu_struct *value)
{
    struct Menu_struct *temp,*prev,*current;
    current = head;

    while(current != NULL)
    {
        if(value == current->stuff) {
            if(current == head)
            {
                head = current->submenu;
                free(current);
            }
            else
            {
                prev->submenu = current->submenu;
                temp = current;
                current = current->submenu;
                free(temp);
            }
        }
        else
        {
            prev = current;
            current = current->submenu;
        }
    }
}

void Clear()
{
    struct Menu_struct *newHead, *delete;
    delete = head;

    while(delete)
    {
        newHead = delete->submenu;
        free(delete);
        delete = newHead;
    }
    head = NULL;
}