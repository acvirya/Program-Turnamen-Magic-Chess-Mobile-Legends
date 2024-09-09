#include <iostream>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

typedef struct data{
    int status;
    char nama[20];

    int poin_BO1,poin_BO2,poin_Total;
    int win_BO1,win_BO2,win_Total;
    int power_BO1,power_BO2,power_Total;
    int ranking;
}data;

int isnull(){
    FILE *fp;
    fp = fopen("Data Player.txt","a+");
    int a = getc(fp);
    fclose(fp);
    if (a == EOF) return 1;
    return 0;
}

void getTourData(int *slot, int *group){
    FILE *fp = fopen("Data Tournament.txt","r");
    fscanf(fp,"%d#%d\n",&*slot,&*group);
    fclose(fp);
}

void createNewTour(){
    int pilihan,slot,group;
    printf("Peringatan: Seluruh data tour akan terhapus permanen!\nTekan 1 untuk melanjutkan: "); scanf("%d",&pilihan);getchar();
    if (pilihan == 1){
        printf("Jumlah slot: ");scanf("%d",&pilihan);getchar();
        slot = pilihan % 8;
        group = pilihan / 8;
        if (!slot){
            printf("Turnamen berhasil dibuat!\n");
            FILE *fp = fopen("Data Tournament.txt","w");
            fprintf(fp,"%d#%d\n",pilihan,group);
            fclose(fp);
            fp = fopen("Data Player.txt","w");
            fprintf(fp,"");
            fclose(fp);
        }
        else{
            printf("Jumlah slot harus kelipatan 8!\n");
            printf("Turnamen baru dibatalkan\n");
        }
    }
    else printf("Turnamen baru dibatalkan\n");
}

data **getPlayerData(data **player,int *counter){
    if (!isnull()){
            FILE *fp = fopen("Data Player.txt","r");
            while(!feof(fp)){
                data newp;
                int group1,position1;
                fscanf(fp,"%d#%[^#]#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",&newp.status,newp.nama,&group1,&position1,
                                                                        &newp.poin_BO1,&newp.win_BO1,&newp.power_BO1,
                                                                        &newp.poin_BO2,&newp.win_BO2,&newp.power_BO2,
                                                                        &newp.poin_Total,&newp.win_Total,&newp.power_Total);
                *counter += 1;
                player[group1][position1] = newp;
            }
            fclose(fp);
        }
    return player;
}

data **sortPlayer(data **player,int slot,int group){
    for(int i = 0; i < slot; i++){
        for(int k = 0; k < group; k++){
            for(int l = 0; l < 7; l++){
                if (player[k][l].poin_Total < player[k][l+1].poin_Total){
                    data swapplayer = player[k][l];
                    player[k][l] = player[k][l+1];
                    player[k][l+1] = swapplayer;
                }
                else if (player[k][l].poin_Total == player[k][l+1].poin_Total){
                    if (player[k][l].win_Total < player[k][l+1].win_Total){
                        data swapplayer = player[k][l];
                        player[k][l] = player[k][l+1];
                        player[k][l+1] = swapplayer;
                    }
                    else if (player[k][l].win_Total == player[k][l+1].win_Total){
                        if (player[k][l].power_Total < player[k][l+1].power_Total){
                            data swapplayer = player[k][l];
                            player[k][l] = player[k][l+1];
                            player[k][l+1] = swapplayer;
                        }
                        else continue;
                    }
                }
            }
        }
    }
    return player;
}

