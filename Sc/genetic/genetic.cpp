#include <iostream>
#include <algorithm>
#include <map>
#include <cstdio>
#include <stack>
#include <cstdlib>
#include <set>
#include <queue>
#include <map>
#include <unordered_set>
#include<fstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <climits>
#include <cmath>
#include <cstring>
#include <utility>
#include <iterator>

using namespace std;
#define dbg(x) { cout << #x << ": " << x << endl; }
#define bp { int kk ; cin >> kk ;}
const int PC = 600; // Problem Count
const int NOC = 500; // No of Chromosomes
const int CC = 11; // Companies Count
const int DC = 16; // Data Structure Count
const int MT = 300; // Maximum Time in Hours
const int CR = 400; // Cross Ratio
const int TM = 50; // Tournament

vector<int> companyPriority(CC);
vector<int> dsPriority(DC);

struct problem
{
    int correctSubmissions;
    int totalSubmissions;
    int difficulty;
    vector<bool> dsUsed = vector<bool>(DC);
    vector<int> companyCount = vector<int>(CC);
} problems[PC];

struct chromosome
{
    vector<bool> select = vector<bool>(PC);
    int importance;
    double time;
};

void initialize()
{
    companyPriority = {10,10,8,8,8,7,8,7,8,7,7};
    dsPriority = {9,8,7,8,6,6,7,8,6,7,8,9,6,6,5,8};

    for(int i=0 ; i<PC ; i++) {
        problems[i].correctSubmissions = rand()%1000 + 1;
        problems[i].totalSubmissions = problems[i].correctSubmissions + rand()%1000;
        problems[i].difficulty = rand()%10 + 1;

        for(int j=0 ; j<DC ; j++) {
            problems[i].dsUsed[j] = (rand()%2 == 1);
        }

        for(int j=0 ; j<CC ; j++) {
            problems[i].companyCount[j] = rand()%5;
        }
    }
}

double companyImportance(problem &p)
{
    double ans = 0;
    for(int i=0 ; i<CC ; i++) {
        ans += companyPriority[i] * p.companyCount[i];
    }
    return ans;
}

double dsImportance(problem &p)
{
    double ans = 0;
    for(int i=0 ; i<DC ; i++) {
        ans += dsPriority[i] * p.dsUsed[i];
    }
    return ans;
}

int calculateImportance(problem &p)
{
    int importance = 0;
    importance += (int) (0.3 * companyImportance(p));
    importance += (int) (0.7 * dsImportance(p));
    return importance;
}

double calculateTime(problem &p)
{
    double accuracy = (double) p.correctSubmissions / (double)p.totalSubmissions;
    double time = (double) p.difficulty / accuracy;
    time /= 25.00;
    time = min(time, 7.00);
    time = max(time, 1.00);
    return time;
}

void makePopulation(vector<chromosome> &c)
{
    for(int i=0 ; i<NOC ; i++) {
        c[i].importance = 0;
        c[i].time = 0;
        double time = 0;
        while(1) {
            int random = rand()%PC;
            if(!c[i].select[random]) {
                double timeReq = calculateTime(problems[random]);
                if(time + timeReq <= MT) {
                    time += timeReq;
                    c[i].select[random] = true;
                    c[i].importance += calculateImportance(problems[random]);
                }
                else break;
            }
        }
        c[i].time = time;
    }
}

bool comparator(chromosome &a, chromosome &b)
{
    return a.importance > b.importance;
}

pair<chromosome, chromosome> selectTournament(vector<chromosome> &c)
{
    vector<chromosome> selected;
    for(int i=0 ; i<TM ; i++) {
        int random = rand()%NOC;
        selected.push_back(c[random]);
    }
    sort(selected.begin(), selected.end(), comparator);
    return { selected[0], selected[1] };
}

