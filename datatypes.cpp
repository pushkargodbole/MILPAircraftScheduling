#include <iostream>
#include <string>

using namespace std;

struct arc_datatype
{
	public:
		int node1;
        int node2;
        float length;
        //vector<int> occ_times;
};

struct flight_datatype
{
    public:
        int start;
        int end;
        float start_time;
        float speed;
        float sep;
        float priority;
};

struct file_datatype
{
    public:
        vector<vector<arc_datatype> > map;
        vector<flight_datatype> flights;
};
