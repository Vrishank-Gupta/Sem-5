#include<iostream>
#include<list>
#include<stack>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<map>
#include<vector>
#include<iomanip>
#include<bitset>
#include<unordered_set>
#include<set>
#include<sstream>
#include<fstream>
#include<climits>
#define FIO ios_base::sync_with_stdio(false); cin.tie(NULL)
#define endl "\n"
#define ll long long int
#define trav(n) for(long long int i = 0;i<n;++i)
#define trav2(n) for(long long int j = 0;j<n;++j)
#define travm(n) for(long long i = 1;i<n;++i)
#define ul unsigned long
#define ull unsigned long long
#define M 32
#define bigmod 1000000007
#define INF INT_MAX
#define tc long long t; cin>>t; while(t--)
#define MAXDELAY 48
#define MAXPHYS 3
#define POPSIZE 250
#define CROSSOVERRATIO 100

using namespace std;

struct chromosome{
    vector<ll> effect;
    vector<ll> physical;
    double fitness;
    double delay;
    
    chromosome() : effect(13,0), physical(5,0), fitness(0), delay(0) {};
};

struct pedal{
    ll priority;
    ll time;
} pedalref[13];

struct play{
    double influence;
} playref[5];

void initialise(){
    playref[0].influence = 11;
    playref[1].influence = 13;
    playref[2].influence = 21;
    playref[3].influence = 17;
    playref[4].influence = 9;
    
    pedalref[0].priority = 14;
    pedalref[0].time = 12;
    
    pedalref[1].priority = 7;
    pedalref[1].time = 3;
    
    pedalref[2].priority = 9;
    pedalref[2].time = 5;
    
    pedalref[3].priority = 18;
    pedalref[3].time = 4;
    
    pedalref[4].priority = 4;
    pedalref[4].time = 1;
    
    pedalref[5].priority = 19;
    pedalref[5].time = 11;
    
    pedalref[6].priority = 8;
    pedalref[6].time = 6;
    
    pedalref[7].priority = 21;
    pedalref[7].time = 14;
    
    pedalref[8].priority = 11;
    pedalref[8].time = 10;
    
    pedalref[9].priority = 16;
    pedalref[9].time = 4;
    
    pedalref[10].priority = 22;
    pedalref[10].time = 9;
    
    pedalref[11].priority = 10;
    pedalref[11].time = 2;
    
    pedalref[12].priority = 5;
    pedalref[12].time = 5;
}

void mutateinvert(chromosome &child1, chromosome &child2){
    ll index = rand()%13;
    ll index2 = rand()%13;
    if(index > index2) swap(index, index2);
    while(index > index2){
        swap(child1.effect[index], child1.effect[index2]);
        swap(child2.effect[index], child2.effect[index2]);
    }
    index = rand()%5;
    index2 = rand()%5;
    if(index>index2) swap(index, index2);
    while(index>index2){
        swap(child1.physical[index], child1.physical[index2]);
        swap(child2.physical[index], child2.physical[index2]);
    }
}

void mutateswap(chromosome &child1, chromosome &child2){
    ll index = rand()%13;
    ll index2 = rand()%13;
    swap(child1.effect[index], child1.effect[index2]);
    swap(child2.effect[index], child2.effect[index2]);
    ll newind = rand()%5;
    ll newind2 = rand()%5;
    swap(child1.physical[newind], child1.physical[newind2]);
    swap(child2.physical[newind], child2.physical[newind2]);
}

void crossoveronepoint(chromosome &first, chromosome &second, chromosome &child1, chromosome &child2){
    ll random = rand()%2;
    ll ind = rand()%13;
    ll ind2 = rand()%5;
    
    if(random){
        trav(ind){
            child1.effect[i] = first.effect[i];
            child2.effect[i] = second.effect[i];
        }
        for(ll i = ind;i<13;i++){
            child1.effect[i] = second.effect[i];
            child2.effect[i] = first.effect[i];
        }
        trav(ind2){
            child1.physical[i] = first.physical[i];
            child2.physical[i] = second.physical[i];
        }
        for(ll i = ind2;i<5;i++){
            child1.physical[i] = second.physical[i];
            child2.physical[i] = first.physical[i];
        }
    }
    else{
        trav(ind){
            child2.effect[i] = first.effect[i];
            child1.effect[i] = second.effect[i];
        }
        for(ll i = ind;i<13;i++){
            child2.effect[i] = second.effect[i];
            child1.effect[i] = first.effect[i];
        }
        trav(ind2){
            child2.physical[i] = first.physical[i];
            child1.physical[i] = second.physical[i];
        }
        for(ll i = ind2;i<5;i++){
            child2.physical[i] = second.physical[i];
            child1.physical[i] = first.physical[i];
        }
    }
    
    ll choosemutation = rand()%1000;
    if(choosemutation <= 5) mutateswap(child1, child2);
    else if(choosemutation <= 10) mutateinvert(child1, child2);
}

