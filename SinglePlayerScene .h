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

#ifndef __SINGLEPLAYER_SCENE_H__
#define __SINGLEPLAYER_SCENE_H__

#include "cocos2d.h"

class SinglePlayer : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createSceneMulti();

    virtual bool init();
    void iniciarScena();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SinglePlayer);

    cocos2d::Sprite* _Player;
    cocos2d::Sprite* _EnemyA;
    cocos2d::Sprite* _EnemyV;
    cocos2d::Sprite* _EnemyR;
    cocos2d::Sprite* _LuzPlayer;
    cocos2d::Sprite* _LuzEnemyA;
    cocos2d::Sprite* _LuzEnemyV;
    cocos2d::Sprite* _LuzEnemyR;
    cocos2d::Sprite* _VidasPlayer1;
    cocos2d::Sprite* _VidasPlayer2;
    cocos2d::Sprite* _VidasPlayer3;
    cocos2d::Sprite* _BonusVelocity;

    int NVidasPlayer;
    int Level;
    cocos2d::Label* labelPause;
    cocos2d::Label* labelResume;
    cocos2d::Label* labelScore;
    double velocityPlayer1;
    double velocityEnemy;
    int potenciadorPlayer1;
    int auxPotenciador;
    int puntosPlayer;
    int enemysinArena;

    void keyPressedPlayer(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void keyPressedPerdiste(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void generarLuzPlayer(float, float, float);
    void generarLuzEnemy(cocos2d::Sprite* _Enemy,float, float, float);

    void movimientoPlayer(float);
    void movimientoEnemy(float);
    void IAmovimiento(cocos2d::Sprite* _Enemy);
    void updateScore();

    void setVidasPlayer();
    void verificarChoque(float);
    void EnemysByLevel(int);
    void guardarPuntos();
    void GANASTE();
    void PERDISTE();

    bool posicionesOcupadas[481][701];
    bool pasoporAqui(float, float);
};

#endif // __HELLOWORLD_SCENE_H__
