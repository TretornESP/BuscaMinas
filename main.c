#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <assert.h>


int playermove(char hidden_matrix[100][100], int x, int y, int k, int bomb, clock_t t_inicio);
void gen_map(char matrix[100][100], int x, int y, int bomb);
void draw(char matr[100][100], int x, int y);
void comparar(char matrix[100][100], char hidden_matrix[100][100], int coord_x, int coord_y, int x, int y, clock_t t_inicio);
void gameover(int x, int y, int win, clock_t t_inicio);
int randint(int n);
void explosion(char matrix[100][100], char hidden_matrix[100][100], int coord_x, int coord_y, int x, int y); // NO TOCAR ESTA FUNCION ES SATANAS

char matrix[100][100];
char hidden_matrix[100][100];

int main()
{
    system("title Buscaminas 1.0 by Tretorn");

    srand(time(NULL));

    int i, j, x, y, bomb, opt;
    int k=0;
    clock_t inicio;

    size_select:

    system("cls");
    printf("\n      [Seleccione tamano del mapa]\n\n");
    printf("+--------------------------------------+\n");
    printf("|  PEQUENO: 8x8 10 minas OPCION: 1     |\n");
    printf("+--------------------------------------+\n");
    printf("|  MEDIANO: 16x16 40 minas OPCION: 2   |\n");
    printf("+--------------------------------------+\n");
    printf("|  GRANDE: 16x30 99 minas OPCION 3     |\n");
    printf("+--------------------------------------+\n");
    printf("|  PERSONALIZADO: MAX 100x100 OPCION 4 |\n");
    printf("+--------------------------------------+\n\n");
    printf("Introduzca opcion: ");
    fflush(stdin);
    scanf("%d", &opt);
    switch (opt){
    case 1:
        x=8;
        y=8;
        bomb=10;
        goto matrix_gen;
    case 2:
        x=16;
        y=16;
        bomb=40;
        goto matrix_gen;
    case 3:
        x=16;
        y=30;
        bomb=99;
        goto matrix_gen;
    case 4:
        goto input_1;
    default:
        printf("Opcion no valida (1-4)\n");
        system("pause");
        goto size_select;
    }

    input_1:

    system("cls");
    printf("X: \n");
    fflush(stdin);
    scanf("%d", &x);
    printf("Y: \n");
    fflush(stdin);
    scanf("%d", &y);
    printf("Bomb: \n");
    fflush(stdin);
    scanf("%d", &bomb);

    if (x < 0 || y < 0 || x > 100 || y > 100){
        printf("Tamaño inválido, tamaño max 100x100\n");
        goto input_1;
    }

    matrix_gen:

    for (i=0; i < 100; i++){
        for (j=0; j < 100; j++){
            matrix[i][j] = '\0';
            hidden_matrix[i][j] = '\0';
        }
    }

    gen_map(matrix, x, y, bomb);

    //INICIO RELOJ
    inicio = clock();

    GAMELOOP:

    for (i=0; i<x; i++){
        for (j=0; j<y; j++){
            if (hidden_matrix[i][j]=='b' || matrix[i][j]=='*') {
                continue;
            } else {
                if (k<=bomb){
                    if (hidden_matrix[i][j]!=matrix[i][j]){
                        goto nowin;
                    }
                }
            }

        }
    }
    gameover(x, y, 1, inicio);
    nowin:

    draw(hidden_matrix, x, y);
    k = playermove(hidden_matrix, x, y, k, bomb, inicio);

    goto GAMELOOP;

    return 0;
}

int playermove(char hidden_matrix[100][100], int x,int y, int k, int bomb, clock_t t_inicio){

    int coord=0;
    char opt;

    input_2:

    printf("Introduzca coordenada x: ");
    fflush(stdin);
    scanf("%d", &coord);
    int coord_y = coord;
    printf("Introduzca coordenada y: ");
    fflush(stdin);
    scanf("%d", &coord);
    int coord_x = coord;

    if (coord_x < 0 || coord_x >= x || coord_y < 0 || coord_y >= y){
        printf("coordenada invalida, introduzca una en el rango [(0-%d), (0-%d)]\n", x-1, y-1);
        goto input_2;
    }

    input_3:

    printf("Bombas restantes: %d\n", bomb-k);
    printf("Abrir casila (o) / marcar bomba (b) / desmarcar bomba (u): ");
    fflush(stdin);
    scanf("%c", &opt);

    switch (opt){
    case 'o':
        comparar(matrix, hidden_matrix, coord_x, coord_y, x, y, t_inicio);
        return k;
        break;
    case 'b':
        hidden_matrix[coord_x][coord_y]='b';
        k++;
        return k;
        break;
    case 'u':
        if (hidden_matrix[coord_x][coord_y]=='b'){
            hidden_matrix[coord_x][coord_y]=0;
            k--;
            return k;
        } else {
            printf("No existe bandera en la casilla\n");
            goto input_3;
        }
        break;
    default:
        printf("opción no válida (o/b/u)\n");
        goto input_3;
    }

    return k;
}

