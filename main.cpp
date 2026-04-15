//pong
#include "include/raylib.h"
#include <iostream>
#include <string>
#include <vector>
#define pass 0
using Rector = std::vector<Rectangle>; //a vector containing multiple rectangle (basically player paddle rect and 2 half rects)
//class player
class Player{
    // private:
    public:
        float size_x = 20;
        Color plColor = {255,255,255,255};
        float x = 30, y = 360;
        float size_y = 100;
        // 2 half rects = paddle
        // first half from midpoint will send the ball upward i.e. dx = -dx and dy => -5 (because at any angle the ball hits, it reflects in the direction of origin)
        // second half from midpoint => ball downward ie dx = -dx and dy => +5 (ball goes away from normal)
        const int ySpeed = 5; // prototyping const

        // Half rects
        float half_height = size_y/2; // these will be 
        // float half1_ycoord = y;
        float half2_ycoord = y+half_height; // yCoord of 1st half + height of 1st half = top left y coord of second half
        Rector getRect(){
            Rectangle playerRect = {x,y,size_x,size_y};
            Rectangle halfrec1 = (Rectangle){x,y,size_x,half_height};
            Rectangle halfrec2 = (Rectangle){x,y+(size_y/2),size_x,half_height};
            Rector playerRecs = {playerRect,halfrec1,halfrec2};
            return playerRecs;
        }
        void Draw(){
            Rector Recs = getRect();
            Rectangle halfrec1 = Recs[1];
            Rectangle halfrec2 = Recs[2];
            DrawRectangleRounded(Recs.at(0),10,100,plColor);
            // DrawRectangleRec(halfrec1,RED);
            // DrawRectangleRec(halfrec2,BLUE);
        }
};
//class ball
class Ball{
    public: 
        float ballRadius = 20;
        float x = 590, y =  360; //init
        float dx = 5.0f, dy = 8.0f;
        Vector2 getPos(){
            return (Vector2){x,y};
        }
        void Draw(){
            DrawCircleV({x,y},ballRadius,BLUE);
        }
};

// GAME OBJECTS AND VARIABLES
bool Start = false;
Player player1 = Player();
Player player2 = Player();
Ball ball = Ball();
Rectangle separator = (Rectangle){595,0,5,720};
Rectangle starButton = (Rectangle){400,400,400,120};
bool wait; //for timer purposes
int FPS = 0;
bool Collided = false; //true when ball collides with paddle and only turns false when the ball reaches separator
int yScore = 0, xScore = 0;
float timer = 0.0f;
bool xWon = false, yWon = false;
Color againColor = BLACK;

int main(){
SetConfigFlags(FLAG_WINDOW_HIDDEN);
InitWindow(1200,720,"PONGo !");
SetTargetFPS(90);
ClearWindowState(FLAG_WINDOW_HIDDEN);
player2.x = 1140.0f ;

// main screen
Color startColor = {255,0,76,255};
while (!WindowShouldClose()){
    Vector2 mouse_pos = GetMousePosition();
    BeginDrawing(); ClearBackground(BLACK);
    
    if (CheckCollisionPointRec(mouse_pos,starButton)){startColor = {255,0,76,150};
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Start=true; break;
        }
    }
    else startColor = {255,0,76,255};
    DrawText("PONGo !",400,100,100,{255,0,76,255});
    DrawText("2 Player Co-op",350,200,60,{60,60,60,255});
    DrawRectangleRounded(starButton,0.5,10,startColor);
    DrawText("START !",460,430,64,{0,0,0,255});
    EndDrawing();
}
if (!Start) {CloseWindow(); return 0;}

