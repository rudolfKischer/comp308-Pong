// Template by: Allan Wei
// allan.wei@mail.mcgill.ca

#include <GL/glut.h>
#include <string>
#include <string.h>

//Represents a point in 2D space
//x and y are in pixels
typedef struct Point{
    int x; //Pixels
    int y; //Pixels
} Point;

//Color struct
//OpenGl can use either float or unsigned char for color values.
//This struct uses GLubyte which is the same as unsigned char.
//Make sure that you are calling the correct color function in OpenGL, when using this struct.
typedef struct Color{
    GLubyte r; //0-255
    GLubyte g; //0-255
    GLubyte b; //0-255
} Color;

//Global variables struct
typedef struct Global{
    Point playerPaddlePosition;
    Point aiPaddlePosition;
    int playerScore;
    int aiScore;
    Point ballPosition;
    int ballSpeed; //Pixels/Frame
    Point ballDirection;
    int lastScore; //0 = player, 1 = ai
    int gameOver; //0 = false, 1 = true
    int introScreen; //0 on intro screen, 1 in game
} Global;
Global global;

//Consts you should not change these values.
#define screenWidth 1920
#define screenHeight 1080
#define paddleOffset 120
#define scoreSize 22
const int aiPaddleSpeed = 30;
const int scoreGap = 50;
const int ballSpeedupFactor = 1;
const int paddleWidth = 40;
const int paddleLength = 200;
const int ballSideLength = 30;
const int initialBallSpeed = 30;
const int scorePosition = screenHeight * 0.9;
const int goalPosition = screenHeight / 2;
const int goalHeight = screenHeight / 3;
const int wallThickness = 20;
const Color paddleColor = (Color){255, 255, 255};
const Point playerScorePosition = (Point){screenWidth - paddleOffset, scorePosition};
const Point aiScorePosition = (Point){paddleOffset - scoreSize, scorePosition};
const Point initialBallPosition = (Point) {screenWidth / 2, screenHeight / 2};
const Point initialBallDirection = (Point) {1, 1};
const Point initialPlayerPaddlePosition = (Point){screenWidth - paddleOffset - paddleWidth, (screenHeight / 2) - paddleLength / 2};
const Point initialAiPaddlePosition = (Point){paddleOffset, (screenHeight / 2) - (paddleLength / 2)};

void initGlobals(){
    //Initializes the global variables
    //They are all under the global struct, and can be access using global.variableName
    //You should not change this function.
    global.playerPaddlePosition = initialPlayerPaddlePosition;
    global.aiPaddlePosition = initialAiPaddlePosition;
    global.playerScore = 0;
    global.aiScore = 0;
    global.ballPosition = initialBallPosition;
    global.ballSpeed = initialBallSpeed;
    global.ballDirection = initialBallDirection;
    global.lastScore = 0;
    global.gameOver = 0;
    global.introScreen = 0;
}


//Helper functions to convert from pixel coordinates into screen space, which OpenGl expects.
//You should use these functions to convert your pixel coordinates into screen space.
//We use pixel coordinates because it is easier to work with in assembly, than floating point numbers.
float pixelToScreenX(int x){
    return (2.0f * (float) x / (float) (screenWidth - 1) - 1.0f);
}
float pixelToScreenY(int y){
    return -(2.0f * (float) y / (float) (screenHeight - 1) - 1.0f);
}

void drawBall(){
    //EXAMPLE:
    //This is an example of how your OpenGL code should look like.
    //You can use this as a template for your own code.
    //You must use glBegin and glEnd to draw your shapes.
    //You must convert the pixel coordinates to screen coordinates using pixelToScreenX and pixelToScreenY.
    //The pixelToScreen functions are nonlinear meaning that f(x + y) != f(x) + f(y).
    //So you have to add the pixel values before you convert to screen space.
    float x = pixelToScreenX(global.ballPosition.x);
    float y = pixelToScreenY(global.ballPosition.y);
    float widthX = pixelToScreenX(global.ballPosition.x + ballSideLength);
    float lengthY = pixelToScreenY(global.ballPosition.y + ballSideLength);

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(paddleColor.r, paddleColor.g, paddleColor.b);
    glVertex2f(x, y);
    glVertex2f(widthX, y);
    glVertex2f(widthX, lengthY);
    glVertex2f(x, y);
    glVertex2f(widthX, lengthY);
    glVertex2f(x, lengthY);
    glEnd();
}