pair<chromosome, chromosome> uniformCrossover(pair<chromosome, chromosome> &parents)
{
    pair<chromosome, chromosome> children;
    for(int i=0 ; i<PC ; i++) {
        if(rand()%2) {
            children.first.select[i] = parents.second.select[i];
            children.second.select[i] = parents.first.select[i];
        }
        else {
            children.first.select[i] = parents.first.select[i];
            children.second.select[i] = parents.second.select[i];
        }
    }
    children.first.importance = 0;
    children.first.time = 0;
    children.second.importance = 0;
    children.second.time = 0;
    for(int i=0 ; i<PC ; i++) {
        if(children.first.select[i]) {
            double tempTime = calculateTime(problems[i]);
            if(children.first.time + tempTime <= MT) {
                children.first.time += tempTime;
                children.first.importance += calculateImportance(problems[i]);
            }
            else {
                children.first.importance = -1;
                break;
            }
        }
        if(children.second.select[i]) {
            double tempTime = calculateTime(problems[i]);
            if(children.second.time + tempTime <= MT) {
                children.second.time += tempTime;
                children.second.importance += calculateImportance(problems[i]);
            }
            else {
                children.second.importance = -1;
                break;
            }
        }
    }
    return children;
}

void mutate(pair<chromosome, chromosome> &children)
{
    int random = rand()%1000 + 1;
    if(random <= 10) {
        random = rand()%PC;
        if(children.first.select[random]) {
            children.first.select[random] = 0;
            children.first.importance -= calculateImportance(problems[random]);
            children.first.time -= calculateTime(problems[random]);
        }
        else {
            children.first.select[random] = 1;
            children.first.importance += calculateImportance(problems[random]);
            children.first.time += calculateTime(problems[random]);
            if(children.first.time > MT) children.first.importance = -1;
        }
        if(children.second.select[random]) {
            children.second.select[random] = 0;
            children.second.importance -= calculateImportance(problems[random]);
            children.second.time -= calculateTime(problems[random]);
        }
        else {
            children.second.select[random] = 1;
            children.second.importance += calculateImportance(problems[random]);
            children.second.time += calculateTime(problems[random]);
            if(children.second.time > MT) children.second.importance = -1;
        }
    }
}

vector<chromosome> scmWrapper(vector<chromosome> &c)
{
    vector<chromosome> children;
    for(int i=0 ; i<CR/2 ; i++) {
        pair<chromosome, chromosome> parents = selectTournament(c);
        pair<chromosome, chromosome> child = uniformCrossover(parents);
        mutate(child);
        if(child.first.importance != -1) {
            children.push_back(child.first);
        }
        if(child.second.importance != -1) {
            children.push_back(child.second);
        }
    }
    return children;
}

void printChromosome(chromosome &c)
{
    for(auto it: c.select) {
        cout << it << " ";
    }
    cout << "\n" ;
    cout << "Importance " << c.importance << endl;
    cout << "Time Taken " << c.time << endl;
}

void survivorSelection(vector<chromosome> &c, vector<chromosome> &children)
{
    for(int i=NOC-children.size(), k=0 ; i<NOC ; i++, k++) {
        c[i] = children[k];
    }
}

bool terminateCondition(vector<chromosome> &c)
{
    unordered_map<int,int> m;
    for(auto it: c) m[it.importance]++;
    int Max = 0;
    for(auto it: m) Max = max(Max, it.second);
    return Max >= 450;
}

void printSoln(vector<chromosome> &c)
{
    chromosome ans = c[0];
    for(auto it: c) {
        if(it.importance > ans.importance) ans = it;
    }
    cout<<"Solve following questions: "<<endl;
    for(int i=0 ; i<PC ; i++) if(ans.select[i]) cout<<i<<" ";
    cout<<endl;
}

int averageFitness(vector<chromosome> &c)
{
    long long int sum = 0;
    for(int i=0 ; i<NOC ; i++) {
        sum += c[i].importance;
    }
    return sum/NOC;
}

int main()
{
    ofstream output("outputga.csv");
    vector<chromosome> c(NOC);
    initialize();
    makePopulation(c);
    int gen = 1;
    cout << "Generation 0 " << averageFitness(c) << endl;
    while(1) {
        sort(c.begin(), c.end(), comparator);
        vector<chromosome> children = scmWrapper(c);
        survivorSelection(c,children);
        cout << "Generation " << gen <<" " << averageFitness(c) << endl;
        output << gen << "," << averageFitness(c) << endl;
        gen++;
        if(terminateCondition(c)) break;
    }
    output.close();
    cout<<"Solution is ";
    printSoln(c);
    return 0;
}
