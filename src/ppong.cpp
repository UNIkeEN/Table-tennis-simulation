#include "ppong.h"

bool Game::Random(float prob){
    float ran;
    ran= rand()/double(RAND_MAX+1);
    if(ran<=prob) return true;
    else return false;
}

int Game::Simulation(){
    memset(og_score,0,sizeof(og_score));
    memset(wg_score,0,sizeof(wg_score));
    int wggoal; //wggoal为一场比赛结束所需赢场次数
    int oggoal; //oggoal为一局比赛结束所需赢小分数
    switch (whole_game){
        case 3:wggoal=2;break;
        case 4:wggoal=3;break;
        case 5:wggoal=4;break;
        case 6:wggoal=1;break;
    }
    switch (one_game){
        case 1:oggoal=11;break;
        case 2:wggoal=21;break;
    }
    //比赛开始抽签决定第一局发球方
        Random(0.5)?plr_sev=0:plr_sev=1;
        plr_alrsev=0;
        plr_firsev=plr_sev;
    //一场比赛开始
    while(wg_score[0]<wggoal&&wg_score[1]<wggoal)
    {   
        //清空小比分
        memset(og_score,0,sizeof(og_score));
        //交换该局首发球方（第一次页交换，由于概率相等不影响）
        plr_firsev=1-plr_firsev;
        plr_sev=plr_firsev;
        //一局比赛开始
        MoodandRand();
        while(!Checkover()){
            if(demo){
                copts_cls();
                printf("本场比赛采用11分制\n");
                Sleep(500);
            } 
            Hit(plr_sev,1); //发球
            //计算是否更改发球方
            if(og_score[0]>=oggoal-1 && og_score[1]>=oggoal-1) plr_sev=1-plr_sev;
            else{
                plr_alrsev++;
                if(plr_alrsev==2){
                    plr_sev=1-plr_sev;
                    plr_alrsev=0;
                }
            }
            MoodandRand();
        }
        //一局比赛结束
    }
    //一场比赛结束
    if(demo){
        copts_cls();
        printf("比赛结束 小比分 %d : %d\n\n",og_score[0],og_score[1]);
    }
    if (wg_score[0]==wggoal) 
        {if (demo)printf("最终胜者为:选手A\n");
        return 0;}
    else 
        {if(demo)printf("最终胜者为:选手B\n");
        return 1;}
}

