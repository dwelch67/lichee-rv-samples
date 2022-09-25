
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSET 0x600
unsigned char data[0x10000];
unsigned int header[16];
unsigned char data_out[0x4000000];
unsigned int data_word[0x1000000];

typedef struct sbrom_toc1_head_info
{
    char name[16];
    unsigned int  magic;
    unsigned int  add_sum;
    unsigned int  serial_num;
    unsigned int  status;
    unsigned int  items_nr;
    unsigned int  valid_len;
    unsigned int  version_main;
    unsigned int  version_sub;
    unsigned int  reserved[3];
    unsigned int  end;
}
sbrom_toc1_head_info_t;

typedef struct sbrom_toc1_item_info
{
    char name[64];
    unsigned int  data_offset;
    unsigned int  data_len;
    unsigned int  encrypt;
    unsigned int  type;
    unsigned int  run_addr;
    unsigned int  index;
    unsigned int  reserved[69];
    unsigned int  end;
}sbrom_toc1_item_info_t;

sbrom_toc1_head_info_t toc1_head;
sbrom_toc1_item_info_t toc1_item;

int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int len;
    unsigned int binlen;
    FILE *fp;

    printf("head %lu\n",sizeof(toc1_head));
    printf("item %lu\n",sizeof(toc1_item));
    if(sizeof(toc1_head)!=64) return(1);
    if(sizeof(toc1_item)!=368) return(1);

    if(argc<3)
    {
        printf("mkheader input output\n");
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    printf("%u bytes read\n",rb);
    binlen=((rb+3)>>2)<<2;
    binlen+=(0x10000-1);
    binlen>>=16;
    binlen<<=16;
    printf("0x%08X\n",binlen);

    memset(&toc1_head,0,sizeof(toc1_head));
    toc1_head.magic=0x89119800;
    toc1_head.add_sum=0x5F0A6C39;
    toc1_head.items_nr=1;
    toc1_head.valid_len=OFFSET+binlen;
    toc1_head.end=0x3B45494D;

    memset(&toc1_item,0,sizeof(toc1_item));
    strcpy(toc1_item.name,"u-boot");
    toc1_item.data_offset=OFFSET;
    toc1_item.data_len=binlen;
    toc1_item.run_addr=0x4A000000;
    toc1_item.index=0;
    toc1_item.end=0x3B454949;

    memset(data_out,0,sizeof(data_out));
    ra=0;
    memcpy(&data_out[ra],&toc1_head,sizeof(toc1_head));
    ra+=sizeof(toc1_head);
    memcpy(&data_out[ra],&toc1_item,sizeof(toc1_item));
    ra=OFFSET;
    memcpy(&data_out[ra],data,binlen);
    ra+=binlen;

    memcpy(data_word,data_out,sizeof(data_word));
    len=(ra+3)>>2;

    rc=0;
    for(rb=0;rb<len;rb++)
    {
        rc+=data_word[rb];
    }
    printf("0x%08X\n",rc);
    data_word[5]=rc;

    fp=fopen(argv[2],"wb");
    if(fp==NULL)
    {
        printf("Error creating file [%s]\n",argv[2]);
        return(1);
    }
    fwrite(data_word,4,len,fp);
    fclose(fp);

    printf("Done.\n");
    return(0);
}
