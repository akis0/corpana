#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define BUFLEN 1024
FILE *fpoint;
char buf[1024];
char con[150];
void insidetag();
int fileopen(char *filename);
void clearbuf(char* b);
void insidetag();

int main(int argc, char* argv[])
{
    fileopen(argv[1]);
    insidetag(); 
    return 0;
}



int fileopen(char *filename)
{
    if ((fpoint = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "file open error");
        return -1;
    }
    else
    {
        return 0;
    }
}
void clearbuf(char* b){
    int i=0;
    for(int i=0; i<BUFLEN;i++){
        b[i]='\0';
    }
}

void insidetag(){
    char c;
    int f = 0;
    int f0=0;
    while ((c = fgetc(fpoint)) != EOF){
        clearbuf(buf);
        int i=0;
        int f=0;
        if (c == '<'){
            clearbuf(buf);
            i=0;
            while ((c = getc(fpoint)) != EOF){
                if (c == '>'){
                    f=1;
                    break;
                }else{
                    buf[i]=c;
                    i++;
                }
            }
        }else{
            continue;
        }
        if(f==1){
            if( (strstr(buf,"MajorShareholders")!=NULL)&&(strstr(buf,"MajorShareholdersText")==NULL) ){
                int m=0;
                int x=0;
                while((c=getc(fpoint))!=EOF){
                    if(c!='<'){
                        printf("%c",c);
                    }else if(c=='<'){
                        char intag[BUFLEN];
                        clearbuf(intag);
                        int t=0;
                        while((c=getc(fpoint))!=EOF){
                            if(c=='>'){
                                break;
                            }else{
                                intag[t++]=c;
                            }
                        }
                        if(strstr(intag,"/ix")!=NULL){
                            x=1;
                        }
                    }else{
                        printf("\n\n");
                        break;
                    }
                    if(x==1){
                        printf("\n\n");
                        break;
                    }
                }
            }
        }
    }
}