void Game::Hit(int phit,int mod){
    //demo模式输出
    if(demo){
        switch(mod){
            case 0:{printf("\n选手 %c 非进攻回打，进入相持 ",strindex[phit]);Sleep(300);break;}
            case 1:{printf("\n选手 %c 发球 ",strindex[phit]);Sleep(300);break;}
            case 2:{printf("\n选手 %c 接球抢攻 ",strindex[phit]);Sleep(300);break;}
            case 3:{printf("\n选手 %c 发球抢攻 ",strindex[phit]);Sleep(300);break;}
            case 4:{printf("\n选手 %c 发球反攻 ",strindex[phit]);Sleep(300);break;}
            case 5:{printf("\n选手 %c 接发球 ",strindex[phit]);Sleep(300);break;}
        }
    }

    if (mod==1){ //第一板发球
        if(Random(plr[phit].first_sev_scr)) 
            {og_score[phit]++; 
            if(demo){printf("直接得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
            return;}
    }
    //是否命中,先讨论未命中
    if(mod>=2&&mod<=4){
        if(!Random(plr[phit].atk_hit*plr[phit].addi_hit)) 
        {og_score[1-phit]++;
        if(demo){printf("出界，对方得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        return;}
    }
    else if(!Random(plr[phit].nor_hit*plr[phit].addi_hit)) 
        {og_score[1-phit]++;
        if(demo){printf("出界，对方得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        return;}
    //命中后转到对手的Catch
    Catch(1-phit,mod);
    return;
}

void Game::Catch(int pcth,int mod){
    //是否接住，先讨论未接住
    if(mod>=2&&mod<=4){
        if(!Random(plr[pcth].atk_catch*plr[pcth].addi_cth)) 
        {og_score[1-pcth]++; 
        if(demo){printf("对手未接住，己方得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        return;}
    }
    else if(!Random(plr[pcth].nor_catch*plr[pcth].addi_cth)) 
        {og_score[1-pcth]++; 
        if(demo){printf("对手未接住，己方得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        return;}
    //接住后判断对策
    if(demo) printf("\n");
    switch (mod){
        case 0:{Stalemate(); return;}
        case 1:{ //接到对方发球
            if(Random(plr[pcth].cthatk_usg)) Hit(pcth,2);//接抢
            else Hit(pcth,5); //接发球
            break;
        }
        case 2:{ //接到对方接抢
            if(Random(plr[pcth].sevctratk_usg)) Hit(pcth,4);//发球反攻
            else Hit(pcth,0); //进入相持
            break;
        }
        case 3: Hit(pcth,0); break;
        case 4: Hit(pcth,0); break;
        case 5:{ //接到对方接发球
            if(Random(plr[pcth].sevatk_usg)) Hit(pcth,3);//发球抢攻
            else Hit(pcth,0); //进入相持
            break;
        }
    }
}

void Game::Stalemate(){
    int a=plr[0].stalemate_lv,b=plr[1].stalemate_lv;
    if(Random(a/(float)(a+b))) 
        {og_score[0]++;
        if(demo){printf("\n\n相持结束，选手A得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        }
    else 
        {og_score[1]++;
        if(demo){printf("\n\n相持结束，选手B得分---当前小比分 %d : %d \n",og_score[0],og_score[1]);Sleep(1100);}
        }
    return;
}

bool Game::Checkover(){
    int t;
    if(abs(og_score[0]-og_score[1])<=1||
        (og_score[0]>og_score[1]?og_score[0]:og_score[1])<(one_game==1?11:21))
    return false;
    t=(og_score[0]>og_score[1]?0:1);
    wg_score[t]++;
    if(demo){
        printf("\n本局结束 小比分 %d : %d\n\n本局胜者为:选手%c\n"
            ,og_score[0],og_score[1],strindex[t]);
        Sleep(1500);
    }
    return true;
} 

void Game::MoodandRand(){
    plr[0].addi_hit=RandomVal(0.95,1.1);
    plr[1].addi_hit=RandomVal(0.95,1.1);
    plr[0].addi_cth=RandomVal(0.95,1.1);
    plr[1].addi_cth=RandomVal(0.95,1.1);
    for(int i=0;i<=1;i++){
        if(plr[i].addi_hit*plr[i].atk_hit>=1||plr[i].addi_hit*plr[i].nor_hit>=1)
        plr[i].addi_hit=1;
        if(plr[i].addi_cth*plr[i].atk_catch>=1||plr[i].addi_cth*plr[i].nor_catch>=1)
        plr[i].addi_cth=1;
    }
    if (!(abs(og_score[0]-og_score[1])<=1||
        (og_score[0]>og_score[1]?og_score[0]:og_score[1])<(one_game==1?11:21)))
        return;
    int plead=(og_score[0]>og_score[1]?0:1);
    if (og_score[plead]-og_score[1-plead]>=3){
        if(Random(0.125)){ 
            if(demo){printf("\n分差较大，领先方选手 %c 有所放松 (下一球接球率、命中率略微下降)\n",strindex[plead]); Sleep(1500);}
            plr[plead].addi_hit-=RandomVal(0,0.25);
            plr[plead].addi_cth-=RandomVal(0,0.25);
        }
        if(Random(0.333)){
            if(demo) {printf("\n落后方选手 %c 坚持不懈,奋起直追 (下一球命中率略微提升)\n",strindex[1-plead]); Sleep(1500);}
            plr[1-plead].addi_hit+=RandomVal(0,0.2);
            PlayerLevel &backw=plr[1-plead];
            if(backw.addi_hit*backw.atk_hit>=1||backw.addi_hit*backw.nor_hit>=1)
            backw.addi_hit=0.99/(backw.nor_hit>backw.atk_hit?backw.nor_hit:backw.atk_hit);
        }
    }
}

void Game::CalcsynLevel(PlayerLevel &p){
    int tot=0;
    tot+=p.first_sev_scr*100;
    tot+=p.nor_hit*150;
    tot+=(p.atk_catch*360+p.nor_catch*240);
    tot+=(p.sevatk_usg*p.atk_hit*150);
    tot+=(p.sevctratk_usg*p.atk_hit*150);
    tot+=(p.cthatk_usg*p.atk_hit*150);
    tot+=p.stalemate_lv*2;
    p.syn_lv=tot;
    return;
}

float RandomVal(float l,float r){
    int ran;
    int L=(int)(l*1000),R=(int)(r*1000);
    ran=rand()%(R-L+1)+L;
    return (ran/1000.00);
}