void crossoveruniform(chromosome &first, chromosome &second, chromosome &child1, chromosome &child2){
    trav(13){
        if(i%2){
            child1.effect[i] = first.effect[i];
            child2.effect[i] = second.effect[i];
        }
        else{
            child1.effect[i] = second.effect[i];
            child2.effect[i] = first.effect[i];
        }
    }
    trav(5){
        if(i%2){
            child1.physical[i] = first.physical[i];
            child2.physical[i] = second.physical[i];
        }
        else{
            child1.physical[i] = second.physical[i];
            child2.physical[i] = first.physical[i];
        }
    }
    
    ll choosemutation = rand()%1000;
    if(choosemutation <= 5) mutateswap(child1, child2);
    else if(choosemutation <= 10) mutateinvert(child1, child2);
}

bool comp(chromosome a, chromosome b){
    return abs(a.fitness - 73.5) + abs(a.delay - 48) < abs(b.fitness - 73.5) + abs(b.delay - 48);
}

void roulette(vector<chromosome> &v, vector<chromosome> &finalchild){
    trav2(CROSSOVERRATIO){
        ll s = 0;
        trav(v.size()){
            s += v[i].fitness;
        }
        ll sel = rand()%s, sel2 = rand()%s;
        ll i = 0,ind = -1, k = 0, ind2 = -1;
        while(true){
            k += v[i].fitness;
            if(k > sel){
                ind =i;
                break;
            }
            else i++;
        }
        k = 0;
        i = 0;
        while (true) {
            k += v[i].fitness;
            if(k > sel2){
                ind2 = i;
                break;
            }
            else i++;
        }
        if(ind == ind2){
            ind2 = ind + 1;
        }
        
        chromosome first = v[ind];
        chromosome second = v[ind2];
        
        chromosome child1,child2;
        child1.delay = 0;
        child1.fitness = 0;
        child2.delay = 0;
        child2.fitness = 0;
        
        ll choice =  rand()%2;
        if(choice) crossoveruniform(first, second, child1, child2);
        
        trav(13){
            if(child1.effect[i] == 1){
                child1.fitness += pedalref[i].priority*0.7;
                child1.delay += pedalref[i].time;
            }
            if(child2.effect[i] == 1){
                child2.fitness += pedalref[i].priority*0.7;
                child2.delay += pedalref[i].time;
            }
            if(child1.physical[i] == 1){
                child1.fitness += playref[i].influence*0.3;
            }
            if(child2.physical[i] == 1){
                child2.fitness += playref[i].influence*0.3;
            }
        }
        if(child1.delay <= MAXDELAY) finalchild.push_back(child1);
        if(child2.delay <= MAXDELAY) finalchild.push_back(child2);
        
    }
}

void tournament(vector<chromosome> &v, ll k, vector<chromosome> &finalchild){
    vector<chromosome> selection;
    
    trav(CROSSOVERRATIO){
        while(selection.size() < k){
            ll ind = rand()%500;
            selection.push_back(v[ind]);
        }
        sort(selection.begin(), selection.end(), comp);
        chromosome first = selection[0];
        chromosome second = selection[1];
        
        chromosome child1, child2;
        child1.delay = 0;
        child1.fitness = 0;
        child2.delay = 0;
        child2.fitness = 0;
        
        ll choice = rand()%2;
        if(choice)
            crossoveronepoint(first, second, child1, child2);
        else crossoveruniform(first, second, child1, child2);
        
        trav(13){
            if(child1.effect[i] == 1){
                child1.fitness += pedalref[i].priority*0.7;
                child1.delay += pedalref[i].time;
            }
            if(child2.effect[i] == 1){
                child2.fitness += pedalref[i].priority*0.7;
                child2.delay += pedalref[i].time;
            }
            if(child1.physical[i] == 1){
                child1.fitness += playref[i].influence*0.3;
            }
            if(child2.physical[i] == 1){
                child2.fitness += playref[i].influence*0.3;
            }
        }
        if(child1.delay <= MAXDELAY) finalchild.push_back(child1);
        if(child2.delay <= MAXDELAY) finalchild.push_back(child2);
    }
}

