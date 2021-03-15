#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>   /* for setitimer */
#include <unistd.h>     /* for pause */
#include <signal.h> 
#include <sys/time.h>
#include <sys/resource.h>
#include <proc/readproc.h>
 #include <sys/sysinfo.h>
 #include <arpa/inet.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

struct object
{
    int xPosition;
    int yPosition;
    int page;
    int totalpage;
    int reste;
    unsigned char zn;
    int test;
    chtype ozn;
};

struct stockage_metric
{
    unsigned long long start_time,cpu_time,cpu_time_2,process_time,process_time_2;
    unsigned long long pid;
    unsigned long long ppid;
    float t;
    char name_process;
    char * ligne;
};
//**************************************************variable global********************************************************//


#define N 10
struct object obj[N]={};
struct stockage_metric refrech[24]={};  
#define IP_PROTOCOL 0 
#define PORT_NO 15050 
#define NET_BUF_SIZE 400 
#define cipherKey 'S' 
#define sendrecvflag 0 

//*********************************************************************************************
int wbox=0;
int lev=0;

struct stockage_metric_table
{
    struct stockage_metric * tab;
    int taille;
    int indices;

};
//**************************************************refrech affichage******************************************************************************
void func()
{
    clear();
    mvprintw(1,8, "Oops Soko! Map -> %d",obj[0].page);
    mvprintw(2,1, "Move-WSAD. Restart-R. Map-M. Exit-Q.");
    
    int x=0, y=0, h=1, w=1, map;
    wbox=0;
    for (y=0; y<24; y++)
    {
        //printf("%s\n",tab.tab[y].ligne);
        mvaddstr(y+4, x,refrech[y].ligne);
        
        
    }
    
    obj[0].ozn = mvinch(4,10);
                obj[0].yPosition = 4;
                obj[0].xPosition = 0;
                obj[0].zn = refrech[y].ligne[0];
                mvaddch(obj[0].yPosition,obj[0].xPosition,obj[0].zn | COLOR_PAIR(3));
                
    move(obj[0].yPosition,obj[0].xPosition);

    
}

int startTimer(double seconds)
{
    struct itimerval it_val;
    double integer, fractional;

    integer = (int)seconds;
    fractional = seconds - integer;

    it_val.it_value.tv_sec = integer;
    it_val.it_value.tv_usec = fractional * 1000000;
    it_val.it_interval = it_val.it_value;

    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
        return 0;

    return 1;
}


//
unsigned long long pid_mem(unsigned long long pid)
 {
            FILE* fp;
            char str[]="/stat";
            char src[] ="";
            char dest[40] = "/proc/";
            unsigned long long process_mem;
            char * line = NULL;
            char * ptr;
            size_t len = 0; 
            sprintf(src,"%llu",(unsigned long long)pid);
            strcat(dest, src);
            strcat(dest,"/statm");
            fp=fopen(dest,"r");
            len = 0;
            getline(&line,&len, fp);
	        ptr = strtok(line, " ");
            
            int i=0;   
            process_mem=0;
            while(ptr != NULL)
	        {
                if(i==1)
                {
                    process_mem=atoi(ptr);
                    
                }
                
                /*
                if(i==21)
                {
                        start_time=atoi(ptr);
               }
               */
		        i=i+1;
	            ptr = strtok(NULL, " ");
            }
            fclose(fp);
            return process_mem;     
 }
 unsigned long long pid_time(unsigned long long pid,unsigned long long * start_time)
 {
            FILE* fp;
            char str[]="/stat";
            char src[] ="";
            char dest[40] = "/proc/";
            unsigned long long process_time;
            char * line = NULL;
            char * ptr;
            size_t len = 0; 
            sprintf(src,"%llu",(unsigned long long)pid);
            strcat(dest, src);
            strcat(dest,"/stat");
            fp=fopen(dest,"r");
            len = 0;
            getline(&line,&len, fp);
	        ptr = strtok(line, " ");
            
            int i=0;   
            process_time=0;
            while(ptr != NULL)
	        {
                if(i==14 || i==13)
                {
                        process_time+=atoi(ptr);
                      }
                
                if(i==21)
                {
                        *(start_time)=atoi(ptr);
               }
               
		        i=i+1;
	            ptr = strtok(NULL, " ");
            }
            fclose(fp);
            return process_time;     
 }
 unsigned long long cpu_t()
 {
            FILE* fp;
            unsigned long long cpu_time;
            char * line = NULL;
            char * ptr;
            char * test;
            size_t len = 0;
            fp=fopen("/proc/stat" ,"r");
            line = NULL;
            len = 0;
            getline(&line,&len, fp);
            test=line+6;
	        ptr = strtok(test, " ");
            cpu_time=0;
            while(ptr != NULL)
	        {
                cpu_time=cpu_time+atoi(ptr);
		        //printf("%s\n", ptr);
                ptr = strtok(NULL, " ");
	        }
            fclose(fp);
            return cpu_time;
          
 }
 float cpu_pourcentage_utilisation(unsigned long long start_time,unsigned long long process_time){
        struct sysinfo info;
        unsigned long long Hertz;
        unsigned long long uptime; 
        float seconds;  
        sysinfo(&info);
        uptime=info.uptime;
        Hertz=sysconf(_SC_CLK_TCK);
        seconds=(float)uptime-(start_time/Hertz);
        return 100*((process_time/Hertz)/seconds);
        

 }
