#include <iostream>
#include "Map.h"
#include <string>

using namespace std;

int main() {
    //创建二维数组代表节点之间关系，row & column 的index都代表节点ID
    /*
     *    0   1   2   3   4
     * 0  1   0   1   1   1
     * 1  0   1   0   0   0
     * 2  1   1   0   1   0
     * 3  0   0   0   0   0
     * 4  1   1   1   1   1
     */
    int arr[5][5]={{1,0,1,1,1},{0,1,0,0,0},{1,1,0,1,0},{0,0,0,0,0},{1,1,1,1,1}};
    int** pp;//创建二级指针指向二维数组用于传递进 map class作为地图表示节点关系
    //一下为传递过程
    pp=new int*[5];
    for (int i = 0; i < 5; i++)
        pp[i] = new int[5];
    for (int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            pp[i][j]=arr[i][j];
        }
    }
    //创建节点，5个，节点名字可以通过get/set方法改写，默认值为"null"
    Territory t0("Red",0);//t0.setOwner("Green")
    Territory t1("Blue",1);
    Territory t2("Black",2);
    Territory t3("White",3);
    Territory t4("Yellow",4);
    cout<<"Print information of all 5 nodes:"<<endl;
    cout<<"Territory 0: owner->"<<t0.getOwnership()<<" id->"<<t0.getId()<<endl;
    cout<<"Territory 1: owner->"<<t1.getOwnership()<<" id->"<<t1.getId()<<endl;
    cout<<"Territory 2: owner->"<<t2.getOwnership()<<" id->"<<t2.getId()<<endl;
    cout<<"Territory 3: owner->"<<t3.getOwnership()<<" id->"<<t3.getId()<<endl;
    cout<<"Territory 4: owner->"<<t4.getOwnership()<<" id->"<<t4.getId()<<endl;
    //打印每个节点每回合可生产的兵力，默认为1，将来会在初始化时候设置一个随机数1-3
    cout<<"Territory t0 production/each turn"<<t0.getProduction()<<endl;
    cout<<"Territory t1 production/each turn"<<t1.getProduction()<<endl;
    cout<<"Territory t2 production/each turn"<<t2.getProduction()<<endl;
    cout<<"Territory t3 production/each turn"<<t3.getProduction()<<endl;
    cout<<"Territory t4 production/each turn"<<t4.getProduction()<<endl;
    //打印各个节点驻军数量，默认值0，可以被调用在发生战斗或者增强防御,通过get/set方法
    t0.setGarrison(10);
    cout<<"Territory t0's garrison: "<<t0.getGarrison()<<endl;
    //创建一个领地Continent
    string Cname = "WonderLand";
    Continent c0(Cname);
    cout<<"Continent 0 name: "<<c0.getContinentName()<<endl;
    //初始领地无成员，可以通过add方法添加Territory作为其成员
    cout<<"Initial Continent members: "<<c0.getNumOfMembers()<<endl;
    c0.addMember(&t0);//添加成员
    cout<<"Continent has member: ";
    c0.printMembers();//打印
    c0.addMember(&t1);
    cout<<"Continent has member: ";
    c0.printMembers();
    cout<<endl;
    //传入小2D数组，作为领地的关系图，之后会通过改写方法自动生成，暂时未完成
    //c0.setSubRelation(pp);
    //c0.printRelation();
    //创建地图类并赋予名字
    string mapName = "NewWold";
    Map m(mapName);
    cout<<m.getMapName()<<endl;//打印地图名字
    //添加其成员，建议按照顺序添加因为地图关系图的index是节点ID,方便后来查找
    m.addNode(&t0);
    m.addNode(&t1);
    m.addNode(&t2);
    m.addNode(&t3);
    m.addNode(&t4);
    m.printNodes();//打印全部成员
    //传入关系图，代表节点关系
    m.setAllRelation(pp);
    //打印节点关系图
    m.printMap();
    //全部节点数量
    cout<<m.allNodes.size()<<endl;





    return 0;
}
