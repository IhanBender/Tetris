#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <unistd.h>

// Tetris specific includes
#include <tetris/pieces.h>
float xStep = 0.062;
float yStep = 0.071;
unsigned int numTexture[10];    // Number textures

void drawNumbers(unsigned int value, unsigned int num, int x, int y, Shader ourShader, unsigned int squareVAO);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawPiece(Piece piece, unsigned int texture, Shader ourShader, unsigned int squareVAO, float scale = 1.0) {
    std::vector<int> coords = getPieceCoords(piece);

    for(int i = 0; i < (int)coords.size(); i+=2){
        if(piece.y + coords[i+1] < 20){
            glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3((scale) * xStep * (piece.x + coords[i]),(scale) * yStep * (piece.y + coords[i+1]), 0));
            model = glm::scale(model, glm::vec3(scale));
            ourShader.use();
            ourShader.setMat4("model", model);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
            glBindVertexArray(squareVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}

// settings
const unsigned int SCR_WIDTH = 480;
const unsigned int SCR_HEIGHT = 420;

int main()
{
    srand(time(NULL));
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float BackgroundVertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int BackgroundIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundVertices), BackgroundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BackgroundIndices), BackgroundIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /*
    Main Screen :
    0.368f,  0.635f, 0.0f
    0.368f, -0.785f, 0.0f
    -0.252f, -0.785f, 0.0f
    -0.252f,  0.635f, 0.0f
    */

    float squareVertices[] = {
        // positions              // colors           // texture coords
        -0.190f, -0.714f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        -0.190f, -0.785f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.252f, -0.785f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.252f, -0.714f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int squareIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int squareVBO, squareVAO, squareEBO;
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);
    glGenBuffers(1, &squareEBO);

    glBindVertexArray(squareVAO);

    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    unsigned int texture1;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::getPath("resources/textures/tetrisbg.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // load and create a texture
    // -------------------------
    unsigned int squareTexture;
    glGenTextures(1, &squareTexture);
    glBindTexture(GL_TEXTURE_2D, squareTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::getPath("resources/textures/square2.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //bool squareMatrix[10][40];
    bool ** squareMatrix;
    squareMatrix = (bool **)malloc(sizeof(bool *) * 10);
    for (int i = 0; i < 10; i++){
        squareMatrix[i] = (bool *)malloc(sizeof(bool) * 40);
        for (int j = 0; j < 40; j++)
            squareMatrix[i][j] = false;
    }

    // Sets inicial current piece
    Piece currentPiece = genCurrentPiece(generatePiece());
    // Sleeps to change seed
    sleep(1);
    // Sets iniciaç next piece
    Piece nextPiece = genNextPiece();

    // Statistics
    Piece TPiece { -12.5,   14.5,   0,  't'};
    Piece JPiece { -12.5,   12,     0,  'j'};
    Piece ZPiece { -12.5,   9.5,    0,  'z'};
    Piece OPiece { -12,     6.5,    0,  'o'};
    Piece SPiece { -11.5,   3.5,    0,  's'};
    Piece LPiece { -12.5,   1,      0,  'l'};
    Piece IPiece { -13,     -2,     0,  'i'};

    // Info about current game
    unsigned int numberOfPieces = 0;
    unsigned int numberOfLines = 0;
    unsigned int score = 0;
    unsigned int level = 0;
    // I, O, T, S, Z, J, L
    unsigned int pieces[] = {0,0,0,0,0,0,0};
    // Block falling and dificulty increasing logic variables
    float inicialTime = glfwGetTime();
    float blockTime = 1.0f;
    float timeMultiplier = 0.667f;
    // Delay for activating input keys again
    float sideKeyDelay = 0.3f;
    float downKeyDelay = 0.05f;
    float lastSidePressed = -0.3f;
    float lastDownPressed = -0.05f;
    // control variables
    bool QPressed = false;  // Rotate Right
    bool WPressed = false;  // Rotate Left
    bool mustStop = false;  // Controls piece stop

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        // input
        // -----
        processInput(window);
        // Process game input
        // Rotate Right
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
            QPressed = false;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !QPressed){
            currentPiece.state = rotateRight(currentPiece, squareMatrix);
            QPressed = true;
        }
        // Rotate Left
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
            WPressed = false;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !WPressed){
            currentPiece.state = rotateLeft(currentPiece, squareMatrix);
            WPressed = true;
        }
        // Move Right
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && currentFrame - lastSidePressed > sideKeyDelay){
            if (canMoveRight(currentPiece, squareMatrix)){
                currentPiece.x += 1.0;
                lastSidePressed = currentFrame;
            }
        }
        // Move Left
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && currentFrame - lastSidePressed > sideKeyDelay){
            if (canMoveLeft(currentPiece, squareMatrix)){
                currentPiece.x -= 1.0;
                lastSidePressed = currentFrame;
            }
        }
        // Move Down
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && currentFrame - lastDownPressed > downKeyDelay){
            if (canMoveDown(currentPiece, squareMatrix)){
                currentPiece.y -= 1.0;
                lastDownPressed = currentFrame;
            }
            else{
                mustStop == true;
            }
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render Screen background
        glm::mat4 model = glm::mat4(1);
        ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        bool mustCheck = false;
        // Checks if piece already has to stop
        if (mustStop) {
            updateMap(currentPiece, squareMatrix);
            currentPiece = genCurrentPiece(nextPiece.name);
            nextPiece = genNextPiece();
            inicialTime = currentFrame;
            mustStop = false;
            mustCheck = true;
        } else {
            // Checks for block vertical movement (natural movement, not user's input)
            if (currentFrame - inicialTime > blockTime) {
                inicialTime = currentFrame;
                // If has to move, check if there is room for the piece
                if(!canMoveDown(currentPiece, squareMatrix)){
                    updateMap(currentPiece, squareMatrix);
                    // maintenance
                    currentPiece = genCurrentPiece(nextPiece.name);
                    nextPiece = genNextPiece();
                    mustStop = false;
                    mustCheck = true;
                } else {
                    currentPiece.y -= 1;
                }
            }
        }

        if (mustCheck){

            if(gameOver(squareMatrix)){
                glfwSetWindowShouldClose(window, true);
                printf("Pontuação: %d\n", score);
            }
            // Checks line maintenence
            unsigned int removedLines = removeFullLines(squareMatrix);
            if (removedLines == 1) score+=40*(level + 1);
            if (removedLines == 2) score+=100*(level + 1);
            if (removedLines == 3) score+=300*(level + 1);
            if (removedLines == 4) score+=1200*(level + 1);
            numberOfLines += removedLines;
            // Number of pieces and level system
            numberOfPieces++;
            if (numberOfPieces % 20 == 0){
                level++;
                blockTime *= timeMultiplier;
            }
            // Checks for piece statistics
            switch (currentPiece.name){
                case 'i':   pieces[0]++;break;
                case 'o':   pieces[1]++;break;
                case 't':   pieces[2]++;break;
                case 's':   pieces[3]++;break;
                case 'z':   pieces[4]++;break;
                case 'j':   pieces[5]++;break;
                case 'l':   pieces[6]++;break;
                default:    break;
            }

        }

        // Render score

        // Render statistics

        // Render level

        // Render lines

        // render already set blocks
        glBindTexture(GL_TEXTURE_2D, squareTexture);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 20; j++){
                if (squareMatrix[i][j]){
                    model = glm::translate(glm::mat4(1), glm::vec3(xStep * i, yStep * j, 0));
                    ourShader.use();
                    ourShader.setMat4("model", model);
                    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
                    glBindVertexArray(squareVAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }

        // Render current piece
        drawPiece(currentPiece, squareTexture, ourShader, squareVAO);

        // Render next piece
        drawPiece(nextPiece, squareTexture, ourShader, squareVAO);

        // Render Statistics
        model = glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.use();
        ourShader.setMat4("model", model);
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
        glBindVertexArray(squareVAO);
        float scale = 0.75;
        drawPiece(IPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(OPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(SPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(LPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(ZPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(JPiece, squareTexture, ourShader, squareVAO, scale);
        drawPiece(TPiece, squareTexture, ourShader, squareVAO, scale);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
    glDeleteBuffers(1, &squareEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void drawNumbers(unsigned int value, unsigned int num, int x, int y, Shader ourShader, unsigned int squareVAO){
    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x,y,0));
    model = glm::scale(model, glm::vec3(0.5, 2.0, 1.0));
    ourShader.use();
    ourShader.setMat4("model", model);

    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    glBindVertexArray(squareVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}