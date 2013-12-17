#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include "mystringfuncs.cpp"
#include "datatypes.cpp"
#include "getdata.cpp"
#include "gen_paths.cpp"
#include "solve.cpp"
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        time_t t0 = time(NULL);
        file_datatype file = getdata(argv[1], argv[2]);
        //Finding required pairs of end nodes
        
        vector<vector<int> > endnodes;
        vector<int> flightpath_index;
        for(int i=0;i<file.flights.size();i++)
        {
            vector<int> temp;
            temp.push_back(file.flights[i].start);
            temp.push_back(file.flights[i].end);
            int found = 0;
            for(int j=0;j<endnodes.size();j++)
            {
                if(temp[0] == endnodes[j][0] && temp[1] == endnodes[j][1])
                {
                    found = 1;
                    flightpath_index.push_back(j);
                    break;
                }
            }
            if(found==0)
            {
                endnodes.push_back(temp);
                flightpath_index.push_back(endnodes.size()-1);
            }
        }
        //Finding all paths between all these pairs
        
        vector<vector<vector<arc_datatype> > > allpaths;
        for(int i=0;i<endnodes.size();i++)
        {
            vector<vector<arc_datatype> > paths = gen_paths(endnodes[i][0], endnodes[i][1], file.map);
            allpaths.push_back(paths);
        }
        //for(int i=0;i<endnodes.size();i++)
        //{
            //for(int j=0;j<allpaths[i].size();j++)
            //{
                //for(int k=0;k<allpaths[i][j].size();k++)
                //{
                    //cout << "(" << allpaths[i][j][k].node1 << ", " << allpaths[i][j][k].node2 << ") ";
                //}
                //cout << endl;
            //}
            //cout << endl;
        //}
        //Generating all possible combinations of path allocation
        vector<int> paths_count;
        for(int i=0;i<file.flights.size();i++)
        {
            paths_count.push_back(allpaths[flightpath_index[i]].size());
            cout << i<< " : " << allpaths[flightpath_index[i]].size() << endl;
        }
        vector<vector<int> > combs = allcombs(paths_count);
        cout << "Combs : " << combs.size() << endl;
        //for(int i=0;i<combs.size();i++)
        //{
            //for(int j=0;j<combs[i].size();j++)
            //{
                //cout << combs[i][j] << " ";
            //}
            //cout << endl;
        //}
        float cost = 1000000.0;
        for(int i=0;i<combs.size();i++)
        {
            if((i+1)%(combs.size()/100+1) == 0) cout << ((i+1)*100)/combs.size() << "% (" << (time(NULL) - t0)/60 << " mins elapsed)" << endl;
            vector<vector<arc_datatype> > chosenpaths;
            for(int j=0;j<file.flights.size();j++)
            {
                chosenpaths.push_back(allpaths[flightpath_index[j]][combs[i][j]]);
            }
            //for(int j=0;j<chosenpaths.size();j++)
            //{
                //for(int k=0;k<chosenpaths[j].size();k++)
                //{
                    //cout << "(" << chosenpaths[j][k].node1 << ", " << chosenpaths[j][k].node2 << ") ";
                //}
                //cout << endl;
            //}
            IloEnv env;
            IloModel model(env);
            NumVarMatrix nodetime(env, file.flights.size());
            populate(file.flights, chosenpaths, model, nodetime);
            IloCplex cplex(model);
            cplex.setOut(env.getNullStream());
            bool solve_status = cplex.solve();
            if(solve_status==1)
            {
                float newval = cplex.getObjValue();
                if(newval <= cost)
                {
                    ofstream solfile;
                    solfile.open(argv[3]);
                    cost = newval;
                    solfile << "Cost : " << newval << endl; 
                    cout << "Cost : " << newval << endl; 
                    vector<vector<float> > alltimes;
                    for(int j=0;j<file.flights.size();j++)
                    {
                        vector<float> times;
                        solfile << j << " : ";
                        cout << j << " : ";
                        for(int k=0;k<chosenpaths[j].size();k++)
                        {
                            times.push_back(cplex.getValue(nodetime[j][k]));
                            float t = cplex.getValue(nodetime[j][k]);
                            if(t < 1e-10) t = 0.0;
                            solfile << chosenpaths[j][k].node1+1 << " " << t << " - ";
                            cout << chosenpaths[j][k].node1+1 << " " << t << " - ";
                        }
                        float tf = cplex.getValue(nodetime[j][chosenpaths[j].size()]);
                        if(tf < 1e-10) tf = 0.0;
                        solfile << chosenpaths[j][chosenpaths[j].size()-1].node2+1 << " " << tf << endl;
                        cout << chosenpaths[j][chosenpaths[j].size()-1].node2+1 << " " << tf << endl;
                        alltimes.push_back(times);
                    }
                    solfile.close();
                    cout << "------------------------------------------" << endl;
                }
            }
            env.end();
        }
        return 0;
    }
    else
    {
        cout << "3 arguments expected, (map file, flights file, output file)" << argc-1 << " given." << endl;
        return 1;
    }
}

/*
int main()
{
    int a[] = {2,3,3};
    vector<int> b (a, a + sizeof(a) / sizeof(int) );
    vector<vector<int> > c = allcombs(b);
    for(int i=0;i<c.size();i++)
    {
        for(int j=0;j<c[i].size();j++)
        {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
*/
