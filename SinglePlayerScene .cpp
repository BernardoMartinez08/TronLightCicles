#include "SinglePlayerScene .h"
#include "HelloWorldScene.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "string"

using namespace std;

USING_NS_CC;

Scene* SinglePlayer::createSceneMulti()
{
    // return the scene
    return SinglePlayer::create();;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool SinglePlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    NVidasPlayer = 3;
    Level = 1;
    enemysinArena = 1;
    puntosPlayer = 0;
    iniciarScena();
    return true;
}

void SinglePlayer::iniciarScena() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    // add a label shows "Hello World"
    // create and initialize a label

    auto labelTitulo = Label::createWithTTF("TRON: Ligth Cicles", "fonts/Demoness.otf", 10);
    if (labelTitulo == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        labelTitulo->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - labelTitulo->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(labelTitulo, 1);
    }

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    labelPause = Label::createWithTTF("GAME PAUSED", "fonts/Demoness.otf", 15);
    if (labelPause == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        labelPause->setPosition(tamano.width / 2, tamano.height * 0.50);

        // add the label as a child to this layer
        this->addChild(labelPause, 3);
        labelPause->setVisible(false);
    }
   
    labelResume = Label::createWithTTF("Presioan  [R]  para volver al juego", "fonts/arial.ttf", 7);
    if (labelResume == nullptr)
    {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        labelResume->setPosition(tamano.width / 2, tamano.height * 0.45);

        // add the label as a child to this layer
        this->addChild(labelResume, 3);
        labelResume->setVisible(false);
    }


    // add "HelloWorld" splash screen"
    auto spriteFondo = Sprite::create("Fondos/FondoGame.jpg");
    if (spriteFondo == nullptr)
    {
        problemLoading("'Fondos/FondoGame.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        //spriteFondo->setAnchorPoint(Vec2::ZERO);
        spriteFondo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(spriteFondo, 0);
    }

    //Agregar Sprites de Jugadores a la pantalla

    //Player1
    _Player = Sprite::create("Players/MotoAzul.png");

    if (_Player == nullptr)
    {
        problemLoading("'Players/MotoAzul.png'");
    }
    else
    {
        _Player->setPosition(tamano.width / 2, tamano.height * 0.23);
        addChild(_Player, 2);

    }

    _BonusVelocity = Sprite::create("Players/Bonus.png");

    if (_BonusVelocity == nullptr)
    {
        problemLoading("'Players/Bonus.png'");
    }
    else
    {

        _BonusVelocity->setPosition(tamano.width / 3, tamano.height * 0.72);
        addChild(_BonusVelocity, 2);
    }


    velocityPlayer1 = 1;
    velocityEnemy = 1;
    potenciadorPlayer1 = 0;

    //Movimiento Player 1
    auto keyboardListener1 = EventListenerKeyboard::create();
    keyboardListener1->onKeyPressed = CC_CALLBACK_2(SinglePlayer::keyPressedPlayer, this);
    EnemysByLevel(Level);
    setVidasPlayer();
    updateScore();
    for (int i = 0; i < 479; i++) {
        for (int j = 0; j < 689; j++) {
            posicionesOcupadas[i][j] = false;
        }
    }

    cocos2d::DelayTime* delayEnemySpeed = cocos2d::DelayTime::create(80);

    cocos2d::CallFunc* SpeedUpEnemys = cocos2d::CallFunc::create([=]() {
        velocityEnemy = velocityEnemy + 1;
        });

    runAction(cocos2d::Sequence::create(delayEnemySpeed, SpeedUpEnemys, NULL));

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener1, _Player);

    schedule(CC_SCHEDULE_SELECTOR(SinglePlayer::movimientoPlayer));
    schedule(CC_SCHEDULE_SELECTOR(SinglePlayer::movimientoEnemy));
    schedule(CC_SCHEDULE_SELECTOR(SinglePlayer::verificarChoque));
}

void SinglePlayer::setVidasPlayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int count = 10;
    for (int i = 0; i < NVidasPlayer; i++) {
        _VidasPlayer1 = Sprite::create("Players/Vida1.png");
        _VidasPlayer1->setPosition(Vec2(origin.x + (visibleSize.width - count),
            origin.y + visibleSize.height - _VidasPlayer1->getContentSize().height));
        count += 10;
        addChild(_VidasPlayer1, 1);
    }

}

void SinglePlayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application

    Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void SinglePlayer::EnemysByLevel(int level) {
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();
    srand(time(NULL));
    int height = rand();
    height = rand() % (int(tamano.height - 20));

    int height2 = rand();
    height2 = rand() % (int(tamano.height - 20));

    int height3 = rand();
    height3 = rand() % (int(tamano.height - 20));

    if (level == 1 || level == 2) {
        //Player2
        _EnemyA = Sprite::create("Players/MotoAmarilla.png");

        if (_EnemyA == nullptr)
        {
            problemLoading("'Players/MotoAmarilla.png'");
        }
        else
        {
            _EnemyA->setFlippedY(false);
            _EnemyA->setPosition((tamano.width / 2) + 90, height);
            _EnemyA->setRotation(270);
            addChild(_EnemyA, 2);
        }
    }else if (level == 3 || level == 4 || level == 5) {
        //Player2
        _EnemyA = Sprite::create("Players/MotoAmarilla.png");

        if (_EnemyA == nullptr)
        {
            problemLoading("'Players/MotoAmarilla.png'");
        }
        else
        {
            _EnemyA->setFlippedY(false);
            _EnemyA->setPosition((tamano.width / 2) + 90 , height);
            _EnemyA->setRotation(270);
            addChild(_EnemyA, 2);
        }

        _EnemyV = Sprite::create("Players/MotoVerde.png");

        if (_EnemyV == nullptr)
        {
            problemLoading("'Players/MotoVerde.png'");
        }
        else
        {
            _EnemyV->setFlippedY(false);
            _EnemyV->setPosition((tamano.width / 2) + 90 , height2);
            _EnemyV->setRotation(270);
            addChild(_EnemyV, 2);
        }
    }else if (level == 6 || level == 7 || level == 8 || level == 9) {
        //Player2
        _EnemyA = Sprite::create("Players/MotoAmarilla.png");

        if (_EnemyA == nullptr)
        {
            problemLoading("'Players/MotoAmarilla.png'");
        }
        else
        {
            _EnemyA->setFlippedY(false);
            _EnemyA->setPosition((tamano.width / 2) + 90, height);
            _EnemyA->setRotation(270);
            addChild(_EnemyA, 2);
        }

        _EnemyV = Sprite::create("Players/MotoVerde.png");

        if (_EnemyV == nullptr)
        {
            problemLoading("'Players/MotoVerde.png'");
        }
        else
        {
            _EnemyV->setFlippedY(false);
            _EnemyV->setPosition((tamano.width / 2) + 90, height2);
            _EnemyV->setRotation(270);
            addChild(_EnemyV, 2);
        }

        _EnemyR = Sprite::create("Players/MotoRoja.png");

        if (_EnemyR == nullptr)
        {
            problemLoading("'Players/MotoRoja.png'");
        }
        else
        {
            _EnemyR->setFlippedY(false);
            _EnemyR->setPosition((tamano.width / 2) + 90, height3);
            _EnemyR->setRotation(270);
            addChild(_EnemyR, 2);
        }
    }


}

void SinglePlayer::generarLuzPlayer(float rot, float x, float y) {
    _LuzPlayer = Sprite::create("Players/RayoAzul2.png");

    if (_LuzPlayer == nullptr)
    {
        problemLoading("'Players/RayoAzul2.png'");
    }
    else
    {  
       
        _LuzPlayer->setRotation(rot);
        _LuzPlayer->setPosition(x, y);
        addChild(_LuzPlayer, 1);

    }
}

