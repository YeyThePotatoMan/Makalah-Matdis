#include <vector>
#include <algorithm>
#include "overtaking.h"
using namespace std;

typedef long long ll;

struct Bus {
    ll arrivalTime;
    ll pace;
    int id;
};

int N, L, reservePace, M;
vector<int> stations;
vector<Bus> buses;

bool cmp(Bus &a, Bus &b) {
    if (a.arrivalTime == b.arrivalTime) return a.pace < b.pace;
    return a.arrivalTime < b.arrivalTime;
}

ll dist(int i) { return stations[i] - stations[i-1]; }

vector<vector<pair<ll, ll>>> arrivalRanges;
vector<vector<ll>> arrivalStarts;
vector<vector<ll>> prefixMaxArrival;

void init(int Lp, int Np, vector<ll> Tp, vector<int> Wp, int Xp, int Mp, vector<int> Sp) {
    // Load Data
    L = Lp; reservePace = Xp; M = Mp;
    stations = Sp;
    N = Np;

    // Load regular bus
    for (int i = 0; i < N; i++) {
        if (Wp[i] >= reservePace)
            buses.push_back({Tp[i], Wp[i], i});
    }
    N = (int)buses.size();

    // Initialize vectors before accessing
    arrivalRanges.assign(M, vector<pair<ll,ll>>());
    arrivalStarts.assign(M, vector<ll>());
    prefixMaxArrival.assign(M, vector<ll>());

    // Simulate the process and store the arrival times in each station
    ll prevPos = 0;
    for (int i = 1; i < M; i++) {
        sort(buses.begin(), buses.end(), byArrivalThenPace);

        ll maxArrival = 0;
        arrivalRanges[i].clear();
        arrivalStarts[i].clear();

        for (int j = 0; j < N; j++) {
            ll start = buses[j].arrivalTime;
            ll expected = start + dist(i) * buses[j].pace;

            if (expected <= maxArrival) {
                arrivalRanges[i].emplace_back(start, maxArrival);
                buses[j].arrivalTime = maxArrival;
            } else {
                arrivalRanges[i].emplace_back(start, expected);
                buses[j].arrivalTime = expected;
                maxArrival = expected;
            }
            arrivalStarts[i].push_back(start);
        }
        prevPos = stations[i];
    }


    for (int i = 0; i < M; i++) {
        sort(arrivalRanges[i].begin(), arrivalRanges[i].end());
        sort(arrivalStarts[i].begin(), arrivalStarts[i].end());

        prefixMaxArrival[i].resize(arrivalRanges[i].size() + 1);
        prefixMaxArrival[i][0] = 0;
        for (int j = 1; j <= (int)arrivalRanges[i].size(); j++)
            prefixMaxArrival[i][j] = max(prefixMaxArrival[i][j-1], arrivalRanges[i][j-1].second);
    }
}

ll arrival_time(ll Y) {
    ll prevPos = 0, arrival = Y;

    for (int i = 1; i < M; i++) {
        auto &v =arrivalStarts[i];
        int idx = (upper_bound(v.begin(), v.end(), arrival - 1) - v.begin());
        ll d = stations[i] - prevPos;
        if (arrival + d * reservePace < prefixMaxArrival[i][idx])
            arrival = prefixMaxArrival[i][idx];
        else
            arrival += d * reservePace;
        prevPos = stations[i];
    }
    return arrival;
}
