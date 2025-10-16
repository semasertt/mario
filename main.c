#include "raylib.h"
#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
#include<stdio.h>

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool active;
} Coin;

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool active;
    float speed;
    float minPos;
    float maxPos;
} Enemy;

typedef struct {
    Vector2 mushrooms;
    Texture2D mushTexture;
    bool active1;
} Mush;

typedef struct {
    float x;
    float y;
    float width;
    float height;
}position_s;

void InitObjects(Rectangle*, int, int, position_s*, int, int);
void InitCoins(Coin coins[], int, int, position_s objects_s[], int, int b);
void InitMush(Mush mush[], int, int, position_s objects_s[], int, int);  //mush initleme
void DrawObjects(Rectangle*, Texture2D, int);
void UpdatePlayerMovement(Rectangle*, Rectangle*, Vector2*, bool*, int*, int, int*, int*, int*, Texture2D, const float, float, Music, Rectangle obstacles[]);
void UpdateCoins(Coin*, Rectangle, int*);
void UpdateMush(Mush*, Rectangle, int*, int*);
void ResetCoins(Coin coins[], int);
void ResetMush(Mush mush[], int);
void MyUpdateCamera(Camera2D*, Rectangle, int, int, int);
void InitEnemies(Enemy enemies[], int, int, position_s enemyPositions[], int, int);
void DrawEnemies(Enemy enemies[], int);
void UpdateEnemyMovement(Enemy*, int);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const float gravity = 800.0f;
    const float jumpSpeed = 450.0f;
    const int worldWidth = 18000; // World genişliği
    const int groundHeight = 342; // Zemin yüksekliği

    InitWindow(screenWidth, screenHeight, "SUPER MANO BROS");

    InitAudioDevice();
    Music music = LoadMusicStream("mario.mp3");
    Music music2 = LoadMusicStream("jump.mp3");
    PlayMusicStream(music);


 position_s objects_s[112] = { {1940, 400 , 130 , 20},{2140, 350, 130 , 20},{2340, 300, 100 , 20},{3380, 400, 130 , 20},{3630, 300, 130 , 20},
                     {7800,410,130,20},{8060,360,130,20},{9300,350,130,20},{9600,310,130,20},{9900,270,130,20},{10700,350,130,20},{11000,300,130,20},{11300,250,130,20},{11600,250,130,20},{11900,250,130,20},{12200,350,130,20},
                       {3110, 440, 60 , 80},{3900, 440, 60 , 80},{4780 ,440, 60 , 80},{8500,440,60,80},{7510,440,60,80},{9048,440,60,80},{10100,440,60,80},{12750,440,60,80},{13000,440,60,80},{13250,440,60,80},{7200,440,60,80},
                       {0, 558, 1380 , 48},{1380, 558, 1380 , 48},{2950, 558, 1380 , 48},{4500, 558, 1380 , 48},{5820, 558, 1380 , 48},{7200, 558, 1380 , 48},{8800, 558, 1380 , 48},{10400, 558, 1380 , 48},{11700,558,1380,48},{12900,558,1380,48},{14200 ,558,1380,48},
                       {1990, 350},{3270, 515},{2390, 250},{3430, 350},{3680, 515},{7850,360},{8110,310},{9350,300},{9650,515},{9950,220},{11350,200},{11650,200},{11950,200},
                         {5000,515},{5060,515},{5120,515},{5180,515},{5240,515},{5300,515},{5000,455},{5060,455},{5120,455},{5180,455},{5240,455},{5300,455},
                         {5000,395},{5060,395},{5120,395},{5180,395},{5240,395},{5300,395},{5000,335},{5060,335},{5120,335},{5180,335},{5240,335},{5300,335},
                         {13600,515},{13660,515},{13720,515},{13780,515},{13840,515},{13900,515},{13600,455},{13660,455},{13720,455},{13780,455},{13840,455},{13900,455},
                         {13600,395},{13660,395},{13720,395},{13780,395},{13840,395},{13900,395},{13600,335},{13660,335},{13720,335},{13780,335},{13840,335},{13900,335},
                         {2180,280},{3630,230},{9600,240},{11250,500},
                       {3180 , 515, 3170, 3850},{9130 , 515, 9120, 10040},{10030 , 515,  9120, 10040},{10800 , 515, 10700, 11450},
                       {4790, 395, 60 , 80},{7520, 395, 60 , 80},{12760 ,395, 60 , 80},{13010,395,60,80},{13260,395,60,80} };



    Rectangle obstacles[16];
    Rectangle customObstacles[11];
    Rectangle obstacles_g[11];
    Rectangle plant[5];

    Texture2D obstacleTexture = LoadTexture("engel.png"); // Dikdörtgen engel resmini yükle
    Texture2D customObstacleTexture = LoadTexture("boru.png"); // Özel engel resmini yükle
    Texture2D obstacle_gTexture = LoadTexture("yer1.png");
    Texture2D background = LoadTexture("back.png");
    Texture2D background2 = LoadTexture("back1.png");
    Texture2D startTexture = LoadTexture("start.png"); // Resmi yükle
    Texture2D sembol = LoadTexture("sembol.png");
    Texture2D prenses = LoadTexture("siteprens1.png"); //prenses sonda kalenin yanında
    Texture2D gameoverTexture = LoadTexture("gameover.png"); // düşman yükle
    Texture2D plantTexture = LoadTexture("siteplant.png");

    Enemy enemies[4]; // Düşman dizisi
    enemies->active = true;
    enemies->texture = LoadTexture("dusman1.png");

    InitObjects(obstacles, screenWidth, groundHeight, objects_s, 16, 0);           //engelleri oluştur
    InitObjects(customObstacles, screenWidth, groundHeight, objects_s, 11, 16);   // boruları oluştur
    InitObjects(obstacles_g, screenWidth, groundHeight, objects_s, 11, 27);       //yerleri oluştur
    InitEnemies(enemies, screenWidth, groundHeight, objects_s, 4, 103);
    InitObjects(plant, screenWidth, groundHeight, objects_s, 5, 107);

    Coin coins[61];
    InitCoins(coins, screenWidth, groundHeight, objects_s, 61, 38);
    int coinCount = 0; // Coin sayısı

    Mush mush[4];
    InitMush(mush, screenWidth, groundHeight, objects_s, 4, 99);
    int mushCount = 0; // mush sayısı

    Rectangle mario = {30, 0, 60, 130};
    Texture2D marioTexture = LoadTexture("frame.png");
    Rectangle frameRec = { 0.0f, 0.0f, (float)marioTexture.width / 12, (float)marioTexture.height };
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;

    Camera2D camera = { 0 };  // Kamera oluştur
    camera.target = (Vector2){ screenWidth / 2, screenHeight / 2 }; // Kamerayı ekranın ortasına yerleştir
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 }; // Kamerayı ekranın ortasına yerleştir
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    //Mario özellikleri
    Vector2 marioVelocity = { 0 };
    bool canJump = true;
    int jumpCount = 0;
    const int maxJumpCount = 2;
    int health = 3;
    float imageSpeed = 2.0f;
    bool movingRight = true;
    bool isGameOver = false;
    Vector2 deathPosition = {0, 0};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {


        UpdateMusicStream(music);
        UpdateMusicStream(music2);

        // Müziğin ne kadar süre çaldığını kontrol et
        if (GetMusicTimePlayed(music2) > 2.0f) {
            // 2 saniye sonra müziği durdur
            StopMusicStream(music2);
        }

        // Coin'leri güncelle
        UpdateCoins(coins, mario, &coinCount);
        // mantarı güncelle
        UpdateMush(mush, mario, &mushCount, &health);
        // Düşmanları güncelle
        UpdateEnemyMovement(enemies, 4);
        // mario hareket
        UpdatePlayerMovement(&mario, &frameRec, &marioVelocity, &canJump, &jumpCount, maxJumpCount, &framesCounter,
            &currentFrame, &framesSpeed, marioTexture, gravity, jumpSpeed, music2, obstacles);

        // Oyuncu/Mario'nun engellerle çarpışma algılama
        for (int i = 0; i < 16; i++) {
            if (CheckCollisionPointRec((Vector2) { mario.x + mario.width / 2, mario.y + mario.height }, obstacles[i])) {
                marioVelocity.x = 0;
                marioVelocity.y = 0;
                mario.y = obstacles[i].y - mario.height;
                canJump = true;
                jumpCount = 0;
            }
            if (CheckCollisionRecs((Rectangle) { mario.x, mario.y, mario.width, mario.height }, obstacles[i]) && marioVelocity.y < 0) {
                marioVelocity.y = 0;
                mario.y = obstacles[i].y  ;
            }
        }

        // Oyuncu/Mario'nun yerr ile çarpışma algılama
        for (int i = 0; i < 11; i++) {
            if (CheckCollisionPointRec((Vector2) { mario.x + mario.width / 2, mario.y + mario.height }, obstacles_g[i])) {
                // Engelle çarpışma durumunda, oyuncunun x ve y hızını sıfırla
                marioVelocity.x = 0;
                marioVelocity.y = 0;

                // Oyuncuyu/Mario'yu engelin üzerine yerleştir
                mario.y = obstacles_g[i].y - mario.height;
                // Oyuncu/Mario engelin üzerindeyse zıplama izni ver
                canJump = true;
                // Zıplama sayacını sıfırla
                jumpCount = 0;
            }
        }
        bool isOnTop = false;

        // Mario'nun üstünde bir çarpışma var mı kontrol et
        for (int i = 0; i < 11; i++) {
            if ((mario.x + mario.width >= customObstacles[i].x && mario.x <= customObstacles[i].x + customObstacles[i].width) &&
                (mario.y + mario.height >= customObstacles[i].y && mario.y <= customObstacles[i].y + customObstacles[i].height)) {

                // Mario'nun üstünde mi kontrol et
                if (mario.y + mario.height <= 450) {
                    isOnTop = true;
                }
            }
        }

        // Mario'nun üstünde ise, hareket ve zıplama yeteneği sağla
        if (isOnTop) {
            // Mario'nun y eksenindeki hızını sıfırla, böylece yere düşemez
            marioVelocity.y = 0;

            // Oyuncuyu/Mario'yu engelin üzerine yerleştir
            for (int i = 0; i < 11; i++) {
                mario.y = customObstacles[i].y - mario.height;
            }

            // Mario'ya zıplama yeteneği ver
            if (IsKeyPressed(KEY_SPACE)) {
                marioVelocity.y = -jumpSpeed; // Zıplama yap
                canJump = false;
                jumpCount++;
            }
        }
        else {
            // Mario'nun üstünde değilse, dışarıya çıkar
            for (int i = 0; i < 11; i++) {
                if ((mario.x + mario.width >= customObstacles[i].x && mario.x <= customObstacles[i].x + customObstacles[i].width) &&
                    (mario.y + mario.height >= customObstacles[i].y && mario.y <= customObstacles[i].y + customObstacles[i].height)) {

                    // Mario'nun x hızını sıfırla, böylece ileri gidemez
                    marioVelocity.x = 0;

                    // Mario'nun borunun dışına çıkmasını sağla
                    if (mario.x < customObstacles[i].x + customObstacles[i].width / 2) {
                        mario.x = customObstacles[i].x - mario.width;
                    }
                    else {
                        mario.x = customObstacles[i].x + customObstacles[i].width;
                    }
                }
            }
        }
        // Mario ile custom engellerin nokta çarpışmasını kontrol edin
       for (int i = 0; i < 5; i++) {
           if (CheckCollisionPointRec((Vector2) { mario.x + mario.width / 2, mario.y + mario.height }, plant[i])) {
               // Çarpışma durumunda canı 1 azaltın
               health--;
               // Mario'nun konumu custom engelden büyükse x konumu + 50, değilse -50 olsun
               if (mario.x > plant[i].x) {
                   mario.x += 60;
               }
               else {
                   mario.x -= 60;
               }
           }
       }

       for (int i = 0; i < 4; i++) {
            Rectangle enemyRect = { enemies[i].position.x, enemies[i].position.y, 55, 55 };
            if (CheckCollisionRecs(mario, enemyRect)) {
            // Düşmanla çarpışma durumunda, sağlık puanını azalt
            health--;
            // Mario'nun başlangıç konumuna geri dön
            if (mario.x > enemies[i].position.x) {
                   mario.x += 50;
               }
               else {
                   mario.x -= 50;
               }
            }
        }


        // Oyuncu/Mario'yu ve kamerayı dünya sınırlarında tutma
        if (mario.x < 0) mario.x = 0;
        else if (mario.x > worldWidth - marioTexture.width) mario.x = worldWidth - marioTexture.width;


        // Döngü içinde
        if (0 <= mario.x && mario.x <= 1100) {
                marioVelocity.x = 0;
                marioVelocity.y = 0;
                if (IsKeyPressed(KEY_ENTER)){
                    // Mario ve kamerayı doğrudan 1200 konumuna taşı
                    camera.target.x = 1600 + screenWidth / 2;
                    mario.x = 1600;
                    }
                }
                // Döngü içinde
        if (5700 <= mario.x && mario.x <= 5800) {
                mario.x = 6300;
            }
                        // Döngü içinde
        if (1100 <= mario.x && mario.x <= 1150) {
                mario.x = 1600;
            }



        if(mario.y > 600){
            health = health -1;
            if(mario.x >5700 && mario.x < 14700){
                mario.x = 6300;
                mario.y = 47;
            }
            else{
                mario.x = 1600;
                mario.y = 47;
            }
        }

        if (health <= 0) {
            isGameOver = true;
            deathPosition.x = mario.x;
            deathPosition.y = mario.y;
            // Kamerayı Game Over ekranına taşı
            camera.target.x = 15000 + screenWidth / 2;
            camera.target.y = screenHeight / 2;
        }
        if (isGameOver && IsKeyPressed(KEY_R)) {
            isGameOver = false;
            health = 3;
            coinCount = 0;
            mushCount = 0;
            ResetCoins(coins,61);
            ResetMush(mush,4);
            if (deathPosition.x > 5700 && deathPosition.x < 14900) {
                mario.x = 6300;
                mario.y = 47;
            } else {
                mario.x = 1600;
                mario.y = 47;
            }
            camera.target.x = mario.x + mario.width / 2;
            camera.target.y = mario.y + mario.height / 2;
        }

        if(mario.x >= 14678){
            mario.x = 30;
            mario.y = 47;
            // Reset
            coinCount = 0;
            mushCount = 0;
            health = 3;
            ResetCoins(coins,61);
            ResetMush(mush,4);
        }

        // Kamera döngüsü
        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(RAYWHITE);

        // Arka planı çiz

        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(background2, 5840, 0, WHITE);
        DrawTexture(background2, 14800, 0, WHITE);
        DrawTexture(background, 9000, 0, WHITE);
        // Engelleri, yerleri ve boruları çiz
        DrawObjects(obstacles, obstacleTexture, 16);
        DrawObjects(obstacles_g, obstacle_gTexture, 11);
        DrawObjects(customObstacles, customObstacleTexture, 11);
        DrawObjects(plant, plantTexture, 5);
        DrawTexture(prenses, 14800, 465, WHITE);


        DrawTexture(sembol, camera.target.x + (screenWidth / 2) - 140, camera.target.y - (screenHeight / 2) + 15, WHITE);
        DrawEnemies(enemies, 4);

        DrawTextureRec(marioTexture, frameRec, (Vector2) { mario.x, mario.y}, WHITE);
        DrawTexture(startTexture, 0, 0, WHITE);
        DrawTexture(gameoverTexture, 15000, 0, WHITE);

        DrawText(TextFormat("%s", "PRESS ENTER TO START"), 250, 300, 20, BLACK);
        DrawText(TextFormat("%s", "LEVEL 1\n\n\n\n\n-------->"), 1240, 130, 90, BLACK);
        DrawText(TextFormat("%s", "LEVEL 1 ----> LEVEL 2"), 6000, 200, 80, BLACK);
        DrawText(TextFormat("%s", "PRESS R FOR RETURN"), 15200, 500, 20, DARKBROWN);

        for (int i = 0; i < 61; i++) {
            if (coins[i].active) {
                DrawTexture(coins[i].texture, coins[i].position.x, coins[i].position.y, WHITE);
            }
        }

        for (int i = 0; i < 4; i++) {
            if (mush[i].active1) {
            DrawTexture(mush[i].mushTexture, mush[i].mushrooms.x, mush[i].mushrooms.y, WHITE);
            }
        }

        EndMode2D();
        // Coin sayısını ekrana yazdır
        DrawText(TextFormat("Coins: %02d", coinCount), GetScreenWidth() - 100, 20, 20, BLACK);

        // mush sayısını ekrana yazdır
        DrawText(TextFormat("Mush: %02d", mushCount), GetScreenWidth() - 100, 66, 20, BLACK);

        //canının azalmasının yazılması
        DrawText(TextFormat("Health: %02d", health), GetScreenWidth() - 100, 43, 20, BLACK);

        // Kamerayı güncelle
        MyUpdateCamera(&camera, mario, worldWidth, screenWidth, screenHeight);

        EndDrawing();
        }

    CloseWindow();
    return 0;
}

