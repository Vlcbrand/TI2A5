struct _USER_CONFIG {
    unsigned char len;          /* Length of this structure. */
    unsigned int count;         /* Number of reboots. */
    unsigned int timezone_set;
    int timezone;
};

void set_timezone(int timezone);

void set_timezone_set(int val);

int get_timezone_set();

int get_timezone();

int get_bootcount();

void memory_init();