void affichage(struct stockage_metric_table tab,int page)
{
    
    clear();
    mvprintw(1,8, "htop tu oubliera -> %d",obj[0].page);
    mvprintw(2,1, "Move-WSAD.  Exit-w.");
    
    int x=0, y=0, h=1, w=1, map;
    wbox=0;
    for (y=page*24; y<(page+1)*24; y++)
    {
        //printf("%s\n",tab.tab[y].ligne);
        mvaddstr(y+4, x,tab.tab[y].ligne);
        
        
    }
    
    obj[0].ozn = mvinch(4,10);
                obj[0].yPosition = 4;
                obj[0].xPosition = 0;
                obj[0].zn = tab.tab[0].ligne[0];
                mvaddch(obj[0].yPosition,obj[0].xPosition,obj[0].zn | COLOR_PAIR(3));
                
    move(obj[0].yPosition,obj[0].xPosition);

    
}

void palette()
{
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
}


void Play(int input,struct stockage_metric_table tab)
{
    bool restart = FALSE;
    int i=0;
            for (int y=obj[0].page*24; y<(obj[0].page+1)*24; y++)
                {
                    mvaddstr(i+4, 0,tab.tab[y].ligne);
                    i++;
                }
    switch (input)
    {
        case 'z' :
        case 'Z' :
        if(obj[0].yPosition>4 ){
                
                mvaddstr(obj[0].yPosition,0 ,tab.tab[((obj[0].yPosition-4)+obj[0].page*24)].ligne);
                
        obj[0].yPosition -= 1;
        }
        else if(obj[0].page>0)
        {
            
            obj[0].yPosition=4;
            clear();
            obj[0].page--;
            mvprintw(1,8, "Oops Soko! Map -> %d iteration %d",obj[0].page,obj[0].test);
            mvprintw(2,1, "Move-WSAD. Restart-R. Map-M. Exit-Q.");
            int i=0;
            for (int y=obj[0].page*24; y<(obj[0].page+1)*24; y++)
                {
                    refrech[i].ligne=tab.tab[y].ligne;
                    mvaddstr(i+4, 0,tab.tab[y].ligne);
                    i++;
                }
            
            
            
        }
        break;
        case 's' :
        case 'S' :
        if(obj[0].yPosition<24)
        {
            if(obj[0].totalpage==obj[0].page && obj[0].yPosition>=obj[0].reste+3){

            }
            else 
            {
            mvaddstr(obj[0].yPosition,0 ,tab.tab[((obj[0].yPosition-4)+obj[0].page*24)].ligne);
            obj[0].yPosition +=1;
            }
        
        }
        else if (obj[0].page<obj[0].totalpage)
        {

            obj[0].yPosition=4;
            clear();
            obj[0].page++;
            mvprintw(1,8, "htop tu oubliera -> %d",obj[0].page);
            mvprintw(2,1, "Move-WSAD.  Exit-w.");
            int i=0;
                for (int y=obj[0].page*24; y<(obj[0].page+1)*24; y++)
                {
                    refrech[i].ligne=tab.tab[y].ligne;
                    mvaddstr(i+4, 0,tab.tab[y].ligne);
                    i++;
                }
            
        }
            
        break;

        
        break;

        case 'q' :
        case 'Q' :
        if(obj[0].page>0)
        {
            
            obj[0].yPosition=4;
            clear();
            obj[0].page--;
            mvprintw(1,8, "htop tu oubliera -> %d",obj[0].page);
            mvprintw(2,1, "Move-WSAD.  Exit-w.");   
            int i=0;
            for (int y=obj[0].page*24; y<(obj[0].page+1)*24; y++)
                {
                    refrech[i].ligne=tab.tab[y].ligne;
                    mvaddstr(i+4, 0,tab.tab[y].ligne);
                    i++;
                }
            
            
            
        }

        break;

        case 'd' :
        case 'D' :
        if (obj[0].page<obj[0].totalpage)
        {

            obj[0].yPosition=4;
            clear();
            obj[0].page++;
            mvprintw(1,8, "Oops Soko! Map -> %d iteration %d",obj[0].page,obj[0].test);
            mvprintw(2,1, "Move-WSAD. Restart-R. Map-M. Exit-Q.");
            int i=0;
                for (int y=obj[0].page*24; y<(obj[0].page+1)*24; y++)
                {
                    refrech[i].ligne=tab.tab[y].ligne;
                    mvaddstr(i+4, 0,tab.tab[y].ligne);
                    i++;
                }
            
        }
        break;
        default : 
        break;
    }
if (!restart)
{
    for (int o=0; o <= wbox; o++)
    {
        obj[o].ozn = mvinch(obj[o].yPosition, obj[o].xPosition);
        for (int i=0;i<strlen(tab.tab[((obj[0].yPosition-4)+obj[0].page*24)].ligne);i++)
        {
            mvaddch(obj[o].yPosition,i,tab.tab[((obj[0].yPosition-4)+obj[0].page*24)].ligne[i] | ((o == 0 ) ? COLOR_PAIR(3) : COLOR_PAIR(5)));    
        }
        
    }
    move(obj[0].yPosition,obj[0].xPosition);
}
else restart = FALSE;
}