void drawRect(float x1, float y1, float x2, float y2, Color color){

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(color.r, color.g, color.b);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawPaddle(){
    //Draws the player paddle and the AI paddle
    //The paddle is a rectangle with a width of paddleWidth and a length of paddleLength
    //Both paddles are white
    //The player paddle is on the right, the AI paddle is on the left
    //The paddles are placed at global.playerPaddlePosition and global.aiPaddlePosition


    //player paddle coordinates
    float px1 = pixelToScreenX(global.playerPaddlePosition.x);
    float py1 = pixelToScreenY(global.playerPaddlePosition.y);
    float px2 = pixelToScreenX(global.playerPaddlePosition.x + paddleWidth);
    float py2 = pixelToScreenY(global.playerPaddlePosition.y + paddleLength);

    //AI paddle coordinates
    float ax1 = pixelToScreenX(global.aiPaddlePosition.x);
    float ay1 = pixelToScreenY(global.aiPaddlePosition.y);
    float ax2 = pixelToScreenX(global.aiPaddlePosition.x + paddleWidth);
    float ay2 = pixelToScreenY(global.aiPaddlePosition.y + paddleLength);


    //draw player
    drawRect(px1,py1,px2,py2, paddleColor);

    //draw AI
    drawRect(ax1,ay1,ax2,ay2, paddleColor);
}

void drawScore(){
    //Draws the score for both the player and the AI
    //Player score is green, AI score is red
    //Player score is on the right, AI score is on the left
    //Score is drawn as a series of squares
    //The number of squares is equal to the score
    //The squares are drawn in a row, with the scoreGap between each square
    //The squares have a side length of scoreSize

    Color playerColor = (Color){0, 255, 0};
    int y1 =aiScorePosition.y;
    int y2 =y1 + scoreSize;
    float y2f =pixelToScreenY(y2);
    float y1f =pixelToScreenY(y1);

    //draw players score
    for (int i = 1; i <= global.playerScore; i++ ) {

        //set position
        int x1 = screenWidth - (wallThickness + (scoreSize + scoreGap) * i);
        int x2 =x1 + scoreSize;
        float x1f =pixelToScreenX(x1);
        float x2f =pixelToScreenX(x2);
        drawRect(x1f, y1f, x2f, y2f, playerColor);
    }

    //draw AIs score
    Color aiColor = (Color){255, 0, 0};
    for (int i = 1; i <= global.aiScore; i++ ) {

        //set position
        int x1 = wallThickness + (scoreSize + scoreGap) * i;
        int x2 =x1 + scoreSize;
        float x1f =pixelToScreenX(x1);
        float x2f =pixelToScreenX(x2);
        drawRect(x1f, y1f, x2f, y2f, aiColor);
    }





}

void drawWalls(){

    float wallX =  pixelToScreenX(wallThickness);
    float wallY = pixelToScreenY(wallThickness);

    float bottomH = pixelToScreenX(0);
    float bottomV = pixelToScreenY(0);

    float screenW = pixelToScreenX(screenWidth);
    float screenH = pixelToScreenY(screenHeight);

    float goalPost = pixelToScreenY(goalHeight);

    float goalPostBottomStart = pixelToScreenY(screenHeight - goalHeight);

    float goalPostRightStart = pixelToScreenX(screenWidth - wallThickness);

    float bottomWallTop = pixelToScreenY(screenHeight - wallThickness);

    //top wall
    drawRect(bottomH, bottomV, screenW, wallY, paddleColor);
    //bottom wall
    drawRect(bottomH, bottomWallTop, screenW, screenH, paddleColor);

    //right bottom
    drawRect(goalPostRightStart, goalPostBottomStart, screenW, screenH, paddleColor);
    //right top
    drawRect(goalPostRightStart, bottomV, screenW, goalPost, paddleColor);

    //left bottom
    drawRect(bottomH, goalPostBottomStart, wallX, screenH, paddleColor);
    //left top
    drawRect(bottomH, bottomV, wallX, goalPost, paddleColor);

}


void drawMidfieldLine() {
    int lineSegs = 50;
    int lineSegSize = 10;

    float xf = pixelToScreenX(screenWidth/2);
    float x2f = pixelToScreenX(screenWidth/2 + lineSegSize );
    Color midlineColor = (Color){100,100,255};


    for (int i = 0; i< lineSegs ; i++) {
        int y = i * (screenHeight / lineSegs);
        int y2 = y + lineSegSize;
        float yf = pixelToScreenY(y);
        float y2f = pixelToScreenY(y2);
        drawRect(xf, yf, x2f, y2f, midlineColor);
    }
}

void drawString(char* message, int x, int y, Color color) {
    int length = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)message);
    float xf = pixelToScreenX(x - (length/2));
    float yf = pixelToScreenY(y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(color.r , color.g, color.b);

    glRasterPos2f(xf, yf);
    for (const char* m=message; *m; m++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *m);
    }
}

