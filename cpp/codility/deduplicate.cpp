#include <iostream>
#include <string>

std::string & deduplicate(std::string & s)
{
    if (s.size() < 2) return s;

    size_t tail = 1;

    for (size_t i = 1; i < s.size(); ++i)
    {
        size_t j = 0;
        for (j = 0; j < tail; ++j)
        {
            if (s[i] == s[j]) break;
        }
        if (j == tail)
        {
            s[tail] = s[i];
            ++tail;
        }
    }
    s.resize(tail);

    return s;
}

int main()
{
    using namespace std;

    string s1 = "abca";
    clog << deduplicate( s1 ) << "\n";
}
