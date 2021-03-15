// server code for UDP socket programming 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
#define IP_PROTOCOL 0 
#define PORT_NO 15050 
#define NET_BUF_SIZE 200 
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 
struct stockage_metric
{
    unsigned long long start_time,cpu_time,cpu_time_2,process_time,process_time_2;
    unsigned long long pid;
    unsigned long long ppid;
    float t;
    char name_process;
    char * ligne;
};
  struct stockage_metric_table
{
    struct stockage_metric * tab;
    int taille;
    int indices;

};

// function to clear buffer 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < NET_BUF_SIZE; i++) 
        b[i] = '\0'; 
} 
  
// function to encrypt 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// function sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, nofile); 
        len = strlen(nofile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = Cipher(buf[i]); 
        return 1; 
    } 
  
    char ch, ch2; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
        ch2 = Cipher(ch); 
        buf[i] = ch2; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
} 

//driver code 
int main() 
{ 
    int z=0;
    int json_var=0;
    char temp[]=" ";
    char **tab=(char **)malloc(sizeof(char *)*100000);    
    int sockfd, nBytes;
    struct stockage_metric_table smt; 
    struct sockaddr_in addr_con; 
    int addrlen = sizeof(addr_con);
    int len=sizeof(struct stockage_metric); 
    addr_con.sin_family = AF_INET; 
    addr_con.sin_port = htons(PORT_NO); 
    addr_con.sin_addr.s_addr = INADDR_ANY; 
    char net_buf[NET_BUF_SIZE]; 
    FILE* fp; 
    FILE* fp1;
    char test[]="";
    char * ptr;
    // socket() 
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 
    /*        
    if (sockfd < 0) 
        printf("\nfile descriptor not received!!\n"); 
    else
        printf("\nfile descriptor %d received\n", sockfd); 
  */
    // bind() 
    bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con));
     //   printf("\nSuccessfully binded!\n"); 
    //else
    //    printf("\nBinding Failed!\n"); 
    smt.tab=(struct stockage_metric*)malloc(sizeof(struct stockage_metric)*100000);
    int i=0;
    fp = fopen("result.txt", "w"); 
    fp1=fopen("result.json", "w");
    int open=0;
    int ordre=0;      
    while (1) { 
        if(open==2)
        {
            open=0;
            if(ordre%2==1)
            {
                fp = fopen("result1.txt", "w");
                fp1=fopen("result1.json", "w");

            
            }
            else
            {

            fp = fopen("result.txt", "w");
            fp1=fopen("result.json", "w");
            }
        }
        // receive file name 
        tab[i]=malloc(sizeof(char)*500);
        nBytes = recvfrom(sockfd, tab[i], 
                          NET_BUF_SIZE, sendrecvflag, 
                          (struct sockaddr*)&addr_con, &addrlen);
       
       if(tab[i][0]=='0' || tab[i][0]=='1' || tab[i][0]=='2' || tab[i][0]=='3' || tab[i][0]=='4' || tab[i][0]=='5' || tab[i][0]=='6' || tab[i][0]=='7' || tab[i][0]=='8' || tab[i][0]=='9')
        {
           
            
             
        }
        else
        {
            
            for(int j=0;j<i;j++)
            {
                /*
                json_var=0;
                z=0;
                while (json_var<4)
                {
                strcpy(temp,"");
                int cpt=z;
                while(tab[j][z]!=' ')
                {
                
                z++;
                }
                strncat(temp,tab[j]+cpt,z-cpt);
                switch(json_var)
                {
                    case 0:  
                    if(j==0)
                    {
                        fprintf (fp, 'process information:[\n{\n"pid":%d,\n',atoi(temp));
                    }
                    else
                    {
                        fprintf (fp, '"pid":%d,\n',atoi(temp));
                    }
                      break;
                    case 1:  
                       fprintf (fp, '"ppid":%d,\n',atoi(temp));
                    
                      break;
                    case 2:  
                       fprintf (fp, '"cpu_usage":%f%,\n',atof(temp));
                      break;
                    case 3:    
                           fprintf (fp, '"name_process":%s,\n},\n',temp);
                    break;
                }
                json_var++;
                }
                */
                fprintf (fp, "%s\n",tab[j]);
        }
        i=0;
        open=2;
        ordre+=1;
        fclose(fp);


        }
        /*
        inet_ntop(AF_INET, &(addr_con.sin_addr), net_buf, INET_ADDRSTRLEN);
        printf("\nip: %s \n",net_buf);
        if (fp == NULL) 
            printf("\nFile open failed!\n"); 
        else
            printf("\nFile Successfully opened!\n"); 
  
            // process 
            
            // send
            strcpy(net_buf,"missoum");
        
            sendto(sockfd, &smt,sizeof(smt), 
                   sendrecvflag, 
                (struct sockaddr*)&addr_con, addrlen); 
            clearBuf(net_buf); 
         
        if (fp != NULL) 
             
            */
    i++;
    
    
    } 
    
            
    return 0; 
} 