#include "cmd_opts.h" //控制台相关操作
#include "ppong.h" //模拟比赛
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
/* 一些关于窗口位置的参数 
    窗口大小 X=90,Y=30
    左侧列 X=10 选择符号 X=8
    右侧菜单列 X=40（参数详细设置）选择符号X=38
    最右侧列 X=65（退格和下一步）
    顶行 Y=3 
*/
void InitializeWindow(); //窗口初始化
void Page_MainMenu(); //主菜单页
void Page_Custom_Paraset(); //自定义计算参数设置
void Page_Custom_Paraset_RMenu(const int index); //自定义计算参数界面右侧菜单（此函数只负责显示）
void Page_Custom_Paraset_Input(PlayerLevel &y,int id); //自定义选手参数输入
bool Page_CustomII();
void Page_Demo(); //演示计算I
void Page_DemoII(); //演示计算II
void ShowPlrPara(int i); //显示选手参数（子函数）
void Page_Result(); //显示结果

string to_string2(float x);//保留两位小数的将0-1之间的小数转为两位

Game usergame; //用户定义赛制

int main()
{
    InitializeWindow();
    srand(time(NULL));
    while(1){
        Page_MainMenu();
    }
    system("pause");
    return 0;
}

void InitializeWindow(){
    copts_setconsoleborder(90, 30, 90, 30);
    copts_setconsoletitle("乒乓球胜率计算程序");
    copts_fixconsolesize();
    copts_hidecursor();
}

void Page_MainMenu(){
    int keyin;
    copts_cls();
    copts_printxy(10,5,"乒乓球胜率计算程序");
    copts_printxy(10,7,"Developed by UNIkeEN");
    copts_printxy(10,9,"---------------------------------");
    copts_printxy(10,11,"[1] 开始新的自定义计算");
    copts_printxy(10,13,"[2] 预设计算演示");
    copts_printxy(10,15,"[ESC] 退出本程序");
    copts_printxy(65,29,"Released_1.2.1_20211217");
    while(1){
        keyin=_getch()-48;
        if (keyin==-21) exit(0); //ESC 27-48
        if (keyin==1||keyin==2) break;
    }
        if (keyin==1) Page_Custom_Paraset();
        else if (keyin==2) Page_Demo();
    return;
}

void Page_Custom_Paraset(){
    para:
    int keyin,select_index=1;
    copts_cls();
    copts_printxy(10,3,"[Backspace] 上一级菜单");
    copts_printxy(65,3,"[ESC] 退出本程序");
    copts_printline(10,5,70);
    copts_printxy(10,7,"在这里修改有关选手、比赛、计算模型的相关参数");
    copts_printxy(10,9,"(按方向键选择一级菜单，按数字键选择二级菜单)");
    copts_printxy(65,27,"[N] 下一步");
    string Paralist[5]=
        {"","I.选手参数","II.赛制参数","III.计算模型"};
    for(int i=1;i<=3;i++){
        copts_printxy(10,9+2*i,Paralist[i]);  
    }
    copts_printxy(8,9+2*select_index,"> "); 
    Page_Custom_Paraset_RMenu(select_index); 
    while(1){
        keyin=_getch();
        if (keyin==27) exit(0); //ESC 27
        if (keyin==8) return; //退格 8
        else if(keyin==78||keyin==110){ //N 78 n 110开始
        usergame.demo=false;
        bool ret;
        ret=Page_CustomII();
        if(ret) goto para;
        else return;
        }
        else if (keyin==(224)){ //方向键控制左侧列表上下
            int dirct;
            dirct=_getch();
            switch (dirct){
            case 72:{
                if(select_index>1) select_index--;
                break;
                }
            case 80:{
                if(select_index<3) select_index++;
                break;
                }   
            }
            for(int i=1;i<=3;i++) copts_printxy(8,9+2*i," ");  
            copts_printxy(8,9+2*select_index,"> "); 
            Page_Custom_Paraset_RMenu(select_index);
        }
        else if(keyin>=48&&keyin<=57){ //数字键选择二级菜单
            switch(select_index){
                case 1:{
                    switch(keyin){
                        case 49:Page_Custom_Paraset_Input(usergame.plr[0],0);goto para;break;
                        case 50:Page_Custom_Paraset_Input(usergame.plr[1],1);goto para;break;
                        case 51:{
                            usergame.plr[1]=usergame.plr[0];
                            goto para;
                            break;
                        }
                    }
                }
                case 2:{
                    if(keyin>=49&&keyin<=50){
                    for(int i=1;i<=2;i++) copts_printxy(38,9+2*i," ");   
                    copts_printxy(38,9+2*(keyin-48),"> ");
                    usergame.one_game=keyin-48;   
                    }
                    else if(keyin>=51&&keyin<=54){
                    for(int i=3;i<=6;i++) copts_printxy(38,9+2*i," ");   
                    copts_printxy(38,9+2*(keyin-48),"> ");   
                    usergame.whole_game=keyin-48;   
                    } 
                    break;
                }
                case 3:break;
            }
        }
    }
}

