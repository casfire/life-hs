#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "simulation.hpp"
#include "SDLHelper.hpp"
#include "SDLGui.hpp"
#include "SDL_gfxPrimitives.h"
#include "params.hpp"
#include "CasfireSplash.hpp"
#include "SDLHelper.hpp"
#include "icon.h"

#define WINDOW_CAPTION "Casfire Artificial life v1.1 - 2012"
#define TEXTFREEZETIME 1000 //in miliseconds
#define BACKCOLOR 0xFFFFFFFF

int main(int argc, char* args[]){
	argc = argc; args = args;
	SDLInit(); SDL_EnableKeyRepeat(400,40);
	
	SDL_Surface *iconSurface = SDLNew(32,32,true);
	for(int x = 0; x<32; x++){
		for(int y = 0; y<32; y++){
			SDLPutPixel(iconSurface,x,y,*((Uint32*)(&icon_bin[(y + x*32)*sizeof(Uint32)])));
		}
	}
	SDL_WM_SetIcon(iconSurface,NULL);

	bool run = true;
	int screen_x = 1000, screen_y = 600;
	SDLTimer timer; SDL_Event SDLEvent; SDLEvents events;

	Simulation sim;
	MovableView view; view.SetZoom(4.5); view.SetCanDrag(SDL_BUTTON_LEFT,true); int viewX1=0, viewY1=0, viewX2=0, viewY2=0; Uint32 MouseLX = 0, MouseLY = 0;
	bool isFullScreen = false, runSim = false; Uint32 lastFrameTime = 0; bool frameCalculated;
	GUIScrollBar scrollRotate; scrollRotate.SetHorizontal(true); scrollRotate.Register();
	GUIButton butFullScreen; butFullScreen.Register();
	GUIButton butRun, butNextFrame; butRun.Register(); butNextFrame.Register();
	GUIScrollBar scrollSpeed; scrollSpeed.SetHorizontal(true); scrollSpeed.Register();

	GUIButton butSave, butLoad, butReset, butDefault;
	butSave.Register(); butLoad.Register(); butReset.Register(); butDefault.Register();

	GUICheckBox chxFollowRotation, chxFollowLocation; GUILabel txtFollowRotation, txtFollowLocation; chxFollowRotation.SetChecked(true); chxFollowLocation.SetChecked(true);

	GUITextDefault.SetFontStyle(TTF_STYLE_BOLD); GUITextDefault.Render(WINDOW_CAPTION,0x0000FFFF,18); GUITextDefault.SetFontStyle(TTF_STYLE_NORMAL);
	GUIText textTitle; textTitle.SwitchRender(&GUITextDefault);
	GUIText text; char textC[255]; textC[0]=0; Uint32 textColor=0x000000FF; text.Load("font.ttf",13); Uint32 textFreeze = 0;
	
	char GUIS = 0, GUISFast = 0; /* 0 = params, 1 = brain, 2 = info */
	GUIButton butSParams, butSBrain, butSInfo, butSFast; butSParams.SetDisabled(true);
	butSParams.Register(); butSBrain.Register(); butSInfo.Register(); butSFast.Register();
	Params paramsGUI; paramsGUI.Register();

	SDLSetVideo(screen_x,screen_y,true);
	SDL_WM_SetCaption(WINDOW_CAPTION,"Life");
	CasfireSplash splash;
	splash.SetTimings(600,1600);
	splash.Generate(screen_x,screen_y);
	splash.AnimateIn(screen,"Loading fonts...");
	splash.FrameIn(screen,"Loading fonts...");
	SDL_Flip(screen);
	splash.FrameIn(screen,"Loading simulation");
	SDL_Flip(screen);
	GUILoadFont("font.ttf");
	if(sim.Load()){
		textFreeze = 0;
		sprintf(textC,"Simulation loaded.");
		textColor = 0x006600FF;
	}else{
		sim.SetDefaultParams();
		sim.Start();
		textFreeze = 0;
		sprintf(textC,"Simulation not found. Created default.");
		textColor = 0x006600FF;
	}
	paramsGUI.Init(&sim.params);

	SDLPushResizeEvent(screen_x,screen_y);
	while(run){
		timer.Frame((runSim)?1:30);
		frameCalculated = false;
		while(SDL_PollEvent(&SDLEvent)){
			events.HandleEvent(&SDLEvent);
			if(GUIEvent(&SDLEvent))continue;
			if(paramsGUI.Event(&SDLEvent,&sim.params))continue;
			switch(SDLEvent.type){
			case SDL_QUIT: run=false; break;
			case SDL_VIDEORESIZE:
				screen_x=SDLEvent.resize.w; if(screen_x<800)screen_x=800;
				screen_y=SDLEvent.resize.h; if(screen_y<600)screen_y=600;
				SDLSetVideo(screen_x,screen_y,true);
				SDL_WM_SetCaption(WINDOW_CAPTION,"Life");

				viewX1 = 10; viewY1 = 40;
				viewX2 = (isFullScreen)?screen_x-10:screen_x-450; viewY2 = screen_y-40;
				view.SetMiddle((viewX1+viewX2)/2.,(viewY1+viewY2)/2.);
				scrollRotate.SetPos(viewX1,viewY1-30,viewX2,viewY1-10);
				butFullScreen.SetPos(screen_x-110,screen_y-30,screen_x-10,screen_y-10);
				butFullScreen.SetText("Fullscreen",15);
				butRun.SetPos(viewX1, viewY2+10, viewX1+100, viewY2+30);
				butRun.SetText((runSim)?"Stop":"Run");
				butNextFrame.SetPos(viewX2-((isFullScreen)?210:100), viewY2+10, viewX2-((isFullScreen)?110:0), viewY2+30);
				butNextFrame.SetText("Next");
				scrollSpeed.SetPos(viewX1+110, viewY2+10, viewX2-110-(isFullScreen?110:0), viewY2+30);

				chxFollowRotation.SetPos(screen_x-440,163,screen_x-427,176); txtFollowRotation.SetPos(screen_x-420,160,screen_x-200,180); txtFollowRotation.SetCenterX(false); txtFollowRotation.Render("Follow rotation",0x000000FF);
				chxFollowLocation.SetPos(screen_x-440,183,screen_x-427,196); txtFollowLocation.SetPos(screen_x-420,180,screen_x-200,200); txtFollowLocation.SetCenterX(false); txtFollowLocation.Render("Follow location",0x000000FF);

				butSave.SetPos(screen_x-440,screen_y-60,screen_x-340,screen_y-40); butSave.SetText("Save");
				butLoad.SetPos(screen_x-330,screen_y-60,screen_x-230,screen_y-40); butLoad.SetText("Load");
				butReset.SetPos(screen_x-220,screen_y-60,screen_x-120,screen_y-40); butReset.SetText("Reset");
				butDefault.SetPos(screen_x-110,screen_y-60,screen_x-10,screen_y-40); butDefault.SetText("Default");

				butSParams.SetPos(screen_x-440,screen_y-90,screen_x-340,screen_y-70); butSParams.SetText("Params");
				butSBrain.SetPos(screen_x-330,screen_y-90,screen_x-230,screen_y-70); butSBrain.SetText("Brain");
				butSInfo.SetPos(screen_x-220,screen_y-90,screen_x-120,screen_y-70); butSInfo.SetText("Info");
				butSFast.SetPos(screen_x-110,screen_y-90,screen_x-10,screen_y-70); butSFast.SetText("Fast");

				paramsGUI.Set(screen_x,screen_y);

				break;
			case SDL_KEYDOWN:
				if(SDLEvent.key.keysym.sym==SDLK_SPACE){
					runSim = !runSim;
					SDLPushResizeEvent(screen_x,screen_y);
				}else if(SDLEvent.key.keysym.sym==SDLK_RETURN){
					sim.Frame();
					frameCalculated = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(SDLEvent.button.button==SDL_BUTTON_LEFT){
					MouseLX = events.mouseX; MouseLY = events.mouseY;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(SDLEvent.button.button==SDL_BUTTON_LEFT){
					if((abs(MouseLX-events.mouseX)<5)&&(abs(MouseLY-events.mouseY)<5)&&(events.mouseX>viewX1)&&(events.mouseX<viewX2)&&(events.mouseY>viewY1)&&(events.mouseY<viewY2)){
						float rx,ry;
						view.GetRevCoords(events.mouseX,events.mouseY,&rx,&ry);
						if(sim.Click(rx,ry)&&(GUIS == 2)&&(!isFullScreen)){
							chxFollowRotation.Register(); txtFollowRotation.Register();
							chxFollowLocation.Register(); txtFollowLocation.Register();
						}else{
							chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
							chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
						}
					}
				}
				break;
			case SDL_USEREVENT:
				if(SDLEvent.user.code==GUIBUTTON_RELEASE){
					if(SDLEvent.user.data1==&butFullScreen){
						isFullScreen = !isFullScreen;
						SDLPushResizeEvent(screen_x,screen_y);
						if(isFullScreen){
							butSave.Unregister(); butLoad.Unregister(); butReset.Unregister(); butDefault.Unregister();
							butSParams.Unregister(); butSInfo.Unregister(); butSBrain.Unregister(); butSFast.Unregister();
							paramsGUI.Unregister();
							chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
							chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
						}else{
							butSave.Register(); butLoad.Register(); butReset.Register(); butDefault.Register();
							butSParams.Register(); butSInfo.Register(); butSBrain.Register(); butSFast.Register();
							if(GUIS == 0)paramsGUI.Register();
							else if(GUIS == 2){
								if(sim.IsSelected()){
									chxFollowRotation.Register(); txtFollowRotation.Register();
									chxFollowLocation.Register(); txtFollowLocation.Register();
								}else{
									chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
									chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
								}
							}
						}
					}else if(SDLEvent.user.data1==&butRun){
						runSim = !runSim;
						SDLPushResizeEvent(screen_x,screen_y);
					}else if(SDLEvent.user.data1==&butNextFrame){
						sim.Frame();
						frameCalculated = true;
					}else if(SDLEvent.user.data1==&butSave){
						textFreeze = timer.total;
						if(sim.Save()){
							sprintf(textC,"Simulation saved.");
							textColor = 0x006600FF;
						}else{
							sprintf(textC,"Error saving simulation. Save directory missing?");
							textColor = 0x660000FF;
						}
					}else if(SDLEvent.user.data1==&butLoad){
						textFreeze = timer.total;
						if(sim.Load()){
							paramsGUI.Init(&sim.params);
							sprintf(textC,"Simulation loaded.");
							textColor = 0x006600FF;
						}else{
							sprintf(textC,"Error loading simulation.");
							textColor = 0x660000FF;
						}
					}else if(SDLEvent.user.data1==&butReset){
						textFreeze = timer.total;
						sim.Start();
						sprintf(textC,"Simulation has reset to frame 0.");
						textColor = 0x006600FF;
					}else if(SDLEvent.user.data1==&butDefault){
						textFreeze = timer.total;
						sim.SetDefaultParams();
						paramsGUI.Init(&sim.params);
						sprintf(textC,"Parameters reset to defaults.");
						textColor = 0x006600FF;
					}else if(SDLEvent.user.data1==&butSParams){
						butSParams.SetDisabled(true);
						butSBrain.SetDisabled(false);
						butSInfo.SetDisabled(false);
						paramsGUI.Register();
						GUIS = 0;
						chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
						chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
					}else if(SDLEvent.user.data1==&butSBrain){
						butSParams.SetDisabled(false);
						butSBrain.SetDisabled(true);
						butSInfo.SetDisabled(false);
						paramsGUI.Unregister();
						GUIS = 1;
						chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
						chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
					}else if(SDLEvent.user.data1==&butSInfo){
						butSParams.SetDisabled(false);
						butSBrain.SetDisabled(false);
						butSInfo.SetDisabled(true);
						paramsGUI.Unregister();
						GUIS = 2;
						if(sim.IsSelected()){
							chxFollowRotation.Register(); txtFollowRotation.Register();
							chxFollowLocation.Register(); txtFollowLocation.Register();
						}else{
							chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
							chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
						}
					}else if(SDLEvent.user.data1==&butSFast){
						GUISFast = 1;
					}
				}else if(SDLEvent.user.code==GUISCROLLBAR_SCROLL){
					if(SDLEvent.user.data1==&scrollRotate){
						view.SetRot(scrollRotate.GetScrollPos()*M_PI*2);
					}
				}
				break;
			}
			view.Event(&SDLEvent,viewX1,viewY1,viewX2,viewY2);
		}

		if(runSim){
			Uint32 b = scrollSpeed.GetScrollPos()*300;
			if(timer.total-lastFrameTime>b){ sim.Frame(); lastFrameTime = timer.total; frameCalculated = true; }
		}

		if(GUISFast == 1){
			SDLSetVideo(400,40,false);
			SDL_WM_SetCaption(WINDOW_CAPTION,"Life");
			boxColor(screen,0,0,400,40,0xFFFFFFFF);
			sprintf(textC,"Press escape to stop.");
			stringColor(screen,5,10,textC,0x000000FF);
			SDL_Flip(screen);
			bool runGUIS = true;
			while(runGUIS){
				while(SDL_PollEvent(&SDLEvent)){
					switch(SDLEvent.type){
					case SDL_QUIT: runGUIS=false; break;
					case SDL_KEYDOWN:
						if(SDLEvent.key.keysym.sym==SDLK_ESCAPE){
							runGUIS=false;
						}
					}
				}
				sim.Frame();
				sprintf(textC,"Frame %"PRIu64" Crit(%d) Food(%d)",sim.GetFrameCount(),sim.GetCritterCount(),sim.GetFoodCount());
				boxColor(screen,48,20,400,28,0xFFFFFFFF);
				stringColor(screen,5,20,textC,0x000000FF);
				SDL_Flip(screen);
			}
			GUISFast = 0;
			SDLSetVideo(screen_x,screen_y,true);
			SDL_WM_SetCaption(WINDOW_CAPTION,"Life");
		}
		if(sim.CenterSelected(&view,chxFollowLocation.IsChecked(),chxFollowRotation.IsChecked())){
			if(chxFollowRotation.IsChecked()){
				float r = view.GetRot();
				if(r < 0.)r += M_PI*2;
				scrollRotate.SetScrollPos(r/(M_PI*2));
			}
		}else{
			if(sim.IsSelected()){
				chxFollowRotation.Register(); txtFollowRotation.Register();
				chxFollowLocation.Register(); txtFollowLocation.Register();
			}else{
				chxFollowRotation.Unregister(); txtFollowRotation.Unregister();
				chxFollowLocation.Unregister(); txtFollowLocation.Unregister();
			}
		}

		boxColor(screen,viewX1,viewY1,viewX2,viewY2,BACKCOLOR); //clear view area
		sim.DrawAxes(screen,&view);
		sim.Draw(screen,&view);
		boxColor(screen,0,0,screen_x,viewY1,0xF0F0F0FF);
		boxColor(screen,0,viewY2,screen_x,screen_y,0xF0F0F0FF);
		boxColor(screen,0,viewY1,viewX1,viewY2,0xF0F0F0FF);
		boxColor(screen,viewX2,viewY1,screen_x,viewY2,0xF0F0F0FF);
		rectangleColor(screen,viewX1,viewY1,viewX2,viewY2,0x000000FF);

		if(!isFullScreen){
			hlineColor(screen,screen_x-440,screen_x-10,40,0x000000FF);
			hlineColor(screen,screen_x-440,screen_x-10,screen_y-100,0x000000FF);
			textTitle.Blit(screen,screen_x-440,10,430,20,true,true);
			if(((timer.total-textFreeze>TEXTFREEZETIME)&&frameCalculated)){
				sprintf(textC,"Frame %"PRIu64" Crit(%d) Food(%d)",sim.GetFrameCount(),sim.GetCritterCount(),sim.GetFoodCount());
				textColor = 0x000000FF;
			}
			text.Render(textC,textColor);
			text.Blit(screen,screen_x-440,screen_y-30,320,20,true,true);
			if(GUIS == 1){
				sim.DrawBrainGUI(screen,screen_x-440,50,screen_x-10,screen_y-100);
			}else if(GUIS == 2){
				sim.DrawInfoGUI(screen,screen_x-440,50,screen_x-10,screen_y-100);
			}
		}

		GUIDraw(screen);
		splash.FrameOut(screen);
		SDL_Flip(screen);
	}
	SDLExit(); return 0;
}
