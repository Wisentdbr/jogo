#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define STD_SIZE_X 32
#define STD_SIZE_Y 32
#define LARGURA_BALA 10
#define ALTURA_BALA 15

int gg = 0;

typedef struct Barreira{
    Rectangle pos;
    Color color;
    int vida;
}Barreira;

typedef struct Bala{
    Rectangle pos;
    Color color;
    int ativa;
    int tempo;
    int velocidade;
    Sound tiro;
}Bala;

typedef struct Nave{
    Rectangle pos;
    Color color;
    Bala bala;
    int velocidade;
    int direcao;
    int hp;
}Nave;

typedef struct Heroi{
    Texture2D sprite;
    Rectangle pos;
    Color color;
    Bala bala;
    int velocidade;   
    int hp;
    int esquerda;
    int direita;
}Heroi;

typedef struct Bordas{
    Rectangle pos;
} Bordas;

typedef struct Assets{
    Texture2D naveVerde;
    Sound tiro;
}Assets;

typedef struct Jogo{
    Nave nave[4][9];
    Heroi heroi;
    Bordas bordas[4];
    Assets assets;
    int alturaJanela;
    double velocidadenave;
    int larguraJanela;
    int tempoAnimação;
    int pontos;
    float velocidadeuniversal;
    int telas;
    FILE *arq;
}Jogo;

void AtiraBalasHeroi(Jogo *j);
void DesenhaBalasHeroi(Jogo *j);
void colisaoBordasHeroi(Jogo *j);
void IniciaJogo(Jogo *j);
void IniciaNaves(Jogo *j);
void AtualizaJogo(Jogo *j);
void DesenhaJogo(Jogo *j);
void AtualizaFrameDesenho(Jogo *j);
void AtualizaNavePos(Jogo *j, int i, int k);
void DesenhaNaves(Jogo *j);
void DesenhaHeroi(Jogo *j);
void ColisaoBordas(Jogo *j);
void DesenhaBordas(Jogo *j);
void ColisaoBalas(Jogo *j, int i, int k);
void DesenhaBordas(Jogo *j);
void AtiraBalas(Jogo *j, int i, int k);
void CarregaImagens(Jogo *j);
void DescarregaImagens(Jogo *j);
void movimentaHeroi(Jogo *j);
void colisaoBalaheroi(Jogo *j);
void Telas(Jogo *j);
void DescarregaImagensnave(Jogo *j);
void AtualizadorNaves(Jogo *j);

int main(){
    InitAudioDevice();

    Jogo jogo;

    jogo.alturaJanela = ALTURA_JANELA;
    jogo.larguraJanela = LARGURA_JANELA;

    InitWindow(jogo.larguraJanela, jogo.alturaJanela, "Space Invaders");
    SetTargetFPS(60);
    IniciaJogo(&jogo);
    CarregaImagens(&jogo);
    Music musicaJogo = LoadMusicStream("assets/musica.mp3");
    PlayMusicStream(musicaJogo);

    while(!WindowShouldClose()){
        UpdateMusicStream(musicaJogo);
        AtualizaFrameDesenho(&jogo);
    }
    UnloadMusicStream(musicaJogo);
    DescarregaImagens(&jogo);
    CloseWindow();
    return 0;
}

