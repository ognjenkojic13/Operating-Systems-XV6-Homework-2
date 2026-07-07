#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void help(){
    printf("Upotreba: encr [filename] [options]\n");
    printf("Sifruje jednu ili vise datoteka koristeci encr sistemski poziv.\n");
    printf("Ako ime fajla nije navedeno, prikazite meni pomoci.\n\n");
    printf("Opcije:\n");
    printf("   -h, --help          prikazuje help meni\n");
    printf("  -a, --encrypt-all   sifrujte sve nesifrovane fajlove u trenutnom direktorijumu\n");
    exit();
}

char*
fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;

	for(p=path+strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	if(strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	return buf;
}

void prepare(char *filename){
    int fd;
    fd = open(filename, O_RDWR);
    int e = encr(fd);
    if(e == 0)
        printf("Uspesno sifrovan fajl: %s\n", filename);
    else
        printf("Neuspesno sifrovan fajl: %s\n", filename);
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc < 2){
        help();
    }
	for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help();
        } else if (strcmp(argv[i], "--encrypt-all") == 0 || strcmp(argv[i], "-a") == 0) {
            char buf[512], *p;
	        int fd;
	        struct dirent de;
            
            fd = open(".", 0);

            strcpy(buf, ".");
		    p = buf+strlen(buf);
		    *p++ = '/';
		    while(read(fd, &de, sizeof(de)) == sizeof(de)){
			    if(de.inum == 0)
				    continue;
                
			    memmove(p, de.name, DIRSIZ);
			    p[DIRSIZ] = 0;
                prepare(fmtname(buf));
            }

         } else {
            prepare(argv[i]);
         }
    }

	exit();
}