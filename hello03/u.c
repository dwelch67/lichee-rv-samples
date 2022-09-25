

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sbrom_toc1_head_info
{
    char name[16]   ;   //user can modify
    unsigned int  magic  ;   //must equal TOC_unsigned int_MAGIC
    unsigned int  add_sum    ;
    unsigned int  serial_num ;   //user can modify
    unsigned int  status     ;   //user can modify,such as TOC_MAIN_INFO_STATUS_ENCRYP_NOT_USED
    unsigned int  items_nr;  //total entry number
    unsigned int  valid_len;
    unsigned int  version_main;  //only one byte
    unsigned int  version_sub;   //two bytes
    unsigned int  reserved[3];   //reserved for future
    unsigned int  end;
}
sbrom_toc1_head_info_t;

typedef struct sbrom_toc1_item_info
{
    char name[64];          //such as ITEM_NAME_SBROMSW_CERTIF
    unsigned int  data_offset;
    unsigned int  data_len;
    unsigned int  encrypt;           //0: no aes   //1: aes
    unsigned int  type;              //0: normal file, dont care  1: key certif  2: sign certif 3: bin file
    unsigned int  run_addr;          //if it is a bin file, then run on this address; if not, it should be 0
    unsigned int  index;             //if it is a bin file, this value shows the index to run; if not
                           //if it is a certif file, it should equal to the bin file index
                           //that they are in the same group
                           //it should be 0 when it anyother data type
    unsigned int  reserved[69];     //reserved for future;
    unsigned int  end;
}sbrom_toc1_item_info_t;

sbrom_toc1_head_info_t toc1_head;
sbrom_toc1_item_info_t toc1_item;



int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    FILE *fp;

    fp=fopen("notmain.header","rb");
    if(fp==NULL) return(1);
	rb=fread(&toc1_head,1,sizeof(toc1_head),fp);
	printf("%u bytes read\n",rb);

    printf("name[16]    [%s]\n",toc1_head.name    );   //user can modify
    printf("magic       0x%08X\n",toc1_head.magic       );   //must equal TOC_unsigned int_MAGIC
    printf("add_sum     0x%08X\n",toc1_head.add_sum     );
    printf("serial_num  0x%08X\n",toc1_head.serial_num  );   //user can modify
    printf("status      0x%08X\n",toc1_head.status      );   //user can modify,such as TOC_MAIN_INFO_STATUS_ENCRYP_NOT_USED
    printf("items_nr    0x%08X\n",toc1_head.items_nr    );  //total entry number
    printf("valid_len   0x%08X\n",toc1_head.valid_len   );
    printf("version_mai 0x%08X\n",toc1_head.version_main);  //only one byte
    printf("version_sub 0x%08X\n",toc1_head.version_sub );   //two bytes
    printf("reserved[0] 0x%08X\n",toc1_head.reserved[0] );   //reserved for future
    printf("reserved[1] 0x%08X\n",toc1_head.reserved[1] );   //reserved for future
    printf("reserved[2] 0x%08X\n",toc1_head.reserved[2] );   //reserved for future
    printf("end         0x%08X\n",toc1_head.end         );

//name[16]    []
//magic       0x89119800
//add_sum     0x053A85AD
//serial_num  0x00000000
//status      0x00000000
//items_nr    0x00000003
//valid_len   0x000F3A00
//version_mai 0x00000000
//version_sub 0x00000000
//reserved[0] 0x00000000
//reserved[1] 0x00000000
//reserved[2] 0x00000000
//end         0x3B45494D


	for(ra=0;ra<toc1_head.items_nr;ra++)
	{
		rb=fread(&toc1_item,1,sizeof(toc1_item),fp);
		printf("%u bytes read\n",rb);

		printf("\n");
		printf("name        %s\n",    toc1_item.name          );      //such as ITEM_NAME_SBROMSW_CERTIF
		printf("data_offset 0x%08X\n",toc1_item.data_offset  );
		printf("data_len    0x%08X\n",toc1_item.data_len     );
		printf("encrypt     0x%08X\n",toc1_item.encrypt      );      //0: no aes   //1: aes
		printf("type        0x%08X\n",toc1_item.type         );      //0: normal file, dont care  1: key certif  2: sign certif 3: bin file
		printf("run_addr    0x%08X\n",toc1_item.run_addr     );      //if it is a bin file, then run on this address                  ; if not, it should be 0
		printf("index       0x%08X\n",toc1_item.index        );      //if it is a bin file, this value shows the index to run                  ; if not
		printf("end         0x%08X\n",toc1_item.end          );
		printf("\n");
	}






	return(0);
}






