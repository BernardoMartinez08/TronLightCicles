#include "MultiPlayerScene.h"
#include "HelloWorldScene.h"


USING_NS_CC;

Scene* MultiPlayer::createSceneMulti()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    // 'layer' is an autorelease object
    auto layer = MultiPlayer::create();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // add layer as a child to scene
    scene->addChild(layer);


    // return the scene
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void MultiPlayer::inicializarFisica(cocos2d::Sprite* sprite)
{
    auto cuerpo = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    cuerpo->setContactTestBitmask(true);
    cuerpo->setDynamic(true);
    sprite->setPhysicsBody(cuerpo);
}


// on "init" you need to initialize your instance
bool MultiPlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    NVidasPlayer1 = 3;
    NVidasPlayer1 = 3;
    dibujarVidas(NVidasPlayer1, NVidasPlayer2);
    IniciarScena();

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    return true;
}

void MultiPlayer::IniciarScena() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();


    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "Botones/Salir2.png",
        "Botones/SalirSelected.png",
        CC_CALLBACK_1(MultiPlayer::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'Botones/Salir2.png' and 'Botones/SalirSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    

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
    _Player1 = Sprite::create("Players/MotoRoja.png");

    if (_Player1 == nullptr)
    {
        problemLoading("'Players/MotoRoja.png'");
    }
    else
    {
        _Player1->setPosition(tamano.width / 2, tamano.height * 0.23);
        // inicializarFisica(_Player1);
        addChild(_Player1, 2);

    }

    //Player2
    _Player2 = Sprite::create("Players/MotoAmarilla.png");

    if (_Player2 == nullptr)
    {
        problemLoading("'Players/MotoAmarilla.png'");
    }
    else
    {

        _Player2->setFlippedY(false);
        _Player2->setPosition(tamano.width / 2, tamano.height * 0.43);
        // inicializarFisica(_Player2);
        addChild(_Player2, 2);
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


    //Movimiento Player 1
    auto keyboardListener1 = EventListenerKeyboard::create();
    keyboardListener1->onKeyPressed = CC_CALLBACK_2(MultiPlayer::keyPressedPlayer1, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener1, _Player1);

    //Movimiento Player 2
    auto keyboardListener2 = EventListenerKeyboard::create();
    keyboardListener2->onKeyPressed = CC_CALLBACK_2(MultiPlayer::keyPressedPlayer2, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener2, _Player2);

    velocityPlayer1 = 1;
    velocityPlayer2 = 1;

    potenciadorPlayer1 = 20;
    potenciadorPlayer2 = 0;
    auxPotenciador = 0;
    auxPosisicon = 0;

    for (int i = 0; i < 479; i++) {
        for (int j = 0; j < 689; j++) {
            posicionesOcupadas[i][j] = false;
        }
    }

    schedule(CC_SCHEDULE_SELECTOR(MultiPlayer::movimientoPlayer1));
    //schedule(CC_SCHEDULE_SELECTOR(MultiPlayer::movimientoPlayer2));
    schedule(CC_SCHEDULE_SELECTOR(MultiPlayer::verificarChoque));
}

void MultiPlayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application

    Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void MultiPlayer::generarLuzPlayer1(float rot, float x, float y) {
    _LuzPlayer1 = Sprite::create("Players/RayoRojo2.png");

    if (_LuzPlayer1 == nullptr)
    {
        problemLoading("'Players/RayoRojo2.png'");
    }
    else
    {  
       
        _LuzPlayer1->setRotation(rot);
        _LuzPlayer1->setPosition(x, y);
       // inicializarFisica(_LuzPlayer1);
        _LucesRojas.pushBack(_LuzPlayer1);
        this->addChild(_LuzPlayer1, 1);

    }
}

void MultiPlayer::generarLuzPlayer2(float rot, float x, float y) {
    _LuzPlayer2 = Sprite::create("Players/RayoAmarillo2.png");

    if (_LuzPlayer2 == nullptr)
    {
        problemLoading("'Players/RayoAmarillo2.png'");
    }
    else
    {
       
        _LuzPlayer2->setRotation(rot);
        _LuzPlayer2->setPosition(x, y);
        //inicializarFisica(_LuzPlayer2);
        _LucesAmarillas.pushBack(_LuzPlayer2);
        this->addChild(_LuzPlayer2, 1);

    }
}

void MultiPlayer::keyPressedPlayer1(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    Vec2 loc = event->getCurrentTarget()->getPosition();

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 90)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 270)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        break;

    case EventKeyboard::KeyCode::KEY_D:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(90);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player1->setPosition(loc.x, loc.y);
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
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        break;

    case EventKeyboard::KeyCode::KEY_S:
        if (event->getCurrentTarget()->getRotation() == 180)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 0)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 90) {
            _Player1->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(180);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player1->setPosition(loc.x, loc.y);
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
            runAction(cocos2d::Sequence::create(delay, NULL));
            
            cocos2d::CallFunc* Acelerar = cocos2d::CallFunc::create([=]() {
                velocityPlayer1 = 2;
                potenciadorPlayer1 -= 20;
            });

            cocos2d::CallFunc* VelocidadNormal = cocos2d::CallFunc::create([=]() {
                velocityPlayer1 = 1;
            });

            runAction(cocos2d::Sequence::create(Acelerar, delay, VelocidadNormal,NULL));
        }
        break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));
        break;
    }

}