void addNewPlayer(data **player,int counter,int slot,int group){
    int gro,pos,flag = 1,pilihan;

    if (counter == slot){
        printf("Slot penuh!\n"); getchar();
        return;
    }

    printf("[1] Acak grup otomatis\n[2] Pilih grup manual\nPilihan: ");scanf("%d",&pilihan); getchar();
    if (pilihan == 1){
        srand(time(0));
        while(1){
            if (flag == 0) break;
            gro = rand()%group;
            for(int i = 0; i < 8; i++){
                if (player[gro][i].status == 0){
                    player[gro][i].status = 1;
                    pos = i;
                    flag = 0;
                    break;
                }
            }
        }
    }
    else{
        printf("Grup: "); scanf("%d",&gro); getchar(); gro--;
        for(int i = 0; i < 8; i++){
            if (player[gro][i].status == 0){
                player[gro][i].status = 1;
                pos = i;
                flag = 0;
                break;
            }
        }
        if (flag == 1){
            printf("Grup penuh!\n");
        }
    }
    if (flag == 0){
    printf("Nama player: "); scanf("%[^\n]",player[gro][pos].nama); getchar();
    FILE *fp = fopen("Data Player.txt","w");
    for(int i = 0; i < group; i++){
        for(int j= 0; j < 8; j++){
            if (player[i][j].status == 1){
                fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                                player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                                player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                                player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
            }
        }
    }
    fclose(fp);
    printf("Player dimasukkan kedalam grup\n");
    }
}

void changePlayerName(data **player,int group){
    char nama[20];
    int flag = 1;
    printf("Nama player : "); scanf("%[^\n]",nama); getchar();
    for(int i = 0; i < group; i++){
        for(int j = 0; j < 8; j++){
            if (flag == 0) break;
            if (player[i][j].status == 1){
                if (strcmp(player[i][j].nama,nama) == 0){
                    printf("Ubah menjadi: ");scanf("%[^\n]",player[i][j].nama); getchar();
                    flag = 0;
                }
            }
        }
    }
    if (flag == 0){
        FILE *fp = fopen("Data Player.txt","w");
        for(int i = 0; i < group; i++){
            for(int j = 0; j < 8; j++){
                if (player[i][j].status == 1){
                    fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                        player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                        player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                        player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                }
            }
        }
        fclose(fp);
        printf("Nama berhasil diubah!\n");
    }
    else printf("Nama player tidak ditemukan\n");

}

void deletePlayer(data **player,int group){
    char nama[20];
    int flag = 1;
    printf("Nama player: "); scanf("%[^\n]",nama); getchar();

    int a,b;
    for(int i = 0; i < group; i++){
        for(int j = 0; j < 8; j++){
            if (flag == 0) break;
            if (player[i][j].status == 1){
                if (strcmp(player[i][j].nama,nama) == 0){
                    player[i][j].status = 0;
                    flag = 0;
                }
            }
        }
    }
    if (flag == 0){
        FILE *fp = fopen("Data Player.txt","w");
        for(int i = 0; i < group; i++){
            for(int j = 0; j < 8; j++){
                if (player[i][j].status == 1){
                    fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                        player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                        player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                        player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                }
            }
        }
        fclose(fp);
        printf("Player berhasil dihapus!\n");
    }
    else{
        printf("Player tidak ditemukan\n");
    }
}

void addScore(data **player,int group){
    int match,pilihan;

    system("cls");
    printf("     Tambahkan score player per grup     \n");
    printf("=========================================\n");
    for(int i = 0; i < group; i++){
        printf("%d. Group %d\n",i+1,i+1);
    }
    printf("Pilihan: "); scanf("%d",&pilihan); getchar();pilihan--;
    printf("1. Match 1\n");
    printf("2. Match 2\n");
    printf("Pilihan: "); scanf("%d",&match); getchar();
    for(int i = 0; i < 8; i++){
        if (player[pilihan][i].status == 1){
            printf("%s\n",player[pilihan][i].nama);
            if (match == 1){
                printf("Point: ");scanf("%d",&player[pilihan][i].poin_BO1);getchar();
                printf("Win  : ");scanf("%d",&player[pilihan][i].win_BO1);getchar();
                printf("Power: ");scanf("%d",&player[pilihan][i].power_BO1);getchar();
            }
            else if (match == 2){
                printf("Point: ");scanf("%d",&player[pilihan][i].poin_BO2);getchar();
                printf("Win  : ");scanf("%d",&player[pilihan][i].win_BO2);getchar();
                printf("Power: ");scanf("%d",&player[pilihan][i].power_BO2);getchar();
            }
            player[pilihan][i].poin_Total = player[pilihan][i].poin_BO1 + player[pilihan][i].poin_BO2;
            player[pilihan][i].win_Total = player[pilihan][i].win_BO1 + player[pilihan][i].win_BO2;
            player[pilihan][i].power_Total = player[pilihan][i].power_BO1 + player[pilihan][i].power_BO2;
        }
    }

    FILE *fp = fopen("Data Player.txt","w");
    for(int i = 0; i < group; i++){
        for(int j = 0; j < 8; j++){
            if (player[i][j].status == 1){
                fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                    player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                    player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                    player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
            }
        }
    }
    fclose(fp);
    printf("Data berhasil disimpan!\n");
}