void gen_map(char matrix[100][100], int x, int y,int bomb)
{

    int i, j, x_rnd, y_rnd;

    for (i=0; i < x; i++){
        for (j=0; j < y; j++){
            matrix[i][j]=0;
        }

    }

    for (i=0; i < bomb; i++) {

        x_rnd = randint(x);
        y_rnd = randint(y);

        printf("loading %d/%d\n",i,bomb);

        if (matrix[x_rnd][y_rnd]!='*'){
            matrix[x_rnd][y_rnd]='*';
        } else {
            i--;
        }

    }

    for (i=0; i < y; i++){
        for (j=0; j < x; j++){
            if (matrix[i][j]=='*') {
                if (j==0){
                    if (i==0){
                        if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                        if (matrix[i+1][j+1]!='*') {matrix[i+1][j+1]++;}
                        if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                    } else if (i==y-1){
                        if (matrix[i-1][j+1]!='*') {matrix[i-1][j+1]++;}
                        if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                        if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                    }
                } else if (j==x-1){
                    if (i==0){
                        if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                        if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                        if (matrix[i+1][j-1]!='*') {matrix[i+1][j-1]++;}
                    } else if (i==y-1) {
                        if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                        if (matrix[i-1][j-1]!='*') {matrix[i-1][j-1]++;}
                        if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                    }
                } else if (j!=x-1 && j!=0 && i==0){
                    if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                    if (matrix[i+1][j+1]!='*') {matrix[i+1][j+1]++;}
                    if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                    if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                    if (matrix[i+1][j-1]!='*') {matrix[i+1][j-1]++;}
                } else if (j!=x-1 && j!=0 && i==y-1){
                    if (matrix[i-1][j+1]!='*') {matrix[i-1][j+1]++;}
                    if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                    if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                    if (matrix[i-1][j-1]!='*') {matrix[i-1][j-1]++;}
                    if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                } else if (i!=0 && j!=0 && i!=y-1 && j!=x-1){
                    if (matrix[i-1][j+1]!='*') {matrix[i-1][j+1]++;}
                    if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                    if (matrix[i+1][j+1]!='*') {matrix[i+1][j+1]++;}
                    if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                    if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                    if (matrix[i-1][j-1]!='*') {matrix[i-1][j-1]++;}
                    if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                    if (matrix[i+1][j-1]!='*') {matrix[i+1][j-1]++;}
                }
                if (i!=y-1 && i!=0 && j==x-1){
                    if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                    if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                    if (matrix[i-1][j-1]!='*') {matrix[i-1][j-1]++;}
                    if (matrix[i][j-1]!='*') {matrix[i][j-1]++;}
                    if (matrix[i+1][j-1]!='*') {matrix[i+1][j-1]++;}
                } else if (i!=0 && i!=y-1 && j==0){
                    if (matrix[i-1][j+1]!='*') {matrix[i-1][j+1]++;}
                    if (matrix[i][j+1]!='*') {matrix[i][j+1]++;}
                    if (matrix[i+1][j+1]!='*') {matrix[i+1][j+1]++;}
                    if (matrix[i-1][j]!='*') {matrix[i-1][j]++;}
                    if (matrix[i+1][j]!='*') {matrix[i+1][j]++;}
                }
            }
        }
    }
}

