#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <ncurses.h>
#include <pthread.h>
int sockfd;
int crash_stat=0;
int work_flag=1, start_flag=0, key_pressed_1=0, key_pressed_2=0;
void handler(int none)
{
  work_flag = 0;
}

struct car{
        int x;
        int y;
        uint32_t color;
    int turn_r;
    int turn_l;
    int turn_d;
    int turn_up;
    int go_up;
    int go_d;
    int go_l;
    int go_r;
    int crash_stat;


};

int car_wid=5, car_h = 8;


void init_walls_n_cars(uint32_t *ptr, struct car *car1, struct car *car2, int xres_virtual, int x_window, int y_window, int xres, int yres){
        if(start_flag==0){
                while (key_pressed_1 != 1 || key_pressed_2 != 1){
                        if(x_window < xres){
                                for(int i = 0; i < y_window; i++){
                                        ptr[i * xres_virtual + x_window] = 0x00FFFFFF;
                                }
                        }

                        if(y_window < yres){
                                for(int i = 0; i < x_window; i++){
                                        ptr[y_window * xres_virtual + i] = 0x00FFFFFF;
                                }
                        }
                        if (car1->go_r == 1){
                                for(int i=car1->x-car_h; i<=car1->x;i++){
                                        for(int j=car1->y-car_wid/2;j<=car1->y+car_wid/2;j++){
                                                ptr[j * xres_virtual + i] = car1->color;
                                        }
                                }
                        }

                        if (car2->go_r == 1){
                                for(int i=car2->x-car_h; i<=car2->x;i++){
                                        for(int j=car2->y-car_wid/2;j<=car2->y+car_wid/2;j++){
                                                ptr[j * xres_virtual + i] = car2->color;
                                        }
                                }
                        }

                        if (car2->go_l == 1){
                                for(int i=car2->x; i<=car2->x+car_h;i++){
                                        for(int j=car2->y-car_wid/2;j<=car2->y+car_wid/2;j++){
                                                ptr[j * xres_virtual + i] = car2->color;
                                        }
                                }
                        }
                        if (car1->go_l == 1){
                                for(int i=car1->x; i<=car1->x+car_h;i++){
                                        for(int j=car1->y-car_wid/2;j<=car1->y+car_wid/2;j++){
                                                ptr[j * xres_virtual + i] = car1->color;
                                        }
                                }
                        }




                }



        start_flag = 1;

        }


}



void draw_go(uint32_t *ptr, struct car *carn, int xres_virtual, int x_window, int y_window){

        if(carn->go_r == 1){
                for(int i=carn->x-car_h; i<=carn->x;i++){
                        for(int j=carn->y-car_wid/2;j<=carn->y+car_wid/2;j++){
                                if ((j < 0 || j >= y_window || i < 0 || i >= x_window) || ptr[j * xres_virtual + i] != 0)
                                                carn->crash_stat=1;
                                else
                                        ptr[j * xres_virtual + i] = carn->color;
                        }
                }
        }

        if (carn->go_l==1){
                        for(int i=carn->x; i<=carn->x+car_h;i++){
                                for(int j=carn->y-car_wid/2;j<=carn->y+car_wid/2;j++){
                                        if (j < 0 || j >= y_window || i < 0 || i >= x_window || ptr[j * xres_virtual + i] != 0)
                                                carn->crash_stat=1;
                                        else
                                                ptr[j * xres_virtual + i] = carn->color;
                                }
                        }
                }
        if (carn->go_up==1){
                for(int i=carn->x-car_wid/2; i<=carn->x+car_wid/2;i++){
                        for(int j=carn->y;j<=carn->y+car_h;j++){
                                        if (j < 0 || j >= y_window || i < 0 || i >= x_window || ptr[j * xres_virtual + i] != 0)
                                                carn->crash_stat=1;
                                        else
                                                ptr[j * xres_virtual + i] = carn->color;
                        }
                }
        }
        if (carn->go_d==1){
                for(int i=carn->x-car_wid/2; i<=carn->x+car_wid/2;i++){
                        for(int j=carn->y-car_h;j<=carn->y;j++){
                                        if (j < 0 || j >= y_window || i < 0 || i >= x_window || ptr[j * xres_virtual + i] != 0)
                                                carn->crash_stat=1;
                                        else
                                                ptr[j * xres_virtual + i] = carn->color;

                        }
                }
        }
}

int sender(char *ch){

        if(sizeof(char) != send(sockfd, ch, sizeof(char), 0)){
        perror("Send error");
        return 0;
}
}


