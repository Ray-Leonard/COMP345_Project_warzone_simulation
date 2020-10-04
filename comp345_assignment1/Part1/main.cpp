#include <iostream>
#include "Map.h"
#include <string>

using namespace std;

int main() {
    int arr[2][2]={{1,2},{3,4}};
    int** pp;
    pp=new int*[2];
    for (int i = 0; i < 2; i++)
        pp[i] = new int[2];
    for (int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            pp[i][j]=arr[i][j];
        }
    }
    cout<<pp[1][1]<<endl;
    cout<<pp[0][1]<<endl;
    Territory t0("Red",0);
    cout<<"k"<<t0.getOwnership()<<" "<<t0.getId()<<endl;
    cout<<t0.getProduction()<<endl;
    t0.setGarrison(10);
    cout<<t0.getGarrison()<<endl;
    t0.setOwner("Green");
    cout<<t0.getOwnership()<<endl;
    string Cname = "WonderLand";
    Continent c0(Cname);
    cout<<c0.getContinentName()<<endl;
    cout<<c0.getNumOfMembers()<<endl;
    c0.addMember(&t0);
    c0.printMembers();
    Territory t1("Blue",1);
    c0.addMember(&t1);
    c0.printMembers();
    cout<<t1.getId()<<endl;
    c0.setSubRelation(pp);
    c0.printRelation();


    return 0;
}