int main ()
{
    //initialisation
            int len=sizeof(struct stockage_metric); 
            char ppid[]="";
            int reste;
            struct stockage_metric ms;
            struct stockage_metric_table smt;
            smt.taille=100000;
            smt.indices=0;
            smt.tab=(struct stockage_metric*)malloc(sizeof(struct stockage_metric)*smt.taille);
            unsigned long long s=1417;
            unsigned long long start_time,cpu_time,cpu_time_2,process_time,process_time_2;
            double cpu_per;
            float t;
            char * ptr;
            char * temps;
            PROCTAB* proc;
            proc_t proc_info;
            proc_t* temp;
            int test;
            int compteur=0;
            int sockfd, nBytes; 
            struct sockaddr_in addr_con; 
            int addrlen = sizeof(addr_con); 
            addr_con.sin_family = AF_INET; 
            addr_con.sin_port = htons(PORT_NO); 
            addr_con.sin_addr.s_addr = INADDR_ANY;
            sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 
  
    

    
            
    //fin init
    
            ms.ligne=malloc(sizeof(char)*3000);
     int ch;
     initscr();
     
     if (!has_colors())
     {
         endwin();
         printf("Eroor initialising colors.n");
         exit(1);
     }
     
     start_color();
     palette();
    //************
    

     //**********
     //affichage(&smt,0);
     //Level(lev);
     obj[0].test=0;
     while((ch = getch()) != 'w')
     {
          proc= openproc(PROC_FILLARG | PROC_FILLSTAT);
    memset(&proc_info, 0, sizeof(proc_info));
    //printf("%-10d %-10d \n", proc_info.tid, proc_info.ppid);
    int j=0;
    temp=readproc(proc, &proc_info);
    //printf("pointeur readproc:%p \n",temp);
    test=1;
    smt.indices=0;
    while (temp != NULL) 
    {
        
//****************************************premiere iteration*******************************************************//           
        smt.tab[smt.indices].process_time=pid_time(proc_info.tid,&start_time);
        cpu_time=cpu_t();
        usleep(10);     
//****************************************deuxieme iteration*****************************************************//            
        //process_time_2=pid_time(proc_info.tid);
        //cpu_time_2=cpu_t();   
//**********************************cpu usage*******************************************//        
        smt.tab[smt.indices].t=cpu_pourcentage_utilisation(start_time,smt.tab[smt.indices].process_time);
        if (proc_info.cmdline != NULL) {
            // affiche comme htop
            char affiche[]="";
            temps=*proc_info.cmdline;
            ptr = strtok(temps, " ");
            int i=0;
            while(ptr != NULL && i<3)
	        {
                strcat(affiche,ptr);
                ptr = strtok(NULL, " ");
                i++;
            }
            //fin afiche comme htop
            
            strcpy(ms.ligne,affiche);
            smt.tab[smt.indices].ligne=malloc(sizeof(char)*300);
            sprintf(smt.tab[smt.indices].ligne,"%-10d ",proc_info.tid);
            sprintf(ppid,"%-10d ",proc_info.ppid);
            strcat(smt.tab[smt.indices].ligne,ppid);
            sprintf(ppid,"%-10f%",smt.tab[smt.indices].t);
            strcat(smt.tab[smt.indices].ligne,ppid);
            strcat(smt.tab[smt.indices].ligne,"         ");
            strcat(smt.tab[smt.indices].ligne,ms.ligne);
            
            //smt.tab[smt.indices]=ms;
            //printf("%s\n",smt.tab[smt.indices].ligne);
            
            
        } 
        else {
            
            smt.tab[smt.indices].ligne=malloc(sizeof(char)*300);
            sprintf(smt.tab[smt.indices].ligne,"%-10d ",proc_info.tid);
            sprintf(ppid,"%-10d ",proc_info.ppid);
            strcat(smt.tab[smt.indices].ligne,ppid);
            sprintf(ppid,"%-10f",smt.tab[smt.indices].t);
            strcat(smt.tab[smt.indices].ligne,ppid);
            strcat(smt.tab[smt.indices].ligne,"         ");
            strcat(smt.tab[smt.indices].ligne,proc_info.cmd);
            //smt.tab[smt.indices]=ms;
            //printf("%s\n",smt.tab[smt.indices].ligne);
           }
         j++;
         /*
         sendto(sockfd,&smt.tab[smt.indices], sizeof(struct stockage_metric), 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 
        */
              sendto(sockfd, smt.tab[smt.indices].ligne,NET_BUF_SIZE, 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 
  
        temp=readproc(proc, &proc_info);
    smt.indices+=1;
    }
    sendto(sockfd,(void *) &smt, sizeof(struct stockage_metric), 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 
    
    if(compteur==0)
    {
        obj[0].totalpage=(int)smt.indices/24;
        obj[0].page=0;
        obj[0].reste=smt.indices%24;
        affichage(smt,0);
        compteur++;
        printf("%i",obj[0].totalpage);
        
    }
    //sleep(3);
     closeproc(proc);
    
    Play(ch,smt);
    obj[0].test++;
     }
     endwin();
     return 0;
}