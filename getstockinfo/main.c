/*有報の株主一覧の部分から情報を抽出する。今は注目するタグの内側のデータがspan等なしに直書きされてたら,具体的にはNTTDATAのものからは取得できる。*/
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define BUFLEN 1024
FILE *fpoint;
char buf[BUFLEN];
char con[150];
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
            if( (strstr(buf,"MajorShareholders")!=NULL) ){
                while((c=getc(fpoint))!=EOF){
                    if(c!='<'){
                        printf("%c",c);
                    }else{
                        printf("\n\n");
                        break;
                    }
                }
                
            }
        }
    }
}

int main(int argc, char* argv[])
{
    fileopen(argv[1]);
    insidetag(); 
    return 0;
}