void SinglePlayer::generarLuzEnemy(cocos2d::Sprite* _Enemy, float rot, float x, float y) {
    
    if (_Enemy == _EnemyA) {
        _LuzEnemyA = Sprite::create("Players/RayoAmarillo2.png");

        if (_LuzEnemyA == nullptr)
        {
            problemLoading("'Players/RayoAmarillo2.png'");
        }
        else
        {

            _LuzEnemyA->setRotation(rot);
            _LuzEnemyA->setPosition(x, y);
            addChild(_LuzEnemyA, 1);

        }
    }else if (_Enemy == _EnemyV) {
        _LuzEnemyV = Sprite::create("Players/RayoVerde2.png");

        if (_LuzEnemyV == nullptr)
        {
            problemLoading("'Players/RayoVerde2.png'");
        }
        else
        {

            _LuzEnemyV->setRotation(rot);
            _LuzEnemyV->setPosition(x, y);
            addChild(_LuzEnemyV, 1);

        }
    }
    else if (_Enemy == _EnemyR) {
        _LuzEnemyR = Sprite::create("Players/RayoRojo2.png");

        if (_LuzEnemyR == nullptr)
        {
            problemLoading("'Players/RayoRojo2.png'");
        }
        else
        {

            _LuzEnemyR->setRotation(rot);
            _LuzEnemyR->setPosition(x, y);
            addChild(_LuzEnemyR, 1);

        }
    }
}

void SinglePlayer::keyPressedPlayer(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    Vec2 loc = event->getCurrentTarget()->getPosition();

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 90)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 270)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        break;

    case EventKeyboard::KeyCode::KEY_D:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(90);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(90);
        }

        if (event->getCurrentTarget()->getRotation() == 90)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 270)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        break;

    case EventKeyboard::KeyCode::KEY_W:
        if (event->getCurrentTarget()->getRotation() == 180)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 0)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 90) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        break;

    case EventKeyboard::KeyCode::KEY_S:
        if (event->getCurrentTarget()->getRotation() == 180)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 0)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 90) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(180);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(180);
        }

        break;

    case EventKeyboard::KeyCode::KEY_P:
        labelPause->setVisible(true);
        labelResume->setVisible(true);
        this->pause();
        break;

    case EventKeyboard::KeyCode::KEY_R:
        labelPause->setVisible(false);
        labelResume->setVisible(false);
        this->resume();
        break;

    case EventKeyboard::KeyCode::KEY_LEFT_SHIFT:
        if (potenciadorPlayer1 >= 20) {

            cocos2d::DelayTime* delay = cocos2d::DelayTime::create(3);

            cocos2d::CallFunc* Acelerar = cocos2d::CallFunc::create([=]() {
                velocityPlayer1 = velocityPlayer1 + 1;
                potenciadorPlayer1 -= 20;
                });

            cocos2d::CallFunc* VelocidadNormal = cocos2d::CallFunc::create([=]() {
                velocityPlayer1 = velocityPlayer1 - 1;
                });

            runAction(cocos2d::Sequence::create(Acelerar, delay, VelocidadNormal, NULL));
        }
        break;

    case EventKeyboard::KeyCode::KEY_ESCAPE:
        guardarPuntos();
        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));
        break;
    }

}

void SinglePlayer::keyPressedPerdiste(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        guardarPuntos();
        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));
        break;
    }
}