void MultiPlayer::keyPressedPlayer2(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    Vec2 loc = event->getCurrentTarget()->getPosition();
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(270);
        }

        if (event->getCurrentTarget()->getRotation() == 90)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 270)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (event->getCurrentTarget()->getRotation() == 180) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(90);
        }

        if (event->getCurrentTarget()->getRotation() == 0) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(90);
        }

        if (event->getCurrentTarget()->getRotation() == 90)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 270) 
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        break;

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        if (event->getCurrentTarget()->getRotation() == 180)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 0)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 90) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(0);
        }

        break;

    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        if (event->getCurrentTarget()->getRotation() == 180)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 0)
            event->getCurrentTarget()->setRotation(event->getCurrentTarget()->getRotation());

        if (event->getCurrentTarget()->getRotation() == 90) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(180);
        }

        if (event->getCurrentTarget()->getRotation() == 270) {
            _Player2->setPosition(loc.x, loc.y);
            event->getCurrentTarget()->setRotation(180);
        }

        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_SHIFT:
        if (potenciadorPlayer2 >= 20) {

            cocos2d::DelayTime* delay = cocos2d::DelayTime::create(3);
            runAction(cocos2d::Sequence::create(delay, NULL));

            cocos2d::CallFunc* Acelerar = cocos2d::CallFunc::create([=]() {
                velocityPlayer2 = 2;
                potenciadorPlayer2 -= 20;
                });

            cocos2d::CallFunc* VelocidadNormal = cocos2d::CallFunc::create([=]() {
                velocityPlayer2 = 1;
                });

            runAction(cocos2d::Sequence::create(Acelerar, delay, VelocidadNormal, NULL));
        }
        break;
    }

}