void* ch_handler(void *arg){
        char ch;
        char buffer[1];
    struct car *carn = arg;
    while(work_flag || (ch!='q')){
        ch = getch();
                buffer[0] = ch;
                if (key_pressed_1 ==    1){
                        switch (ch){
                                case 'w':
                                        if (carn->go_up == 0 && carn->go_d == 0){
                                                carn->turn_up = 1;
                                                sender(buffer);
                                        }
                                        break;
                                case 'a':
                                        if (carn->go_l == 0 && carn->go_r == 0){
                                                carn->turn_l = 1;
                                                sender(buffer);
                                        }
                                        break;
                                case 'd':
                                        if (carn->go_l == 0 && carn->go_r == 0){
                                                carn->turn_r = 1;
                                                sender(buffer);
                                        }
                                        break;
                                case 's':
                                        if (carn->go_up == 0 && carn->go_d == 0){
                                                carn->turn_d = 1;
                                                sender(buffer);
                                        }
                                        break;
                                default:
                                        break;
                        }
                }

                if (key_pressed_1 == 0){
                        if (ch != 0){
                                key_pressed_1 = 1;
                                if(carn->go_r == 1){
                                        ch='d';
                                        buffer[0] = ch;
                                        sender(buffer);
                                }
                                else if (carn->go_l == 1){
                                        ch='a';
                                        buffer[0] = ch;
                                        sender(buffer);
                                }
                        }
                }
    }
}

void* ch_handler_opp(void *arg){
        char ch;
        char buffer[1];
        struct car *carn = arg;

        while(work_flag || (ch!='q')){
        if(sizeof(char) != recv(sockfd, buffer, sizeof(char), 0)){
                        perror("Recieve error");
                }
                ch = buffer[0];
                if (key_pressed_2 == 1){
                        switch (ch){
                                case 'w':
                                        if (carn->go_up == 0 && carn->go_d == 0){
                                                carn->turn_up = 1;
                                        }
                                        break;
                                case 'a':
                                        if (carn->go_l == 0 && carn->go_r == 0){
                                                carn->turn_l = 1;
                                        }
                                        break;
                                case 'd':
                                        if (carn->go_l == 0 && carn->go_r == 0){
                                                carn->turn_r = 1;
                                        }
                                        break;
                                case 's':
                                        if (carn->go_up == 0 && carn->go_d == 0){
                                                carn->turn_d = 1;
                                        }
                                        break;
                                default:
                                        break;
                        }
                }

                if (key_pressed_2 == 0){
                        if (ch != 0){
                                key_pressed_2 = 1;
                        }
                }
    }



}

void clean_tail(uint32_t *ptr, struct car *carn, int xres_virtual, int flag){
                if (carn->go_r==1){
                        for(int i=carn->x-car_h; i<=carn->x;i++){
                                for(int j=carn->y-car_wid/2;j<=carn->y+car_wid/2;j++){
                                        if(flag==0){
                                                if (ptr[j * xres_virtual + i]== carn->color){
                                                        if (!(i==carn->x-car_h && j==carn->y))
                                                                ptr[j * xres_virtual + i] = 0;
                                                        else
                                                                ptr[j * xres_virtual + i] = carn->color-1;
                                                }
                                        }
                                        else
                                                ptr[j * xres_virtual + i] = 0;
                                }
                        }
                }
                if (carn->go_l==1){
                        for(int i=carn->x; i<=carn->x+car_h;i++){
                                for(int j=carn->y-car_wid/2;j<=carn->y+car_wid/2;j++){
                                        if(flag==0){
                                                if (ptr[j * xres_virtual + i]== carn->color){
                                                        if(!(i==carn->x+car_h && j==carn->y))
                                                                ptr[j * xres_virtual + i] = 0;
                                                        else
                                                                ptr[j * xres_virtual + i] = carn->color-1;
                                                }
                                        }
                                        else
                                                ptr[j * xres_virtual + i] = 0;
                                }
                        }
                }
                if (carn->go_up==1){
                        for(int i=carn->x-car_wid/2; i<=carn->x+car_wid/2;i++){
                                for(int j=carn->y;j<=carn->y+car_h;j++){
                                        if(flag==0){
                                                if (ptr[j * xres_virtual + i]== carn->color){
                                                        if(!(i==carn->x && j==carn->y+car_h))
                                                                ptr[j * xres_virtual + i] = 0;
                                                        else
                                                                ptr[j * xres_virtual + i] = carn->color-1;
                                                }
                                        }
                                        else
                                                ptr[j * xres_virtual + i] = 0;
                                }
                        }
                }
                if (carn->go_d==1){
                        for(int i=carn->x-car_wid/2; i<=carn->x+car_wid/2;i++){
                                for(int j=carn->y-car_h;j<=carn->y;j++){
                                        if(flag==0){
                                                if (ptr[j * xres_virtual + i]== carn->color){
                                                        if( !(i==carn->x && j==carn->y-car_h))
                                                                ptr[j * xres_virtual + i] = 0;
                                                        else
                                                                ptr[j * xres_virtual + i] = carn->color-1;
                                                }
                                        }
                                        else
                                                ptr[j * xres_virtual + i] = 0;
                                }
                        }
                }

}