// oluşturma fonksiyonu
void InitObjects(Rectangle objects[], int screenWidth, int groundHeight, position_s objects_s[], int count, int b) {
    for (int i = 0; i < count; i++) {
        objects[i].x = objects_s[b].x;
        objects[i].y = objects_s[b].y;
        objects[i].width = objects_s[b].width;
        objects[i].height = objects_s[b].height;

        b++;
    }
}

// coinleri oluşturan fonksiyon
void InitCoins(Coin coins[], int screenWidth, int groundHeight, position_s objects_s[10], int count, int b) {
    for (int i = 0; i < count; i++) {
        coins[i].position.x = objects_s[b].x;
        coins[i].position.y = objects_s[b].y;
        coins[i].texture = LoadTexture("coin2.png");
        coins[i].active = true;
        b++;
    }
}

// mushlari oluşturan fonksiyon
void InitMush(Mush mush[], int screenWidth, int groundHeight, position_s objects_s[], int count, int b) {
    for (int i = 0; i < count; i++) {
        mush[i].mushrooms.x = objects_s[b].x;
        mush[i].mushrooms.y = objects_s[b].y;
        mush[i].mushTexture = LoadTexture("mantar2.png");
        mush[i].active1 = true;
        b++;
    }

}
// Düşmanı oluşturan
void InitEnemies(Enemy enemies[], int screenWidth, int groundHeight, position_s object_s[], int count, int b) {
    for (int i = 0; i < count; i++) {
        enemies[i].position.x = object_s[b].x;  // X pozisyonunu atayın
        enemies[i].position.y = object_s[b].y;  // Y pozisyonunu atayın
        enemies[i].minPos = object_s[b].width;  // Minimum X pozisyonunu atayın
        enemies[i].maxPos = object_s[b].height; // Maksimum X pozisyonunu atayın
        enemies[i].speed = 80.0f;
        b++;
    }
}