void MultiPlayer::movimientoPlayer1(float dt) {

    Vec2 loc = _Player1->getPosition();
    Vec2 loc2 = _Player2->getPosition();
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    if (_Player1->getRotation() == 180) {
        _Player1->setPosition(loc.x, loc.y - velocityPlayer1);
        ocupadosx.push_back(loc.x);
        ocupadosy.push_back(loc.y);
        posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        auxPosisicon++;

    }
    else if (_Player1->getRotation() == 0) {
        _Player1->setPosition(loc.x, loc.y + velocityPlayer1);
        ocupadosx.push_back(loc.x);
        ocupadosy.push_back(loc.y);
        posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        auxPosisicon++;
    }
    else if (_Player1->getRotation() == 270) {
        _Player1->setPosition(loc.x - velocityPlayer1, loc.y);
        ocupadosx.push_back(loc.x);
        ocupadosy.push_back(loc.y);
        posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        auxPosisicon++;
    }
    else if (_Player1->getRotation() == 90) {
        _Player1->setPosition(loc.x + velocityPlayer1, loc.y);
        ocupadosx.push_back(loc.x);
        ocupadosy.push_back(loc.y);
        posicionesOcupadas[int(loc.x)][int(loc.y)] = true;
        auxPosisicon++;
    }
    else
        _Player1->setPosition(loc.x, loc.y);

    
    if (_Player2->getRotation() == 180) {
        _Player2->setPosition(loc2.x, loc2.y - velocityPlayer2);
        ocupadosx.push_back(loc2.x);
        ocupadosy.push_back(loc2.y);
        posicionesOcupadas[int(loc2.x)][int(loc2.y)] = true;
        auxPosisicon++;
    }
    else if (_Player2->getRotation() == 0) {
        _Player2->setPosition(loc2.x, loc2.y + velocityPlayer2);
        ocupadosx.push_back(loc2.x);
        ocupadosy.push_back(loc2.y);
        posicionesOcupadas[int(loc2.x)][int(loc2.y)] = true;
        auxPosisicon++;
    }
    else if (_Player2->getRotation() == 270) {
        _Player2->setPosition(loc2.x - velocityPlayer2, loc2.y);
        ocupadosx.push_back(loc2.x);
        ocupadosy.push_back(loc2.y);
        posicionesOcupadas[int(loc2.x)][int(loc2.y)] = true;
        auxPosisicon++;
    }
    else if (_Player2->getRotation() == 90) {
        _Player2->setPosition(loc2.x + velocityPlayer2, loc2.y);
        ocupadosx.push_back(loc2.x);
        ocupadosy.push_back(loc2.y);
        posicionesOcupadas[int(loc2.x)][int(loc2.y)] = true;
        auxPosisicon++;
    }
    else
        _Player2->setPosition(loc2.x, loc2.y);
  
    generarLuzPlayer1(_Player1->getRotation(), loc.x, loc.y);
    generarLuzPlayer2(_Player2->getRotation(), loc2.x, loc2.y);

}

//void MultiPlayer::movimientoPlayer2(float dt){
//    Vec2 loc = _Player2->getPosition();
//    auto director = Director::getInstance();
//    auto tamano = director->getWinSize();
//
//    /*if (loc.x < tamano.width || loc.x > 0 || loc.y < tamano.height || loc.y > 0) {*/
//        if (_Player2->getRotation() == 180) {
//            _Player2->setPosition(loc.x, loc.y - velocityPlayer2);
//            ocupadosx.push_back(loc.x);
//            ocupadosy.push_back(loc.y);
//        }
//        else if (_Player2->getRotation() == 0) {
//            _Player2->setPosition(loc.x, loc.y + velocityPlayer2);
//            ocupadosx.push_back(loc.x);
//            ocupadosy.push_back(loc.y);
//        }
//        else if (_Player2->getRotation() == 270) {
//            _Player2->setPosition(loc.x - velocityPlayer2, loc.y);
//            ocupadosx.push_back(loc.x);
//            ocupadosy.push_back(loc.y);
//        }
//        else if (_Player2->getRotation() == 90) {
//            _Player2->setPosition(loc.x + velocityPlayer2, loc.y);
//            ocupadosx.push_back(loc.x);
//            ocupadosy.push_back(loc.y);
//        }
//        else
//            _Player2->setPosition(loc.x, loc.y);
//    /*}else
//        _Player2->setPosition(tamano.width / 2, tamano.height * 0.23);*/
//
//    generarLuzPlayer2(_Player2->getRotation(), loc.x, loc.y);
//
//}

