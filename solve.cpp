#include <iostream>
#include <string.h>
#include <vector>
#include <ilcplex/ilocplex.h>
#include <math.h>

ILOSTLBEGIN

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<IloRangeArray> RangeMatrix;

static void
populate (vector<flight_datatype> flights, vector<vector<arc_datatype> > paths, IloModel model, NumVarMatrix t)
{
    IloEnv env = model.getEnv();
	IloObjective obj = IloMinimize(env);
	RangeMatrix mintime_con(env, flights.size());
    for(int i=0;i<flights.size();i++)
    {
		t[i] = IloNumVarArray(env, paths[i].size()+1, flights[i].start_time, IloInfinity, ILOFLOAT);
        mintime_con[i] = IloRangeArray(env);
        for(int j=0;j<paths[i].size();j++)
		{
            mintime_con[i].add(t[i][j+1] - t[i][j] >= ceil(paths[i][j].length/flights[i].speed));
        }
        model.add(mintime_con[i]);
    }
    IloNumVarArray x(env);
    int xi = 0;
    IloRangeArray overlap_con(env);
    double M = 100000.0;
    for(int i=0;i<flights.size();i++)
    {
        for(int j=i+1;j<flights.size();j++)
        {
            for(int k=0;k<paths[i].size();k++)
            {
                for(int l=0;l<paths[j].size();l++)
                {
                    if(paths[i][k].node1 == paths[j][l].node1)
                    {
                        x.add(IloNumVar(env, 0, 1, ILOINT));
                        if(paths[j][l].length < flights[j].sep)
                        {
                            overlap_con.add(M*x[xi] + t[i][k] - t[j][l+1] >= 1e-10);
                        }
                        else
                        {
                            overlap_con.add(M*x[xi] + t[i][k] - t[j][l] - ceil(flights[j].sep/flights[j].speed) >= 1e-10);
                        }
                        if(paths[i][k].length < flights[i].sep)
                        {
                            overlap_con.add(M*(1-x[xi]) + t[j][l] - t[i][k+1] >= 1e-10);
                        }
                        else
                        {
                            overlap_con.add(M*(1-x[xi]) + t[j][l] - t[i][k] - ceil(flights[i].sep/flights[i].speed) >= 1e-10);
                        }
                        if(paths[i][k].node2 != paths[j][l].node2) xi++;
                    }
                    if(paths[i][k].node2 == paths[j][l].node2)
                    {
                        if(paths[i][k].node1 != paths[j][l].node1)
                        {
                            x.add(IloNumVar(env, 0, 1, ILOINT));
                        }
                        if(paths[i][k].length < flights[j].sep)
                        {
                            overlap_con.add(M*x[xi] + t[i][k] - t[j][l+1] >= 1e-10);
                        }
                        else
                        {
                            overlap_con.add(M*x[xi] + t[i][k+1] - t[j][l+1] - ceil(flights[j].sep/flights[i].speed) >= 1e-10);
                        }
                        if(paths[j][l].length < flights[i].sep)
                        {
                            overlap_con.add(M*(1-x[xi]) + t[j][l] - t[i][k+1] >= 1e-10);
                        }
                        else
                        {
                            overlap_con.add(M*(1-x[xi]) + t[j][l+1] - t[i][k+1] - ceil(flights[i].sep/flights[j].speed) >= 1e-10);
                        }
                        xi++;
                    }
                    if(paths[i][k].node1 == paths[j][l].node2 && paths[i][k].node2 == paths[j][l].node1)
                    {
                        x.add(IloNumVar(env, 0, 1, ILOINT));
                        overlap_con.add(M*x[xi] + t[i][k] - t[j][l+1] >= 1e-10);
                        overlap_con.add(M*(1-x[xi]) + t[j][l] - t[i][k+1] >= 1e-10);
                        xi++;
                    }
                }
            }
        }
    }
    for(int i=0;i<flights.size();i++)
    {
        obj.setLinearCoef(t[i][paths[i].size()], flights[i].priority);
    }
    model.add(overlap_con);
    model.add(obj);
}