void drawMessageGameOver() {
    //displays the message for game over

    //draw game over


    char* msg1 = "End of Game!";
    drawString(msg1, screenWidth / 2, screenHeight / 2, (Color){255, 255, 255});


    //draw message for restart

    char* msg2 = "Press any key to end.";
    drawString(msg2, screenWidth / 2, screenHeight / 2 + 60, (Color){255, 255, 0});

    char* msg3 = "Press r to restart.";
    drawString(msg3, screenWidth / 2, screenHeight / 2 + 90, (Color){255, 255, 0});

}


void drawIntroScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);



    // Set up the lighting and material properties
    GLfloat light_position[] = { 1.0f, 2.0f, -2.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);

    // Draw the sphere
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glTranslatef(0.0f,  0.0f, -0.0f);
    glutSolidSphere(1.5, 30, 30);
    glPopMatrix();

    GLfloat mat_diffuse2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);

    // Draw the left rectangular prism
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 0.0f); // position on the left side of the screen
    glScalef(0.5f, 2.0f, 0.5f); // stretch into a rectangular prism
    glColor3f(1.0f, 0.0f, 0.0f); // red color
    glutSolidCube(2.0f); // draw the cube
    glPopMatrix();


    GLfloat mat_diffuse3[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);

    // Draw the left rectangular prism
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 0.0f); // position on the left side of the screen
    glScalef(0.5f, 2.0f, 0.5f); // stretch into a rectangular prism
    glColor3f(1.0f, 0.0f, 0.0f); // red color
    glutSolidCube(2.0f); // draw the cube
    glPopMatrix();

    // Set up text rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

//    glScalef(2.0f, 2.0f, 1.0f);

    glDisable(GL_LIGHTING);
    // Render text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 - 200, glutGet(GLUT_WINDOW_HEIGHT) / 2 + 10);
    const char* text1 = "PONG!";
    for (int i = 0; i < strlen(text1); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    }

    glColor3f(1.0f, 1.0f, 0.5f);
    glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 - 200, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 40);
    const char* text2 = "Press any button to play";
    for (int i = 0; i < strlen(text2); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
    }

    // Restore the previous matrices

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glDisable(GL_CULL_FACE);

    glFlush();



}

