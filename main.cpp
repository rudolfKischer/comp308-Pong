// Template by: Allan Wei
// allan.wei@mail.mcgill.ca

#include <GL/glut.h>

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
} Global;
Global global;

//Consts you should not change these values.
#define screenWidth 1920
#define screenHeight 1080
#define paddleOffset 120
#define scoreSize 22
const int aiPaddleSpeed = 5;
const int scoreGap = 50;
const int ballSpeedupFactor = 1;
const int paddleWidth = 40;
const int paddleLength = 200;
const int ballSideLength = 30;
const int initialBallSpeed = 5;
const int scorePosition = screenHeight * 0.9;
const Color paddleColor = (Color){255, 255, 255};
const Point playerScorePosition = (Point){screenWidth - paddleOffset, scorePosition};
const Point aiScorePosition = (Point){paddleOffset - scoreSize, scorePosition};
const Point initialBallPosition = (Point) {screenWidth / 2, screenHeight / 2};
const Point initialBallDirection = (Point) {1, 1};

void initGlobals(){
    //Initializes the global variables
    //They are all under the global struct, and can be access using global.variableName
    //You should not change this function.
    global.playerPaddlePosition = (Point){screenWidth - paddleOffset - paddleWidth, (screenHeight / 2) - paddleLength / 2};
    global.aiPaddlePosition = (Point){paddleOffset, (screenHeight / 2) - (paddleLength / 2)};
    global.playerScore = 0;
    global.aiScore = 0;
    global.ballPosition = initialBallPosition;
    global.ballSpeed = initialBallSpeed;
    global.ballDirection = initialBallDirection;
    global.lastScore = 0;
    global.gameOver = 0;
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
}

void updateAI(){
    //The AI is very simple, it just follows the ball on the Y axis only if the ball is on the left side of the screen
    //It moves at the speed set by the global.aiSpeed variable
}

void gameLogic(){
    //The game is over when one of the players reaches 9 points otherwise call updateBall and updateAI
    //Make sure to update the global.gameOver variable
}


void mouse(int x, int y){
    //The paddle is always centered on the mouse
}

void keyboard(unsigned char key, int x, int y){
    //Pressing 'r' resets the game if the game is over
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawPaddle();
    drawBall();
    drawScore();

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
