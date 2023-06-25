#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <conio.h>
#include<time.h>
#include<windows.h>



//EB
struct EB{
    int point,num;
} __attribute__((packed));
//Single Cell
struct cell{
    char name[21];
    int energy, x, y;
    struct cell* next;
    struct cell* preview;
} __attribute__((packed));
//the first cell
struct cell* firstCell(int n,char name[21],int mapC[]){
    struct cell* fCell = (struct cell*) malloc (sizeof(struct cell));
    strcpy(fCell->name, name);
    fCell->energy = 0;
    fCell->preview = NULL;
    fCell->next = NULL;
    fCell->x = fCell->y = n;
    if(fCell-> x != n )
         mapC[fCell->y * n + fCell->x] = 1;
    return fCell;
}
//when a Cell is born
void cellBirth (int n,struct cell** headptr, char name[21],int mapC[]){
    struct cell* new_cell = (struct cell*) malloc(sizeof(struct cell));
    if(new_cell == NULL){
        printf("memory error\n");
        return;
    }
    struct cell* last =  *headptr;
    new_cell->next = NULL;
    while(last->next != NULL)
        last = last->next;
    last->next = new_cell;
    new_cell->preview = last;
    new_cell->x = new_cell->y =n ;
    strcpy(new_cell->name,name);
    new_cell->energy = 0;
    if(new_cell->x != n)
        mapC[new_cell->y * n + new_cell->x] = 1;
}
//when  a cell dies;
void deleteCell (int n,struct cell** headptr, char name[21],int mapC[]){
    struct cell* temp = *headptr, *prev;
    if(temp != NULL && temp->name == name){
        *headptr = temp->next;
        mapC[temp->y * n + temp->x] = 0;
        free(temp);
        return;
    }
    while(temp != NULL && temp->name != name){
        prev = temp;
        temp = temp->next;
    }
    if(temp ==NULL) return;
    prev->next = temp->next;
    free(temp);
}
void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
//print_map
void print_map(int n, char map[],int mapC[],int EB_num,struct EB EBP[]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i % 2 == 1)
                printf("    ");
            if(mapC[i* n + j] == 1) {
                SetColor(11);
                printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
                SetColor(15);
            }
            else if(map[i * n + j] == '3'){
                SetColor(4);
                printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
                SetColor(15);
            }
            else if(map[i * n + j ] == '1'){
                int m;
                for(int k = 0; k < EB_num; k++)
                    if(EBP[k].num == i * n + j) {
                        m = k;
                        k = EB_num;
                    }
                if(EBP[m].point > 50){
                    SetColor(10);
                    printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
                    SetColor(15);
                    }
                else if(EBP[m].point < 20){
                    SetColor(14);
                    printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
                    SetColor(15);
                }
                else{
                    SetColor(13);
                    printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
                    SetColor(15);
                }
            }
            else
                printf(" (%d, %d) : %c\t", j, i, map[i * n + j]);
        }
        printf("\n");
        printf("\n");
    }
}
int  check_pose(struct cell* head,struct cell* cell,int n,char map[],int mapC[]){
    struct cell* temp = head;
    if(map[cell->y* n  + cell->x ] == '3' || cell->x < 0 || cell->y < 0  || cell->x > n || cell->y > n )
        return 0;
         if(mapC[cell->y * n + cell->x] == 1)
            return 0;
    return 1;
}
void rand_pos(int n,struct cell** headPtr, char map[],int mapC[]){
    struct cell* temp = *headPtr;
    temp = *headPtr;
    while(temp != NULL) {
        do {
            srand(time(0));
            temp->x = rand() % n;
            temp->y = rand() % n;
        } while (!check_pose( *headPtr,temp,n,map,mapC));
        mapC[temp->y * n + temp->x] = 1;
        temp = temp->next;
    }
}
//first menu
void first_menu(){
        printf("[1]Load\n");
        printf("[2]New single player game\n");
        printf("[3]New Multiplayer game\n");
        printf("[4]exit\n");
}
void s_menu(struct cell* head) {
    printf("[1]Move\n");
    printf("[2]Split a cell\n");
    printf("[3]Boost energy\n");
    printf("[4]Save\n");
    printf("[5]Exit\n");
}
void print_cell(struct cell* head,int n,char map[]){
    struct cell* temp = head;
    int i = 1;
    while(temp != NULL){
        printf("[%d]%s : (%d,%d)/ %c\n",i, temp->name,temp->x,temp->y,map[n * temp->y + temp->x]);
        temp = temp->next;
        i++;
    }
}
void move(int mapN,char map[],struct cell** headPtr, int m,int d,int mapC[]){
    struct cell* temp = *headPtr;
    struct cell dmy = *temp;
    for(int i = 1; i < m; i++)
        temp = temp->next;
    dmy.x = temp->x;
    dmy.y = temp->y;
    if(d == 1 || d == 3 || d == 4)
        (dmy.y)--;
    else if(d == 2)
        (dmy.y)++;
    if( d == 3 || d == 5)
        (dmy.x)++;
    else if(d == 4 || d == 6)
        (dmy.x)--;
    if(!check_pose(*headPtr,&dmy,mapN,map,mapC)) {
        printf("invalid move\n");
        return;
    }
    else {
        mapC[temp->y * mapN + temp->x] = 0;
        temp->x = dmy.x;
        temp->y = dmy.y;
        mapC[temp->y * mapN + temp->x] = 1;
    }
}
void BE(int n,char map[],struct cell** headPtr, int cell_num,int EB_num, struct EB EBP[EB_num]){
    struct cell* temp = *headPtr;
    int j;
    for(int i = 1; i < cell_num; i++)
        temp = temp->next;
    if(map[temp->y * n + temp->x ] != '1'){
        printf("you are not in energy block\n");
        return;
    }

    for(int i = 0; i < EB_num; i++) {
        if (EBP[i].num ==  temp->y* n + temp->x){
            j = i;
            break;
        }
    }
    if(EBP[j].point < 15){
        if(temp->energy + EBP[j].point > 100) {
            printf("You have already reached 100 point\n");
            return;
        }
        temp->energy += EBP[j].point;
        EBP[j].point = 0;
    }
    else {
        if(temp->energy + 15 > 100){
            printf("You have already reached 100 point\n");
            return;
        }
        temp->energy +=15;
        EBP[j].point -= 15;
    }
}
void split(int n,char map[],struct cell** headPtr,int cell_num,int mapC[]){
    struct cell* temp = *headPtr, dmy,*ncell;
    int x,y;
    char name1[21],name2[21];
    for(int i = 1; i < cell_num; i++)
        temp = temp->next;
    if(map[temp->y * n + temp->x] != '2') {
        printf("cell is not in split block :3\n");
        return;
    }
    if(temp->energy < 80 ){
        printf("cell doensn't have enough energy\n");
        return;
    }
    strcpy(name1,temp->name);
    char temps[20] = "'s child";
    strncat(name1,temps,sizeof(temps));
    cellBirth(n,headPtr,name1,mapC);
    temp->energy = 40;
    ncell = temp;
    while(ncell->next != NULL)
        ncell = ncell->next;
    ncell->energy = 40;
    ncell->x = temp->x;
    ncell->y = temp->y;
    do{

        dmy = *ncell;
        srand(time(0));
        x = rand() % 3 - 1;
        y = rand() % 3 - 1;
        dmy.x += x;
        dmy.y += y;
    }while(!check_pose(*headPtr,&dmy,n,map,mapC));
    mapC[ncell->y * n + ncell->x] = 0;
    ncell->x = dmy.x;
    ncell->y = dmy.y;
    mapC[ncell->y * n + ncell->x] = 1;
    do{
        dmy = *temp;
        srand(time(0));
        x = rand() % 3 - 1;
        y = rand() % 3 - 1;
        dmy.x += x;
        dmy.y += y;
    } while(!check_pose(*headPtr,&dmy,n,map,mapC));
    temp->x = dmy.x;
    temp->y = dmy.y;
    printf("what is the name of new cell no.2? : \n");
    fgets(name2,20,stdin);
    cellBirth(n,headPtr,name2,mapC);
    ncell->next->energy = 40;
    ncell->next->x = temp->x;
    ncell->next->y = temp->y;
    deleteCell(n,headPtr,temp->name,mapC);
    mapC[ncell->next->y * n + ncell->next->x] = 1;
}
void save(struct cell* head1,struct cell* head2,int EB_num,struct EB EBP[EB_num]){
    //printf("Start of Save\n");
    FILE *fptr = fopen("SaveP1.bin","wb");
    struct cell *temp = head1;
    int n1 = 0;
    while(temp != NULL){
        temp = temp->next;
        n1++;
    }
    temp =  head1;
    fwrite(&n1,sizeof(int),1,fptr);
    while(temp != NULL){
        fwrite(temp,sizeof(struct cell),1,fptr);
        temp = temp->next;
    }
    for(int i = 0; i < EB_num; i++) {
        fwrite(&EBP[i], sizeof(struct EB), 1, fptr);
    }
    fclose(fptr);   //end of saving P1
    if(head2 != NULL){
        FILE* fptr2 = fopen("SaveP2.bin","wb");
        struct cell *temp2 = head2;
        int n2 = 0;
        while(temp2 != NULL){
            temp2 = temp2->next;
            n2++;
        }
        temp2 = head2;
        fwrite(&n2,sizeof(int),1,fptr2);
        while(temp2 != NULL){
            printf(" %s is saved\n",temp2->name);
            fwrite(temp2,sizeof(struct cell),1,fptr2);
            temp2 = temp2->next;
        }
        for(int i = 0; i < EB_num; i++) {
            fwrite(&EBP[i], sizeof(struct EB), 1, fptr2);
        }
        fclose(fptr2);
    }
    //printf("End of Save\n");
}
/*void undo(struct cell**headPtr,int n,int mapC[]){
    int n1;
    struct cell *temp1 = (struct cell*) malloc(sizeof(struct cell)),*curr1;
    FILE* fptr1;
    fptr1 = fopen("SaveP1.bin","rb");
    fread(&n1,sizeof(int),1,fptr1);
    fread(temp1,sizeof(struct cell),1,fptr1);
    (*headPtr) = firstCell(n,temp1->name,mapC);
    (*headPtr)->x = temp1->x;
    (*headPtr)->y = temp1->y;
    (*headPtr)->energy = temp1->energy;
    curr1 = *headPtr;
    for(int i = 0; i < n1 - 1; i++){
        fread(temp1,sizeof(struct cell),1,fptr1);
        cellBirth(n,headPtr,temp1->name,mapC);
        curr1 = curr1->next;
        curr1->energy = temp1->energy;
        curr1->x = temp1->x;
        curr1->y = temp1->y;
    }
    free(temp1);
} */
void deleteList(struct cell** headPtr){
    struct cell* current = *headPtr;
    struct cell* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    *headPtr = NULL;
}
void play(int mapN,struct cell** head1Ptr,struct cell** head2Ptr,char map[mapN * mapN],int EB_num,struct EB EBP[EB_num],int limit,int mapC[]){
    if(*head2Ptr == NULL){
        int cell_num,o;
        while(limit > 0) {
            printf("%d move is left\n", limit);
            print_map(mapN, map, mapC, EB_num, EBP);
            print_cell(*head1Ptr, mapN, map);
            scanf("%d", &cell_num);
            s_menu(*head1Ptr);
            scanf("%d", &o);
            switch (o) {
                case (1) : {
                    int d;
                    printf("[1]North\n");
                    printf("[2]South\n");
                    printf("[3]Northeast\n");
                    printf("[4]Northwest\n");
                    printf("[5]Southeast\n");
                    printf("[6]Southwest\n");
                    scanf("%d", &d);
                    move(mapN, map, head1Ptr, cell_num, d, mapC);
                    break;
                }
                case (2) :
                    split(mapN, map, head1Ptr, cell_num, mapC);
                    break;
                case (3) : {
                    BE(mapN, map, head1Ptr, cell_num, EB_num, EBP);
                    //print point
                    struct cell *temp = *head1Ptr;
                    while (temp != NULL) {
                        printf("%s point : %d\n", temp->name, temp->energy);
                        temp = temp->next;
                    }
                    break;
                }
                case (4) : {
                    save(*head1Ptr, *head2Ptr, EB_num, EBP);
                    break;
                }
                case (5) : {
                    limit = 0;
                    break;
                }
                default: {
                    printf("invalid num\n");
                    break;
                }
            }
            limit--;
            save(*head1Ptr, *head2Ptr, EB_num, EBP);
        }
    } //end of single player
    else{
        int cell_num,o,PT;
        PT = limit;
        while(limit > 0){
            if(limit % 2 == PT % 2) { // its P1 Turn
                print_map(mapN, map,mapC,EB_num,EBP);
                print_cell(*head1Ptr, mapN, map);
                scanf("%d", &cell_num);
                s_menu(*head1Ptr);
                scanf("%d", &o);
                switch (o) {
                    case (1) : {
                        int d;
                        printf("[1]North\n");
                        printf("[2]South\n");
                        printf("[3]Northeast\n");
                        printf("[4]Northwest\n");
                        printf("[5]Southeast\n");
                        printf("[6]Southwest\n");
                        scanf("%d", &d);
                        move(mapN, map, head1Ptr, cell_num, d, mapC);
                        break;
                    }
                    case (2) :
                        split(mapN, map, head1Ptr, cell_num, mapC);
                        break;
                    case (3) : {
                        BE(mapN, map, head1Ptr, cell_num, EB_num, EBP);
                        //print point
                        struct cell *temp = *head1Ptr;
                        while (temp != NULL) {
                            printf("%s point : %d\n", temp->name, temp->energy);
                            temp = temp->next;
                        }
                        break;
                    }
                    case (4) : {
                        save(*head1Ptr, *head2Ptr, EB_num, EBP);
                        printf("start of save_test\n");
                        FILE *fptr = fopen("SaveP1.bin", "rb");
                        if (fptr == NULL)
                            printf("Error in save_test\n");
                        int n1;
                        fread(&n1, sizeof(int), 1, fptr);
                        printf("cell_num in save_test : %d \n", n1);
                        fclose(fptr);
                        break;
                    }
                    case (5) : {
                        limit = 0;
                        break;
                    }
                    default: {
                        printf("invalid num\n");
                        break;
                    }
                }
                limit--;
            }
            else{ //its P2's Turn
                print_map(mapN, map,mapC,EB_num,EBP);
                print_cell(*head2Ptr, mapN, map);
                scanf("%d", &cell_num);
                s_menu(*head1Ptr);
                scanf("%d", &o);
                switch (o) {
                    case (1) : {
                        int d;
                        printf("[1]North\n");
                        printf("[2]South\n");
                        printf("[3]Northeast\n");
                        printf("[4]Northwest\n");
                        printf("[5]Southeast\n");
                        printf("[6]Southwest\n");
                        scanf("%d", &d);
                        move(mapN, map, head2Ptr, cell_num, d, mapC);
                        break;
                    }
                    case (2) :
                        split(mapN, map, head2Ptr, cell_num, mapC);
                        break;
                    case (3) : {
                        BE(mapN, map, head2Ptr, cell_num, EB_num, EBP);
                        //print point
                        struct cell *temp = *head2Ptr;
                        while (temp != NULL) {
                            printf("%s point : %d\n", temp->name, temp->energy);
                            temp = temp->next;
                        }
                        break;
                    }
                    case (4) : {
                        save(*head1Ptr, *head2Ptr, EB_num, EBP);
                        printf("start of save_test\n");
                        FILE *fptr = fopen("SaveP1.bin", "rb");
                        if (fptr == NULL)
                            printf("Error in save_test\n");
                        int n1;
                        fread(&n1, sizeof(int), 1, fptr);
                        printf("cell_num in save_test : %d \n", n1);
                        fclose(fptr);
                        break;
                    }
                    case (5) : {
                        limit = 0;
                        break;
                    }
                    default: {
                        printf("invalid num\n");
                        break;
                    }
                }
                save(*head1Ptr,*head2Ptr,EB_num,EBP);
            }
        }
        }
    }
