
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 2048
#define SBUFLEN 128
#define NAME 1
#define ADDR 2
#define NUMHOLD 3
#define HOLDRATIO 4
#define COMPANYNAME 0
FILE *fpoint;
char buf[BUFLEN];
char contents[5][12 * SBUFLEN];
    int count[5]={0,0,0,0,0};
void insidetag();
int fileopen(char *filename, FILE **f);
void clearbuf(char *b);
void insidetag();
int  output(char *filename);
int to_endofthetag(char *b);

int main(int argc, char *argv[])
{

    fileopen(argv[1],&fpoint);
    insidetag();
    output(argv[2]);
    return 0;
}

int fileopen(char *filename, FILE **f)
{
    if ((*f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "file open error");
        return -1;
    }
    else
    {
        return 0;
    }
}
void clearbuf(char *b)
{
    int i = 0;
    for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++)
    {
        b[i] = '\0';
    }
}
int to_endofthetag(char *b)
{
    clearbuf(b);
    int i = 0;
    char c;
    while ((c = getc(fpoint)) != EOF)
    {
        if (c == '>')
        {
            return 1;
        }
        else
        {
            b[i] = c;
            i++;
        }
        if (i == BUFLEN)
        {
            printf("too long tag");
            return -1;
        }
    }
}

int judgewhichcontent()
{
    if ((strstr(buf, "NameMajorShareholders")) != NULL && (strstr(buf, "MajorShareholdersMember") != NULL))
    {
        return NAME;
    }
    else if ((strstr(buf, "AddressMajorShareholders")) != NULL && (strstr(buf, "MajorShareholdersMember") != NULL))
    {
        return ADDR;
    }
    else if ((strstr(buf, "NumberOfSharesHeld")) != NULL && (strstr(buf, "MajorShareholdersMember") != NULL))
    {
        return NUMHOLD;
    }
    else if ((strstr(buf, "ShareholdingRatio")) != NULL && (strstr(buf, "MajorShareholdersMember") != NULL))
    {
        return HOLDRATIO;
    }else if((strstr(buf, "CompanyNameCoverPage")) != NULL || strstr(buf, "jpsps_cor:FundNameCoverPage")){
        return COMPANYNAME;
    }
    else
    {
        return -1;
    }
}

void insidetag()
{
    char c;
    int f = 0;
    int f0 = 0;
    int m = 0;
    int x = 0;

    int hindex[5] = {0, 0, 0, 0, 0};
        for (int i = 0; i < 5; i++)
    {
        clearbuf(contents[i]);
    }
    while ((c = fgetc(fpoint)) != EOF)
    {
        int i = 0;
        int f = 0;
        if (c == '<')
        {
            f = to_endofthetag(buf);
        }
        else
        {
            continue;
        }
        if (f == 1)
        {
            int ju = judgewhichcontent();
            if (ju >= 0)
            {
                while ((c = getc(fpoint)) != EOF)
                {
                    if (c != '<')
                    {
                        if (c != '\n'&&c!='\t')
                        {
                            contents[ju][hindex[ju]++] = c;
                            if(hindex[ju]==SBUFLEN*12-2){
                                printf("%s","long long long");
                                fprintf(stderr,"%s","long long");
                                break;
                            }
                            
                        }
                    }
                    else if (c == '<')
                    {
                        to_endofthetag(buf);
                        if (strstr(buf, "/jpcrp") != NULL||strstr(buf, "/jpsps") != NULL||strstr(buf,"/xbrl")!=NULL)
                        {
                            contents[ju][hindex[ju]++] = '\n';
                            count[ju]++;
                            break;
                        }
                    }
                }
            }
        }
    }
}


int output(char *filename){

    FILE * ofi;
    if ((ofi = fopen(filename, "r")) == NULL)
    {
        if((ofi = fopen(filename, "w")) == NULL){
            printf("%s","fileerro");
            return -1;
        }
        fprintf(ofi,"-%s\t%s\t%s\t%s\t%s\n","?????????","????????????","??????","????????????","?????????????????????");
    }
    else
    {
        if((ofi = fopen(filename, "a")) == NULL){
            printf("%s","fileerro");
            return -1;
        }
    }
    int oindex[5] = {0, 0, 0, 0, 0};
    for (int corp = 0; corp < count[1]; corp++)
    {
        oindex[COMPANYNAME]=0;
        for (int co = 0; co < 5; co++)
        {
            while (contents[co][oindex[co]] != '\n')
            {
                fprintf(ofi,"%c", contents[co][oindex[co]++]);
            }
            fprintf(ofi,"%s", "\t");
            oindex[co]++;
        }
        fprintf(ofi,"%s", "\n");
    }
    fclose(ofi);
    return 0;
}
