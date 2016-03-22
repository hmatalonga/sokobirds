#include "sokobirds.h"

char **playerMove(char **Map, int *pos_x, int *pos_y, char *lastStep, int *CheckPoint, int *score, char dir) {
    //lastStep Guardar o tipo da última posição onde esteve o Player
    //X Colunas
    //Y Linhas
    if (dir == 'w') { //Up
        if (Map[*pos_x - 1][*pos_y] == CFLOOR) {
            //Chão -> Player andar para baixo
            Map[*pos_x - 1][*pos_y] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 0;
            *lastStep = CFLOOR;
            (*pos_x)--;
        } else if (Map[*pos_x - 1][*pos_y] == CHOLE) {
            //Buraco -> Player andar para baixo
            Map[*pos_x - 1][*pos_y] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 1;
            *lastStep = CHOLE;
            (*pos_x)--;
        }
        else if (Map[*pos_x - 1][*pos_y] == CWALL); //If não necessário uma vez que nenhum valor é alterado
        else if (Map[*pos_x - 1][*pos_y] == CBOX) { //Caixa
            if (Map[*pos_x - 2][*pos_y] == CFLOOR) {
                //Chão -> Caixa+Player andar para baixo
                Map[*pos_x - 2][*pos_y] = CBOX;
                Map[*pos_x - 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 0;
                *lastStep = CFLOOR;
                (*pos_x)--;
            } else if (Map[*pos_x - 2][*pos_y] == CHOLE) {
                //Buraco -> Caixa+Player andar para baixo
                Map[*pos_x - 2][*pos_y] = CDONE;
                Map[*pos_x - 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x + 1][*pos_y] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x + 1][*pos_y] == CFLOOR)
                    *lastStep = CFLOOR;
                (*pos_x)--;
                (*score)--;
                //PlayMusic("sound/coin.wav", 6, 1, MusicVolume, SOUND_NO_LOOP);
            } else if (Map[*pos_x - 2][*pos_y] == CWALL);
            else if (Map[*pos_x - 2][*pos_y] == CBOX);
            else if (Map[*pos_x - 2][*pos_y] == CDONE);
        } else if (Map[*pos_x - 1][*pos_y] == CDONE) { //Objectivo
            if (Map[*pos_x - 2][*pos_y] == CFLOOR) {
                //Chão -> CaixaD+Player andar para baixo
                Map[*pos_x - 2][*pos_y] = CDONE;
                Map[*pos_x - 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                if (*CheckPoint)
                    *CheckPoint = 0;
                else
                    *lastStep = CFLOOR;
                (*pos_x)--;
            } else if (Map[*pos_x - 2][*pos_y] == CHOLE) {
                //Buraco -> CaixaD+Player andar para baixo
                Map[*pos_x - 2][*pos_y] = CDONE;
                Map[*pos_x - 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x + 1][*pos_y] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x + 1][*pos_y] == CFLOOR)
                    *lastStep = CFLOOR;
                *lastStep = CHOLE;
                (*pos_x)--;
            } else if (Map[*pos_x - 2][*pos_y] == CWALL);
            else if (Map[*pos_x - 2][*pos_y] == CBOX);
            else if (Map[*pos_x - 2][*pos_y] == CDONE);
        }
    } else if (dir == 's') { //Down
        if (Map[*pos_x + 1][*pos_y] == CFLOOR) {
            //Chão -> Player andar para a frente
            Map[*pos_x + 1][*pos_y] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 0;
            *lastStep = CFLOOR;
            (*pos_x)++;
        } else if (Map[*pos_x + 1][*pos_y] == CHOLE) {
            //Buraco -> Player andar para a frente
            Map[*pos_x + 1][*pos_y] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 1;
            *lastStep = CHOLE;
            (*pos_x)++;
        }
        else if (Map[*pos_x + 1][*pos_y] == CWALL); //If não necessário uma vez que nenhum valor é alterado
        else if (Map[*pos_x + 1][*pos_y] == CBOX) { //Caixa
            if (Map[*pos_x + 2][*pos_y] == CFLOOR) {
                //Chão -> Caixa+Player andar para a frente
                Map[*pos_x + 2][*pos_y] = CBOX;
                Map[*pos_x + 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 0;
                *lastStep = CFLOOR;
                (*pos_x)++;
            } else if (Map[*pos_x + 2][*pos_y] == CHOLE) {
                //Buraco -> Caixa+Player andar para a frente
                Map[*pos_x + 2][*pos_y] = CDONE;
                Map[*pos_x + 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x - 1][*pos_y] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x - 1][*pos_y] == CFLOOR)
                    *lastStep = CFLOOR;
                (*pos_x)++;
                (*score)--;
                //PlayMusic("sound/coin.wav", 6, 1, MusicVolume, SOUND_NO_LOOP);
            } else if (Map[*pos_x + 2][*pos_y] == CWALL);
            else if (Map[*pos_x + 2][*pos_y] == CBOX);
            else if (Map[*pos_x + 2][*pos_y] == CDONE);
        } else if (Map[*pos_x + 1][*pos_y] == CDONE) { //Objectivo
            if (Map[*pos_x + 2][*pos_y] == CFLOOR) {
                //Chão -> CaixaD+Player andar para a frente
                Map[*pos_x + 2][*pos_y] = CDONE;
                Map[*pos_x + 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                if (*CheckPoint)
                    *CheckPoint = 0;
                else
                    *lastStep = CFLOOR;
                (*pos_x)++;
            } else if (Map[*pos_x + 2][*pos_y] == CHOLE) {
                //Buraco -> CaixaD+Player andar para a frente
                Map[*pos_x + 2][*pos_y] = CDONE;
                Map[*pos_x + 1][*pos_y] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x - 1][*pos_y] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x - 1][*pos_y] == CFLOOR)
                    *lastStep = CFLOOR;
                *lastStep = CHOLE;
                (*pos_x)++;
            } else if (Map[*pos_x + 2][*pos_y] == CWALL);
            else if (Map[*pos_x + 2][*pos_y] == CBOX);
            else if (Map[*pos_x + 2][*pos_y] == CDONE);
        }       
    } else if (dir == 'a') { //Left
        if (Map[*pos_x][*pos_y - 1] == CFLOOR) {
            //Chão -> Player andar para esquerda
            Map[*pos_x][*pos_y - 1] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 0;
            *lastStep = CFLOOR;
            (*pos_y)--;
        } else if (Map[*pos_x][*pos_y - 1] == CHOLE) {
            //Buraco -> Player andar para esquerda
            Map[*pos_x][*pos_y - 1] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 1;
            *lastStep = CHOLE;
            (*pos_y)--;
        }
        else if (Map[*pos_x][*pos_y - 1] == CWALL); //If não necessário uma vez que nenhum valor é alterado
        else if (Map[*pos_x][*pos_y - 1] == CBOX) { //Caixa
            if (Map[*pos_x][*pos_y - 2] == CFLOOR) {
                //Chão -> Caixa+Player andar para esquerda
                Map[*pos_x][*pos_y - 2] = CBOX;
                Map[*pos_x][*pos_y - 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 0;
                *lastStep = CFLOOR;
                (*pos_y)--;
            } else if (Map[*pos_x][*pos_y - 2] == CHOLE) {
                //Chão -> Caixa+Player andar para esquerda
                Map[*pos_x][*pos_y - 2] = CDONE;
                Map[*pos_x][*pos_y - 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x][*pos_y + 1] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x][*pos_y + 1] == CFLOOR)
                    *lastStep = CFLOOR;
                (*pos_y)--;
                (*score)--;
                //PlayMusic("sound/coin.wav", 6, 1, MusicVolume, SOUND_NO_LOOP);
            } else if (Map[*pos_x][*pos_y - 2] == CWALL);
            else if (Map[*pos_x][*pos_y - 2] == CBOX);
            else if (Map[*pos_x][*pos_y - 2] == CDONE);
        } else if (Map[*pos_x][*pos_y - 1] == CDONE) { //Objectivo
            if (Map[*pos_x][*pos_y - 2] == CFLOOR) {
                //Chão -> CaixaD+Player andar para esquerda
                Map[*pos_x][*pos_y - 2] = CDONE;
                Map[*pos_x][*pos_y - 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                if (*CheckPoint)
                    *CheckPoint = 0;
                else
                    *lastStep = CFLOOR;
                (*pos_y)--;
            } else if (Map[*pos_x][*pos_y - 2] == CHOLE) {
                //Buraco -> CaixaD+Player andar para esquerda
                Map[*pos_x][*pos_y - 2] = CDONE;
                Map[*pos_x][*pos_y - 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x][*pos_y + 1] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x][*pos_y + 1] == CFLOOR)
                    *lastStep = CFLOOR;
                *lastStep = CHOLE;
                (*pos_y)--;
            } else if (Map[*pos_x][*pos_y - 2] == CWALL);
            else if (Map[*pos_x][*pos_y - 2] == CBOX);
            else if (Map[*pos_x][*pos_y - 2] == CDONE);
        }
    } else if (dir == 'd') { //Right   
        if (Map[*pos_x][*pos_y + 1] == CFLOOR) {
            //Chão -> Player andar para direita
            Map[*pos_x][*pos_y + 1] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 0;
            *lastStep = CFLOOR;
            (*pos_y)++;
        } else if (Map[*pos_x][*pos_y + 1] == CHOLE) {
            //Buraco -> Player andar para direita
            Map[*pos_x][*pos_y + 1] = CPLAYER;
            Map[*pos_x][*pos_y] = *lastStep;
            *CheckPoint = 1;
            *lastStep = CHOLE;
            (*pos_y)++;
        }
        else if (Map[*pos_x][*pos_y + 1] == CWALL); //If não necessário uma vez que nenhum valor é alterado
        else if (Map[*pos_x][*pos_y + 1] == CBOX) { //Caixa
            if (Map[*pos_x][*pos_y + 2] == CFLOOR) {
                //Chão -> Caixa+Player andar para direita
                Map[*pos_x][*pos_y + 2] = CBOX;
                Map[*pos_x][*pos_y + 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *lastStep = CFLOOR;
                (*pos_y)++;
            } else if (Map[*pos_x][*pos_y + 2] == CHOLE) {
                //Buraco -> Caixa+Player andar para direita
                Map[*pos_x][*pos_y + 2] = CDONE;
                Map[*pos_x][*pos_y + 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x][*pos_y - 1] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x][*pos_y - 1] == CFLOOR)
                    *lastStep = CFLOOR;
                (*pos_y)++;
                (*score)--;
                //PlayMusic("sound/coin.wav", 6, 1, MusicVolume, SOUND_NO_LOOP);
            } else if (Map[*pos_x][*pos_y + 2] == CWALL);
            else if (Map[*pos_x][*pos_y + 2] == CBOX);
            else if (Map[*pos_x][*pos_y + 2] == CDONE);
        } else if (Map[*pos_x][*pos_y + 1] == CDONE) { //Objectivo
            if (Map[*pos_x][*pos_y + 2] == CFLOOR) {
                //Chão -> CaixaD+Player andar para direita
                Map[*pos_x][*pos_y + 2] = CDONE;
                Map[*pos_x][*pos_y + 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                if (*CheckPoint)
                    *CheckPoint = 0;
                else
                    *lastStep = CFLOOR;
                (*pos_y)++;
            } else if (Map[*pos_x][*pos_y + 2] == CHOLE) {
                //Buraco -> CaixaD+Player andar para direita
                Map[*pos_x][*pos_y + 2] = CDONE;
                Map[*pos_x][*pos_y + 1] = CPLAYER;
                Map[*pos_x][*pos_y] = *lastStep;
                *CheckPoint = 1;
                if (Map[*pos_x][*pos_y - 1] == CHOLE)
                    *lastStep = CHOLE;
                else if (Map[*pos_x][*pos_y - 1] == CFLOOR)
                    *lastStep = CFLOOR;
                *lastStep = CHOLE;
                (*pos_y)++;
            } else if (Map[*pos_x][*pos_y + 2] == CWALL);
            else if (Map[*pos_x][*pos_y + 2] == CBOX);
            else if (Map[*pos_x][*pos_y + 2] == CDONE);
        }
    }
    return Map;
}

void updateGame(Game *game) {
    if (game->Score == 0)
        game->Level++;
    game->Score = 5;
}