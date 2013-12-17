#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

vector<vector<arc_datatype> > gen_paths(int start, int end, vector<vector<arc_datatype> > map)
{
	vector<vector<arc_datatype> > allpaths;
	vector<vector<arc_datatype> > splitpaths;
	vector<int> splitnodes;
	vector<arc_datatype> open;
	vector<arc_datatype> path;
	int curr_node = start;
    for(int i=0;i<map[curr_node].size();i++) open.push_back(map[curr_node][i]);
	do
	{
        if(map[curr_node].size()>1)
        {
            int newsplitnode = 1;
            for(int i=0;i<splitnodes.size();i++)
            {
                if(splitnodes[i] == curr_node)
                {
                    splitpaths[i] = path;
                    newsplitnode = 0;
                    break;
                }
            }
            if(newsplitnode == 1)
            {
                splitnodes.push_back(curr_node);
                splitpaths.push_back(path);
            }
        }
        int jump;
        do
        {
            //for(int i=0;i<open.size();i++)
            //{
                //cout << "(" << open[i].node1 << ", " << open[i].node2 << ") ";
            //}
            //cout << endl;
            jump = 0;
            for(int i=0;i<path.size();i++)
            {
                if(open[open.size()-1].node2 == path[i].node1 && open[open.size()-1].node1 == path[path.size()-1].node2)
                {
                    open.pop_back();
                    i = -1;
                    jump = 1;
                }
            }
            if(path.size()!=0)
            {
                if(map[path[path.size()-1].node2].size()==0)
                {
                    jump = 1;
                }
            }
            if(open.size()!=0)
            {
                if(jump == 1)
                {
                    for(int i=0;i<splitnodes.size();i++)
                    {
                        if(open[open.size()-1].node1 == splitnodes[i])
                        {
                            path = splitpaths[i];
                        }
                    }
                }
            }
        }
        while(jump==1);
        int change = 0;
        if(open.size()!=0)
        {
            curr_node = open[open.size()-1].node2;
            path.push_back(open[open.size()-1]);
            open.pop_back();
            for(int i=0;i<map[curr_node].size();i++) open.push_back(map[curr_node][i]);
            change = 1;
        }
		if(path.size()!=0)
		{
			if(path[path.size()-1].node2 == end && change == 1)
			{
				allpaths.push_back(path);
                //for(int i=0;i<path.size();i++)
                //{
                    //cout << "* (" << path[i].node1 << ", " << path[i].node2 << ") ";
                //}
                //cout << endl;
			}
		}
	}while(open.size()!=0);
	return allpaths;
}

vector<vector<int> > allcombs(vector<int> elems)
{
    vector<vector<int> > newcombs;
    if(elems.size()==1)
    {
        for(int i=0;i<elems[0];i++)
        {
            vector<int> comb;
            comb.push_back(i);
            newcombs.push_back(comb);
        }
        return newcombs;
    }
    else
    {
        vector<int> elems1 (elems.begin(), elems.end()-1);
        vector<vector<int> > combs = allcombs(elems1);
        vector<vector<int> > newcombs;
        for(int i=0;i<combs.size();i++)
        {
            for(int j=0;j<elems[elems.size()-1];j++)
            {
                vector<int> comb;
                comb = combs[i];
                comb.push_back(j);
                newcombs.push_back(comb);
            }
        }
        return newcombs;
    }
}
