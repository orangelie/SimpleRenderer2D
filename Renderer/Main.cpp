#include "pch.h"
#include "GameLoader.h"
#include "01. HelloTriangle.h"

#define MAIN_CLASS      HelloTriangle
#define SCREEN_WIDTH    (920)
#define SCREEN_HEIGHT   (800)












#define DECLARE_MAIN(w, h, program)                                 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,                     
    _In_opt_ HINSTANCE hPrevInstance,                               
    _In_ LPWSTR    lpCmdLine,                                       
    _In_ int       nCmdShow)                                        
{                                                                   
    UNREFERENCED_PARAMETER(hPrevInstance);                          
    UNREFERENCED_PARAMETER(lpCmdLine);                              
                                                                    
    shared_ptr<IGameInterface> demo = make_shared<MAIN_CLASS>();
    GameLoader::GetInstance()->Init(hInstance, SCREEN_WIDTH, SCREEN_HEIGHT, demo);
   GameLoader::GetInstance()->Run();
}                                                                   
