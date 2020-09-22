#include<bits/stdc++.h>
using namespace std;

const long long maxS = 3e5 + 5 , maxElement = 1e9 + 7;
/* Segment trees for max and min elements */
long long segmMax[4*maxS] , segmMin[4*maxS];

void build(long long a[] , int pos , int l , int r , int n){
    if(l >= n || r >= n) return ;
    
    if(l == r){
        segmMax[pos] = a[l];
        segmMin[pos] = a[l];
        return ;
    }
    int mid = (l + r)/2;
    build(a , 2*pos + 1 , l , mid , n);
    build(a , 2*pos + 2 , mid + 1 , r , n);
    segmMax[pos] = max(segmMax[2*pos + 1] , segmMax[2*pos + 2]);
    segmMin[pos] = min(segmMin[2*pos + 1] , segmMin[2*pos + 2]);
}

long long getMinOrMax(int ql , int qr , int l , int r , int pos , bool maxEle){
    if(ql > r || qr < l){
        if(maxEle) return -1;
        else return maxElement;
    }
    if(ql <= l && qr >= r){
        if(maxEle) return segmMax[pos];
        else return segmMin[pos];
    }
    int mid = (l + r)/2;
    long long mini = min(getMinOrMax(ql , qr , l , mid , 2*pos + 1 , maxEle) , getMinOrMax(ql , qr , mid + 1 , r , 2*pos + 2 , maxEle))
            , maxi = max(getMinOrMax(ql , qr , l , mid , 2*pos + 1 , maxEle) , getMinOrMax(ql , qr , mid + 1 , r , 2*pos + 2 , maxEle));
    return (maxEle)?maxi:mini;
}

int main(){
    long long n;
    cin>>n;
    long long a[n] , minDist = n;
    
    /* Set of nodes that are not visited yet */
    set<long long>notVis;
    queue<long long>q;

    for(int i = 0; i < n; i++){
        cin>>a[i];
        if(i)
            notVis.insert(i);
    }
    if(n <= 2){
        cout<<"1\n";
        return 0;
    }

    build(a , 0 , 0 , n - 1 , n);
    
    q.push(0);
    int tempDist = 0;
    while(q.size()){
        long long count = q.size();

        while(count--){
            /* Will contain the nodes that will get visited in this iteration */
            set<long long>deleteNodes;
            long long present = q.front();
            q.pop();
            for(auto it = notVis.begin(); it != notVis.end(); it++){
                if(*it <= present + 1){
                    continue;
                }

                long long index = *it , maxBetween = getMinOrMax(present + 1 , index - 1 , 0 , n - 1 , 0 , 1) 
                        , minBetween = getMinOrMax(present + 1 , index - 1 , 0 , n - 1 , 0 , 0);
                
                if(maxBetween < min(a[present] , a[index]) || minBetween > max(a[present] , a[index])){
                    minDist = min(minDist , n - 1 - index + tempDist + 1);
                    deleteNodes.insert(index);
                    q.push(index);
                }
            }
            for(auto it = deleteNodes.begin(); it != deleteNodes.end(); it++){
                notVis.erase(*it);
            }
        }
        tempDist++;
    }
    
    cout<<minDist;
}