int main() {
    int n, Cell_num,mp,c,EB_num = 0,EB_pose[64],limit =  100;
    printf("Enter Limit : \n");
    scanf("%d", &limit);
    struct cell* p2_cells = NULL, *p1_cells = NULL;
    FILE * mapPtr = fopen("map6.bin","r");
    fread(&n, sizeof(unsigned int),1,mapPtr);
    char map[n * n];
    int mapC[n * n];
    for(int i = 0; i < n*n ; i++)
        mapC[i] = 0;
    for(int i = 0; i < n* n; i++ ) {
        static int j = 0;
        fread(&map[i], sizeof(char), 1, mapPtr);
        if (map[i] == '1'){
            EB_num++;
            EB_pose[j] = i;
            j++;
            }
    }
    fclose(mapPtr);
    struct EB EBP[EB_num];  //energy block poits
    for(int i = 0, j = 0; i < EB_num; i++,j++){
        EBP[i].point = 100;
        EBP[i].num = EB_pose[j];
    }
    print_map(n, map,mapC,EB_num,EBP);
    first_menu();
    scanf("%d", &c);
    switch(c) {
        case (1) : {
            printf("Begining of the Load\n");
            int n1,n2;
            struct cell *temp1 = (struct cell*) malloc(sizeof(struct cell)),*temp2 =  (struct cell*) malloc(sizeof(struct cell)),*curr1, *curr2;
            FILE* fptr1,*fptr2;
            fptr1 = fopen("SaveP1.bin","rb");
            fptr2 = fopen("SaveP2.bin","rb");
            if(fptr1 == NULL ) {
                perror("Error");
                printf("Couldn't open the file\n");
                return -1;
            }
            if(fptr2 != NULL){ // Loading P2
                fread(&n2,sizeof(int),1,fptr2);
                fread(temp2,sizeof(struct cell),1,fptr2);
                p2_cells = firstCell(n,temp2->name,mapC);
                p2_cells->x = temp2->x;
                p2_cells->y = temp2->y;
                p2_cells->energy = temp2->energy;
                curr2 = p2_cells;
                for(int i = 0; i < n2 - 1; i++){
                    fread(temp2,sizeof(struct cell),1,fptr2);
                    cellBirth(n,&p2_cells,temp2->name,mapC);
                    curr2 = curr2->next;
                    curr2->energy = temp2->energy;
                    curr2->x = temp2->x;
                    curr2->y = temp2->y;
                }
                free(temp2);
            }
            fread(&n1,sizeof(int),1,fptr1);
            fread(temp1,sizeof(struct cell),1,fptr1);
            p1_cells = firstCell(n,temp1->name,mapC);
            p1_cells->x = temp1->x;
            p1_cells->y = temp1->y;
            p1_cells->energy = temp1->energy;
            curr1 = p1_cells;
            for(int i = 0; i < n1 - 1; i++){
                fread(temp1,sizeof(struct cell),1,fptr1);
                cellBirth(n,&p1_cells,temp1->name,mapC);
                curr1 = curr1->next;
                curr1->energy = temp1->energy;
                curr1->x = temp1->x;
                curr1->y = temp1->y;
            }
            free(temp1);
            printf("End of Loading Cells\n");
            for(int i = 0; i < EB_num; i++)
                fread(&EBP[i],sizeof(struct EB),1,fptr1);
            fclose(fptr1);
            fclose(fptr2);
            printf("End of Load\n");
            play(n,&p1_cells,&p2_cells,map,EB_num,EBP,limit,mapC);
            break;
        }
        case (2) : {
            print_map(n, map,mapC,EB_num,EBP);
            int n1;
            char name[21];
            printf("Enter the numer of Cell : ");
            scanf("%d", &n1);
            printf("and the name? : ");
            getchar();
            fgets(name,20,stdin);
            p1_cells = firstCell(n1,name,mapC);
            for(int i = 0; i < n1-1;i++){
                struct cell* temp = p1_cells->next;
                printf("Enter the name you wish : ");
                fgets(name,20,stdin);
                cellBirth(n,&p1_cells,name,mapC);
            }
            rand_pos(n,&p1_cells,map,mapC);
                play(n,&p1_cells,&p2_cells,map,EB_num,EBP,limit,mapC);
                print_cell(p1_cells,n,map);
            break;
        }
        case (3) : {
            print_map(n, map,mapC,EB_num,EBP);
            int n1,n2;
            char name1[21], name2[21];
            printf("P1 : \n");
            printf("Enter the numer of Cell : ");
            scanf("%d", &n1);
            printf("and the name? : ");
            getchar();
            fgets(name1,20,stdin);
            p1_cells = firstCell(n1,name1,mapC);
            for(int i = 0; i < n1-1;i++){
                struct cell* temp = p1_cells->next;
                printf("Enter the name you wish : ");
                fgets(name1,20,stdin);
                cellBirth(n,&p1_cells,name1,mapC);
            }
            rand_pos(n,&p1_cells,map,mapC);
            printf("P2 : \n");
            printf("Enter the numer of Cell : ");
            scanf("%d", &n2);
            printf("and the name? : ");
            getchar();
            fgets(name2,20,stdin);
            p2_cells = firstCell(n2,name2,mapC);
            for(int i = 0; i < n2-1;i++){
                struct cell* temp = p2_cells->next;
                printf("Enter the name you wish : ");
                fgets(name2,20,stdin);
                cellBirth(n,&p2_cells,name2,mapC);
            }
            rand_pos(n,&p2_cells,map,mapC);
            play(n,&p1_cells,&p2_cells,map,EB_num,EBP,limit,mapC);
            break;
        }
        case (4) :
            break;
        default : {
            printf("The number is not in the rangr,Please Enter a number between 1 to 4 \n");
            break;
        }
    }
    int score1 = 0,score2 = 0;
    struct cell* temp1 = p1_cells;
    struct cell* temp2 = p2_cells;
    while(temp1 != NULL){
        score1 += temp1->energy;
        temp1 = temp1->next;
    }
    while(temp2 != NULL){
        score2 += temp2->energy;
        temp2 = temp2->next;
    }
    printf("score P1 : %d \n", score1);
    printf("score P2 : %d \n", score2);
    deleteList(&p2_cells);
    deleteList(&p1_cells);
    return 0;
}