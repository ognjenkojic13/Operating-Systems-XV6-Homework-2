#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void secret(){
    char key[10];
    printf("Unesi kljuc: ");
    setecho(0);
    int n = read(0, &key, sizeof(key)-1);
    setecho(1);
    key[n] = '\0';
    for(int i = 0; i < n-1; i++){
        if(key[i] < '0' || key[i] > '9'){
            printf("Kljuc mora biti broj.\n");
            exit();
        }
    }
    setkey(atoi(key));
}

void help(){
    printf("Postavlja sistemski kljuc.\n");
	printf("\n");
	printf("Komanda linija opcije:\n");
	printf("\t\t -h, --help: meni za pomoc.\n");
	printf("\t\t -s, --secret: uzima kljuc iz STDIN-a i sakriva ga pomoccu setecho-a.\n");
    exit();
}

int
main(int argc, char *argv[])
{   
    if(argc < 2){
        help();
    }
	for(int i = 1; i < argc; i++){
       if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
		    help();
	    } else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--secret") == 0){
            secret();
	    } else{
            char key[10];
            strcpy(key, argv[i]);
            int j = 0;
            while(key[j] != '\0'){
                if(key[j] < '0' || key[j] > '9'){
                    printf("Kljuc mora biti broj.\n");
                    exit();
                }
                j++;
            }
            setkey(atoi(argv[i]));
        }
    }
	exit();
}