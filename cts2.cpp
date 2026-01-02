#include <bits/stdc++.h>
using namespace std;

int led[10]={0b1111110,0b0110000,0b1101101,0b1111001,0b0110011,0b1011011,0b1011111,0b1110000,0b1111111,0b1111011};

int countBits(int n){return __builtin_popcount(n);}

int main(){
    string timeSt;
    cin>>timeSt;
    int X,Y;
    cin>>X>>Y;
    int h=stoi(timeSt.substr(0,2));
    int m=stoi(timeSt.substr(3,2));
    vector<int> orig={timeSt[0]-'0',timeSt[1]-'0',timeSt[3]-'0',timeSt[4]-'0'};
    int minCost=INT_MAX;
    string ans="";
    for(int nh=1;nh<=12;nh++){
        for(int nm=0;nm<60;nm++){
            vector<int> newD={nh/10,nh%10,nm/10,nm%10};
            int diffSeg=countBits(led[orig[0]]^led[newD[0]])+
                        countBits(led[orig[1]]^led[newD[1]])+
                        countBits(led[orig[2]]^led[newD[2]])+
                        countBits(led[orig[3]]^led[newD[3]]);
            if(diffSeg!=1) continue;
            int h1=(h==12?0:h);
            int h2=(nh==12?0:nh);
            int diffHr=min(abs(h2-h1),12-abs(h2-h1))*60;
            int diffMin=min(abs(nm-m),60-abs(nm-m));
            int totalCost=diffHr*X+diffMin*Y;
            if(totalCost<minCost){
                minCost=totalCost;
                char buffer[6];
                sprintf(buffer,"%02d:%02d",nh,nm);
                ans=buffer;
            }
        }
    }
    if(ans=="") cout<<"No closest valid time possible";
    else cout<<ans;
    return 0;
}
