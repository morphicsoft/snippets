int solution(vector<int> &A) 
{
    vector< long long > B( A.size() );
    
    for ( size_t i = 0; i < A.size(); ++i )
    {
        if ( A[i] <= A.size() ) B[A[i]-1]++;
        else return 0;
    }
    
    for ( size_t i = 0; i < B.size(); ++i )
    {
        if ( B[i] != 1 ) return 0;
    }
    
    return 1;
}