void SinglePlayer::movimientoPlayer(float dt) {

    Vec2 loc = _Player->getPosition();
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    if (_Player->getPositionX() < tamano.width || _Player->getPositionX() > 0 || _Player->getPositionY() < tamano.height || _Player->getPositionY() > 0) {
        if (_Player->getRotation() == 180) {
            _Player->setPosition(loc.x, loc.y - velocityPlayer1);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Player->getRotation() == 0) {
            _Player->setPosition(loc.x, loc.y + velocityPlayer1);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Player->getRotation() == 270) {
            _Player->setPosition(loc.x - velocityPlayer1, loc.y);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Player->getRotation() == 90) {
            _Player->setPosition(loc.x + velocityPlayer1, loc.y);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else
            _Player->setPosition(loc.x, loc.y);
    }
    else
        _EnemyA->setPosition(tamano.width / 2, tamano.height * 0.23);

    generarLuzPlayer(_Player->getRotation(), loc.x, loc.y);

}

void SinglePlayer::movimientoEnemy(float dt) {
    if (Level >= 1) {
        IAmovimiento(_EnemyA);
        generarLuzEnemy(_EnemyA,_EnemyA->getRotation(),_EnemyA->getPositionX(),_EnemyA->getPositionY());
    }

    if (Level >= 3) {
        IAmovimiento(_EnemyV);
        generarLuzEnemy(_EnemyV, _EnemyV->getRotation(), _EnemyV->getPositionX(), _EnemyV->getPositionY());
    }

    if (Level >= 6) {
        IAmovimiento(_EnemyR);
        generarLuzEnemy(_EnemyR, _EnemyR->getRotation(), _EnemyR->getPositionX(), _EnemyR->getPositionY());
    }
}

void SinglePlayer::verificarChoque(float det) {

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();
    Rect sensorColisionPlayer = _Player->getBoundingBox();
    Rect sensorPotenciador = _BonusVelocity->getBoundingBox();
    Vec2 loc = _Player->getPosition();
    srand(time(NULL));
    int BonusY = rand();
    BonusY = rand() % 700;

    int BonusX = rand();
    BonusX = rand() % 480;

    if (NVidasPlayer > 0) {
        if (Level >= 1 && Level <= 2) {
            Rect sensorColisionEnemyA = _EnemyA->getBoundingBox();
            Vec2 locEnemyA = _EnemyA->getPosition();

            if (enemysinArena > 0) {
                if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyA)) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (pasoporAqui(_Player->getPositionX(), _Player->getPositionY()) == true) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (sensorColisionEnemyA.intersectsRect(sensorColisionPlayer)) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (pasoporAqui(_EnemyA->getPositionX(), _EnemyA->getPositionY()) == true) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (sensorColisionPlayer.intersectsRect(sensorPotenciador)) {
                    potenciadorPlayer1 += 20;
                    _BonusVelocity->setPosition(BonusX, BonusY);
                }
                else if (loc.x > tamano.width || loc.x < 0 || loc.y > tamano.height - 20 || loc.y < 0) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (locEnemyA.x > tamano.width || locEnemyA.x < 0 || locEnemyA.y > tamano.height - 20 || locEnemyA.y < 0) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
            }
            else {
                Level++;
                puntosPlayer += 20;
                velocityPlayer1 = 1;
                cleanup();
                removeAllChildren();
                SinglePlayer::iniciarScena();

                if (Level == 2)
                    enemysinArena = 1;
                else
                    enemysinArena = 2;
            }
        }
        else if (Level >= 3 && Level <= 5) {
            Rect sensorColisionEnemyA = _EnemyA->getBoundingBox();
            Rect sensorColisionEnemyV = _EnemyV->getBoundingBox();
            Vec2 locEnemyA = _EnemyA->getPosition();
            Vec2 locEnemyV = _EnemyA->getPosition();

            if (enemysinArena > 0) {
                if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyA)) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyV)) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (sensorColisionEnemyA.intersectsRect(sensorColisionPlayer)) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (sensorColisionEnemyV.intersectsRect(sensorColisionPlayer)) {
                    _EnemyV->cleanup();
                    _EnemyV->setVisible(false);
                    enemysinArena--;
                }
                else if (pasoporAqui(_Player->getPositionX(), _Player->getPositionY()) == true) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (pasoporAqui(_EnemyA->getPositionX(), _EnemyA->getPositionY()) == true) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (pasoporAqui(_EnemyV->getPositionX(), _EnemyV->getPositionY()) == true) {
                    _EnemyV->cleanup();
                    _EnemyV->setVisible(false);
                    enemysinArena--;
                }
                else if (sensorColisionPlayer.intersectsRect(sensorPotenciador)) {
                    potenciadorPlayer1 += 20;
                    _BonusVelocity->setPosition(BonusX, BonusY);
                }
                else if (loc.x > tamano.width || loc.x < 0 || loc.y > tamano.height - 20 || loc.y < 0) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (locEnemyA.x > tamano.width || locEnemyA.x < 0 || locEnemyA.y > tamano.height - 20 || locEnemyA.y < 0) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (locEnemyV.x > tamano.width || locEnemyV.x < 0 || locEnemyV.y > tamano.height - 20 || locEnemyV.y < 0) {
                    _EnemyV->cleanup();
                    _EnemyV->setVisible(false);
                    enemysinArena--;
                }
            }
            else {
                Level++;
                puntosPlayer += 20;
                velocityPlayer1 = 1;
                cleanup();
                removeAllChildren();
                if (Level == 3 || Level == 4 || Level == 5)
                    enemysinArena += 2;
                else
                    enemysinArena += 3;
                SinglePlayer::iniciarScena();
            }
        }
        else if (Level >= 6 <= 9) {
            Rect sensorColisionEnemyA = _EnemyA->getBoundingBox();
            Rect sensorColisionEnemyR = _EnemyR->getBoundingBox();
            Rect sensorColisionEnemyV = _EnemyV->getBoundingBox();
            Vec2 locEnemyA = _EnemyA->getPosition();
            Vec2 locEnemyV = _EnemyA->getPosition();
            Vec2 locEnemyR = _EnemyA->getPosition();


            if (enemysinArena > 0) {
                if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyA)) {
                    cleanup();
                    removeAllChildren(); (true);
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyV)) {
                    cleanup();
                    removeAllChildren(); (true);
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (sensorColisionPlayer.intersectsRect(sensorColisionEnemyR)) {
                    cleanup();
                    removeAllChildren();;
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (pasoporAqui(_Player->getPositionX(), _Player->getPositionY()) == true) {
                    cleanup();
                    removeAllChildren();;
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (pasoporAqui(_EnemyA->getPositionX(), _EnemyA->getPositionY()) == true) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (pasoporAqui(_EnemyV->getPositionX(), _EnemyV->getPositionY()) == true) {
                    _EnemyV->cleanup();
                    _EnemyV->setVisible(false);
                    enemysinArena--;
                }
                else if (pasoporAqui(_EnemyR->getPositionX(), _EnemyR->getPositionY()) == true) {
                    _EnemyR->cleanup();
                    _EnemyR->setVisible(false);
                    enemysinArena--;
                }
                else if (sensorColisionPlayer.intersectsRect(sensorPotenciador)) {
                    potenciadorPlayer1 += 20;
                    _BonusVelocity->setPosition(BonusX, BonusY);
                }
                else if (loc.x > tamano.width || loc.x < 0 || loc.y > tamano.height - 20 || loc.y < 0) {
                    cleanup();
                    removeAllChildren();
                    NVidasPlayer--;
                    SinglePlayer::iniciarScena();
                }
                else if (locEnemyA.x > tamano.width || locEnemyA.x < 0 || locEnemyA.y > tamano.height - 20 || locEnemyA.y < 0) {
                    _EnemyA->cleanup();
                    _EnemyA->setVisible(false);
                    enemysinArena--;
                }
                else if (locEnemyV.x > tamano.width || locEnemyV.x < 0 || locEnemyV.y > tamano.height - 20 || locEnemyV.y < 0) {
                    _EnemyV->cleanup();
                    _EnemyV->setVisible(false);
                    enemysinArena--;
                }
                else if (locEnemyR.x > tamano.width || locEnemyR.x < 0 || locEnemyR.y > tamano.height - 20 || locEnemyR.y < 0) {
                    _EnemyR->cleanup();
                    _EnemyR->setVisible(false);
                    enemysinArena--;
                }
            }
            else {
                Level++;
                puntosPlayer += 30;
                velocityPlayer1 = 1;
                cleanup();
                removeAllChildren();
                if (Level == 6 || Level == 7 || Level == 8 || Level == 9) {
                    enemysinArena += 3;
                    SinglePlayer::iniciarScena();
                }
                else if (Level > 9) {
                    cleanup();
                    removeAllChildren();
                    GANASTE();
                }
            }
        }
        else if (Level > 9) {
            cleanup();
            removeAllChildren();
            GANASTE();
        }
    }
    else {
        guardarPuntos();
        cleanup();
        removeAllChildren();
        
        PERDISTE();
    }

}