void DrawObjects(Rectangle objects[], Texture2D objectsTexture, int count) {
    for (int i = 0; i < count; i++) {
        DrawTexture(objectsTexture, objects[i].x, objects[i].y, WHITE);
    }
}

void DrawEnemies(Enemy enemies[], int count) {
    for (int i = 0; i < count; i++) {
        if (enemies[i].active) {
            DrawTexture(enemies->texture, enemies[i].position.x, enemies[i].position.y, WHITE);
        }
    }
}

void UpdatePlayerMovement(Rectangle* mario, Rectangle* frameRec, Vector2* marioVelocity, bool* canJump,
    int* jumpCount, int maxJumpCount, int* framesCounter, int* currentFrame, int* framesSpeed,
    Texture2D marioTexture, const float gravity, float jumpSpeed, Music music2, Rectangle* obstacles) {
    // Mario hareketi
    if (IsKeyDown(KEY_RIGHT)) {
        mario->x += 4;
        (*framesCounter)++;

        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            (*currentFrame)++;

            if (*currentFrame > 5) *currentFrame = 0;

            frameRec->x = (float)(*currentFrame) * (float)marioTexture.width / 12;
        }

        // Control frames speed
        if (IsKeyPressed(KEY_RIGHT)) (*framesSpeed)++;
        else if (IsKeyPressed(KEY_LEFT)) (*framesSpeed)--;

        if (*framesSpeed > MAX_FRAME_SPEED) *framesSpeed = MAX_FRAME_SPEED;
        else if (*framesSpeed < MIN_FRAME_SPEED) *framesSpeed = MIN_FRAME_SPEED;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        mario->x -= 4;
        (*framesCounter)++;

        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            (*currentFrame)++;

            if (*currentFrame > 11) *currentFrame = 6;

            frameRec->x = (float)(*currentFrame) * (float)marioTexture.width / 12;
        }

        // Control frames speed
        if (IsKeyPressed(KEY_RIGHT)) (*framesSpeed)++;
        else if (IsKeyPressed(KEY_LEFT)) (*framesSpeed)--;

        if (*framesSpeed > MAX_FRAME_SPEED) *framesSpeed = MAX_FRAME_SPEED;
        else if (*framesSpeed < MIN_FRAME_SPEED) *framesSpeed = MIN_FRAME_SPEED;
    }

    // Zıplama
    if ((IsKeyPressed(KEY_SPACE) && *canJump) || (IsKeyPressed(KEY_SPACE) && *jumpCount < maxJumpCount)) {
        marioVelocity->y = -jumpSpeed; // Zıplama yap
        *canJump = false;
        (*jumpCount)++;
        PlayMusicStream(music2);

    }

    // Yerçekimi etkisi
    marioVelocity->y += gravity * GetFrameTime();
    mario->y += marioVelocity->y * GetFrameTime();
}

