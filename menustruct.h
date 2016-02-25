typedef struct Menu_struct{
	char name[17];
	Menu_struct *submenu; //is handig voor bijvoorbeeld alarmen
	Menu_struct *hoofdmenuItem;
	void *stuff;
} Menu_struct;


//struct Menu_struct;
Menu_struct head;

void Insert(Menu_struct *x);
void Remove(Menu_struct *value);
void Clear();
