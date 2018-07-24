#include "bdefs.h"

#include <stddef.h>
#include "render.h"
#include "consolecommands.h"
#include "dsys.h"
#include "clientmgr.h"
#include "sysclientde_impl.h"
#include "varsetter.h"
#include "videomgr.h"
#include "sysconsole_impl.h"
#include "dtxmgr.h"
#include "bindmgr.h"
#include <SDL.h>
//IClientFileMgr
#include "client_filemgr.h"
static IClientFileMgr *client_file_mgr;
define_holder(IClientFileMgr, client_file_mgr);

//the ILTClient game interface
#include "iltclient.h"
static ILTClient *ilt_client;
define_holder(ILTClient, ilt_client);

//IClientShell game client shell object.
#include "iclientshell.h"
static IClientShell *i_client_shell;
define_holder(IClientShell, i_client_shell);

//ILTCommon client instance.
#include "iltcommon.h"
static ILTCommon *ilt_common_client;
define_holder_to_instance(ILTCommon, ilt_common_client, Client);

// ------------------------------------------------------------ //
// Globals..
// ------------------------------------------------------------ //
SysCache g_SysCache;

extern int32	g_CV_CursorCenter;

RMode g_RMode;

RenderStruct g_Render;

IRenderer *g_pRenderer;
define_holder(IRenderer, g_pRenderer);
/*
void rdll_RenderDLLSetup(RenderStruct *pStruct)
{
	r_InitRenderStruct(true);
}
*/

void r_InitRenderStruct(bool bFullClear) {
	if(bFullClear)
	    g_Render = RenderStruct{};
	else
	    g_Render.clear();
	
	g_Render.m_GlobalLightDir.Init(0.0f, -2.0f, -1.0f);
    g_Render.m_GlobalLightDir.Norm();

}

LTObject*
r_ProcessAttachment(LTObject *pParent, Attachment *pAttachment)
{
	return nullptr;
}

void r_TerminateAllRecreatableTextureData()
{}

void r_BindTexture(SharedTexture *pSharedTex, bool bTextureChanged )
{}
void r_UnbindTexture(SharedTexture *pSharedTex, bool bUnload )
{}

LTRESULT r_InitRender(RMode *pMode, const char *window_name)
{
    r_TermRender(0, false);
    SDL_Window *window = g_ClientGlob.m_window;
	SDL_DestroyWindow(window);
	window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, pMode->m_Width, pMode->m_Height, 0);
	g_ClientGlob.m_window = window;

    // get the function pointer from the renderModule and pass our stuct to it for setup.
    auto getRender = (RenderDLLSetupFn)bm_GetFunctionPointer(g_pClientMgr->m_hRenderModule, "rdll_RenderDLLSetup");
    getRender(&g_Render);

	// Store these.. the renderer may change them for pixel doubling.
	g_Render.m_Width = pMode->m_Width;
	g_Render.m_Height = pMode->m_Height;
    RenderStructInit init{0, *pMode, (void*)window};
    int initStatus = g_Render.Init(&init);
	if(initStatus != RENDER_OK || init.m_RendererVersion != LTRENDER_VERSION)
	{
        g_pClientMgr->SetupError(LT_ERRORLOADINGRENDERDLL, "Init Failed");
        return LT_ERRORLOADINGRENDERDLL;
	}

	g_pClientMgr->InitConsole();

    return LT_OK;
}

LTRESULT r_TermRender(int surfaceHandle, bool unload)
{
    return LT_OK;
}

uint32 GetInternalTextureMemory()
{
	return 0;
}

uint32 GetRendererTextureMemory()
{
	return 0;
}

int GetFrameStats(LTRendererStats &Stats)
{
	return (g_pRenderer != nullptr) ? g_pRenderer->getFrameStats(Stats) : 0 ;
}
