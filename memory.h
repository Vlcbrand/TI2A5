struct _USER_CONFIG {
    unsigned char len;          /* Length of this structure. */
    unsigned int count;         /* Number of reboots. */
    int timezone;
};

void set_timezone(int timezone);

int get_timezone();

void memory_init();

