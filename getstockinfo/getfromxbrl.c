#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define BUFLEN 2048
#define SBUFLEN 512
#define NAME 1
#define ADDR 2
#define NUMHOLD 3
#define HOLDRATIO 4
FILE *fpoint;
char buf[BUFLEN];
void insidetag();
int fileopen(char *filename);
void clearbuf(char* b);
void insidetag();
int  to_endofthetag(char * b);


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
    for(int i=0; i<sizeof(b)/sizeof(b[0]);i++){
        b[i]='\0';
    }
}
int to_endofthetag(char *b){
    clearbuf(b);
    int i=0;
    char c;
    while ((c = getc(fpoint)) != EOF){
        if (c == '>'){
            return 1;
        }else{
        b[i]=c;
        i++;
        }
        if(i==BUFLEN){
            printf("too long tag");
            return -1;
        }
    }
}

int judgewhichcontent(){
    if((strstr(buf,"NameMajorShareholders"))!=NULL&&(strstr(buf,"MajorShareholdersMember")!=NULL)){
        return NAME;
    }else if((strstr(buf,"AddressMajorShareholders"))!=NULL&&(strstr(buf,"MajorShareholdersMember")!=NULL)){
        return ADDR;
    }else if((strstr(buf,"NumberOfSharesHeld"))!=NULL&&(strstr(buf,"MajorShareholdersMember")!=NULL)){
        return NUMHOLD;
    }else if((strstr(buf,"ShareholdingRatio"))!=NULL&&(strstr(buf,"MajorShareholdersMember")!=NULL)){
        return HOLDRATIO;
    }else{
        return 0;
    }
}

void insidetag(){
    char c;
    int f = 0;
    int f0=0;
    int m=0;
    int x=0;
    // char contents[5][12*SBUFLEN];
    // int hindex[5];
    // char cont[SBUFLEN];
    while ((c = fgetc(fpoint)) != EOF){
        int i=0;
        int f=0;
        if (c == '<'){
            f=to_endofthetag(buf);
        }else{
            continue;
        }
        if(f==1){
            int ju=judgewhichcontent();
            if(ju>0){
                while((c=getc(fpoint))!=EOF){
                    if(c!='<'){
                        if(c!='\n'){
                        //contents[ju][hindex[ju]++]=c;
                        printf("%c",c);
                        }
                    }else if(c=='<'){
                        to_endofthetag(buf);
                        if(strstr(buf,"/jpcrp")!=NULL){
                        // contents[ju][hindex[ju]++]='\n';
                        printf("\n\n");
                        break;
                        }
                    }
                }
            }
        }
    }
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         printf("%c",contents[i][j]);
    //     }
    // }
}
