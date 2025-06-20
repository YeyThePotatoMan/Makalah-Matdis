#include "overtaking.h"
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Bus {
    ll arrivalTime; // updated as the bus progresses
    ll pace;
    int id;
};

bool byArrivalThenPace(const Bus &a, const Bus &b) {
    if (a.arrivalTime == b.arrivalTime)
        return a.pace < b.pace;
    return a.arrivalTime < b.arrivalTime;
}

vector<Bus> buses;        // current simulation state
vector<Bus> buses_init;   // initial state 
vector<int> stations;
int N, M;

ll computeExpectedArrival(const Bus& b, int j) {
    ll dist = stations[j] - stations[j - 1];
    return b.arrivalTime + dist * b.pace;
}

void init(int L, int Np, vector<ll> T, vector<int> W, int X, int Mp, vector<int> S) {
    buses.clear();
    stations = S;

    // Load regular bus data
    for (int i = 0; i < Np; i++) {
        buses.push_back({T[i], W[i], i});
    }

    // Add reserve bus
    buses.push_back({0, X, Np});

    N = (int)buses.size();
    M = Mp;

    // Save initial state for resetting on each query
    buses_init = buses;
}

ll arrival_time(ll Y) {
    // Reset buses to initial state for this query
    buses = buses_init;

    // Set reserve bus departure time
    for (auto &b : buses) {
        if (b.id == N - 1) {
            b.arrivalTime = Y;
            break;
        }
    }

    // Simulate each station
    for (int j = 1; j < M; j++) {
        sort(buses.begin(), buses.end(), byArrivalThenPace);

        ll curMax = 0;
        for (int i = 0; i < N; i++) {
            ll expected = computeExpectedArrival(buses[i], j);
            buses[i].arrivalTime = max(expected, curMax);
            curMax = max(curMax, buses[i].arrivalTime);
        }
    }

    // Find and return reserve bus's final arrival time
    ll ret = 0;
    for (const auto &b : buses) {
        if (b.id == N - 1)
            ret = b.arrivalTime;
    }

    return ret;
}
