#pragma once
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include "cmd_opts.h"
using namespace std;
struct PlayerLevel{
    float first_sev_scr; //发球直接得分率
    float atk_hit; //进攻时命中率（未命中则对手加分，命中则进行对方接球判定）
    float nor_hit; //非进攻时命中率
    float atk_catch; //受进攻时接球成功率
    float nor_catch; //受非进攻时接球成功率
    float sevatk_usg; //发球抢攻使用率（己方第二板）
    float sevctratk_usg; //发球反攻使用率（己方第二板）
    float cthatk_usg; //接球抢攻使用率（对方第一板）
    int stalemate_lv; //相持状态能力（按两方能力计算相持状态胜率）
    int syn_lv; //综合总能力
    float addi_hit=1; //心理状态和随机情况对命中率的加成倍率
    float addi_cth=1; //心理状态和随机情况对接球成功率的加成倍率
    int power=100; //体能剩余
};

//比赛类,数据成员小写开头，成员函数大写开头
class Game{
    public:
    int calcmod;
    int one_game;//1为11分制，2为21分制
    int whole_game;//3456代表三局两胜/五局三胜/七局四胜/仅一局
    bool demo; //是否演示每一局具体过程
    char strindex[2]={'A','B'};
    PlayerLevel plr[2]; //双方球员编号为0/1

    private:
    int og_score[2];//小比分
    int wg_score[2];//大比分
    int plr_hold; //持球方（plr_hold=1-plr_hold实现球权交换）
    int plr_sev; //发球方（同上实现球权交换） 
    int plr_alrsev; //当前发球方已发球个数
    int plr_firsev; //本局首发球方（每局开始同上实现交换，第一次随机）

    //随机函数,根据prob参数作为真的概率,随机并返回真假
    bool Random(float prob);

    //判断比赛是否结束,函数返回true则结束
    bool Checkover();

    //发球函数,phit->(1-phit)
    //mod=0为常规非进攻发球,1为第一板发球,2为接抢,3为发抢,4为发球反攻,5为接发球
    void Hit(int phit,int mod);

    //接球函数，球员接到球后的思考过程，决定策略后调用Hit（是否接到球在上一个Hit中计算）
    //mod=0为常规非进攻接球,1为接到对方发球,2为接到对方接抢,3为接到对方发抢,4为接到对方发球反攻
    void Catch(int pcth,int mod);

    //相持函数
    void Stalemate();

    //处理选手心理状况+
    //随机改变选手状态的函数
    void MoodandRand();
    /*心理影响
    当单内分差达到临界值（设为4）时，进行随机
    领先方有1/8概率放松（接球率、命中率下降）
    失败方有1/2概率反追（命中率提升）*/

    public:
    Game(int a=1,int b=4,int cm=1):one_game(a),whole_game(b),calcmod(cm)
    {
        memset(og_score,0,sizeof(og_score));
        memset(wg_score,0,sizeof(wg_score));
        demo=false;
    }
    //模拟比赛 
    int Simulation(); 

    //计算总能力值   
    void CalcsynLevel(PlayerLevel &p);                                                                          
};

float RandomVal(float l,float r); //随机数生成，产生一个l-r的数(0<l<r<1)