void resetBall(){
    //EXAMPLE:
    //This is an example of how your assembly functions should look like.
    //You can use this as a template for your own code.
    //I've provided the original C code that I wrote, and the corresponding assembly code.
    //I recommend that you first write the C code and test if it works. Then you can convert it to assembly.

    //Resets the ball to the initial position and speed.
    //The ball will go in the opposite direction of the last player to score

    //C code:
    //global.ballPosition = initialBallPosition;
    //if (global.lastScore == 0){
    //    global.ballDirection = (Point) {-initialBallDirection.x, initialBallDirection.y};
    //}
    //else {
    //    global.ballDirection = initialBallDirection;
    //}
    //global.ballSpeed = initialBallSpeed;

    //Assembly code:
    //You should always use __volatile__ to prevent the compiler from incorrectly optimizing away your code.
    //__asm__ is the keyword to start writing assembly code.
    //GCC uses AT&T syntax, which is different from Intel syntax that we have been using so far.
    //The biggest changes are that the parameters are in reverse order.
    //Registers are prefixed with a % sign. (Two % in our case to avoid the compiler from interpreting it as a positional argument)
    //Integer literals are prefixed with a $ sign.
    //To refer to the memory pointed to by a register you must use the () syntax.
    //When a label is created, it can be referenced from anywhere in the code, so ensure your labels are unique.
    __asm__ __volatile__(
        //%0 is the first parameter, %1 is the second parameter, and so on.
        //The parameters start counting from the output parameters then to the input parameters.
        //So %5 in this case is the first input parameter(initialBallPosition.x).
        //And %0 is the first output parameter(global.ballPosition.x).
        //You must include the newline character at the end of each line.
            "mov %5, %0\n" //This is equivalent to global.ballPosition.x = initialBallPosition.x; in C.
            "mov %6, %1\n" // global.ballPosition.y = initialBallPosition.y;
            "cmp $0, %10\n" // if (global.lastScore == 0)
            "jne resetBallPlayer\n" // {
            "mov %7, %%eax\n" // eax = initialBallDirection.x
            "imul $-1, %%eax\n" // eax = -initialBallDirection.x
            "mov %%eax, %2\n" // global.ballDirection.x = -initialBallDirection.x;
            "mov %8, %3\n" // global.ballDirection.y = initialBallDirection.y;
            "jmp resetBallEnd\n" // }
            "resetBallPlayer:\n" // else {
            "mov %7, %2\n" // global.ballDirection.x = initialBallDirection.x;
            "mov %8, %3\n" // global.ballDirection.y = initialBallDirection.y;
            "resetBallEnd:\n" // }
            "mov %9, %4\n" // global.ballSpeed = initialBallSpeed;
            //An example of how to use the eax register, and integer literals.
            "mov $0, %%eax\n" //Now eax is 0
            : "=m" (global.ballPosition.x), "=m" (global.ballPosition.y), "=m" (global.ballDirection.x), "=m" (global.ballDirection.y), "=m" (global.ballSpeed)
        //Output parameters go here. Use "=r" for values stored in registers, use "=m" for values stored in memory
            : "r" (initialBallPosition.x), "r" (initialBallPosition.y), "r" (initialBallDirection.x), "r" (initialBallDirection.x), "r" (initialBallSpeed), "r" (global.lastScore)
        //Input parameters go here use "r" for values stored in registers, use "m" for values stored in memory
            : "eax"
        //You should list all the registers you use here, because they will be clobbered and the compiler has to know which ones to save
            );
}