void general(uint32_t *ptr, struct car *carn, int xres_virtual, int x_window, int y_window){
    curs_set(1);

                if (carn->turn_up == 1){

                        if (carn->go_r==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y-1) * xres_virtual + carn->x-car_h] = carn->color;
                                carn->x=carn->x-car_h;
                                carn->y=carn->y - car_h - 1;
                                carn->go_r=0;
                                carn->go_up=1;
                                carn->turn_up = 0;
                        }
                        if (carn->go_l==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y-1) * xres_virtual +carn->x+car_h] = carn->color;
                                carn->x=carn->x+car_h;
                                carn->y=carn->y - car_h - 1;
                                carn->go_l=0;
                                carn->go_up=1;
                                carn->turn_up = 0;
                        }
                }

                if (carn->turn_d == 1){
                        if (carn->go_r==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y+1) * xres_virtual + carn->x-car_h] = carn->color;
                                carn->x = carn->x-car_h;
                                carn->y = carn->y+car_h+1;
                                carn->go_r=0;
                                carn->go_d=1;
                                carn->turn_d = 0;
                        }
                        if (carn->go_l==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y+1) * xres_virtual + carn->x+car_h] = carn->color;
                                carn->x = carn->x + car_h;
                                carn->y = carn->y + car_h +1;
                                carn->go_l=0;
                                carn->go_d=1;
                                carn->turn_d = 0;
                        }
                }

                if (carn->turn_r == 1){
                        if (carn->go_up==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y+car_h) * xres_virtual + carn->x+1] = carn->color;
                                carn->x=carn->x+car_h+1;
                                carn->y=carn->y+car_h;
                                carn->go_up=0;
                                carn->go_r=1;
                                carn->turn_r = 0;
                        }
                        if (carn->go_d==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y-car_h) * xres_virtual + carn->x+1] = carn->color;
                                carn->x=carn->x+car_h+1;
                                carn->y=carn->y-car_h;
                                carn->go_d=0;
                                carn->go_r=1;
                                carn->turn_r = 0;
                        }
                }

                if (carn->turn_l == 1){
                        if (carn->go_up==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y+car_h) * xres_virtual + carn->x-1] = carn->color;
                                carn->x=carn->x-car_h-1;
                                carn->y=carn->y+car_h;
                                carn->go_up=0;
                                carn->go_l=1;
                                carn->turn_l = 0;
                        }
                        if (carn->go_d==1){
                                clean_tail(ptr, carn, xres_virtual,0);
                                ptr[(carn->y-car_h) * xres_virtual + carn->x-1] = carn->color;
                                carn->x=carn->x-car_h-1;
                                carn->y=carn->y-car_h;
                                carn->go_d=0;
                                carn->go_l=1;
                                carn->turn_l = 0;
                        }
                }

                if (carn->go_up == 1){
                        clean_tail(ptr, carn, xres_virtual,0);
                        carn->y--;
                        curs_set(0);
                        draw_go(ptr, carn, xres_virtual, x_window, y_window);
                }

                if (carn->go_d == 1){
                        clean_tail(ptr, carn, xres_virtual,0);
                        carn->y++;
                        curs_set(0);
                        draw_go(ptr, carn, xres_virtual, x_window, y_window);
                }

                if (carn->go_l == 1){
                        clean_tail(ptr, carn, xres_virtual,0);
                        carn->x--;
                        curs_set(0);
                        draw_go(ptr, carn, xres_virtual, x_window, y_window);
                }

                if (carn->go_r == 1){
                        clean_tail(ptr, carn, xres_virtual,0);
                        carn->x++;
                        curs_set(0);
                        draw_go(ptr, carn, xres_virtual, x_window, y_window);
                }

}





