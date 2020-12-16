/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "MultiPlayerScene.h"
#include "SinglePlayerScene .h"
#include "Ranking.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}




// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "Botones/Salir2.png",
                                           "Botones/SalirSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'Botones/Salir2.png' and 'Botones/SalirSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menuClose = Menu::create(closeItem, NULL);
    menuClose->setPosition(Vec2::ZERO);
    this->addChild(menuClose, 1);

    //Crear boton de Single Player
    auto SinglePlayerItem = MenuItemImage::create(
                                            "Botones/SinglePlayer2.png",
                                            "Botones/SinglePlayerSelected.png",
                                            CC_CALLBACK_1(HelloWorld::SinglePlayerOpen, this));

    if (SinglePlayerItem == nullptr ||
        SinglePlayerItem->getContentSize().width <= 0 ||
        SinglePlayerItem->getContentSize().height <= 0)
    {
        problemLoading("'Botones/SinglePlayer2.png' and 'Botones/SinglePlayerSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - SinglePlayerItem->getContentSize().width / 2;
        float y = origin.y + 60 + SinglePlayerItem->getContentSize().height / 2;
        SinglePlayerItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menuSingle = Menu::create(SinglePlayerItem, NULL);
    menuSingle->setPosition(Vec2::ZERO);
    this->addChild(menuSingle, 1);

    //Crear boton de Multi Player
    auto MultiPlayerItem = MenuItemImage::create(
                                            "Botones/MultiPlayer2.png",
                                            "Botones/MultiPlayerSelected.png",
                                             CC_CALLBACK_1(HelloWorld::MultiPlayerOpen, this));

    if (MultiPlayerItem == nullptr ||
        MultiPlayerItem->getContentSize().width <= 0 ||
        MultiPlayerItem->getContentSize().height <= 0)
    {
        problemLoading("'Botones/MultiPlayer2.png' and 'Botones/MultiPlayerSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - MultiPlayerItem->getContentSize().width / 2;
        float y = origin.y + 30 + MultiPlayerItem->getContentSize().height / 2;
        MultiPlayerItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menuMulti = Menu::create(MultiPlayerItem, NULL);
    menuMulti->setPosition(Vec2::ZERO);
    this->addChild(menuMulti, 1);


    //Crear boton de Single Player
    auto RankingItem = MenuItemImage::create(
        "Botones/Rank.png",
        "Botones/RankSelected.png",
        CC_CALLBACK_1(HelloWorld::RankingOpen, this));

    if (RankingItem == nullptr ||
        RankingItem->getContentSize().width <= 0 ||
        RankingItem->getContentSize().height <= 0)
    {
        problemLoading("'Botones/Rank.png' and 'Botones/RankSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - RankingItem->getContentSize().width / 2;
        float y = origin.y + 90 + RankingItem->getContentSize().height / 2;
        RankingItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menuRank = Menu::create(RankingItem, NULL);
    menuRank->setPosition(Vec2::ZERO);
    this->addChild(menuRank, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("TRON: Ligth Cicles", "fonts/Demoness.otf", 18);
    if (label == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto spriteFondo = Sprite::create("Fondos/FondoInicio.jpg");
    if (spriteFondo == nullptr)
    {
        problemLoading("'Fondos/FondoInicio.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        //spriteFondo->setAnchorPoint(Vec2::ZERO);
        spriteFondo->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(spriteFondo, 0);
    }
    return true;


}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::MultiPlayerOpen(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFadeBL::create(1, MultiPlayer::createSceneMulti()));
}

void HelloWorld::SinglePlayerOpen(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionJumpZoom::create(1, SinglePlayer::createSceneMulti()));
}

void HelloWorld::RankingOpen(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionJumpZoom::create(1, Ranking::createScene()));
}