void updateBall(){
    //Check if the ball collides with the edges of the screen, and check if it collides with the paddles.
    //If the ball collides with the edges of the screen, it will add a point to the other player and reset the ball.
    //If the ball collides with the paddles, it will change the x direction of the ball.
    //If the ball collides with the top or bottom of the screen, it will change the y direction of the ball.
    //The ball will also increase in speed every time it collides with the AI paddle.
    //Update the ball position using the global.ballSpeed and global.ballDirection variables
    //Make sure to update the global.lastScore variable to indicate who scored the last point


    //paddle collisions

    //ball
    int ballX1 = global.ballPosition.x;
    int ballY1 = global.ballPosition.y;
    int ballX2 = global.ballPosition.x + ballSideLength;
    int ballY2 = global.ballPosition.y + ballSideLength;

    //player collision
    int playerX1 = global.playerPaddlePosition.x;
    int playerY1 = global.playerPaddlePosition.y;
    int playerX2 = global.playerPaddlePosition.x + paddleWidth;
    int playerY2 = global.playerPaddlePosition.y + paddleLength;
    bool playerCollisionY = (playerY1 <= ballY1 && ballY1 <= playerY2) || (playerY1 <= ballY2 && ballY2 <= playerY2);
    bool playerCollisionX = (playerX1 <= ballX1 && ballX1 <= playerX2) || (playerX1 <= ballX2 && ballX2 <= playerX2);
    bool playerCollision = playerCollisionX && playerCollisionY;

    //AICollision
    int AIX1 = global.aiPaddlePosition.x;
    int AIY1 = global.aiPaddlePosition.y;
    int AIX2 = global.aiPaddlePosition.x + paddleWidth;
    int AIY2 = global.aiPaddlePosition.y + paddleLength;
    bool AICollisionY = (AIY1 <= ballY1 && ballY1 <= AIY2) || (AIY1 <= ballY2 && ballY2 <= AIY2);
    bool AICollisionX = (AIX1 <= ballX1 && ballX1 <= AIX2) || (AIX1 <= ballX2 && ballX2 <= AIX2);
    bool AICollision = AICollisionX && AICollisionY;

    if (playerCollision || AICollision) {
        global.ballDirection.x = global.ballDirection.x * -1.0f;
    }



    //wall collision
    bool pastCeiling = global.ballPosition.y <= (0 + wallThickness);
    bool pastFloor = global.ballPosition.y >= (screenHeight - wallThickness);
    bool pastLWall = global.ballPosition.x <= (0 + wallThickness);
    bool pastRWall = global.ballPosition.x >= (screenWidth - wallThickness);
    if(pastCeiling || pastFloor) {
        global.ballDirection.y = global.ballDirection.y * -1.0f;
    }
    if(pastLWall || pastRWall) {
        global.ballDirection.x = global.ballDirection.x * -1.0f;
    }
    if (pastCeiling) {
        global.ballPosition.y = 0 + wallThickness;
    }
    if (pastFloor) {
        global.ballPosition.y = screenHeight - wallThickness;
    }

    //goal collision
    int goalTop = goalPosition - (goalHeight/2);
    int goalBottom = goalPosition + (goalHeight/2);
    bool ballInGoalBoundsY1 = ballY1 >= goalTop && ballY1 <= goalBottom ;
    bool ballInGoalBoundsY2 = ballY2 >= goalTop && ballY2 <= goalBottom ;
    bool ballInGoalBounds = ballInGoalBoundsY1 && ballInGoalBoundsY2;
    if (ballInGoalBounds && pastLWall) {
        global.playerScore +=1;
        global.lastScore =1;
        resetBall();
    }
    else if (ballInGoalBounds && pastRWall) {
        global.aiScore +=1;
        global.lastScore =0;
        resetBall();
    } else {
        //move ball
        global.ballPosition.x = global.ballPosition.x + (global.ballDirection.x * global.ballSpeed);
        global.ballPosition.y = global.ballPosition.y + (global.ballDirection.y * global.ballSpeed);
    }







}

