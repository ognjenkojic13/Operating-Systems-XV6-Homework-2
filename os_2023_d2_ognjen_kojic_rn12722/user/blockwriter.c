#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fcntl.h"

char karakteri[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

void makeFile(int brSectors, char *name){
    char buf[512];
    int fd, i, sectors;
    fd = open(name, O_CREATE | O_WRONLY);
        if(fd < 0){
            printf("Nije moguce otvoriti fajl za pisanje\n");
            exit();
        }
        sectors = 0;
        int ind = 0;
        while(sectors < brSectors){
            *(int*)buf = sectors;
            int cc;
            for(int i = 0; i < sizeof(buf)/sizeof(char); i++){
                if(ind > 25)
                    ind = 0;
                *(int*) buf = karakteri[ind++];
                cc = write(fd, buf, sizeof(char));
            }
            printf("Upisivanje u : %d\n", sectors);
            if(cc <= 0)
                break;
            sectors++;
        }

        close(fd);

        fd = open(name, O_RDONLY);
        if(fd < 0){
            printf("Ne mogu da otvorim ponovo %s.file za citanje\n",name);
            exit();
        }
        for(i = 0; i < sectors; i++){
            int cc = read(fd, buf, sizeof(buf));
            if(cc <= 0){
                printf("greska citanja u sektoru %d\n", name,i);
                exit();
            }
        }
        printf("gotovo; ok\n");
        exit();
}

void help(){
    printf("Koristite ovaj program da napravite veliku datoteku ispunjenu znakovima a-z.\n");
	printf("Podrazumevano ime fajla: long.txt\n");
	printf("Podrazumevani blokovi: 150\n");
	printf("Upotreba blockwritera[OPTION]...\n");
    printf("\n");
	printf("Opcije komandne linije:\n");
    printf("\t\t -h, --help: Pokazati pomoc meni.\n");
    printf("\t\t -b, --blocks: Broj blokova za pisanje.\n");
	printf("\t\t -o, --output-file: Podesi naziv izlazne datoteke\n");
    exit();
}

int
main(int argc, char *argv[])
{
    int sectors1 = 150;
    char *name = "long.txt";
    if(argc < 2){
        makeFile(150, "long.txt");
        exit();
    }

    if(argc > 5){
        printf("Previse poziva\n");
        help();
    }

	for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
		    help();
	    } else if(strcmp(argv[i-1], "-b") == 0 || strcmp(argv[i-1], "--blocks") == 0){
            char c[15];
            strcpy(c, argv[i]);
            int j = 0;
            while(c[j] != '\0'){
                if(c[j] < '0' || c[j] > '9'){
                    printf("Unos mora biti broj.\n");
                    exit();
                }
                j++;
            }
            sectors1 = atoi(argv[i]);

	    } else if(strcmp(argv[i-1], "-o") == 0 || strcmp(argv[i-1], "--output-file") == 0){
            strcpy(name, argv[i]);
	    }
    }

    makeFile(sectors1, name);

    exit();
}