void populate(vector<chromosome> &v){
    trav(500){
        ll delay = 0;
        while(true){
            ll ind = rand()%13;
            if(v[i].effect[ind] == 0 && delay <= MAXDELAY){
                v[i].effect[ind] = 1;
                v[i].delay += pedalref[ind].time;
                v[i].fitness += 0.7*pedalref[ind].priority;
                delay += pedalref[ind].time;
            }
            if(delay > MAXDELAY){
                v[i].fitness -= 0.7*pedalref[ind].priority;
                v[i].delay -= pedalref[ind].time;
                v[i].effect[ind] = 0;
                break;
            }
        }
        ll phys =0;
        while(phys < MAXPHYS){
            ll ind2 = rand()%5;
            if(v[i].physical[ind2] == 0){
                v[i].physical[ind2] = 1;
                phys++;
                v[i].fitness += 0.3*playref[ind2].influence;
            }
        }
    }
}

void keepbest(vector<chromosome> &finalchild, vector<chromosome> &v, ll k){
    sort(finalchild.begin(), finalchild.end(), comp);
    sort(v.begin(),v.end(),comp);
    ll ind = v.size()- 1;
    trav(k){
        v[ind] = finalchild[i];
        ind--;
    }
    sort(v.begin(),v.end(),comp);
}

void displayres(vector<chromosome> &v, ll num){
    cout<<"THE CHROMOSOMES FOR GENERATION - "<<num<<" ARE :"<<endl;
    
    trav(500){
        trav2(v[i].effect.size()) cout<<v[i].effect[j]<<" ";
        cout<<" -- ";
        trav2(v[i].physical.size()) cout<<v[i].physical[j]<<" ";
        cout<<" -- ";
        cout<<v[i].delay<<" -- "<<v[i].fitness<<endl;
    }
}
bool checkterminate(vector<chromosome> &v, chromosome sample, chromosome &ans){
    double fitref = sample.fitness;
    double timeref = sample.delay;
    
    bool flag1 = false, flag2 = false;
    ll fitnesscalc = 0;
    
    trav(v.size()){
        if(abs(v[i].fitness - fitref) <= 0.1 && abs(v[i].delay - timeref) == 0){
            ans = v[i];
            flag1 = true;
            fitnesscalc = v[i].fitness;
        }
    }
    unordered_map<ll, ll> mp;
    trav(v.size()){
        mp[v[i].fitness]++;
    }
    unordered_map<ll,ll> :: iterator it;
    for(it = mp.begin();it!=mp.end();it++){
        if(it->second >= 300) {
            flag2 = true;
        }
    }
    
    return flag1 && flag2;
}

void initsample(chromosome &sample){
    sample.effect[0] = 1;
    sample.effect[1] = 0;
    sample.effect[2] = 1;
    sample.effect[3] = 1;
    sample.effect[4] = 1;
    sample.effect[5] = 0;
    sample.effect[6] = 0;
    sample.effect[7] = 0;
    sample.effect[8] = 1;
    sample.effect[9] = 0;
    sample.effect[10] = 1;
    sample.effect[11] = 1;
    sample.effect[12] = 1;
    
    sample.physical[0] = 1;
    sample.physical[1] = 0;
    sample.physical[2] = 1;
    sample.physical[3] = 1;
    sample.physical[4] = 0;
}

int main(){
    FIO;
    tc{
        ofstream output("/Users/Rajiv/Desktop/outputga.csv");
        
        chromosome sample,ans;
        initsample(sample);
        
        trav(13){
            if(sample.effect[i] == 1){
                sample.fitness += 0.7*pedalref[i].priority;
                sample.delay += pedalref[i].time;
            }
        }
        
        trav(5){
            if(sample.physical[i] == 1){
                sample.fitness += 0.3*playref[i].influence;
            }
        }
        
        sample.fitness = 73.5;
        sample.delay = 48;
        
        cout<<"SAMPLE FITNESS : "<<sample.fitness<<endl;
        cout<<"SAMPLE DELAY : "<<sample.delay<<endl;
        
        vector<chromosome> v(500), finalchild(500);
        initialise();
        populate(v);
        
        ll i =0;
        while(true){
            roulette(v, finalchild);
            //tournament(v, 50, finalchild);
            
            keepbest(finalchild, v, 25);
            displayres(v,i+1);
            i++;
            trav2(500){
                output<<i<<","<<v[j].fitness<<endl;
            }
            if(checkterminate(v, sample, ans)) break;
        }
        
        output.close();
        
        cout<<endl<<"THE OPTIMAL CHROMOSOME IS : "<<endl;
        cout<<ans.fitness<<" "<<ans.delay<<endl;
    }
    return 0;
}


