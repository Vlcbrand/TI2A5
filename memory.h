struct _USER_CONFIG {
    unsigned char len;          /* Length of this structure. */
    unsigned int count;         /* Number of reboots. */
    int timezone;
	int volume;
};

void set_timezone(int timezone);

int get_timezone();

void set_volume(int);
int get_volume();

void memory_init();

