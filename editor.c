#include <stdio.h>
#include<string.h>


void print_map(int n, char map[]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i % 2 == 1)
                printf("    ");
            printf("(%d, %d) %c", j, i, map[i * n + j]);
        }
        printf("\n");
        printf("\n");
    }
}
int main() {
    int n,x,y,check = 1;
    char type;
    FILE * fptr = fopen("map6.bin","rb");
    fread(&n,sizeof(unsigned int),1,fptr);
    char map[n * n];
    for(int i = 0; i < n * n; i++)
        fread(&map[i],sizeof(char),1,fptr);
    fclose(fptr);
    print_map(n,map);
    while(check){
        printf("Enter x and y of the block and the type\n ");
        scanf(" %d %d %c", &x,&y,&type);
        map[y * n + x] = type;
        print_map(n,map);
        printf("continue? 1)yes 0) no\n");
        scanf(" %d", &check);
    }
    fptr = fopen("map6.bin","wb");
    fwrite(&n,sizeof(unsigned int), 1, fptr);
    for(int i = 0; i < n * n; i++ ){
        fwrite(&map[i],sizeof(char),1,fptr);
    }
    fclose(fptr);
    return 0;
}