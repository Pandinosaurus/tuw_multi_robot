/*
 * Copyright (c) 2017, <copyright holder> <email>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef POINT_EXPANDER_H
#define POINT_EXPANDER_H

#define POT_HIGH 1.0e10
#include <algorithm>
#include <memory>
#include <queue>
#include <tuw_global_planner/utils.h>
#include <grid_map_ros/grid_map_ros.hpp>

class PointExpander
{
private:        template <class T, class S, class C>
                void clearpq(std::priority_queue<T, S, C>& q){
                    q=std::priority_queue<T, S, C>();
                }
private:        class Index 
                {
                public:
                    
                    Index(int index, float c, float d, float p) 
                    {
                        i = index;
                        weight = c;
                        dist = d;
                        potential = p;
                    }
                    
                    Index(int x_val, int y_val, int nx, float c, float d, float p)
                    {
                        i = x_val + y_val*nx;
                        weight = c;
                        dist = d;
                        potential = p;
                    }
                    
                    Index offsetDist(int dist_x, int dist_y, int nx, int ny)
                    {
                        int x_val = (i % nx) + dist_x;
                        int y_val = (i / nx) + dist_y;
                        if(x_val < 0 || x_val > nx || y_val < 0 || y_val > ny)
                            return Index(-1,-1,-1,-1);
                        return Index(x_val,y_val,nx,0,0,0);
                    }
                    
                    int getX(int nx)
                    {
                        return (i % nx);
                    }
                    
                    int getY(int nx)
                    {
                        return (i / nx);
                    }
                    
                    int i;
                    float weight;
                    float dist;
                    float cost;
                    float potential;
                };
    
                struct greater1 {
                    bool operator()(const Index& a, const Index& b) const {
                        return a.weight > b.weight;
                    }
                };
                std::priority_queue<Index, std::vector<Index>, greater1> queue_;
                int nx_, ny_, ns_;

                
public:         void initialize(const grid_map::GridMap &_map);
public:         bool findVoronoiEndPoint(Point _start, int _cycles, float* _potential, Point &_p);

private:        void getMaps(float *_distance_field, int8_t *_voronoi_graph, int8_t *_global_map, const grid_map::GridMap &_voronoi_map);
private:        Index findVoronoiEndPoint(Index _start, int _cycles, float* _potential);
private:        void addPotentialExpansionCandidate(Index _current, int _next_x, int _next_y, float* _potential);

private:        std::unique_ptr<float[]> distance_field_;
private:        std::unique_ptr<int8_t[]> voronoi_graph_;
private:        std::unique_ptr<int8_t[]> global_map_;
private:        float neutral_cost_ = 1;

};

#endif // VORONOI_EXPANDER_H