void changeScore(data **player,int group){
    char nama[20];
    int flag = 1,match;

    printf("Nama player : "); scanf("%[^\n]",nama); getchar();
    printf("1. Match 1\n2. Match 2\nPilihan: "); scanf("%d",&match); getchar();

    for(int i = 0; i < group; i++){
        for(int j = 0; j < 8; j++){
            if (flag == 0) break;
            if (player[i][j].status == 1){
                if (strcmp(player[i][j].nama,nama) == 0){
                    flag = 0;
                    if (match == 1){
                        printf("Point: "); scanf("%d",&player[i][j].poin_BO1); getchar();
                        printf("Win  : "); scanf("%d",&player[i][j].win_BO1); getchar();
                        printf("Power: "); scanf("%d",&player[i][j].power_BO1); getchar();
                    }
                    else if (match == 2){
                        printf("Point: "); scanf("%d",&player[i][j].poin_BO2); getchar();
                        printf("Win  : "); scanf("%d",&player[i][j].win_BO2); getchar();
                        printf("Power: "); scanf("%d",&player[i][j].power_BO2); getchar();
                    }
                    player[i][j].poin_Total = player[i][j].poin_BO1 + player[i][j].poin_BO2;
                    player[i][j].win_Total = player[i][j].win_BO1 + player[i][j].win_BO2;
                    player[i][j].power_Total = player[i][j].power_BO1 + player[i][j].power_BO2;
                }
            }
        }
    }

    if (flag == 0){
        FILE *fp = fopen("Data Player.txt","w");
        for(int i = 0; i < group; i++){
            for(int j = 0; j < 8; j++){
                if (player[i][j].status == 1){
                    fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                        player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                        player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                        player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                }
            }
        }
        fclose(fp);
        printf("Data berhasil diubah!\n");
    }
    else{
        printf("Nama player tidak ditemukan\n");
    }
}

void mergePlayer(data **player,int group,int slot){
    int pilihan;
    FILE *fp = fopen("Data Player Backup.txt","w");
            for(int i = 0; i < group; i++){
                for(int j = 0; j < 8; j++){
                    if (player[i][j].status == 1){
                        fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                    player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                    player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                    player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                    }
                }
            }
    fclose(fp);

    player = sortPlayer(player,slot,group);
    char newplayer[slot/2][100];
    int c = 0;
    for(int i = 0; i < group; i++){
        for(int j = 0; j < 4; j++){
            strcpy(newplayer[c],player[i][j].nama);
            c++;
        }
    }
    while(1){
        system("cls");
        printf("     Masukkan player dengan ranking 4 besar disetiap grup     \n");
        printf("==============================================================\n");
        if (slot == 8) break;
        for(int i = 0; i < group; i++){
        for(int j = 0; j < 8; j++){
            player[i][j].status = 0;
            }
        }
        printf("Memindahkan player. . .\n");
        int group2;
        srand(time(0));
        for(int i = 0; i < slot/2; i++){
            int flag = 1;
            while(1){
                group2 = rand() % (group/2);
                for(int j = 0; j < 8; j++){
                    if (player[group2][j].status == 0){
                        player[group2][j].status = 1;
                        strcpy(player[group2][j].nama,newplayer[i]);
                        flag = 0;
                        break;
                    }
                }
                if (flag == 0) break;
            }
        }
        printf("Pemindahan selesai!\n");
        for(int i = 0; i < group/2; i++){
            printf("Group %d\n",i+1);
            for(int j = 0; j < 8; j++){
                printf("%d. %s\n",j+1,player[i][j].nama);
            }
        }
        printf("Tekan 1 untuk acak lagi: "); scanf("%d",&pilihan); getchar();
        if (pilihan != 1)break;
    }
    if (slot != 8){
    FILE *fp = fopen("Data Player.txt","w");
    for(int i = 0; i < group/2; i++){
        for(int j = 0; j < 8; j++){
            fprintf(fp,"1#%s#%d#%d#0#0#0#0#0#0#0#0#0\n",player[i][j].nama,i,j);
        }
    }
    fclose(fp);
    fp = fopen("Data Tournament.txt","w");
    fprintf(fp,"%d#%d\n",slot/2,group/2);
    fclose(fp);
    printf("Data berhasil diubah!\n");
    }
    else{
        printf("Sudah tidak dapat membagi grup lagi!\n");
    }
}