while (!WindowShouldClose()){
    Vector2 mouse_pos = GetMousePosition();
    BeginDrawing(); ClearBackground({0,0,0,255});
    /* divider */ DrawRectangleRec(separator,{20,20,20,255});
    FPS = GetFPS();
    if (Start){
        timer += 1/(float)FPS;
        DrawRectangle(560,195,120,70,BLACK);
        DrawText("GO !",560,200,64,RAYWHITE);
        ball.x = 600;
        ball.y = 360;
        if (timer >= 1.0f){
            Start = false;
            timer = 0.0f;
        }
    }
    //KEY STATE HANDLING
    if (IsKeyDown(KEY_S)) player1.y += player1.ySpeed;
    else if (IsKeyDown(KEY_W)) player1.y -= player1.ySpeed;

    if(IsKeyDown(KEY_DOWN)) player2.y += player2.ySpeed;
    else if (IsKeyDown(KEY_UP)) player2.y -= player2.ySpeed;

    // UPDATE STATE
    if (CheckCollisionCircleRec(ball.getPos(),ball.ballRadius,separator)) Collided = false;
    if (player1.y <= 10) player1.y = 10;
    else if (player1.y >= 710 - player1.size_y) player1.y = 710 - player1.size_y ; // 710 = screen height (720 px) - slab_size_y (ini. 60px) - y_padding (10 px)

    if (player2.y <= 10) player2.y = 10;
    else if (player2.y >= 710 - player2.size_y) player2.y = 710 - player2.size_y;

    // ball.dx is added with or subt. with 0.2 based on which paddle is collides with
    // player A paddle => dx was negative, so subtract and multiply with -1 to make positive ;vice versa
    if (!Collided && CheckCollisionCircleRec(ball.getPos(), ball.ballRadius, player1.getRect().at(1))){
        if (ball.dy > 0 ) {
            ball.dy *= -1;
            ball.dy -= 0.1;
        } else ball.dy -= 0.1;

        ball.dx -= 0.2; //(ball.dx is negative in this direction so subtract before negation)
        ball.dx *= -1;
        Collided = true;
    } else if (!Collided && CheckCollisionCircleRec(ball.getPos(), ball.ballRadius, player1.getRect().at(2))){
        if (ball.dy < 0){
            ball.dy *= -1;
            ball.dy += 0.1;
        } else ball.dy += 0.1;

        ball.dx -= 0.2;
        ball.dx *= -1; 
        Collided = true;
    }

    if (!Collided && CheckCollisionCircleRec(ball.getPos(), ball.ballRadius, player2.getRect().at(1))){
        if (ball.dy > 0 ) {
            ball.dy *= -1;
            ball.dy -= 0.1;
        } else ball.dy -= 0.1;
        ball.dx += 0.2;
        ball.dx *= -1;
        Collided = true;
    } else if (!Collided && CheckCollisionCircleRec(ball.getPos(), ball.ballRadius, player2.getRect().at(2))){
        if (ball.dy < 0){
            ball.dy *= -1;
            ball.dy += 0.1;
        } else ball.dy += 0.1;
        ball.dx += 0.2;
        ball.dx *= -1;
        Collided = true;
    }

    //speed control
    if (ball.dy >= 15) ball.dy = 15;
    else if (ball.dy <= -15) ball.dy = -15;

    if (ball.dx >= 10) ball.dx = 10;
    else if(ball.dx <= -10) ball.dx = -10;

    // SCORING SYSTEM:
    if (ball.x <= 0){
        player1.y = player2.y = 360;
        yScore += 1;
        ball.dx = 6;
        ball.dy = 8;
        ball.x = 600; ball.y = 360; ball.dx *= -1;
        wait = true;
    }
    if (ball.x >= 1200){
        player1.y = player2.y = 360;
        xScore += 1;
        ball.dx = 6;
        ball.dy = 8;
        ball.x = 600; ball.y = 360; ball.dx *= -1;
        wait = true;
    }
    
    if (xScore == 10){
        wait = true;
        xWon = true;
    } else if (yScore == 10){
        yWon = true;
        wait = true;
    }

    // win screen
    if (wait && (xWon || yWon)){
        player1.y = player2.y = 360;
        ball.x = 600; ball.y = 360;
        if (xWon){
            DrawRectangle(560,195,120,70,BLACK);
            DrawText("PLAYER A WINS !",300,200,64,RAYWHITE);
        }
        if (yWon){
            DrawRectangle(560,195,120,70,BLACK);
            DrawText("PLAYER B WINS !",300,200,64,RAYWHITE);
        }

        Rectangle Again = {400,400,400,120};
        if (CheckCollisionPointRec(mouse_pos,Again)){
            againColor = {20,20,20,255};
            if (IsMouseButtonPressed(0)){
                wait = false;
                Start = true;
                xWon = yWon = false;
                timer = 0;
                xScore = yScore = 0;
            }
        } else againColor = BLACK;
        DrawRectangleRec(Again,againColor);
        DrawRectangleRoundedLines(Again,0.5,10,WHITE);
        DrawText("AGAIN ?",460,430,64,{WHITE});
    }

    //wait screen
    if (wait && !(xWon || yWon)){
        timer += 1/(float)FPS;
        DrawRectangle(560,195,120,70,BLACK);
        DrawText("GO !",560,200,64,RAYWHITE);
        ball.x = 600;
        ball.y = 360;
    }

    if (wait && timer >= 1.0f){wait = false; timer = 0.0f;}

    if (ball.y <= 20 || ball.y >= 700) ball.dy *= -1;
    ball.x += ball.dx; ball.y += ball.dy;
    
    // DRAW ELEMENTS
    /* fps */ DrawText(("FPS : "+std::to_string(FPS)).c_str(),10,690,28,{70,70,70,255);
    
    player1.Draw();
    player2.Draw();
    ball.Draw();

    DrawText(("PLAYER A :  "+std::to_string(xScore)).c_str(),180,20,40,RAYWHITE);
    DrawText(("PLAYER B :  "+std::to_string(yScore)).c_str(),750,20,40,RAYWHITE);
    EndDrawing();
}

CloseWindow();
return 0;
}
//lol