void Page_Custom_Paraset_RMenu(const int index){
    for(int i=11;i<25;i++) copts_printspace(40,i,52);
    for(int i=11;i<25;i++) copts_printxy(38,i," ");
    switch (index){
        case 1:{
            copts_printxy(40,11,"[1] 修改选手A能力参数");
            copts_printxy(40,13,"[2] 修改选手B能力参数");
            copts_printxy(40,15,"[3] 使选手B能力参数与A一致");
            break;
        }
        case 2:{
            copts_printxy(40,11,"[1] 11分制");
            copts_printxy(40,13,"[2] 21分制");
            copts_printxy(40,15,"[3] 三局两胜制");
            copts_printxy(40,17,"[4] 五局三胜制");
            copts_printxy(40,19,"[5] 七局四胜制");
            copts_printxy(40,21,"[6] 一局定胜负");
            copts_printxy(38,9+2*usergame.one_game,"> ");
            copts_printxy(38,9+2*usergame.whole_game,"> ");
            break;
        }
        case 3:{
            copts_printxy(40,11,"[1] 仿真比赛");
            copts_printxy(38,9+2*usergame.calcmod,"> ");
            break;
        }
    }
}

void Page_Custom_Paraset_Input(PlayerLevel &y,int id){
    PlayerLevel *p=&y;
    copts_showcursor();
    copts_cls();
    printf("请输入选手%c的参数(请保证数据合法，本程序不做判断)\n",usergame.strindex[id]);
    printf("请输入发球直接得分率(0<x<1):\n");
    scanf("%f",&p->first_sev_scr);
    printf("请输入进攻时命中率(0<x<1):\n");
    scanf("%f",&p->atk_hit);
    printf("请输入非进攻时命中率(0<x<1):\n");
    scanf("%f",&p->nor_hit);
    printf("请输入受进攻接球成功率(0<x<1):\n");
    scanf("%f",&p->atk_catch);
    printf("请输入非受进攻接球成功率(0<x<1):\n");
    scanf("%f",&p->nor_catch);
    printf("请输入发球抢攻使用率(0<x<1):\n");
    scanf("%f",&p->sevatk_usg);
    printf("请输入发球反攻使用率(0<x<1):\n");
    scanf("%f",&p->sevctratk_usg);
    printf("请输入接球抢攻使用率(0<x<1):\n");
    scanf("%f",&p->cthatk_usg);
    printf("请输入相持状态能力(0<x<100):\n");
    scanf("%d",&p->stalemate_lv);
    int keyin;
    printf("\n输入完成，按任意键继续\n");
    keyin=_getch();
    copts_hidecursor();
    return;
}
bool Page_CustomII(){
    int keyin;
    copts_cls();
    copts_printxy(10,3,"[Backspace] 上一级菜单");
    copts_printxy(65,3,"[ESC] 退出本程序");
    copts_printxy(65,27,"[N] 开始计算");
    copts_printline(10,5,70);
    copts_printxy(10,7,"选手A");
    copts_printxy(40,7,"选手B");
    //随机生成两位选手数值
    PlayerLevel *p;
    for(int i=0;i<=1;i++){
        p=&usergame.plr[i];
        ShowPlrPara(i);
    }
    while(1){
        keyin=_getch();
        if (keyin==27) exit(0); //ESC 27
        if (keyin==8) return 1; //退格 8  
        else if(keyin==78||keyin==110){ //N 78 n 110开始
        usergame.demo=false;
        Page_Result();
        return 0;
    }
    }
}
void Page_Demo(){
    int keyin;
    copts_cls();
    copts_printxy(10,3,"[Backspace] 上一级菜单");
    copts_printxy(65,3,"[ESC] 退出本程序");
    copts_printxy(65,27,"[N] 开始计算");
    copts_printline(10,5,70);
    copts_printxy(10,7,"第一步 随机生成选手能力 请稍后");
    Sleep(500);
    copts_printxy(10,7,"选手A                         ");
    copts_printxy(40,7,"选手B");
    //随机生成两位选手数值
    PlayerLevel *p;
    for(int i=0;i<=1;i++){
        p=&usergame.plr[i];
        p->first_sev_scr=RandomVal(0.05,0.16); 
        p->atk_hit=RandomVal(0.65,0.9);
        p->nor_hit=RandomVal(0.9,0.98);
        p->atk_catch=RandomVal(0.65,0.75);
        p->nor_catch=RandomVal(0.9,0.95);
        p->sevatk_usg=RandomVal(0.3,0.55);
        p->sevctratk_usg=RandomVal(0.3,0.55);
        p->cthatk_usg=RandomVal(0.3,0.45);
        p->stalemate_lv=(int)(RandomVal(0.45,0.55)*100);
        ShowPlrPara(i);
    }
    while(1){
        keyin=_getch();
        if (keyin==27) exit(0); //ESC 27
        if (keyin==8) return; //退格 8  
        else if(keyin==78||keyin==110){ //N 78 n 110开始
        //Page_DemoII();
        usergame.demo=true;
        Page_DemoII();
        return;
    }
    }
}