void printData(data **player,int group,int slot){
    system("cls");
    player = sortPlayer(player,slot,group);

    for (int i = 0; i < group; i++){
        printf("Group %d\n",i+1);
        puts("--------------------------------------------------------------------------------------------------------");
        puts("|     |                        |          BO1          |          BO2          |         TOTAL         |");
        puts("| No. |          Nama          |-----------------------|-----------------------|-----------------------|");
        puts("|     |                        | Point |  win  | power | Point |  win  | power | Point |  win  | power |");
        puts("|-----|------------------------|-----------------------|-----------------------|-----------------------|");
        for(int j = 0; j < 8; j++){
            printf("|  %d  | ",j+1);
            if (player[i][j].status == 1){
                printf("%-22s | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d |\n",
                player[i][j].nama,player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);

            }
            else{
                    printf("                       |       |       |       |       |       |       |       |       |       |\n");
            }
        }
        puts("--------------------------------------------------------------------------------------------------------");
    }
}

void shufflePlayer(data **player,int group,int slot){
    int pilihan;
    FILE *fp = fopen("Data Player Backup.txt","w");
                    for(int i = 0; i < group; i++){
                        for(int j = 0; j < 8; j++){
                            if (player[i][j].status == 1){
                                fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                            player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                            player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                            player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                            }
                        }
                    }
            fclose(fp);

            char newplayer[slot][100];
            int c = 0;
            for(int i = 0; i < group; i++){
                for(int j = 0; j < 8; j++){
                    strcpy(newplayer[c],player[i][j].nama);
                    c++;
                }
            }

            while(1){
                system("cls");
                printf("     Acak seluruh player     \n");
                printf("=============================\n");
                for(int i = 0; i < group; i++){
                for(int j = 0; j < 8; j++){
                    player[i][j].status = 0;
                    }
                }
                printf("Mengacak player. . .\n");
                int group2;
                srand(time(0));
                for(int i = 0; i < slot; i++){
                    int flag = 1;
                    while(1){
                        group2 = rand() % group;
                        for(int j = 0; j < 8; j++){
                            if (player[group2][j].status == 0){
                                player[group2][j].status = 1;
                                strcpy(player[group2][j].nama,newplayer[i]);
                                flag = 0;
                                break;
                            }
                        }
                        if (flag == 0) break;
                    }
                }
                printf("Player selesai diacak!\n");
                for(int i = 0; i < group; i++){
                    printf("Group %d\n",i+1);
                    for(int j = 0; j < 8; j++){
                        printf("%d. %s\n",j+1,player[i][j].nama);
                    }
                }
                printf("Tekan 1 untuk acak lagi: "); scanf("%d",&pilihan); getchar();
                if (pilihan != 1)break;
            }

            fp = fopen("Data Player.txt","w");
            for(int i = 0; i < group; i++){
                for(int j = 0; j < 8; j++){
                    fprintf(fp,"1#%s#%d#%d#0#0#0#0#0#0#0#0#0\n",player[i][j].nama,i,j);
                }
            }
            fclose(fp);
            printf("Data berhasil diubah!\n");
}

