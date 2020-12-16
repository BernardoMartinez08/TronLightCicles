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

#ifndef __MULTIPLAYER_SCENE_H__
#define __MULTIPLAYER_SCENE_H__

#include "cocos2d.h"

class MultiPlayer : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createSceneMulti();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void inicializarFisica(cocos2d::Sprite* sprite);
    // implement the "static create()" method manually
    CREATE_FUNC(MultiPlayer);

    cocos2d::Sprite* _Player1;
    cocos2d::Sprite* _Player2;
    cocos2d::Sprite* _LuzPlayer1;
    cocos2d::Sprite* _LuzPlayer2;
    cocos2d::Sprite* _VidasPlayer1;
    cocos2d::Sprite* _VidasPlayer2;
    cocos2d::Sprite* _BonusVelocity;

    cocos2d::Vector<cocos2d::Sprite*> _LucesRojas;
    cocos2d::Vector<cocos2d::Sprite*> _LucesAmarillas;

    int NVidasPlayer1;
    int NVidasPlayer2;
    double velocityPlayer1;
    double velocityPlayer2;
    int potenciadorPlayer1;
    int potenciadorPlayer2;
    int auxPotenciador;

    cocos2d::Label* labelPause;
    cocos2d::Label* labelResume;

    //cocos2d::Vector < cocos2d::Sprite* > _LucesRojas;
    //cocos2d::Vector < cocos2d::Sprite* > _LucesAmarillas;

    void IniciarScena();
    void keyPressedPlayer2(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void keyPressedPlayer1(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void generarLuzPlayer1(float, float, float);
    void generarLuzPlayer2(float, float, float);

    void movimientoPlayer1(float);
    void movimientoPlayer2(float);

    void verificarChoque(float);
    void dibujarVidas(int,int);

    std::vector<double> ocupadosx;
    std::vector<double> ocupadosy;

    bool posicionesOcupadas[481][701];
    int auxPosisicon;

    bool pasoporAqui(float, float);
};

#endif // __HELLOWORLD_SCENE_H__