void ShowPlrPara(int i){
    PlayerLevel *p=&usergame.plr[i];
    copts_printxy(10+30*i,9,"发球得分率:"+to_string2(p->first_sev_scr));
    copts_printxy(10+30*i,10,"进攻命中率:"+to_string2(p->atk_hit));
    copts_printxy(10+30*i,11,"非进攻命中率:"+to_string2(p->nor_hit));
    copts_printxy(10+30*i,12,"受进攻接球率:"+to_string2(p->atk_catch));
    copts_printxy(10+30*i,13,"非受进攻接球率:"+to_string2(p->nor_catch));
    copts_printxy(10+30*i,14,"发抢使用率:"+to_string2(p->sevatk_usg));
    copts_printxy(10+30*i,15,"发球反攻使用率:"+to_string2(p->sevctratk_usg));
    copts_printxy(10+30*i,16,"接抢使用率:"+to_string2(p->cthatk_usg));
    copts_printxy(10+30*i,17,"相持得分能力:"+to_string(p->stalemate_lv));
    copts_printxy(10+30*i,19,"综合能力评分:"+to_string(p->syn_lv));

    usergame.CalcsynLevel(*p);
    copts_printxy(10+30*i,19,"综合能力评分:"+to_string(p->syn_lv));
}

void Page_DemoII(){
    int keyin;
    copts_cls();
    copts_printxy(10,7,"第二步 模拟一场比赛（一局定胜制）\n");
    Sleep(1000);
    usergame.demo=true;
    usergame.one_game=1;
    usergame.whole_game=6;
    int tmp=usergame.Simulation();
    printf("\n按任意键进入下一页\n");
    keyin=_getch();
    Page_Result();
    return;
}

void Page_Result(){
    int keyin;
    copts_cls();
    copts_hidecursor();
    if(usergame.demo){
        usergame.one_game=1;
        usergame.whole_game=5;
        copts_printxy(10,7,"第三步 模拟一千场比赛(11分制，七局四胜)");
        Sleep(1000);
        copts_cls();
        copts_gotoxy(0,0);
        usergame.demo=0;
        int tot[2]={0};
        for(int i=1;i<=1000;i++){
            int tmp=usergame.Simulation();
            tot[tmp]++;
            printf("已模拟%d场比赛，当前总胜场数 %d : %d \n",i,tot[0],tot[1]);
            Sleep(5);
        }
        printf("\n按任意键进入下一页\n");
        keyin=_getch();
        usergame.one_game=2;
        usergame.whole_game=3;
        copts_cls();
        copts_printxy(10,7,"第四步 模拟一千场比赛(21分制，三局两胜)");
        Sleep(1000);
        copts_cls();
        usergame.demo=0;
        int tot2[2]={0};
        for(int i=1;i<=1000;i++){
            int tmp=usergame.Simulation();
            tot2[tmp]++;
            printf("已模拟%d场比赛，当前总胜场数 %d : %d \n",i,tot2[0],tot2[1]);
            Sleep(5);
        }
        printf("\n按任意键进入下一页\n");
        keyin=_getch();
        copts_cls();
        printf("11分制（七局四胜）下 总胜场数 %d : %d \n\n",tot[0],tot[1]);
        printf("选手A胜率 : %.2lf %\n",(100*(double)tot[0]/(tot[0]+tot[1])));
        printf("选手B胜率 : %.2lf %\n\n",(100*(double)tot[1]/(tot[0]+tot[1])));
        printf("21分制（三局两胜）下 总胜场数 %d : %d \n\n",tot2[0],tot2[1]);
        printf("选手A胜率 : %.2lf %\n",(100*(double)tot2[0]/(tot2[0]+tot2[1])));
        printf("选手B胜率 : %.2lf %\n\n",(100*(double)tot2[1]/(tot2[0]+tot2[1])));
        printf("\n按任意键返回\n");
        keyin=_getch();
        return;
    }

    else{
        int tot[2]={0};
        for(int i=1;i<=10000;i++){
            int tmp=usergame.Simulation();
            tot[tmp]++;
            if(i%1000==0) printf("已模拟%d场比赛，当前总胜场数 %d : %d \n",i,tot[0],tot[1]);
        }  
        Sleep(500); 
        copts_cls();
        if(usergame.one_game==1)printf("本场比赛采用:\n11分制 ");
        else printf("本场比赛采用:\n21分制 ");
        switch (usergame.whole_game){
            case 3:printf("三局两胜\n\n");break;
            case 4:printf("五局三胜\n\n");break;
            case 5:printf("七局四胜\n\n");break;
            case 6:printf("一局定胜\n\n");break;
        }
        printf("本次共模拟%d场比赛，总胜场数 %d : %d \n",10000,tot[0],tot[1]);
        printf("\n选手A胜率 : %.2lf %\n",(100*(double)tot[0]/(tot[0]+tot[1])));
        printf("选手B胜率 : %.2lf %\n",(100*(double)tot[1]/(tot[0]+tot[1])));
    printf("\n按任意键返回\n");
    keyin=_getch();
    return;
    }
}


string to_string2(float x){
    if(x<0||x>1) return("ERROR");
    if(x==1) return("1.00");
    int a=(int)(x*100);
    if(a<10) return ("0.0"+to_string(a));
    return ("0."+to_string(a));
}