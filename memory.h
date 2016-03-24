struct _USER_CONFIG {
    unsigned char len;          /* Length of this structure. */
    unsigned int count;         /* Number of reboots. */
    unsigned int timezone_set;
    int timezone;
	int volume;
};

void set_timezone(int timezone);

void factory_reset();


void save_volume(int);
int get_volume(void);

void set_timezone_set(int val);

int get_timezone_set();

int get_timezone();

int get_bootcount();

void memory_init();
