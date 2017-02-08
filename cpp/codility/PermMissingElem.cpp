#include <algorithm>
int solution(vector<int> &A) 
{
    using namespace std;
    
    long long N = A.size();
    long long sum = (N+2)*(N+1)/2;
    
    for ( size_t i = 0; i < A.size(); ++i )
    {
        sum -= A[i];
    }
    
    return int(sum);
}
