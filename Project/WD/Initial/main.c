#include "stdio.h"
#include "SymTab.h"
#include "IOMngr.h"

extern int yyparse();

struct SymTab *table;

int main (int argc, char *argv[]) {
    table = CreateSymTab(17);
    if (!OpenFiles(argv[1], argv[2])) {
        printf("open failed\n");
        return 0;
    }
    yyparse();


//    char buf[1024] = "";
//    FILE *file = fopen("source.txt", "r");
//    fgets(buf, 1024, file);
//    printf("%s", buf);
//    fclose(file);
//    printf("haha");
    return 0;
}
