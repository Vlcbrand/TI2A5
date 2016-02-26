#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define NEWLINE "\n"
#define TRUE 1
#define FALSE 0



MENU_NODE *create_menu_node(char s[17], MENU_NODE *par, MENU_NODE *chil) {
    MENU_NODE *tmp = malloc(sizeof(MENU_NODE));
    tmp->name = s;
    tmp->parent = par;
    tmp->child = chil;
    return tmp;
}


void llist_show() {
    NODE *temp = head;
    printf("Printing all values in list");
    printf(NEWLINE);

    struct menu_node *children;

    while (temp != NULL) {
        printf("Value: %s", temp->menu_item->name);
        printf(NEWLINE);

        if (temp->menu_item->child != NULL) {
            children = temp->menu_item->child;
            while (children != NULL) {
                printf("Value: %s", children->name);
                printf(NEWLINE);
                children = children->child;
            }
        }

        temp = temp->next;
    }
}

void llist_add(MENU_NODE *node) {
    NODE *temp = head;
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->menu_item = node;
    if (head == NULL) {
        head = newNode;
        return;
    }
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
    newNode->prev = temp;

}

void llist_remove(char *str) {
    NODE *temp = head;
    while (temp != NULL) {
        //TODO
//        if (strcmp(temp->string, str) == 0) {
//            //found our string, remove it
//            if (temp->next != NULL) {
//                temp->next->prev = temp->prev;
//            }
//            if (temp->prev != NULL) {
//                temp->prev->next = temp->next;
//            }
//            free(temp);
//            temp = NULL;
//            return;
//        }
        temp = temp->next;
    }
}

void llist_clear() {
    NODE *temp = head;
    while (temp != NULL) {
        NODE *nextPtr = temp->next;
        temp->next = NULL;
        temp->prev = NULL;
        temp = nextPtr;
        free(nextPtr);
        //TODO: free children too
    }
    head = NULL;
}

int llist_nrItems() {
    NODE *temp = head;
    int i = 0;
    while (temp != NULL) {
        i++;
        temp = temp->next;
    }
    return i;
}

int llist_exists(char *str) {
    NODE *temp = head;
    //TODO
//    while(temp != NULL) {
//        if (strcmp(temp->string, str) == 0) {
//            return TRUE;
//        }
//        temp = temp->next;
//    }
    return FALSE;
}

void init_menu()
{
    head = NULL;

    MENU_NODE *taalNode = create_menu_node("Taal", NULL, NULL);
    MENU_NODE *tijdNode = create_menu_node("Tijd", NULL, NULL);
    MENU_NODE *alarmNode = create_menu_node("Alarm", NULL, NULL);
    MENU_NODE *ntpNode = create_menu_node("NTP", NULL, NULL);
    MENU_NODE *netwerkNode = create_menu_node("Netwerk", NULL, NULL);
    MENU_NODE *dhcpNode = create_menu_node("DHCP", NULL, NULL);

    alarmNode->child = alarm1Node;
    alarm1Node->child = alarm2Node;

    alarm1Node->parent = alarmNode;
    alarm2Node->parent = alarmNode;

    netwerkNode->child = ntpNode;
    ntpNode->child = dhcpNode;

    ntpNode->parent = ntpNode;
    dhcpNode->parent = netwerkNode;


    llist_add(taalNode);
    llist_add(tijdNode);
    llist_add(alarmNode);
    llist_add(netwerkNode);
    llist_show();
}
