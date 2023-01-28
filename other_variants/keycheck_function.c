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

int kbd_read(KSTR*);
void script_crawler(int*, int*);
int keypress_check(int key_code);

int main()
{
    while(1)
    {
        switch(keypress_check(42)) // Release(0),Press(1),Hold(2)
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

int kbd_read(KSTR* kbd_array)
{
    FILE* key_codes_fp = fopen("c_keycodes.csv","r");
    if(key_codes_fp == NULL)
    {

        return 1;
        fclose(key_codes_fp);
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

void script_crawler(int* lshift, int* rshift)
{
    const char *kbd_path = "/dev/input/event11";
    struct input_event ev;
    int fd;
    ssize_t n;
    KSTR kbd_array[85];
    int kbd_array_status;
    kbd_array_status = kbd_read(kbd_array);
    fd = open(kbd_path,O_RDONLY);
    if(fd == -1)
    {
        printf("Failed to open required files. Terminating.\n");
        exit(0);
    }
    pid_t proc_id = fork();

    if(proc_id == 0)
    {

    }
    else if (proc_id > 0)
    {

    }
    else
    {
        printf("Fork failed\n");
        exit(0);
    }
    return;
}
// void script_crawler(int* lshift, int* rshift)
// {
//     const char *kbd_path = "/dev/input/event11";
//     struct input_event ev;
//     int fd;
//     ssize_t n;
//     KSTR kbd_array[85];
//     int kbd_array_status;
//     kbd_array_status = kbd_read(kbd_array);
//     fd = open(kbd_path,O_RDONLY);
//     if(fd == -1)
//     {
//         printf("Failed to open required files. Terminating.\n");
//         exit(0);
//     }
//     while(1)
//     {
//         if(!(*lshift) || !(*rshift))
//         {
//             printf("stopped");
//             return;
//         }
//         n = read(fd, &ev, sizeof(ev));
//         if(n == (ssize_t)-1)
//         {
//             if(errno = EINTR)//Read into this
//                 continue;
//             else
//                 break;
//         }
//         if (n != sizeof(ev))
//         {
//             errno = EIO;//Read into this
//             continue;
//         }
//         if((ev.type == EV_KEY) && (ev.value <=2 && ev.value >=0))
//         {
//             if((ev.code == 42 && ev.value == 0))
//             {
//                 *lshift = 0;
//             }
//             if((ev.code == 54 && ev.value == 0))
//             {
//                 *rshift = 0;
//             }
//         }
//         printf("Inside script_crawler\n"); // replace this with some script ting
//     }

//     close(fd);
//     return;
// }