int main(int argc, char *argv[]){

        if (argc!=3){
                perror("Incorrect usage: <resolution> <ip addr>");
                return 1;
        }

        unsigned long int dest_ip = 0;
    struct sockaddr_in addr;
        int size = sizeof(addr);

    if(0 > (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))){
        perror("socket err");
        return __LINE__;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(0 > bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))){
                close(sockfd);
        perror("bind err");
        return __LINE__;
    }

        if(0 == inet_aton(argv[2], &addr.sin_addr)){
                close(sockfd);
                perror("ip is incorrect");
                return __LINE__;
        }
        if(0 > connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))){
                close(sockfd);
                perror("connect err");
                return __LINE__;
        }

        dest_ip = ntohl(addr.sin_addr.s_addr);
        if (-1==getsockname(sockfd, (struct sockaddr *)&addr, &size)){
                close(sockfd);
                perror("getsockname err");
                return __LINE__;
        }


        int fb, x_window, y_window;
        struct fb_var_screeninfo info;
        size_t fb_size, map_size, page_size;
        uint32_t *ptr, color;
    signal(SIGINT, handler);
        page_size = sysconf(_SC_PAGESIZE);
    sscanf(argv[1], "%d%*c%d", &x_window, &y_window);



        if ( 0 > (fb = open("/dev/fb0", O_RDWR))) {
    perror("open");
    return __LINE__;
        }

        if ( (-1) == ioctl(fb, FBIOGET_VSCREENINFO, &info)) {
    perror("ioctl");
    close(fb);
    return __LINE__;
        }

        fb_size = sizeof(uint32_t) * info.xres_virtual * info.yres_virtual;
        map_size = (fb_size + (page_size - 1 )) & (~(page_size-1));

        ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
        if ( MAP_FAILED == ptr ) {
        perror("mmap");
        close(fb);
        return __LINE__;
        }

        if (x_window > info.xres || y_window > info.yres){
                perror("incorrect resolution size");
                return 1;
        }

        if(NULL == initscr()){
                perror("Initscr error");
                return __LINE__;
        }

        noecho();
        //curs_set(0);

        memset(ptr, 0, map_size);

        struct car car1;
        struct car car2;


    car1.go_d = 0;
    car1.go_up = 0;
    car1.go_r = 0;
    car1.go_l = 0;
    car1.turn_d = 0;
    car1.turn_up = 0;
    car1.turn_l = 0;
    car1.turn_r = 0;
    car1.crash_stat = 0;


    car2.go_d = 0;
    car2.go_up = 0;
    car2.go_r = 0;
    car2.go_l = 0;
    car2.turn_d = 0;
    car2.turn_up = 0;
    car2.turn_l = 0;
    car2.turn_r = 0;
    car2.crash_stat = 0;







        if(dest_ip < ntohl(addr.sin_addr.s_addr)){
                car1.x = car_h;
                car1.y = car_wid/2;
                car1.color = 0x00FF0000;
                car1.go_r = 1;

                car2.x = (x_window - 1 - car_h);
                car2.y = y_window - 1 - car_wid/2 ;
                car2.color = 0x000000FF;
                car2.go_l = 1;
        }
        else{
                car2.x = car_h;
                car2.y = car_wid/2;
                car2.color = 0x00FF0000;
                car2.go_r = 1;

                car1.x = (x_window - 1 - car_h);
                car1.y = y_window - 1 - car_wid/2;
                car1.color =0x000000FF;
                car1.go_l = 1;

        }

        pthread_t func1, func2;
    pthread_create(&func1, NULL, ch_handler, &car1);
        pthread_create(&func1, NULL, ch_handler_opp, &car2);


        while(work_flag == 1 && car1.crash_stat==0 && car2.crash_stat==0){

                init_walls_n_cars(ptr, &car1, &car2, info.xres_virtual, x_window, y_window, info.xres, info.yres);



                general(ptr, &car1, info.xres_virtual, x_window, y_window);
                general(ptr, &car2, info.xres_virtual, x_window, y_window);

                usleep(62500);
        }


        memset(ptr, 0, map_size);
        usleep(5000);
    munmap(ptr, map_size);
    close(fb);
        close(sockfd);
    endwin();

        if (car1.crash_stat == 1 && car2.crash_stat==0){
                fprintf(stderr, "I'm winner!\n");
        }

        if (car2.crash_stat == 1 && car1.crash_stat == 0){
                fprintf(stderr, "I'm loser!\n");
        }

        if (car2.crash_stat == 1 && car1.crash_stat == 1){
                fprintf(stderr, "No one\n");
        }

        return 0;

}
