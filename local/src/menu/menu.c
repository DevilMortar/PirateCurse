#include "../kernel/header.h"

int menu(BUTTON * buttonList, SKIN * skinList, SDL_Renderer *renderer, TEXTURE map, TEXTURE title, GAME *game){
    SDL_bool menu_active = SDL_TRUE;
    enum functions options = mainmenu; 
    int skinChoice=1;
    SKIN *skinListTMP=skinList;
    SKIN *firstSkin=skinList;
    Uint32 frameStart;
    unsigned int frameTime;

    while (menu_active)
    {
        SDL_Event event;
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                options=checkClickButtons(buttonList, options, event.motion.x, event.motion.y);
                break;
            case SDL_QUIT:
                menu_active = SDL_FALSE;
                break;
            default:
                resetButtonState(buttonList);
                checkOverButtons(buttonList, options, event.motion.x, event.motion.y);
                break;
            }
        }
        switch(options){
            case play:
                while(firstSkin!=skinList){
                    firstSkin=firstSkin->next;
                    skinChoice+=1;
                }
                return skinChoice;
            case mainmenu:
                displayMainMenu(buttonList, skinList, renderer, map, title, game);
                break;
            case skin:
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title, game);
                break;
            case left:
                if (skinListTMP!=NULL){
                    skinListTMP=skinListTMP->previous;
                    if (skinListTMP==NULL){
                        skinListTMP=skinList;
                    }
                }
                if(skinListTMP->price<0 && game->best>abs(skinListTMP->price))
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title, game);
                options=skin;
                break;
            case right:
                if (skinListTMP!=NULL){
                    skinListTMP=skinListTMP->next;
                    if (skinListTMP==NULL){
                        skinListTMP=skinList;
                    }
                }
                if(skinListTMP->price<0 && game->best>abs(skinListTMP->price))
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title, game);
                options=skin;
                break;
            case confirm:
                if(skinListTMP->state==1){
                    skinList=skinListTMP;
                    options=mainmenu;
                    displayMainMenu(buttonList, skinList, renderer, map, title, game);
                }
                else{
                    options=skin;
                    displaySkinMenu(buttonList, skinListTMP, renderer, map, title, game);
                }
                break;
            case locker:
                if(skinListTMP->state==0 && skinListTMP->price > 0 && skinListTMP->price <= game->money){
                    skinListTMP->state=-1;
                }
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title, game);
                options=skin;
                break;
            case leave:
                return -6;
            default:
                break;
        }
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);
    }
    return -1;
}

void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, TEXTURE map, TEXTURE title, GAME *game){
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);
    SDL_RenderCopy(renderer, title.texture, NULL, &title.dstrect);
    //Display Record
    SDL_Color color = {255, 255, 255};
    SDL_Rect recordRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 150, 0, 0};
    char record[20];
    sprintf(record, "Best score   %d", game->best);
    SDL_Texture * recordTexture = renderWidgetText(record, color, 20, renderer, &recordRect);
    SDL_RenderCopy(renderer, recordTexture, NULL, &recordRect);
    // Display Buttons
    displayButtons(renderer, buttonList, mainmenu);
    skinList->skin_sprite.srcrect.x=0*skinList->skin_sprite.srcsizew;
    SDL_RenderCopy(renderer, skinList->skin_sprite.texture, &skinList->skin_sprite.srcrect, &skinList->skin_sprite.dstrect);
}

void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, TEXTURE map, TEXTURE title, GAME *game){
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);
    SDL_RenderCopy(renderer, title.texture, NULL, &title.dstrect);
    // Display Money
    SDL_Color color = {255, 255, 255};
    char money[20];
    SDL_Rect moneyRect = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 130, 0, 0};
    sprintf(money, "Money   %d", game->money);
    SDL_Texture * moneyTexture = renderWidgetText(money, color, 20, renderer, &moneyRect);
    SDL_RenderCopy(renderer, moneyTexture, NULL, &moneyRect);
    // Display Skin
    skinListTMP->skin_sprite.srcrect.x=0;
    SDL_RenderCopy(renderer, skinListTMP->skin_sprite.texture, &skinListTMP->skin_sprite.srcrect, &skinListTMP->skin_sprite.dstrect);
    BUTTON *tmp=buttonList;
    int wait=0;

    while(tmp!=NULL){
        if(tmp->menu==skin){
            switch(tmp->state){
                case 1:
                    tmp->button_sprite.srcrect.x=0;
                    break;
                case 2:
                    tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
                    break;
                case 3:
                    tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew;
                    break;
            }
            if(tmp->function==locker && skinListTMP->state==1){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*5;
            }
            if(tmp->function==locker && skinListTMP->state<0){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*(-skinListTMP->state+1);
                 skinListTMP->state=skinListTMP->state-1;
                 if(skinListTMP->state==-4){skinListTMP->state=1;}
                 wait=1;
            }
            if(tmp->function==confirm && skinListTMP->state==0){
                tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
            }
            SDL_RenderCopy(renderer, tmp->button_sprite.texture, &tmp->button_sprite.srcrect, &tmp->button_sprite.dstrect);
        }
        tmp=tmp->next;
    }
}

SKIN * createSkin(SDL_Renderer *renderer, char link[255], SKIN * skinList, int w, int h, int x, int y, int state, int price, int srcsizew, int srcsizeh){
    SKIN *new;
    new=malloc(sizeof(SKIN));
    new->skin_sprite=newSprite(renderer, link, 3, srcsizew, srcsizeh, 0);
    new->skin_sprite.dstrect.w=w;
    new->skin_sprite.dstrect.h=h;
    new->skin_sprite.dstrect.x=x;
    new->skin_sprite.dstrect.y=y;
    new->state=state;
    new->price=price;
    new->previous=NULL;
    new->next=NULL;
    return addSkinInList(skinList, new);
}

SKIN * addSkinInList(SKIN *skinList, SKIN *newSkin){
    if(skinList != NULL && newSkin !=NULL){
        newSkin->next=skinList;
        skinList->previous=newSkin;
        skinList=newSkin;
    }
    else{
        skinList=newSkin;
    }
    return skinList;
}