void draw(char matr[100][100], int x, int y)
{
    int i, j;

    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    printf("      ");
    for (i=0;i<x;i++){
        if (i<10) {
            printf("%d  ",i);
        } else {
            printf("%d ",i);
        }
    }
    printf("\n");
    for (i=0;i<y;i++){
        if (i<10){
            printf(" %d   ",i);
        } else {
            printf(" %d  ",i);
        }
        for (j=0;j<x;j++){
            switch (matr[i][j]){
            case '.':
                printf("[%c]",matr[i][j]);
                break;
            case '*':
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
                printf("[%c]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            case 'b':
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("[%c]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            case 0:
                printf("[%d]",matr[i][j]);
                break;
            case 1:
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                printf("[%d]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            case 2:
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("[%d]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            case 3:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf("[%d]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            case 4:
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("[%d]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            default:
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
                printf("[%d]",matr[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                break;
            }
        }
        printf("\n");
    }
}

int randint(int n)
{
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    assert (end > 0L);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

void comparar(char matrix[100][100],char hidden_matrix[100][100], int coord_x, int coord_y, int x, int y, clock_t t_inicio)
{
    int i,j;

    if (matrix[coord_x][coord_y]=='*'){
        gameover(x, y, 0, t_inicio);
    } else if (matrix[coord_x][coord_y]==0){
        explosion(matrix, hidden_matrix, coord_x, coord_y, x, y);
        for (i=0;i<x;i++){
            for (j=0;j<y;j++){
                if (matrix[i][j]=='n') {matrix[i][j]=0;}
            }
        }
    } else {
        hidden_matrix[coord_x][coord_y]=matrix[coord_x][coord_y];
    }
}

void gameover(int x, int y, int win, clock_t t_inicio)
{
    char opt;
    clock_t t_fin;
    t_fin = clock();
    double cps = CLOCKS_PER_SEC;
    double t_ejecucion = (t_fin-t_inicio)/cps;

    draw(matrix, x, y);

    input_4:
    switch (win){
    case 0:
        printf("Has perdido\n");
        printf("Tiempo: %.1lf \n", t_ejecucion);
        break;
    case 1:
        printf("Has ganado, enhorabuena\n");
        printf("Tiempo: %.1lf \n", t_ejecucion);
        break;
    }
    printf("Fin del juego\n");
    printf("Nueva partida? (s/n): ");
    fflush(stdin);
    scanf("%c", &opt);

    switch (opt){
    case 's': main();
    case 'n': exit(0);
    default:
        printf("Opcion no valida (s/n)\n");
        goto input_4;
    }
}

void explosion(char matrix[100][100], char hidden_matrix[100][100], int coord_x, int coord_y, int x, int y)
{

    hidden_matrix[coord_x][coord_y]=matrix[coord_x][coord_y];
    matrix[coord_x][coord_y]='n';

    if (coord_x-1>=0 && coord_x-1 < x && coord_y-1>=0 && coord_y-1<y){
        if (matrix[coord_x-1][coord_y-1]==0) {
            explosion(matrix, hidden_matrix, coord_x-1, coord_y-1, x, y);
        }
        if (matrix[coord_x-1][coord_y-1]!='n') {hidden_matrix[coord_x-1][coord_y-1]=matrix[coord_x-1][coord_y-1];}
    }
    if (coord_x>=0 && coord_x < x && coord_y-1>=0 && coord_y-1<y){
        if (matrix[coord_x][coord_y-1]==0){
            explosion(matrix, hidden_matrix, coord_x, coord_y-1, x, y);
        }
            if (matrix[coord_x][coord_y-1]!='n') {hidden_matrix[coord_x][coord_y-1]=matrix[coord_x][coord_y-1];}
    }
    if (coord_x+1>=0 && coord_x+1 < x && coord_y-1>=0 && coord_y-1<y){
        if (matrix[coord_x+1][coord_y-1]==0){
            explosion(matrix, hidden_matrix, coord_x+1, coord_y-1, x, y);
        }
            if (matrix[coord_x+1][coord_y-1]!='n') {hidden_matrix[coord_x+1][coord_y-1]=matrix[coord_x+1][coord_y-1];}
    }
    if (coord_x-1>=0 && coord_x-1 < x && coord_y>=0 && coord_y<y){
        if (matrix[coord_x-1][coord_y]==0){
            explosion(matrix, hidden_matrix, coord_x-1, coord_y, x, y);
        }
            if (matrix[coord_x-1][coord_y]!='n') {hidden_matrix[coord_x-1][coord_y]=matrix[coord_x-1][coord_y];}
    }
    if (coord_x+1>=0 && coord_x+1 < x && coord_y>=0 && coord_y<y){
        if (matrix[coord_x+1][coord_y]==0){
            explosion(matrix, hidden_matrix, coord_x+1, coord_y, x, y);
        }
            if (matrix[coord_x+1][coord_y]!='n') {hidden_matrix[coord_x+1][coord_y]=matrix[coord_x+1][coord_y];}
    }
    if (coord_x-1>=0 && coord_x-1 < x && coord_y+1>=0 && coord_y+1<y){
        if (matrix[coord_x-1][coord_y+1]==0){
            explosion(matrix, hidden_matrix, coord_x-1, coord_y+1, x, y);
        }
            if (matrix[coord_x-1][coord_y+1]!='n') {hidden_matrix[coord_x-1][coord_y+1]=matrix[coord_x-1][coord_y+1];}
    }
    if (coord_x>=0 && coord_x < x && coord_y+1>=0 && coord_y+1<y){
        if (matrix[coord_x][coord_y+1]==0){
            explosion(matrix, hidden_matrix, coord_x, coord_y+1, x, y);
        }
            if (matrix[coord_x][coord_y+1]!='n') {hidden_matrix[coord_x][coord_y+1]=matrix[coord_x][coord_y+1];}
    }
    if (coord_x+1>=0 && coord_x+1 < x && coord_y+1>=0 && coord_y+1<y){
        if (matrix[coord_x+1][coord_y+1]==0){
            explosion(matrix, hidden_matrix, coord_x+1, coord_y+1, x, y);
        }
            if (matrix[coord_x+1][coord_y+1]!='n') {hidden_matrix[coord_x+1][coord_y+1]=matrix[coord_x+1][coord_y+1];}
    }
}