void updateAI(){
    //The AI is very simple, it just follows the ball on the Y axis only if the ball is on the left side of the screen
    //It moves at the speed set by the global.aiSpeed variable

    //C++ version of updateAI

    //check to see if the ball is on the AI side


//    int ballX2 = global.ballPosition.x + ballSideLength;
//
//
//
//
//    int midPoint = screenWidth / 2;
//    bool onSide = (ballX2 <= midPoint) ;
//    if (!onSide) {
//        return;
//    }
//
//    int ballCenter = global.ballPosition.y + (ballSideLength / 2);
//    int aiPaddleCenter = global.aiPaddlePosition.y + (paddleLength / 2);
//    int ballDistance = ballCenter - aiPaddleCenter ;
//    //subtract the AIplayer position from ball position
//    //check to see that the ball is more than ball.speed away y direction
//
//    // if the result is positive, subtract the speed from the AI position
//    if (ballDistance > initialBallSpeed) {
//        global.aiPaddlePosition.y += initialBallSpeed;
//    }
//
//    //if the result is negative, add the speed to the AI position
//    if (ballDistance < -initialBallSpeed) {
//        global.aiPaddlePosition.y -= initialBallSpeed;
//    }


    //ASM

    int screenW = screenWidth;

    __asm__ __volatile__(

        //CHECK TO SEE IF BALL IS ON THE CORRECT SIDE

        "mov %6, %%eax\n"//move screen with into eax
        "shr $1, %%eax\n"//divide screen width by two, register eax
        "mov %1, %%ebx\n"//move the ball x position into register ebx
        "add %4, %%ebx\n"//add the ball length to the ball position at ebx
        "cmp %%ebx, %%eax\n"//cmp screen with ball x position
        "jle endUpdateAI\n"//jump to end if the ball x position is less than screen width divided by 2

        //CALCULATE BALL CENTER

        "mov %4, %%eax\n"//move ball side length into register eax
        "shr $1, %%eax\n"//divide it by 2
        "mov %2, %%ebx\n"//move the ball y position into register ebx
        "add %%ebx, %%eax\n"//add it to ball side length at eax

        //CALCULATE PADDLE CENTER
        "mov %3, %%ebx\n"//move paddle length into a register ebx
        "shr $1, %%ebx\n"//divide paddle length by 2 ebx

        //CALCULATE DISTANCE TO BALL FROM PADDLE

        "sub %%ebx, %%eax\n"//subtract the paddle length from the ball center (eax - ebx)
        "mov %0, %%ecx\n"//mov paddle y position into register ecx
        "sub %%ecx, %%eax\n"//subtract the paddle y position from the ball center (eax - ecx)

        //MOVE PADDLE DOWN IF BALL BELOW

        "paddleDown:\n"//paddle down
        "mov %5, %%ebx\n"//mov initial ball speed into register ebx
        "cmp %%ebx, %%eax\n"//cmp with ball distance
        "jle paddleUp\n"//if ball distance less than or equal to init ball speed jump to paddle up (eba <= ebx)

        "mov %7, %%ebx\n" //move paddle speed into register
        "add %%ebx, %%ecx\n"//add paddle speed to position (ecx + ebx)
        "mov %%ecx, %0\n"//move ecx into paddle position mem
        "jmp endUpdateAI\n"//jump to end

        //MOVE PADDLE UP IF BALL ABOVE

        "paddleUp:\n"// label paddle up
        "neg %%ebx\n"//Make initBallSpeed negative
        "cmp %%ebx, %%eax\n"//compare with ball distance
        "jge endUpdateAI\n"//if ball distance greater than or equal to -initialBallSpeed than jump to end (eax >= ebx)

        "mov %7, %%ebx\n" //move paddle speed into register
//        "neg %%ebx\n"
        "sub %%ebx, %%ecx\n"//subtract paddleSpeed from y position (ecx - ebx)
        "mov %%ecx, %0\n"//move ecx into paddle position mem
        "jmp endUpdateAI\n"//jump to end

        "endUpdateAI:\n"//label end update ai

        // %0 -  global.playerPaddlePosition
        : "=m" (global.aiPaddlePosition.y)
        // %1 - ballX                    %2 - bally                 $3 - paddleLength   $4 - ballLength       $5 - ballSpeed         $6 - screenwidth  $7 - paddleSpeed
        : "m" (global.ballPosition.x), "m" (global.ballPosition.y), "r" (paddleLength), "m" (ballSideLength), "m" (initialBallSpeed), "r" (screenW), "m" (aiPaddleSpeed)
        : "eax", "ebx", "ecx" // Clobbered register
    );




}

