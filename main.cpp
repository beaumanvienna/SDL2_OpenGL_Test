// this is a test to see if the SDL2 renderer and bare opengl rendering work in the same program

// compile with
// g++ main.cpp -o main -I/usr/include/SDL2 -I/usr/include/GL -lSDL2 -lGL -lSDL2_image

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>

using namespace std;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define NUM_TEXTURES    1
#define TEX_BACKGROUND  0

GLfloat red;

// main window
SDL_Window *gWindow;
// SDL2 window renderer
SDL_Renderer* gRenderer = NULL;
//textures
SDL_Texture* gTextures[NUM_TEXTURES];

SDL_Texture* loadTextureFromFile(string str)
{
    SDL_Surface* surf = NULL;
    SDL_Texture* texture = NULL;
    
    surf = IMG_Load(str.c_str());
    if (!surf)
    {
        printf("File %s could not be loaded\n",str.c_str());
    }
    else
    {
        texture =SDL_CreateTextureFromSurface(gRenderer,surf);
        if (!texture)
        {
            printf("texture for background could not be created.\n");
        }
        SDL_FreeSurface(surf);
    }
    return texture;
}

bool loadMedia()
{
    bool ok = true;
    
    // background
    gTextures[TEX_BACKGROUND] = loadTextureFromFile("beach.png");
    if (!gTextures[TEX_BACKGROUND])
    {
        ok = false;
    }
    
    return ok;
}

bool createRenderer(void)
{
    bool ok = true;
    
    //Create renderer for main window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( gRenderer == NULL )
    {
        printf( "Error creating renderer. SDL error: %s\n", SDL_GetError() );
        ok = false;
    }
    else
    {
        
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
            ok = false;
        }
        SDL_RenderClear(gRenderer);
        
        //draw backround to main window
        SDL_RenderCopy(gRenderer,gTextures[TEX_BACKGROUND],NULL,NULL);
        SDL_RenderPresent(gRenderer);
    }
    
    
    return ok;
}

int main(int argc, char* args[])
{
        
    bool isRunning=true;
    SDL_Event event;
        
    
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        
        // create main window
        gWindow=SDL_CreateWindow("OpenGL Test",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
            
            
        //Initialize libsdl-image for png files
        if( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
        {
            printf( "Error initialzing image support. SDL_image error: %s\n", IMG_GetError() );
        }
        // create SDL2 renderer
        if (!createRenderer())
        {
            printf( "createRenderer failure after failure\n");
        }    
        
        isRunning=true;
        while(isRunning)
        {
            while(SDL_PollEvent(&event))
            {

                if(event.type==SDL_QUIT)
                    isRunning=false;

                //if esc button is released
                if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_ESCAPE)
                    isRunning=false;

                if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_r)
                {
                    red += 0.1f;
                    if (red >= 1.0f) red = 0;
                    printf("first loop red: %f\n",red);
                }
            }
        }
        /////////// SECOND PART: BARE OPEN GL RENDERING
            
        SDL_GLContext m_context = SDL_GL_CreateContext(gWindow);
        #define immediate_updates 0
        #define updates_synchronized 1
        SDL_GL_SetSwapInterval(updates_synchronized);
        SDL_GL_MakeCurrent(gWindow, m_context);
        
        isRunning=true;
        while(isRunning)
        {
            //Events
            while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_QUIT)
                    isRunning=false;

                //if esc button is released
                if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_ESCAPE)
                    isRunning=false;

                if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_r)
                {
                    red += 0.1f;
                    if (red >= 1.0f) red = 0;
                    printf("second loop red: %f\n",red);
                }
            }
            //Logic rendering
            
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClearColor(red, 0.f, 1.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            SDL_GL_SwapWindow(gWindow);
        }
        SDL_GL_DeleteContext(m_context);
    }
    SDL_Quit();
    return 0;
}