// Coin'leri kontrol eden fonksiyon
void UpdateCoins(Coin coins[], Rectangle player, int* coinCount) {
    for (int i = 0; i < 61; i++) {
        if (coins[i].active && CheckCollisionPointRec(coins[i].position, player)) {
            coins[i].active = false; // Çarpışma durumunda coin'i etkisiz hale getir
            (*coinCount)++;  // Coin sayısını arttır
        }
    }
}

// mush'leri kontrol eden fonksiyon
void UpdateMush(Mush mush[], Rectangle player, int* mushCount, int* health) {
    for (int i = 0; i < 4; i++) {
        if (mush[i].active1 && CheckCollisionPointRec(mush[i].mushrooms, player)) {
            mush[i].active1 = false; // Çarpışma durumunda mush'i etkisiz hale getir
            (*mushCount)++;  // mush sayısını arttır
            (*health)++;
        }
    }
}

void UpdateEnemyMovement(Enemy enemies[], int count) {
    for (int i = 0; i < count; i++) {
        if (enemies[i].position.x >= enemies[i].maxPos) {
            enemies[i].position.x = enemies[i].maxPos;

            enemies[i].speed *= -1; // Yönü tersine çevir
        }
        else if (enemies[i].position.x <= enemies[i].minPos) {
            enemies[i].position.x = enemies[i].minPos;
            enemies[i].speed *= -1; // Yönü tersine çevir
        }

        // Düşmanı belirli bir hızla hareket ettir
        enemies[i].position.x += enemies[i].speed * GetFrameTime();
    }
}
void ResetCoins(Coin coins[], int count) {
    for (int i = 0; i < count; i++) {
        coins[i].active = true;
    }
}

void ResetMush(Mush mush[], int count) {
    for (int i = 0; i < count; i++) {
        mush[i].active1 = true;
    }
}

void MyUpdateCamera(Camera2D* camera, Rectangle mario, int worldWidth, int screenWidth, int screenHeight) {
    // Kamera hedefini oyuncu/Mario'ya odakla, ancak dünya sınırlarını ihlal etme
    camera->target = (Vector2){ mario.x + mario.width / 2, mario.y + mario.height / 2 };
    if (camera->target.x < screenWidth / 2) camera->target.x = screenWidth / 2;
    else if (camera->target.x > worldWidth - screenWidth / 2) camera->target.x = worldWidth - screenWidth / 2;
    if (camera->target.y < screenHeight / 2) camera->target.y = screenHeight / 2;
    else if (camera->target.y > screenHeight - screenHeight / 2) camera->target.y = screenHeight - screenHeight / 2;
}