void switchPlayer(data **player,int group,int slot){
    int grp = 0,pos = 0,flag = 1;
            int grp2 = 0, pos2 = 0;
            char nama[100];
            printf("Nama player: "); scanf("%[^\n]",nama); getchar();
            for(int i = 0; i < group; i++){
                if (flag == 0) break;
                for(int j = 0; j < 8; j++){
                    if (strcmp(nama,player[i][j].nama) == 0){
                        flag = 0;
                        grp2 = i;
                        pos2 = j;
                        break;
                    }
                }
            }
            if (flag == 0){
                printf("Tukar ke grup: "); scanf("%d",&grp); getchar(); grp--;
                printf("Tukar dengan:\n");
                for(int i = 0; i < 8; i++){
                    printf("[%d] %s\n",i+1,player[grp][i].nama);
                    if (player[grp][i].status == 0) break;
                }
                printf("Pilihan: "); scanf("%d",&pos); getchar(); pos--;
                data temp;
                temp = player[grp2][pos2];
                player[grp2][pos2] = player[grp][pos];
                player[grp][pos] = temp;
                printf("Player berhasil ditukar!\n");
                FILE *fp = fopen("Data Player.txt","w");
                    for(int i = 0; i < group; i++){
                        for(int j = 0; j < 8; j++){
                            if (player[i][j].status == 1){
                                fprintf(fp,"1#%s#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n",player[i][j].nama,i,j,
                                                                            player[i][j].poin_BO1,player[i][j].win_BO1,player[i][j].power_BO1,
                                                                            player[i][j].poin_BO2,player[i][j].win_BO2,player[i][j].power_BO2,
                                                                            player[i][j].poin_Total,player[i][j].win_Total,player[i][j].power_Total);
                            }
                        }
                    }
                fclose(fp);
            }
            else{
                printf("Nama tidak ditemukan!\n");
            }
}

int main(){
    system("color 0A");
        while(1){
            int slot = 0;
            int group = 0;
            int counter = 0;
            getTourData(&slot,&group);
            data **player = (data **) malloc (group * sizeof (data*));
            for (int i=0; i<group; i++) player[i] = (data *)malloc(8 * sizeof(data));
            for(int j = 0; j < group; j++){
                for(int k = 0; k < 8; k++){
                    player[j][k].status = 0;
                    player[j][k].poin_BO1 = player[j][k].poin_BO2 = player[j][k].poin_Total = 0;
                    player[j][k].win_BO1 = player[j][k].win_BO2 = player[j][k].win_Total = 0;
                    player[j][k].power_BO1 = player[j][k].power_BO2 = player[j][k].power_Total = 0;
                    strcpy(player[j][k].nama,"-");
                }
            }
            player = getPlayerData(player,&counter);
            int pilihan = 0;
            system("cls");
            printf("     Program Turnamen     \n");
            printf("--------------------------\n");
            printf("[1] Buat turnamen baru\n");
            printf("[2] Tambahkan player baru (%d/%d)\n",counter,slot);
            printf("[3] Ubah nama player\n");
            printf("[4] Hapus seorang player\n");
            printf("[5] Tambahkan score player (per grup)\n");
            printf("[6] Ubah score player (per player)\n");
            printf("[7] Masukkan player dengan ranking 4 besar disetiap grup\n");
            printf("[8] Tampilkan data player\n");
            printf("[9] Acak seluruh player\n");
            printf("[10] Ganti/Tukar grup player\n");
            printf("Pilihan: "); scanf("%d",&pilihan); getchar();
            if (pilihan == 1) createNewTour();
            else if (pilihan == 2) addNewPlayer(player,counter,slot,group);
            else if (pilihan == 3) changePlayerName(player,group);
            else if (pilihan == 4) deletePlayer(player,group);
            else if (pilihan == 5) addScore(player,group);
            else if (pilihan == 6) changeScore(player,group);
            else if (pilihan == 7) mergePlayer(player,group,slot);
            else if (pilihan == 8) printData(player,group,slot);
            else if (pilihan == 9) shufflePlayer(player,group,slot);
            else if (pilihan == 10) switchPlayer(player,group,slot);

            getchar();
        }
}