void gameLogic(){
    //The game is over when one of the players reaches 9 points otherwise call updateBall and updateAI
    //Make sure to update the global.gameOver variable
    if (global.introScreen == 0) {
        return;
    }

    if (global.gameOver == 0) {

        updateBall();
        updateAI();
    }

    if (global.playerScore >= 9 || global.aiScore >= 9) {
        global.gameOver = 1;
    }



}


void mouse(int x, int y){
    //The paddle is always centered on the mouse

    //move players paddle to y coordinate

    __asm__ __volatile__(
        "mov %3, %%eax\n" // move the paddle size into register
        "shr $1, %%eax\n" // divide by paddle size by 2 to get center
        "mov %1, %%ebx\n" //move the mouse position into register
        "sub %%eax, %%ebx\n" // subtract paddle size from mouse position to center it
        "mov %%ebx, %0\n" // Move the value from centered new paddle value into paddle position
        // %0 -  global.playerPaddlePosition
        : "=m" (global.playerPaddlePosition.y)
        // %1 - y   %2 - global.playerPaddlePosition.y  $3 - paddleLength
        : "m" (y), "m" (global.playerPaddlePosition.y), "m" (paddleLength)
        : "eax", "ebx" // Clobbered register
    );
}

void resetGame(){
    global.playerPaddlePosition = initialPlayerPaddlePosition;
    global.aiPaddlePosition = initialAiPaddlePosition;
    global.playerScore = 0;
    global.aiScore = 0;
    global.gameOver = 0;
}

void keyboard(unsigned char key, int x, int y){
    //Pressing 'r' resets the game if the game is over

    //check if game is over, if not return
//    if (global.gameOver == 0) {
//        return;
//    }
//    //if game is over then check the r key was pressed,
//    if (key == 'r') {
//        resetGame();
//    } else {
//        exit(0);
//    }
    //if so then reset the game

    //else then quit the application

    __asm__ __volatile__(

        "mov $1, %4\n"

        "mov %5, %%eax\n"//move gameover into eax
        "cmp $0, %%eax\n"//compare with 0
        "je keyboardEnd\n"//jump to end if equal



        "cmp $0x72, %6\n"//cmp key to r (ascii=0x72)
        "jne otherKeyPressed\n"//if not equal jmp to other key pressed

        "mov %7, %0\n"//set player position to initial position
        "mov %8, %1\n"//set aiposition to initial position
        "mov $0, %2\n"//set player score to 0
        "mov $0, %3\n"//set ai score to 0
        "mov $0, %5\n"//set set gameover to 0
        "jmp keyboardEnd\n"

        //label othekeypressed

        //exit the program

        "otherKeyPressed:\n"


        "keyboardEnd:\n"//label end of function


         // %0 - playerpaddle                 %1 - aipaddle                     %2 - playerscore        %3 - aiscore            %4-introscreen
        : "=m" (global.playerPaddlePosition), "=m" (global.aiPaddlePosition), "=m" (global.playerScore), "=m" (global.aiScore), "=m" (global.introScreen)
        // %5 - gameover          %6 - key   %7 - initplayerpaddle               %8 -initaipaddle
        : "m" (global.gameOver),  "r" (key), "r" (initialPlayerPaddlePosition), "r" (initialAiPaddlePosition)

        : "eax", "ebx", "ecx" // Clobbered register
    );

    if ( global.gameOver ==1 && key != 'r') {
        exit(0);
    }


}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
//    printf("%d", global.aiScore);
    if (global.introScreen == 0) {
        drawIntroScreen();
        glutSwapBuffers();
        glutPostRedisplay();
        return;
    }

    drawMidfieldLine();
    drawPaddle();
    drawBall();
    drawScore();
    drawWalls();
    if (global.gameOver == 1) {
        drawMessageGameOver();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Initialize GLUT and process user parameters
    glutInit(&argc, argv);
    initGlobals();

    // Request double buffered true color window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(0, 0);

    // Create window
    glutCreateWindow("COMP308 Pong");

    // Callback functions
    glutDisplayFunc(draw);
    glutIdleFunc(gameLogic);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);

    // Pass control to GLUT for events
    glutMainLoop();
    return 0;
}