void MultiPlayer::verificarChoque(float det) {

    Rect sensorColisionPlayer1 = _Player1->getBoundingBox();
    Rect sensorColisionPlayer2 = _Player2->getBoundingBox();

    Rect sensorColisionLuzPlayer1 = _LuzPlayer1->getBoundingBox();
    Rect sensorColisionLuzPlayer2 = _LuzPlayer2->getBoundingBox();

    Rect sensorPotenciador = _BonusVelocity->getBoundingBox();

    Vec2 loc = _Player1->getPosition();
    Vec2 loc2 = _Player2->getPosition();

    auto director = Director::getInstance();
    auto tamano = director->getWinSize();

    srand(time(NULL));
    int BonusY = rand();
    BonusY = rand() % 700;

    int BonusX = rand();
    BonusX = rand() % 480;

    if (NVidasPlayer1 > 0 && NVidasPlayer2 > 0) {
        if (sensorColisionPlayer1.intersectsRect(sensorColisionPlayer2)) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer1--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);

        }
        else if (sensorColisionPlayer2.intersectsRect(sensorColisionPlayer1)) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer2--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);
        }
        else if (pasoporAqui(loc.x, loc.y) == true) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer1--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);
        }
        else if (pasoporAqui(loc2.x, loc2.y) == true) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer2--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);
        }
        else if (sensorColisionPlayer1.intersectsRect(sensorPotenciador)) {
            potenciadorPlayer1 += 20;
            _BonusVelocity->setPosition(BonusX, BonusY);
            if (auxPotenciador < 10)
                auxPotenciador++;
            else
                auxPotenciador = 0;
        }
        else if (sensorColisionPlayer2.intersectsRect(sensorPotenciador)) {
            potenciadorPlayer2 += 20;
            _BonusVelocity->setPosition(BonusX, BonusY);
            if (auxPotenciador < 10)
                auxPotenciador++;
            else
                auxPotenciador = 0;
        }
        else if (loc.x > tamano.width-40 || loc.x < 40 || loc.y > tamano.height-20 || loc.y < 0) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer2--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);
        }
        else if (loc2.x > tamano.width-40 || loc2.x < 40 || loc2.y > tamano.height-20 || loc2.y < 0) {
            cleanup();
            removeAllChildrenWithCleanup(true);
            ocupadosx.clear();
            ocupadosy.clear();
            IniciarScena();
            NVidasPlayer2--;
            dibujarVidas(NVidasPlayer1, NVidasPlayer2);
        }
    }
    else {
        cleanup();
        removeAllChildrenWithCleanup(true);
        ocupadosx.clear();
        ocupadosy.clear();
        IniciarScena();
        NVidasPlayer1 = 3;
        NVidasPlayer2 = 3;
        dibujarVidas(NVidasPlayer1, NVidasPlayer2);
    }
}

void MultiPlayer::dibujarVidas(int vidas1, int vidas2) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    NVidasPlayer1 = vidas1;
    int count = 10;
    for (int i = 0; i < NVidasPlayer1; i++) {
        _VidasPlayer1 = Sprite::create("Players/Vida1.png");
        _VidasPlayer1->setPosition(Vec2(origin.x + (visibleSize.width - count),
            origin.y + visibleSize.height - _VidasPlayer1->getContentSize().height));
        count += 10;
        addChild(_VidasPlayer1, 2);
    }

    NVidasPlayer2 = vidas2;
    int count2 = 210;
    for (int i = 0; i < NVidasPlayer2; i++) {
        _VidasPlayer2 = Sprite::create("Players/Vida2.png");
        _VidasPlayer2->setPosition(Vec2(origin.x + (visibleSize.width - count2),
            origin.y + visibleSize.height - _VidasPlayer2->getContentSize().height));
        count2 -= 10;
        addChild(_VidasPlayer2, 2);
    }
}

bool MultiPlayer::pasoporAqui(float x, float y) {
    /*for (int i = 0; i < ocupadosx.size() - 1; i++) {
        if (ocupadosx[i] == x && ocupadosy[i] == y) {
            return true;
        }
        else if (posicionesOcupadas[int(x)][int(y)] == true) {
            return true;
        }
    }*/
    if (posicionesOcupadas[int(x)][int(y)] == true)
        return true;
    return false;
}


