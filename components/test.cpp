#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using pl = pair<ll,ll>;
using pii = pair<int,int>;
using tpl = tuple<int,int,int>;

#define all(a) a.begin(), a.end()
#define filter(a) a.erase(unique(all(a)), a.end())

#include <string>
#include <cmath> // For std::round

std::function<std::string(float)> floatToPercentage = [] (float value) {
    int percentage = static_cast<int>(std::round(value * 100.0f));
    return std::to_string(percentage) + "%";
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cout << floatToPercentage(0.8999293);

    return 0;
}