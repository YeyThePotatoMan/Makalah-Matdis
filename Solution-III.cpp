#include "overtaking.h"
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pii pair<ll,ll>
#define mp make_pair

struct Bus {
    ll pace, arrival;
    Bus(ll p, ll a) : pace(p), arrival(a) {}
};

bool byArrivalThenPace(Bus &a, Bus &b) {
    if (a.arrival == b.arrival) return a.pace < b.pace;
    return a.arrival < b.arrival;
}

int N, M;
ll X;
vector<ll> stations;
vector<Bus> buses;
vector<vector<pii>> arrivalTimes(1005); // Saves pair of {arrivaltime to i'th station, arrival time to i+1'th station}
ll memo[1005][1005]; // Saves the value of each DP

// Implementation of a "long-jump"
int find_pos(int i, ll time) {
    int cur = lower_bound(arrivalTimes[i].begin(), arrivalTimes[i].end(), mp(time, -1LL)) - arrivalTimes[i].begin();
    int p = i, left = i, right = M - 1;
    // Additional binary search to count the number of bus ahead
    while (left <= right) {
        int mid = (left + right) / 2;
        ll expected = time + (stations[mid] - stations[i]) * X;
        int ahead = lower_bound(arrivalTimes[mid].begin(), arrivalTimes[mid].end(), mp(expected, -1LL)) - arrivalTimes[mid].begin();
        if (ahead == cur) {
            p = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return p;
}

// Implementation of DP function
// We apply dynamic programming to reduce redundant computation
ll DP(int i, int j) {
    if (memo[i][j] != -1) return memo[i][j]; // DP(vertex(i,j)) is already computed 
    ll time = arrivalTimes[i][j].first;
    int p = find_pos(i, time);
    if (p == M - 1) return memo[i][j] = time + (stations[M - 1] - stations[i]) * X; // Case 1 : No interruption
    ll ET = time + (stations[p] - stations[i]) * X;
    auto it = lower_bound(arrivalTimes[p].begin(), arrivalTimes[p].end(), mp(ET, 0LL));
    it--;
    int nxt = lower_bound(arrivalTimes[p + 1].begin(), arrivalTimes[p + 1].end(), mp(it->second, 0LL)) - arrivalTimes[p + 1].begin();
    return memo[i][j] = DP(p + 1, nxt); // Case 2 : DP(X) <- DP(Y) where adj(X) = Y
}

void init(int Lp, int Np, vector<ll> Tp, vector<int> Wp, int Xp, int Mp, vector<int> Sp) {
    // Load variables
    X = Xp; M = Mp;
    stations = vector<ll>(Sp.begin(), Sp.end());
    buses.clear();

    // Load regular bus
    for (int i = 0; i < Np; i++) {
        if (Wp[i] > X) buses.emplace_back(Wp[i], Tp[i]); // Filtering
    }
    N = (int)buses.size();

    for (int i = 0; i < M; i++) {
        arrivalTimes[i].clear();
    }

    // Compute and save the arrival time of each bus in every station
    for (int i = 0; i < M; i++) {
        ll mx = 0;
        sort(buses.begin(), buses.end(), byArrivalThenPace);
        for (int j = 0; j < N; j++) {
            ll start = buses[j].arrival;
            if (i != M - 1)
                mx = max(mx, buses[j].arrival + (stations[i + 1] - stations[i]) * buses[j].pace);
            arrivalTimes[i].emplace_back(start, mx);
            buses[j].arrival = mx;
        }
    }
    memset(memo, -1, sizeof(memo));
}

ll arrival_time(ll Y) {
    int p = find_pos(0, Y); // Check whether other bus will interrupt or not
    if (p == M - 1) return Y + stations[M - 1] * X; // No interruption
    
    // Long-Jump
    ll ET = Y + stations[p] * X;
    auto it = lower_bound(arrivalTimes[p].begin(), arrivalTimes[p].end(), mp(ET, 0LL));
    it--;
    int nxt = lower_bound(arrivalTimes[p + 1].begin(), arrivalTimes[p + 1].end(), mp(it->second, 0LL)) - arrivalTimes[p + 1].begin();
    
    return DP(p + 1, nxt);
}
