// source : https://oj.uz/submission/841469
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const ll END = numeric_limits<ll>::max();

map<ll, ll> intervals;

void add_interval(ll start, ll end)
{
    auto it = intervals.lower_bound(end);
    if (it == intervals.begin())
    {
        intervals[start] = end;
        if (!intervals.count(end))
        {
            intervals[end] = END;
        }
        return;
    }
    --it;
    if (it->second != END && it->first <= start)
    {
        return;
    }
    ll dest = it->second == END ? end : it->second;
    if (it->second == END && !intervals.count(end))
    {
        intervals[end] = END;
    }
    intervals[start] = dest;
    while (it->first > start)
    {
        intervals.erase(it--);
    }
}

ll total_time;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    total_time = L * (ll)X;
    vector<pair<ll, int>> initial;
    for (int i = 0; i < N; i++)
    {
        if (W[i] > X)
        {
            initial.emplace_back(T[i], W[i] - X);
        }
    }
    N = initial.size();
    sort(initial.begin(), initial.end());
    vector<vector<ll>> bus_time(M, vector<ll>(N));
    vector<int> bus_rv(N);
    for (int j = 0; j < N; j++)
    {
        bus_time[0][j] = initial[j].first;
        bus_rv[j] = initial[j].second;
    }
    for (int i = 1; i < M; i++)
    {
        ll dist = S[i] - S[i - 1];
        ll m = 0;
        for (int j = 0; j < N; j++)
        {
            int rv = bus_rv[j];
            ll t = bus_time[i - 1][j] + rv * dist;
            if (t <= m)
            {
                bus_time[i][j] = m;
                int k = j;
                while (k >= 1 && bus_time[i][k - 1] == m && bus_rv[k - 1] > rv)
                {
                    bus_rv[k] = bus_rv[k - 1];
                    k--;
                }
                bus_rv[k] = rv;
            }
            else
            {
                bus_time[i][j] = t;
                m = t;
            }
        }
    }
    for (int i = M - 1; i >= 1; i--)
    {
        for (int j = 0; j < N; j++)
        {
            add_interval(bus_time[i - 1][j], bus_time[i][j]);
        }
    }
}

long long arrival_time(long long Y)
{
    auto it = intervals.lower_bound(Y);
    if (it == intervals.begin())
        return Y + total_time;
    --it;
    if (it->second == END)
        return Y + total_time;
    return it->second + total_time;
}

