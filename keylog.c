#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<linux/input.h>//structure definition of input_event is here
#include<unistd.h>//read()
#include<fcntl.h>//has open() and O_RDONLY macro
#include<errno.h>//for errno and EINTR. Look into these and what these mean
#include<time.h>//for struct tm and other tiem related functions. GET BETTER UNDERSTANDING

typedef struct keycodes_structure
{
    char key_name[12];
    int key_code;
}KSTR;

void date_and_time(char*);
int kbd_read(KSTR*);

int main()
{
    const char *kbd_path = "/dev/input/event2";
    struct input_event ev;
    char timenow[30];
    int fd;
    ssize_t n;
    KSTR kbd_array[85];
    int kbd_array_status;

    kbd_array_status = kbd_read(kbd_array);
    char *ev_event[3]={"\x1B[1;31mRELEASED\x1B[0m","\x1B[1;32mPRESSED \x1B[0m","\x1B[1;33mREPEATED\x1B[0m"};
    fd = open(kbd_path,O_RDONLY);
    if(fd == -1)
    {
        printf("Failed to open required files. Terminating.\n");
        exit(0);
    }
    while(1)
    {
        n = read(fd, &ev, sizeof(ev));
        if(n == (ssize_t)-1)
        {
            if(errno = EINTR)//Read into this
                continue;
            else
                break;
        }
        if (n != sizeof(ev))
        {
            errno = EIO;//Read into this
            continue;
        }
        if((ev.type == EV_KEY) && (ev.value <=2 && ev.value >=0))
        {
            date_and_time(timenow);
            if(!kbd_array_status)
            {
                printf("[%s] | %s - %s(%d)\n", timenow, ev_event[ev.value], kbd_array[ev.code].key_name, kbd_array[ev.code].key_code);
            }
            else
                printf("[%s] | %s - (%d)\n", timenow, ev_event[ev.value], ev.code);
        }
    }

    close(fd);
    return 1;
}

int kbd_read(KSTR* kbd_array)
{
    FILE* key_codes_fp = fopen("c_keycodes.csv","r");
    if(!key_codes_fp)
    {
        fclose(key_codes_fp);
        return 1;
    }
    char buffer[700];
    int row=0;
    while(fgets(buffer,700,key_codes_fp))
    {
        strcpy(kbd_array[row].key_name,strtok(buffer,","));
        kbd_array[row].key_code = atoi(strtok(NULL,","));
        row++;
    }
    fclose(key_codes_fp);
    return 0;
}

void date_and_time(char *timenow)
{
    struct tm *time_info;
    time_t t;
    time(&t);
    time_info = localtime(&t);
    strftime(timenow,30,"%x - %X",time_info);
}