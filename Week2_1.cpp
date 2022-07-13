//https://www.acmicpc.net/problem/1712

#include <iostream>

using namespace std;


int main(void){
    int p[3];
    //cout << "고정 비용, 가변 비용, 노트북 가격은? ";
    for(int i=0;i<3;i++){
        cin >> p[i];
    }

    /*
    while (1)
    {
        if(p[1]>=p[2]){//노트북 가격이 가변비용보다 낮은경우 이득을 볼 수 없다.
            count = -1;
            break;
        }
        count++;
        if(p[2]*count>(p[1]*count+p[0])){
            cout<<p[2]*count<<" "<<p[1]*count+p[0];//p[1]*count+p[0]의 결과가 int의 범위를 넘어 버림
            break;
        }
    }
    */
   if(p[1]>=p[2])cout<<-1;
   else{
    unsigned result= p[0]/(p[2]-p[1])+1;
    cout << result;
   }
    
    return 0;
}