void IniciaJogo(Jogo *j){
    j->tempoAnimação = GetTime();

    j->heroi.pos = (Rectangle) {LARGURA_JANELA/2 - STD_SIZE_X/2, ALTURA_JANELA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    j->heroi.color = BLUE;
    j->heroi.velocidade = 5;
    j->heroi.esquerda = 0;
    j->heroi.direita = 0;
    j->heroi.bala.ativa = 0;
    j->heroi.bala.velocidade=6;
    j->heroi.hp = 3;

    j->telas = 0;
   IniciaNaves(j);
    //borda encima
    j->bordas[0].pos = (Rectangle){0, 0, LARGURA_JANELA, 10};
    //borda embaixo
    j->bordas[1].pos = (Rectangle){0, ALTURA_JANELA-10, LARGURA_JANELA, 10};
    //borda esquerda
    j->bordas[2].pos = (Rectangle){0, 0, 10, ALTURA_JANELA};
    //borda direita
    j->bordas[3].pos = (Rectangle){LARGURA_JANELA-10, 0, 10, ALTURA_JANELA};
}

void Telas(Jogo *j){
    if(j->telas == 0){

    char name[10 + 1] = "\0";      
    int letterCount = 0;

    Rectangle textBox = { LARGURA_JANELA/2.0f - 130, 175, 250, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(60);               
   
    while (!IsKeyDown(KEY_ENTER)){     
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;
        if (mouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();

            while (key > 0)
            {
                
                if ((key >= 32) && (key <= 125) && (letterCount < 10))
                {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; 
                    letterCount++;
                }

                key = GetCharPressed();  
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Coloque seu nick aqui:", 230, 140, 30, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);


        EndDrawing();  
    }
    j->telas = 1;
        
    if(j->telas == 3){
        ClearBackground(WHITE);
        DrawText("Acabou!",300, 400, 30, GRAY);
    }

    
}

    
   
}

void IniciaNaves(Jogo *j){
    for(int i = 0; i < 4; i++){
        for(int k = 0; k < 9; k++){
            j->nave[i][k].pos = (Rectangle) {16 + 48 * k, 15 + 78 * i, STD_SIZE_X, STD_SIZE_Y};
            j->nave[i][k].color = RED;
            j->nave[i][k].bala.ativa = 0;
            j->nave[i][k].bala.tempo = GetTime();
            j->nave[i][k].bala.velocidade = 5;
            j->nave[i][k].bala.tiro = LoadSound("assets/shoot.wav");
            j->nave[i][k].hp = 1;
            j->velocidadeuniversal = 0.08;
        }
    } 
}



void AtualizaJogo(Jogo *j){
    if(j->telas == 0){
        Telas(j);
    }
    if(j->telas == 1){
        AtiraBalasHeroi(j);
        AtualizadorNaves(j);
        DrawText(TextFormat("Vidas: %d", j->heroi.hp), 11, 12, 15, PURPLE);
        DrawText(TextFormat("Pontos: %d", j->pontos), 11, 25, 15, PURPLE);
        DesenhaJogo(j);
    }
    if(j->heroi.hp == 0){
        j->telas = 3;
        EndDrawing();
        ClearBackground(BLACK);
    }
}

void AtualizadorNaves(Jogo *j){
    for (int i = 0; i < 4; i ++) {
        for (int k = 0; k < 9; k ++) {
            AtualizaNavePos(j,i,k);
            if (j->nave[i][k].hp > 0) {
            AtiraBalas(j, i, k);
            }
        }
    }
}



void DesenhaJogo(Jogo *j){
    BeginDrawing();
    ClearBackground(BLACK);
    DesenhaNaves(j);
    DesenhaHeroi(j);
    DesenhaBordas(j);
    
    
    EndDrawing();
}

void AtualizaFrameDesenho(Jogo *j){
    AtualizaJogo(j);
    
}

void AtualizaNavePos(Jogo *j, int i, int k){
    ColisaoBordas(j);
    for(int i = 0; i < 4; i++){
        for(int k = 0; k < 9; k++){
            j->nave[i][k].pos.x += j->velocidadeuniversal;
        }
    }
}

void CarregaImagens(Jogo *j){
    j->assets.naveVerde = LoadTexture("assets/GreenAnimation.png");
    j->heroi.sprite = LoadTexture("assets/Nave_heroi.png");
}

void DescarregaImagensnave(Jogo *j){
    UnloadTexture(j->assets.naveVerde);
}

void DescarregaImagens(Jogo *j){
    UnloadTexture(j->assets.naveVerde);
    UnloadTexture(j->heroi.sprite);
}

void DesenhaNaves(Jogo *j){
    for (int i = 0; i < 4; i ++) {
        for (int k = 0; k < 9; k ++) {
    Vector2 tamanhoFrame = {32, 32};
    if (j->nave[i][k].hp > 0 ) {
            Vector2 tamanhoFrame = {32, 32};
            
            static Vector2 frame = {0, 0};
   
            static float tempoUltimaTroca = 0;
                
            if(GetTime() - tempoUltimaTroca >= 1){
                if(frame.x == 0){
                    frame.x = 1;
                }else{
                    frame.x = 0;
                }

                tempoUltimaTroca = GetTime();
            }
            Rectangle frameRecNave = {frame.x * tamanhoFrame.x, frame.y*tamanhoFrame.y,
            tamanhoFrame.x, tamanhoFrame.y};
            DrawTexturePro(j->assets.naveVerde, frameRecNave, (Rectangle){j->nave[i][k].pos.x, j->nave[i][k].pos.y, 32, 32},
            (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
    }
}

void DesenhaHeroi(Jogo *j){
    DrawTexture(j->heroi.sprite, j->heroi.pos.x, j->heroi.pos.y, WHITE);
    movimentaHeroi(j);

}

void movimentaHeroi(Jogo *j){
    if(IsKeyDown(KEY_LEFT) && j->heroi.esquerda != 1){
        j->heroi.pos.x -= j->heroi.velocidade;
        j->heroi.direita = 0;
    }
    if(IsKeyDown(KEY_RIGHT) && j->heroi.direita != 1){
        j->heroi.pos.x += j->heroi.velocidade;
        j->heroi.esquerda = 0;
    }
    colisaoBordasHeroi(j);
}

void colisaoBordasHeroi(Jogo *j){
    if(CheckCollisionRecs(j->heroi.pos, j->bordas[2].pos)){
        j->heroi.esquerda=1;
    }
    if(CheckCollisionRecs(j->heroi.pos, j->bordas[3].pos)){
        j->heroi.direita=1;
    }
}

void DesenhaBordas(Jogo *j){
    for(int i = 0; i < 4; i++){
        DrawRectangleRec(j->bordas[i].pos, LIGHTGRAY);
    }
}

void DesenhaBalas(Jogo *j, int i, int k){
    DrawRectangleRec(j->nave[i][k].bala.pos, YELLOW);
}

void DesenhaBalasHeroi(Jogo *j){
    DrawRectangleRec(j->heroi.bala.pos, BLUE);
}

void AtiraBalas(Jogo *j, int i, int k){
    if(j->nave[i][k].bala.ativa == 0 && GetTime()-j->nave[i][k].bala.tempo > 3){
        j->nave[i][k].bala.pos = (Rectangle){j->nave[i][k].pos.x+j->nave[i][k].pos.width/2, j->nave[i][k].pos.y+j->nave[i][k].pos.height/2, 
        LARGURA_BALA, ALTURA_BALA};
        j->nave[i][k].bala.ativa = 1;
        j->nave[i][k].bala.tempo = GetTime();
        PlaySound(j->nave[i][k].bala.tiro);
    }
    if(j->nave[i][k].bala.ativa == 1){
    ColisaoBalas(j, i, k);
    j->nave[i][k].bala.pos.y += j->nave[i][k].bala.velocidade;
        DesenhaBalas(j, i, k);
    }


}

void AtiraBalasHeroi(Jogo *j){
    if(IsKeyPressed(KEY_SPACE) && j->heroi.bala.ativa == 0){
        j->heroi.bala.pos = (Rectangle) {(j->heroi.pos.x - LARGURA_BALA/2)+j->heroi.pos.width/2, j->heroi.pos.y-j->heroi.pos.height/2, 
            LARGURA_BALA, ALTURA_BALA};
        j->heroi.bala.ativa = 1;
    }

    if(j->heroi.bala.ativa == 1){
        DesenhaBalasHeroi(j);
        j->heroi.bala.pos.y -= j->heroi.bala.velocidade;
        colisaoBalaheroi(j);
        }
    
    
        
}

void ColisaoBordas(Jogo *j){
    if(CheckCollisionRecs(j->nave[0][0].pos, j->bordas[2].pos)){
        j->velocidadeuniversal *= -1;
    }else if(CheckCollisionRecs(j->nave[0][8].pos, j->bordas[3].pos)){
        j->velocidadeuniversal *= -1;
    }
}

void ColisaoBalas(Jogo *j, int i, int k){
    // Colisao bala com heroi
    if(CheckCollisionRecs(j->nave[i][k].bala.pos, j->heroi.pos)){
        j->heroi.hp --;
        j->nave[i][k].bala.ativa = 0;
        if(j->nave[i][k].bala.ativa == 0){
            j->nave[i][k].bala.pos.y = 0;
        }
        if(j->heroi.hp == 0){
            DrawText("Acabou!", 300, 300, 20, RED);   
            gg = 1;
        }
        
    }
    // Colisao bala com borda de baixo
    if(CheckCollisionRecs(j->nave[i][k].bala.pos, j->bordas[1].pos)){
        j->nave[i][k].bala.ativa = 0;
    }

}

void colisaoBalaheroi(Jogo *j){
    if(CheckCollisionRecs(j->heroi.bala.pos, j->bordas[0].pos)){
        j->heroi.bala.ativa=0;
    }
for(int i = 0; i < 4; i++){
    for(int k = 0; k < 9; k++){
        if(CheckCollisionRecs(j->nave[i][k].pos, j->heroi.bala.pos) && j->nave[i][k].hp > 0){
            j->heroi.bala.ativa = 0;
            j->nave[i][k].hp--;
            j->pontos += 20;
                    }
                }
            }
        }
