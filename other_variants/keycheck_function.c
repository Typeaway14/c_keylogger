/**
 * consider editing keypress_check to take in an array of integers depending
 * on if user wants to detect multiple keys states together.
 * For each element, create a fork and pipe system to fork out and check each 
 * keypress so that final function returns only once for multiple keys also
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h> // kill()
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

int keypress_check(int key_code);

int main()
{
    while(1)
    {
        switch(keypress_check(42)) // checks for leftshift
        {
            case 0://only released
                printf("Key Released\n");
            continue;
            case 1: // only pressed
                printf("Key Pressed\n");
            continue;
            case 2: // only held
                printf("Key Held\n");
            continue;
        }
    }
    return 1;
}


int keypress_check(int key_code)
/**
 * key_states
 * 0->released
 * 1->ony pressed
 * 2->only held
 * 3->pressed or held
 * 
 */
{
    const char *kbd_path = "/dev/input/event11";
    struct input_event ev;
    int fd;
    pid_t id = -1;
    ssize_t n;
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
        if((ev.type == EV_KEY) && ev.code == key_code) // 0->release ; 1->keypress ; 2->hold
        {
            close(fd);
            return ev.value;   
        }
    }
}