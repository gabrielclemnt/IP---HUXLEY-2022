#include <stdio.h>

#define Inimigo_Amount 10
#define MAX_VELOCIDADE 7
// Estrutura de telas
typedef enum TelaGame
{
    LOGO = 0,
    MENU,
    GAMEPLAY,
    FIM
} TelaGame;

typedef struct Inimigo
{
    Rectangle rec;
    Color cor;
    int obstaculoTipo;
    int TeveColisao;

} Inimigo;

static Inimigo inimigo[Inimigo_Amount] = {0};

void inimigoAleatorio(Inimigo[], Texture2D[], int);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    int saltoCarroVertical = 35;
    int saltoCarroHorizontal = 67;

    InitWindow(screenWidth, screenHeight, "Velozes e agiotas - CIN, UFPE");

    // Inicializando device de audio
    InitAudioDevice();
    // Inicializando sons
    // Sound intro = LoadSound("assets/intro.mp3");
    Music intro = LoadMusicStream("assets/intro.mp3");
    Sound cashSound = LoadSound("assets/cashSound.mp3");
    Sound explosao = LoadSound("assets/explosao.mp3");
    Sound winner = LoadSound("assets/winner.mp3");
    // Tocando
    // PlaySound(intro);
    PlayMusicStream(intro);
    bool pause = false;

    Texture2D textura = LoadTexture("assets/banner.png");
    Texture2D loseTelaFinal = LoadTexture("assets/lose-telafinal.png");
    Texture2D loseSprite = LoadTexture("assets/lose_sprit.png");
    Texture2D telaWinner = LoadTexture("assets/winner.png");
    Rectangle frame_loseSprite = {0.0f, 0.0f, loseSprite.width / 12, loseSprite.height};
    int loseAtual = 0;

    // Carregando textura de loading
    Texture2D loadingTexture = LoadTexture("assets/loading-bar.png"); // Texture loading

    // Carregando ceu
    Texture2D ceuTexture = LoadTexture("assets/recife_sky.png");
    Vector2 ceuPosicao = {0.0f, 0.0f};

    // Carregando pista
    Texture2D roadTexture = LoadTexture("assets/road2.png");
    Vector2 pistaPosicao = {0.0f, (float)screenHeight - 350};

    // Carregando cidade
    Texture2D cidadeTexture = LoadTexture("assets/recife_city.png");
    Vector2 cidadePosicao = {0.0f, pistaPosicao.y + 135 - cidadeTexture.height + 69};

    // Carregando meio fio
    Texture2D meiofioTexture = LoadTexture("assets/meio_fio2.png");
    Vector2 meioFioPosicao = {0.0f, pistaPosicao.y + roadTexture.height};

    // Carregando moeda
    Texture2D coinTexture = LoadTexture("assets/coin.png");

    // Crregando Carro
    Texture2D carro = LoadTexture("assets/Car-azul.png");
    Rectangle frameCar = {0.0f, 0.0f, (float)carro.width / 2, (float)carro.height}; // Posi????o do frame na sprit do carro
    Vector2 carroPosicaoInicial = {screenWidth / 2 - carro.width / 2, pistaPosicao.y + roadTexture.height - 60 - saltoCarroVertical - 1};
    Vector2 carroPosicao = carroPosicaoInicial;

    // Carregando camel??
    Texture2D camelo = LoadTexture("assets/camelo.png");
    Vector2 cemeloPosicao = {screenWidth - camelo.width - 10, meioFioPosicao.y - camelo.height / 2 + 30};

    // Posi????o na tela da sprit
    // Vector2 position = {306.0f, 170.0f};
    // posi????o da quadro da sprit
    // Rectangle frameRec = {0.0f, 0.0f, (float)loadingTexture.width / 48, (float)loadingTexture.height};

    // Carregar textura obstaculos

    Texture2D brt = LoadTexture("assets/brt_sozinho.png");
    Texture2D buraco = LoadTexture("assets/hole.png");
    Texture2D tubarao = LoadTexture("assets/tubarao.png");
    Texture2D obstaculos[4] = {brt, buraco, tubarao, coinTexture};

    // Menssagem da Logo
    int contador_logo = 0;
    char menssagem[] = "Reypedrof ?? um aluno muito esfor??ado. Finalmente concluiu seu curso de \nFinan??as e decidiu quitar de vez a d??vida com 'LK, o inimigo do trabalho',\nadquirida durante uma das aulas de COMO FUGIR DE UM AGIOTA. Mas ele\nesqueceu o hor??rio e teve que sair ??s pressas de casa, ent??o jogou \nas notas de R$ 100, 00 na mala do carro e saiu correndo do bairro da\nTorre para se encontrar com LK no ponto marcado : o Recife antigo.\nChegando l??... Oh n??o! Nosso her??i percebeu que deixou a mala \naberta e todo o dinheiro se espalhou pelas ruas de Recife!\nAgora, ter?? que voltar dirigindo na contram??o enquanto tenta \nrecuperar a quantia. \n\nA vida de Reypedrof est?? em risco, tome cuidado com os obst??culos\ne ajude-o a resgatar o dinheiro o mais r??pido poss??vel!\n                         Pressione ENTER para iniciar";

    // Frame atual
    int frameAtual = 0;
    int frameAtualCarro = 0;
    // Contador do frame
    int framesCounter = 0;
    int outroContador = 0;
    int framesSpeed = 8;       // n??mero de quadros spritesheet mostrados por segundo
    float velocidadePista = 2; // incrementando com o tempo
    // int velocidadePista = 2;
    float velocidadeParallax = 0.5f;

    // distancia entre inimigos
    int distanciaInimigos = 90;

    // dinheiro
    int dinheiro = 0;
    int metaDinheiro = 5000;

    // saltos do carro

    int limiteBaixo = carroPosicao.y + 2 * saltoCarroVertical;
    int limiteCima = carroPosicao.y - 2 * saltoCarroVertical;
    int limiteEsquerda = carroPosicao.x - 3 * saltoCarroHorizontal;
    int limiteDireita = carroPosicao.x + 3 * saltoCarroHorizontal;
    // Retangulo do carro
    Rectangle carroRetanguloInicial = {carroPosicao.x, carroPosicao.y, frameCar.width, 30};
    Rectangle carroRetangulo = carroRetanguloInicial;

    // Tela atual
    TelaGame telaAtual = LOGO;
    // TelaGame telaAtual = GAMEPLAY;

    int contador_frame = 0;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        UpdateMusicStream(intro); // Refill music stream buffers (if required)
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;
            if (pause)
                // PauseSound(intro);
                StopMusicStream(intro);
            else
            {
                // ResumeSound(intro);
                PlayMusicStream(intro);
            }
        }

        switch (telaAtual)
        {
        case LOGO:
            contador_logo++;
            contador_frame++;
            // depois de 5.5 segundos vai pro menu
            // if (contador_frame > 5.5 * 60)
            // {
            //     telaAtual = MENU;
            // }
            break;

        case MENU:
            if (IsKeyPressed(KEY_ENTER))
            {
                // INICIA OU REINICIALIZA
                // OBSTACULOS
                for (int i = 0; i < Inimigo_Amount; i++)
                {
                    inimigoAleatorio(inimigo, obstaculos, i);

                    if (i == 0)
                        inimigo[i].rec.x = screenWidth;
                    else
                        inimigo[i].rec.x = inimigo[i - 1].rec.x + inimigo[i - 1].rec.width + distanciaInimigos;

                    inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                    if (i > 0)
                        while (inimigo[i].rec.y == inimigo[i - 1].rec.y)
                            inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;

                    inimigoAleatorio(inimigo, obstaculos, i);

                    inimigo[i].rec.height = 30;
                    inimigo[i].TeveColisao = 0;
                }
                // Frame atual
                frameAtual = 0;
                frameAtualCarro = 0;
                // Contador do frame
                framesCounter = 0;
                outroContador = 0;
                framesSpeed = 8;     // n??mero de quadros spritesheet mostrados por segundo
                velocidadePista = 2; // incrementando com o tempo
                // int velocidadePista = 2;
                velocidadeParallax = 0.5f;

                carroRetangulo = carroRetanguloInicial;

                // dinheiro
                dinheiro = 0;

                carroPosicao = carroPosicaoInicial;

                telaAtual = GAMEPLAY;
            }
            break;
        case GAMEPLAY:

            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                frameAtual++;
                frameAtualCarro++;

                if (frameAtual > 50)
                {
                    frameAtual = 0;
                }

                if (frameAtualCarro > 2)
                {
                    frameAtualCarro = 0;
                }

                // frameRec.x = (float)frameAtual * (float)loadingTexture.width / 48;
                frameCar.x = (float)frameAtualCarro * (float)carro.width / 2;
            }

            if (IsKeyPressed(KEY_DOWN) && carroPosicao.y < limiteBaixo)
            {
                carroPosicao.y += saltoCarroVertical;
                carroRetangulo.y += saltoCarroVertical;
            }
            if (IsKeyPressed(KEY_UP) && carroPosicao.y > limiteCima)
            {
                carroPosicao.y -= saltoCarroVertical;
                carroRetangulo.y -= saltoCarroVertical;
            }
            if (IsKeyPressed(KEY_RIGHT) && carroPosicao.x < limiteDireita)
            {
                carroPosicao.x += saltoCarroHorizontal;
                carroRetangulo.x += saltoCarroHorizontal;
            }
            if (IsKeyPressed(KEY_LEFT) && carroPosicao.x > limiteEsquerda)
            {
                carroPosicao.x -= saltoCarroHorizontal;
                carroRetangulo.x -= saltoCarroHorizontal;
            }

            for (int i = 0; i < Inimigo_Amount; i++)
            {

                inimigo[i].rec.x -= velocidadePista;

                if (inimigo[Inimigo_Amount - 1].rec.x <= screenWidth - inimigo[Inimigo_Amount - 1].rec.width - distanciaInimigos)
                {

                    if (i == 0 && inimigo[i].rec.x + brt.width <= 0)
                    {

                        inimigoAleatorio(inimigo, obstaculos, i);
                        inimigo[i].rec.x = screenWidth;
                        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                        inimigo[i].TeveColisao = 0;

                        while (inimigo[i].rec.y == inimigo[Inimigo_Amount - 1].rec.y)
                            inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                    }

                    else if (i > 0 && inimigo[i].rec.x + brt.width <= 0)
                    {

                        inimigoAleatorio(inimigo, obstaculos, i);
                        inimigo[i].rec.x = inimigo[i - 1].rec.x + inimigo[i - 1].rec.width + distanciaInimigos;
                        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                        inimigo[i].TeveColisao = 0;

                        while (inimigo[i].rec.y == inimigo[i - 1].rec.y)
                            inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                    }
                }
            }
            // Checando colis??es
            for (int i = 0; i < Inimigo_Amount; i++)
            {

                if (CheckCollisionRecs(inimigo[i].rec, carroRetangulo))
                {
                    if (inimigo[i].obstaculoTipo == 0)
                    {
                        printf("\nTEVE COLISAO BRT\n");
                        telaAtual = FIM;
                        PlaySound(explosao);
                    }
                    else if (inimigo[i].obstaculoTipo == 1)
                    {
                        printf("\nTEVE COLISAO BURACO\n");
                        telaAtual = FIM;
                        PlaySound(explosao);
                    }
                    else if (inimigo[i].obstaculoTipo == 2)
                    {
                        printf("\nTEVE COLISAO TUBARAO\n");
                        telaAtual = FIM;
                        PlaySound(explosao);
                    }
                    else if (inimigo[i].obstaculoTipo == 3 && inimigo[i].TeveColisao == 0)
                    {

                        inimigo[i].TeveColisao = 1;
                        printf("\nMoeda\n");
                        dinheiro += 100;
                        PlaySound(cashSound);
                    }
                }
            }

            // iNCREMENTANDO Velocidade
            outroContador++;
            if (outroContador >= (60 * 64) / framesSpeed)
            {
                outroContador = 0;
                if (velocidadePista < MAX_VELOCIDADE)
                {
                    velocidadePista = velocidadePista + 0.5;
                }
                else
                {
                    velocidadePista = MAX_VELOCIDADE;
                }
            }

            if (dinheiro == metaDinheiro)
            {
                telaAtual = FIM;
                PlaySound(winner);
            }

            break;
        case FIM:

            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                loseAtual++;

                if (loseAtual > 12)
                {
                    loseAtual = 0;
                }

                frame_loseSprite.x = (float)loseAtual * (float)loseSprite.width / 12;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                telaAtual = MENU;
            }

            break;

        default:
            break;
        }
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        pistaPosicao.x -= velocidadePista;
        if (pistaPosicao.x <= -roadTexture.width)
            pistaPosicao.x = 0;

        meioFioPosicao.x -= velocidadePista;
        if (meioFioPosicao.x <= -meiofioTexture.width)
            meioFioPosicao.x = 0;

        cemeloPosicao.x -= velocidadePista;
        if (cemeloPosicao.x <= -meiofioTexture.width / 2)
            cemeloPosicao.x = screenWidth;

        ceuPosicao.x -= velocidadeParallax;
        if (ceuPosicao.x <= -ceuTexture.width)
            ceuPosicao.x = 0;

        cidadePosicao.x -= velocidadeParallax;
        if (cidadePosicao.x <= -cidadeTexture.width)
            cidadePosicao.x = 0;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
        switch (telaAtual)
        {
        case LOGO:
            // DrawText("Carregando o Jogo Peste", 200, 120, 35, DARKGREEN);
            // DrawTextureRec(loadingTexture, frameRec, position, RAYWHITE);
            DrawText(TextSubtext(menssagem, 0, contador_logo / 2), 30, 30, 20, BLUE);
            if (IsKeyPressed(KEY_ENTER))
            {
                telaAtual = MENU;
            }
            break;

        case MENU:
            DrawTexture(textura, screenWidth / 2 - textura.width / 2, screenHeight / 2 - textura.height / 2, WHITE);
            DrawText("Pressione ENTER para iniciar", 100, 50, 40, RAYWHITE);
            break;
        case GAMEPLAY:
        {

            DrawTextureEx(ceuTexture, ceuPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(ceuTexture, (Vector2){ceuTexture.width + ceuPosicao.x, 0.0f}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(cidadeTexture, cidadePosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(cidadeTexture, (Vector2){cidadeTexture.width + cidadePosicao.x, pistaPosicao.y + 135 - cidadeTexture.height + 69}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(roadTexture, pistaPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(roadTexture, (Vector2){roadTexture.width + pistaPosicao.x, screenHeight - 350}, 0.0f, 1.0f, WHITE);

            DrawTextureRec(carro, frameCar, carroPosicao, WHITE);
            DrawRectangleRec(carroRetangulo, (Color){0, 0, 0, 0});
            // DrawRectangleRec(carroRetangulo, WHITE);

            for (int i = 0; i < Inimigo_Amount; i++)
            {
                DrawRectangleRec(inimigo[i].rec, (Color){0, 0, 0, 0});
                // DrawRectangleRec(inimigo[i].rec, WHITE);
                if (inimigo[i].obstaculoTipo == 0)
                    DrawTexture(brt, inimigo[i].rec.x, inimigo[i].rec.y - 36, WHITE);
                else if (inimigo[i].obstaculoTipo == 1)
                    DrawTexture(buraco, inimigo[i].rec.x, inimigo[i].rec.y + 6, WHITE);
                else if (inimigo[i].obstaculoTipo == 2)
                    DrawTexture(tubarao, inimigo[i].rec.x, inimigo[i].rec.y - 6, WHITE);
                else if (inimigo[i].obstaculoTipo == 3 && !inimigo[i].TeveColisao)
                {
                    // DrawRectanglRec(inimigo[i].rec, BLUE);
                    DrawTexture(coinTexture, inimigo[i].rec.x, inimigo[i].rec.y + 12, WHITE);
                }
            }

            DrawTextureEx(meiofioTexture, meioFioPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(meiofioTexture, (Vector2){meiofioTexture.width + meioFioPosicao.x, pistaPosicao.y + roadTexture.height}, 0.0f, 1.0f, WHITE);
            DrawTextureEx(camelo, cemeloPosicao, 0.75f, 1.0f, WHITE);

            DrawText(TextFormat("Dinheiro: R$ %i,00 / %i", dinheiro, metaDinheiro), screenWidth - 300, 20, 20, WHITE);
            break;
        }
        case FIM:
            if (dinheiro == metaDinheiro)
            {
                DrawTexture(telaWinner, 0.0, 0.0, WHITE);
                // DrawText("Parabens", 0, 0, 40, RED);
            }
            else
            {
                DrawTextureRec(loseSprite, frame_loseSprite, (Vector2){screenWidth / 2 - frame_loseSprite.width / 2, screenHeight / 2 - loseSprite.height / 2}, WHITE);
                DrawTexture(loseTelaFinal, screenWidth / 2 - loseTelaFinal.width / 2, 20, WHITE);
                DrawText("Pressione ENTER para jogar de novo", 85, 240, 35, DARKBLUE);
            }
            break;
        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(textura);
    UnloadTexture(loadingTexture);
    UnloadTexture(roadTexture);
    UnloadTexture(ceuTexture);
    UnloadTexture(cidadeTexture);
    UnloadTexture(brt);
    UnloadTexture(coinTexture);
    UnloadTexture(loseTelaFinal);
    UnloadTexture(loseSprite);
    UnloadTexture(carro);
    UnloadTexture(meiofioTexture);
    UnloadTexture(buraco);
    UnloadTexture(tubarao);
    UnloadTexture(telaWinner);
    UnloadTexture(camelo);
    // UnloadSound(intro);
    UnloadMusicStream(intro);
    UnloadSound(cashSound);
    UnloadSound(explosao);
    UnloadSound(winner);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void inimigoAleatorio(Inimigo inimigo[], Texture2D obstaculos[], int indice)
{

    int random;

    random = GetRandomValue(1, 4);

    if (random == 1)
    {

        inimigo[indice].rec.width = obstaculos[0].width;
        inimigo[indice].obstaculoTipo = 0;
    }
    else if (random == 2)
    {

        inimigo[indice].rec.width = obstaculos[1].width;
        inimigo[indice].obstaculoTipo = 1;
    }
    else if (random == 3)
    {

        inimigo[indice].rec.width = obstaculos[2].width;
        inimigo[indice].obstaculoTipo = 2;
    }
    else if (random == 4)
    {
        inimigo[indice].rec.width = obstaculos[3].width;
        inimigo[indice].obstaculoTipo = 3;
    }
}