bool SinglePlayer::pasoporAqui(float x, float y) {
    if (posicionesOcupadas[int(x)][int(y)] == true)
        return true;
    return false;
}

void SinglePlayer::guardarPuntos() {
    ofstream file;
    file.open("C:/Users/angie/Desktop/TronLigthCicles/Resources/Puntajes.txt", ios::app);
    file.seekp(0, ios::end);
    file << "PLAYER HIZO UN NUEVO RECORD DE: " << puntosPlayer << " PUNTOS, LLEGO HASTA EL NIVEL: " << Level << "\n";
    file.close();
}

void SinglePlayer::IAmovimiento(cocos2d::Sprite* _Enemy) {
    Vec2 loc = _Enemy->getPosition();
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();
    bool Moved = false;
    int TurnUD=0;
    int TurnLR=0;

    if (_Player->getPositionY() > _Enemy->getPositionY()) {
        TurnUD = 0;
    }else if (_Player->getPositionY() < _Enemy->getPositionY()) {
        TurnUD = 1;
    }
    
    if (_Player->getPositionX() > _Enemy->getPositionX()) {
        TurnLR = 4;
    }
    else if (_Player->getPositionX() < _Enemy->getPositionX()) {
        TurnLR = 3;
    }
   

    if (TurnUD == 0) {//up
        if (pasoporAqui(loc.x, loc.y + 1) == false) {
            if (loc.x < tamano.width && loc.x > 0 && loc.y + 1 < tamano.height - 20 && loc.y + 1 > 0) {
                if (_Enemy->getRotation() == 0) {
                    _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }else if (_Enemy->getRotation() == 270 || _Enemy->getRotation() == 90) {
                    _Enemy->setRotation(0);
                    _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 180) {
                    if (pasoporAqui(loc.x - 1, loc.y) == false) {
                        if (loc.x - 1 < tamano.width && loc.x - 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                            _Enemy->setRotation(270);
                            _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }else if (pasoporAqui(loc.x + 1, loc.y) == false) {
                        if (loc.x + 1 < tamano.width && loc.x + 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                            _Enemy->setRotation(90);
                            _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    _Enemy->setRotation(0);
                    _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
            }
        }
    }
    else if (TurnUD == 1) {//down
        if (pasoporAqui(loc.x, loc.y - 1) == false) {
            if (loc.x < tamano.width && loc.x > 0 && loc.y - 1 < tamano.height - 20 && loc.y - 1 > 0) {
                if (_Enemy->getRotation() == 180) {
                    _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 270 || _Enemy->getRotation() == 90) {
                    _Enemy->setRotation(180);
                    _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 0) {
                    if (pasoporAqui(loc.x - 1, loc.y) == false) {
                        if (loc.x - 1 < tamano.width && loc.x - 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                            _Enemy->setRotation(270);
                            _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    else if (pasoporAqui(loc.x + 1, loc.y) == false) {
                        if (loc.x + 1 < tamano.width && loc.x + 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                            _Enemy->setRotation(90);
                            _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    _Enemy->setRotation(180);
                    _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
            }
        }
    }
    
    if (TurnLR == 3) {//left
        if (pasoporAqui(loc.x - 1, loc.y) == false) {
            if (loc.x - 1 < tamano.width && loc.x - 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                if (_Enemy->getRotation() == 270) {
                    _Enemy->setPosition(loc.x - velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 180 || _Enemy->getRotation() == 0) {
                    _Enemy->setRotation(270);
                    _Enemy->setPosition(loc.x - velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 90) {
                    if (pasoporAqui(loc.x, loc.y - 1) == false) {
                        if (loc.x < tamano.width && loc.x > 0 && loc.y - 1 < tamano.height - 20 && loc.y - 1 > 0) {
                            _Enemy->setRotation(180);
                            _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    else if (pasoporAqui(loc.x, loc.y + 1) == false) {
                        if (loc.x < tamano.width && loc.x > 0 && loc.y + 1 < tamano.height - 20 && loc.y + 1 > 0) {
                            _Enemy->setRotation(0);
                            _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    _Enemy->setRotation(270);
                    _Enemy->setPosition(loc.x - velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
            }
        }
    }
    else if (TurnLR == 4) {//Right
        if (pasoporAqui(loc.x + 1, loc.y) == false) {
            if (loc.x + 1 < tamano.width && loc.x + 1 > 0 && loc.y < tamano.height - 20 && loc.y > 0) {
                if (_Enemy->getRotation() == 90) {
                    _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 180 || _Enemy->getRotation() == 0) {
                    _Enemy->setRotation(270);
                    _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
                else if (_Enemy->getRotation() == 270) {
                    if (pasoporAqui(loc.x, loc.y + 1) == false) {
                        if (loc.x < tamano.width && loc.x > 0 && loc.y - 1 < tamano.height - 20 && loc.y - 1 > 0) {
                            _Enemy->setRotation(180);
                            _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    else if (pasoporAqui(loc.x, loc.y + 1) == false) {
                        if (loc.x < tamano.width && loc.x > 0 && loc.y + 1 < tamano.height - 20 && loc.y + 1 > 0) {
                            _Enemy->setRotation(0);
                            _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
                            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                        }
                    }
                    _Enemy->setRotation(90);
                    _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
                    posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
                    Moved = true;
                }
            }
        }
    }

    if (Moved == false) {
        if (_Enemy->getRotation() == 180) {
            _Enemy->setPosition(loc.x, loc.y - velocityEnemy);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Enemy->getRotation() == 0) {
            _Enemy->setPosition(loc.x, loc.y + velocityEnemy);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Enemy->getRotation() == 270) {
            _Enemy->setPosition(loc.x - velocityEnemy, loc.y);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
        else if (_Enemy->getRotation() == 90) {
            _Enemy->setPosition(loc.x + velocityEnemy, loc.y);
            posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        }
    }
}

void SinglePlayer::updateScore() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    string puntos;
    puntos = "SCORE: " + std::to_string(puntosPlayer);

    labelScore = Label::createWithTTF(puntos, "fonts/Demoness.otf", 10);
    if (labelScore == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        labelScore->setPosition(Vec2((origin.x + visibleSize.width / 2) - 90,
            (origin.y + visibleSize.height - labelScore->getContentSize().height)));

        // add the label as a child to this layer
        this->addChild(labelScore, 1);
    }
}

void SinglePlayer::GANASTE() {
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    auto labelGanador = Label::createWithTTF("PASASTE TODOS LOS NIVELES", "fonts/Demoness.otf", 13);
    if (labelGanador == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        labelGanador->setPosition(tamano.width / 2, tamano.height * 0.50);

        // add the label as a child to this layer
        this->addChild(labelGanador, 1);
        labelGanador->setVisible(true);
    }

    auto labelSalir = Label::createWithTTF("Presioan  [ESC]  para salir", "fonts/arial.ttf", 7);
    if (labelSalir == nullptr)
    {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        labelSalir->setPosition(tamano.width / 2, tamano.height * 0.45);

        // add the label as a child to this layer
        this->addChild(labelSalir, 1);
        labelSalir->setVisible(true);
    }

    auto keyboardListener2 = EventListenerKeyboard::create();
    keyboardListener2->onKeyPressed = CC_CALLBACK_2(SinglePlayer::keyPressedPerdiste, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener2, labelSalir);
}

void SinglePlayer::PERDISTE() {
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    auto labelPerdedor = Label::createWithTTF("PERDISTE TODAS TUS VIDAS", "fonts/Demoness.otf", 13);
    if (labelPerdedor == nullptr)
    {
        problemLoading("'fonts/Demoness.otf'");
    }
    else
    {
        // position the label on the center of the screen
        labelPerdedor->setPosition(tamano.width / 2, tamano.height * 0.50);

        // add the label as a child to this layer
        this->addChild(labelPerdedor, 1);
        labelPerdedor->setVisible(true);
    }

    auto labelSalir = Label::createWithTTF("Presioan  [ESC]  para salir", "fonts/arial.ttf", 7);
    if (labelSalir == nullptr)
    {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        labelSalir->setPosition(tamano.width / 2, tamano.height * 0.45);

        // add the label as a child to this layer
        this->addChild(labelSalir, 1);
        labelSalir->setVisible(true);
    }

    auto keyboardListener2 = EventListenerKeyboard::create();
    keyboardListener2->onKeyPressed = CC_CALLBACK_2(SinglePlayer::keyPressedPerdiste, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener2, labelSalir);
}