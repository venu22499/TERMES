////// THe implementation of the termes software.
//////

#include<bits/stdc++.h>
using namespace std;



multimap<int,int> mapp;

vector<pair<int, int> > final_pairs;
typedef  pair<int, int> iPair; 
struct agent
{
    int free;
    int source_loc;
    int destination;
    int current_location;
    vector<int> current_path;
    agent()
    {
        free = 0;
    }
};

struct tree_node
{
    int pos;    
    vector<int> list_vals;
    vector<struct tree_node *> children;
    struct tree_node* parent;
    tree_node(int val)
    {
        this->pos = val;
        list_vals.push_back(0);
        this->parent = NULL;
    }
};
struct super_node;  
struct sub_node
{
    int val;
    vector<pair<sub_node *, super_node *> >children;    
    sub_node(int val)
    {
        this->val = val;
    }    
    friend bool operator< (const sub_node &c1, const sub_node &c2)
    {
        return c1.val < c2.val;
    }     
};
struct super_node
{
    int pos;    
    vector<sub_node> sub_nodes;
    vector<super_node*> children;
    super_node* parent;
    super_node(int pos)
    {
        this->pos = pos;
    }
};
super_node* root_of_tree;


map< pair<int,int> , vector< pair < super_node*, sub_node > > > paths_;
vector<pair<int, int> > order;
vector<pair<int, int> > check;
int no_of_agents;
int max_no_of_squares;
int edges_common = 0;
int raw_time = 0;
int tot_time = 0;

struct tree
{
    tree_node* root;
    int M;
public:
    tree(int root_val)
    {
        this->root= new tree_node(root_val);
        M = 0;
    }
    void addedge(int parent_value, int child_value);
    tree_node* bfs_search(tree_node* x, int val);
    void bfs_way_add_nodes(vector<pair<int, int> >v);
    void bfs_traversal();
    void construct_list(vector<vector<float> > workspace, int workspace_length, int workspace_breadth);
    void construct(tree_node* node, vector<vector<float> > workspace, int workspace_length, int workspace_breadth);
    void generate_plan();
    super_node* construct_positive_tree(int k);
    super_node* construct_negative_tree(int k);
    super_node* dfs_type_build_positive(tree_node* temp, super_node* parent, int k);
    super_node* dfs_type_build_negative(tree_node* temp, super_node* parent, int k);
    void dfs_type_traversal(super_node* temp);
    void dfs_traversal_for_positive_tree(super_node* temp ,int &no);
    void dfs_traversal_for_negative_tree(super_node* temp ,int &no);
    //void find_path(super_node* temp, int elem, stack<int> &q, stack<int> &h, int l);
    void find_path(super_node* temp, sub_node s, int elem, int count, stack<pair<super_node*, sub_node> >&q, stack<pair<super_node*, sub_node> > &h, int l);
    void agent_plan();
    int find_if_okay(vector< pair< super_node*, sub_node> > path, set< vector<pair<super_node*, sub_node> > > active_paths);
};

tree_node* tree::bfs_search(tree_node* x, int val)
{
    list<tree_node*> queue;
    queue.push_back(x);
    while(!queue.empty())
    {
        auto r = queue.front();
        if(r->pos == val)
            return r;
        queue.pop_front();
        for(auto w = r->children.begin(); w!= r->children.end(); w++)
        {
            queue.push_back(*w);
        } 
    }
}
void tree::addedge(int parent_value, int child_value)
{
    auto x = root;
    //cout<<"Searching for "<<parent_value <<" root->pos is "<<root->pos<<"\n";
    auto y = bfs_search(x, parent_value);
    //cout<<y->pos<<"-------------------\n";
    //cout<<"Got the bgs_Search result\n";
    tree_node* new_node = new tree_node(child_value);
    y->children.push_back(new_node);
    new_node->parent = y;
    //cout<<"CHild created\n";
}


void tree::bfs_way_add_nodes(vector<pair<int, int> >v)
{
    //cout<<"\nAhem\n";
    // for(auto x = v.begin(); x!=v.end(); x++)
    // {
    //     cout<<x->first<<" "<<x->second<<"\n";   
    // }
    list<int> queue;
    set<int> s;
    //cout<<"Pushing root->pos : "<<root->pos<<"\n";
    queue.push_back(root->pos);
    while(!queue.empty())
    {
        int r = queue.front();
        // cout<<"R = "<<r<<"\n";
        queue.pop_front();
        if(s.find(r) == s.end())
        {
            for(auto x = v.begin(); x!=v.end(); x++)
            {
                if(x->first == r)
                {
                    if(s.find(x->second) == s.end())
                    {
                        addedge(r, x->second);
                        queue.push_back(x->second);
                    }
                }
                else if(x->second == r)
                {
                    if(s.find(x->first) == s.end())
                    {
                        addedge(r, x->first);
                        queue.push_back(x->first);
                    }
                }
            }
            s.insert(r);
        }
        
    }
}
  
void tree::bfs_traversal()
{
    auto x = root;
    list<tree_node* > queue;
    queue.push_back(x);
    set<int> s;
    while(!queue.empty())
    {
        auto r = queue.front();
        queue.pop_front();
        if(s.find(r->pos) == s.end())
        {
            cout<< r->pos <<"-----";
            cout<<"Its children are : ";
            for(auto w = r->children.begin(); w!= r->children.end(); w++)
            {
                cout<<(*w)->pos<<" ";
                queue.push_back(*w);
            }
            cout<<"\n";
            s.insert(r->pos);
        }
        
    }
}

void tree::construct_list(vector<vector<float> > workspace, int workspace_length, int workspace_breadth)
{
    construct(root, workspace, workspace_length, workspace_breadth);
}

void tree::construct(tree_node* node, vector<vector<float> > workspace, int workspace_length, int workspace_breadth)
{
    if(node->children.size()==0)
    {
        node->list_vals.push_back(workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length]);
        return;
    }
    int len = INT_MIN;
    for(auto x = node->children.begin(); x!=node->children.end();x++)
    {
        construct(*x, workspace, workspace_length, workspace_breadth);
        len = max(len, int((*x)->list_vals.size()));
    }
    node->list_vals = vector<int>(len,0);
    for(int x=2;x<=len;x++)
    {
        if(x%2==0)
        {
            int maxx = INT_MIN;
            for(auto y=node->children.begin();y!=node->children.end();y++)
            {
                if(x <= (*y)->list_vals.size())
                maxx = max(maxx, (*y)->list_vals[x-1] - 1);
            }
            node->list_vals[x-1] = max(node->list_vals[x-2], maxx);
        }
        else
        {
            int minn = INT_MAX;
            for(auto y=node->children.begin();y!=node->children.end();y++)
            {
                if(x <= (*y)->list_vals.size())
                minn = min(minn, (*y)->list_vals[x-1]);
            }
            node->list_vals[x-1] = min(node->list_vals[x-2], minn);   
        }
    }
    if(len%2==0 && node->list_vals[len-1] <= workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length])
        node->list_vals[len-1] = workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length];
    if(len%2==0 && node->list_vals[len-1] > workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length])
        node->list_vals.push_back(workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length]);
    if(len%2!=0 && node->list_vals[len-1] >= workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length])
        node->list_vals[len-1] = workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length];
    if(len%2!=0 && node->list_vals[len-1] < workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length])
        node->list_vals.push_back(workspace[(node->pos - 1)/workspace_length][(node->pos - 1)% workspace_length]); 
    for(auto x = node->list_vals.begin();x!=node->list_vals.end();x++)
    {
        mapp.insert(make_pair(node->pos, *x));
        //cout<<(*x)<<" ";
    }
    // //cout<<"---------------------------------------"<<(node->pos-1)/workspace_length<<" "<<(node->pos - 1)% workspace_length<<"\n";
    // cout<<"---------------------------------------"<<node->pos<<"\n";
    M = max(M, int(node->list_vals.size()));

}

void tree::dfs_type_traversal(super_node* temp)
{
    for(auto x = temp->children.begin();x!=temp->children.end();x++)
        dfs_type_traversal(*x);

    cout<<"\n----------------\n";
    cout<<"Current super_node : "<<temp->pos<<"\n";
    cout<<"Current subnodes : \n";
    for(auto x = temp->sub_nodes.begin();x!=temp->sub_nodes.end();x++)
    {
        cout<<"This subnode "<<x->val<<" is connected to the subnodes: ";
        for(auto y = x->children.begin(); y!=x->children.end(); y++)
        {
            cout<<(*y).first->val<<" of super node "<<(*y).second->pos<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";
    cout<<"Current super node's super-node children are : ";
    for(auto x = temp->children.begin();x!=temp->children.end();x++)
    {
        cout<<(*x)->pos<<" ";    
    }
    cout<<"\n---------------\n\n";
    //cout<<"\n";
}  
struct sort_pred {
    bool operator()(sub_node &left, sub_node &right) {
        return left.val < right.val;
    }
};
void tree:: find_path(super_node* temp, sub_node s, int elem, int count, stack<pair<super_node*, sub_node> >&q, stack<pair<super_node*, sub_node> > &h, int l)
{   
    // if(temp == NULL)
    //     return;
    // if(l)
    // cout<<"FInding path current super node "<<temp->pos<<"elem is "<<elem<<"\n";
    // h.push(temp->pos);
    // if(elem == temp->pos)
    // {
    //     q = h;
    //     return;
    // }
    // for(auto x = temp->children.begin(); x!=temp->children.end(); x++)
    // {
    //     find_path(*x, elem, q, h, l);
    // }
    // h.pop();
    // return;

    if(temp == NULL)
        return;
    if(l)
    cout<<"FInding path current super node "<<temp->pos<<"elem is "<<elem<<"\n";
    h.push(make_pair(temp, s));
    if(elem == temp->pos && s.val == count)
    {
        q = h;
        return;
    }
    for(auto x = s.children.begin(); x!=s.children.end(); x++)
    {
        find_path(x->second, *(x->first), elem, count, q, h, l);
    }
    h.pop();
    return;
}
int tree::find_if_okay(vector< pair< super_node*, sub_node> > path, set< vector<pair<super_node*, sub_node> > > active_paths)
{
    // cout<<"Entered find_if_okay\n";
    // cout<<"Active paths size is "<<active_paths.size()<<"\n";
    // cout<<"Current path : ";
    // for(auto w = path.begin(); w!= path.end(); w++)
    //     cout<<"( "<<(*w).first->pos<<" "<<(*w).second.val<<" )--> ";
    // cout<<"\n";
    // cout<<"Active paths : \n";
    // for(auto q = active_paths.begin(); q!=active_paths.end(); q++)
    // {
    //     for(auto w = (*q).begin(); w!= (*q).end(); w++)
    //     cout<<"( "<<(*w).first->pos<<" "<<(*w).second.val<<" )--> ";
    //     cout<<"\n";
    // }
    if(active_paths.size() == 0)
    {
        // cout<<"Returning 1\n";
        // cout<<"\n---------------------------------\n";
        return 1;
    }
    //cout<<"\n---------------------------------\n";
    int main_flag = 1;
    for(auto x = active_paths.begin(); x!= active_paths.end(); x++)
    {
        int a = 0;
        int b = 0;
        int flag = 1;
        while(a< (*x).size() && b < path.size())
        {
            if((*x)[a].first->pos == path[b].first->pos)
            {
                if(abs(int((*x)[a].second.val) - int(path[b].second.val)) > 1)
                {
                    flag = 0;
                    break;
                }
                else
                {
                    a++;
                    b++;
                }
            }
            else
                break;
        }
        if(a == (*x).size() || b == path.size())
            flag = 0;
        main_flag = main_flag && flag;
    }
    //cout<<"Returning " << main_flag <<" from find_if_okay\n";
    return main_flag;
}
void tree::agent_plan()
{
    set<int> boxes;
    set<int> prob_agents;
    set<pair<int,int> > edges_used;
    vector< vector<pair<super_node*, sub_node> > >agent_paths(no_of_agents);
    vector< vector<pair<super_node*, sub_node> > >agent_paths2(no_of_agents);
    vector<int> agent_locations(no_of_agents, 0);
    vector<int> agent_free(no_of_agents, 1);
    vector<int> agent_path_pointers(no_of_agents, 0);
    set< vector<pair<super_node*, sub_node> > >active_paths;
    vector<int> going_forward_or_backward(no_of_agents, 0); // 0 -> going forward, 1 -> going backward 
    //set<int> active_nodes;
    // for(auto x = order.begin(); x!= order.end(); x++)
    //     cout<<"( "<<x->first<<", "<<x->second<<" ) --> ";
    // cout<<"\n";
    set<pair<int, int> >ord;
    set<pair<int, int> >ord2;
    for(auto x = order.begin(); x!= order.end(); x++)
        ord.insert(*x);
    int needed = ord.size();
    int dummy =0;
    int dummy2 = 0;
    while(!ord.empty() || dummy2!=needed)
    {
        //  cout<<"Dummy 2 is "<<dummy2<<"\n";
        // cout<<"needed is "<<needed<<"\n";
        // cout<<"Order size is "<<order.size()<<"\n";
        for(auto x = order.begin(); x!= order.end(); x++)
        {
            // cout<<"Order size is "<<order.size()<<"\n";
            // cout<<"Current order elem is : "<<x->first<<" ,"<<x->second<<"\n";
            // cout<<"Paths_ size is "<<paths_.size()<<"\n";
            // cout<<"Paths_ elements are : \n";
            // for(auto xx = paths_.begin(); xx!= paths_.end(); xx++)
            // {
            //     cout<<"( "<<xx->first.first<<", "<<xx->first.second<<" ) : ";
            //     for(auto y = xx->second.begin(); y!= xx->second.end(); y++)
            //         cout<<"< "<<y->first->pos<<", "<<y->second.val<<" > --> ";
            //     cout<<"\n";
            // }
            // cout<<"\n";
            // cout<<"Current path is : ";
            // for(auto xx = paths_[*x].begin(); xx!=paths_[*x].end(); xx++)
            //     cout<<"< "<<xx->first->pos<<", "<<xx->second.val<<" > --> ";
            if(ord.find(*x) != ord.end())
            {
                int we = 0;
                vector< pair< super_node*, sub_node> > v = paths_[*x];
                int block_it = 0;
                if(find_if_okay(v, active_paths) == 1) //checking if the node can be built now
                {
                    for(int y=0;y<no_of_agents;y++)
                    {
                        if(going_forward_or_backward[y] == 1 && agent_free[y] == 0)
                        {
                            for(auto itr = v.begin(); itr!=v.end(); itr++)
                            {
                                if(itr->first->pos == agent_paths[y][agent_path_pointers[y]].first->pos)
                                {
                                    block_it = 1;
                                    //cout<<"Blocking it\n";
                                    goto ster;
                                }
                            }
                        }
                    }
                    ster:
                    if(block_it == 0)
                    {
                        //cout<<"FInd if returnded 1\n";
                        for(int y=0;y<no_of_agents;y++)     //finding a free agent
                        {
                            if(agent_free[y])               //if free
                            {
                                // cout<<"Agent free\n";
                                agent_free[y] = 0;
                                going_forward_or_backward[y] = 0;
                                agent_paths[y] = paths_[*x];
                                agent_paths2[y] = paths_[*x];
                                active_paths.insert(paths_[*x]);
                                //cout<<"< "<<x->first<<", "<<x->second<<" >\n";
                                ord.erase(ord.find(*x));
                                // cout<<"Removed from order\n";
                                // cout<<"< "<<x->first<<", "<<x->second<<" >\n";
                                vector<pair<super_node*, sub_node> > reverse(paths_[*x].begin(), paths_[*x].end());
                                reverse.pop_back();
                                vector<pair<super_node*, sub_node> > reverse_(reverse.rbegin(), reverse.rend());
                                agent_paths[y].insert(agent_paths[y].end(), reverse_.begin(), reverse_.end());
                                //cout<<"Checking\n";
                                goto step;
                            }
                        }
                    }
                }
                step:
                dummy++;
                //cout<<"< "<<x->first<<", "<<x->second<<" >\n";
            } 
        }
        // //cout<<"In between\n";
        // //cout<<"Now printing\n";
        // cout<<"Active paths : \n";
        // for(auto q = active_paths.begin(); q!=active_paths.end(); q++)
        // {
        //     for(auto w = (*q).begin(); w!= (*q).end(); w++)
        //     cout<<"( "<<(*w).first->pos<<" "<<(*w).second.val<<" )--> ";
        //     cout<<"\n";
        // }

        // for(int y=0; y<no_of_agents; y++)
        // {
        //     if(agent_free[y] == 0)
        //     {
        //         for(auto x = agent_paths[y].begin(); x!=agent_paths[y].end(); x++)
        //             cout<<x->first->pos<<" --> ";
        //         cout<<"\n";
        //     }
        // }
        // // //just

        prob_agents.clear();
        boxes.clear();
        edges_used.clear();
        tot_time++;                     /////////////////////// each time step
        for(int y=0; y<no_of_agents; y++)
        {
            if(agent_free[y] == 0)
            {
                if(agent_path_pointers[y] + 1 == agent_paths[y].size())
                {
                    //cout<<"Agent number "<<y<<" is at "<< agent_paths[y][agent_path_pointers[y]].first->pos<<"\n";
                    dummy2++;
                    agent_free[y] = 1;
                    agent_path_pointers[y] = 0;
                    active_paths.erase(active_paths.find(agent_paths2[y]));
                    agent_paths2[y].clear();
                    agent_paths[y].clear();
                }
                else
                {
                    //cout<<"Agent number "<<y<<" is at "<< agent_paths[y][agent_path_pointers[y]].first->pos<<"\n";
                    //cout<<"It moves to "<<agent_paths[y][agent_path_pointers[y]].first->pos
                    if(agent_paths[y].size()/2 == agent_path_pointers[y])
                    {
                        //cout<<"Started going backward\n";
                        going_forward_or_backward[y] = 1;
                    }

                    if(boxes.find(agent_paths[y][agent_path_pointers[y]+1].first->pos) == boxes.end() || agent_paths[y][agent_path_pointers[y]+1].first->pos == 0)
                    {
                        //cout<<"Yes\n";
                        int allowed = 1;
                        if(going_forward_or_backward[y] == 1)
                        {
                            for(int p = 0; p < no_of_agents; p++)
                            {
                                if(p == y || going_forward_or_backward[p] != 0)
                                    continue;
                                if(agent_free[p] == 0 && agent_path_pointers[p]<= agent_paths[p].size()/2)
                                {
                                    auto it = agent_paths[p].begin();
                                    advance(it, agent_path_pointers[p] + 1);
                                    auto it2 = agent_paths[p].begin();
                                    advance(it2, agent_paths[p].size()/2 + 1);
                                    for(auto itt = it; itt != it2; itt++)
                                    {
                                        if(itt->first->pos == agent_paths[y][agent_path_pointers[y]+1].first->pos)
                                            allowed = 0;
                                    }
                                }
                            }
                        }
                        
                        if(allowed)
                        {
                            if(edges_used.find(make_pair(agent_paths[y][agent_path_pointers[y]].first->pos, agent_paths[y][agent_path_pointers[y]+1].first->pos)) != edges_used.end()
                                || edges_used.find(make_pair(agent_paths[y][agent_path_pointers[y]+1].first->pos, agent_paths[y][agent_path_pointers[y]].first->pos))!= edges_used.end())
                            {
                                edges_common++;
                                //cout<<"Common edge "<<agent_paths[y][agent_path_pointers[y]].first->pos<<" ," <<agent_paths[y][agent_path_pointers[y]+1].first->pos<<"\n";
                            }
                            //cout<<"It moves to "<<agent_paths[y][agent_path_pointers[y]+1].first->pos<<"\n";
                            edges_used.insert(make_pair(agent_paths[y][agent_path_pointers[y]].first->pos, agent_paths[y][agent_path_pointers[y]+1].first->pos));
                            agent_path_pointers[y] += 1;
                            boxes.insert(agent_paths[y][agent_path_pointers[y]].first->pos);
                            // cout<<"Next block is "<< agent_paths[y][agent_path_pointers[y]].first->pos<<" \n";
                            // cout<<"Inserted "<<agent_paths[y][agent_path_pointers[y]].first->pos<<"\n";
                        }
                        
                    }
                    else
                    {
                        // // Dont insert and dont increment
                        // cout<<"Agent "<<y<<" is being stopped at location "<<agent_paths[y][agent_path_pointers[y]].first->pos<<"\n";
                        prob_agents.insert(y);
                    }
                    //cout<<"No\n";
                }
                
                
            }
        }
        //cout<<"\n-----------------------------------------------------------------------\n";
        //cout<<"\n-@@@@@@@@@@@@@@@@@@@2222#############------------------------\n";
    } 
    cout<<"\n\n";
}


void tree::dfs_traversal_for_positive_tree(super_node* temp ,int &no)
{
    order.clear();
    paths_.clear();
    // for(auto x = temp->children.begin();x!=temp->children.end();x++)
    //     dfs_type_traversal(*x);
    stack<pair<super_node*, sub_node> >s;
    s.push(make_pair(temp, temp->sub_nodes[0]));
    //vector<int> path;
    while(!s.empty())
    {
        auto x = s.top();
        s.pop();
        // cout<<"Super node is "<< x.first->pos<<" sub node is "<<x.second.val<<"\n";
        // //auto z = x.second;
        int count = 0;
        //sort(x.first->sub_nodes.begin(), x.first->sub_nodes.end(), sort_pred());
        for(auto z = x.first->sub_nodes.begin(); z != x.first->sub_nodes.end(); z++)
        {
            if(z->val == x.second.val && count!=0)
            {
                cout<<"Add a block at "<<x.first->pos<<"\n";
                order.push_back(make_pair(x.first->pos, count));
                stack<pair<super_node*, sub_node> >q, h;
                // if(x.first->pos == 6)
                // {
                //     cout<<"Before finding path\n";
                //     cout<<"temp pos = "<<temp->pos<<"\n";
                //     find_path(temp, x.first->pos, q, 1);
                //     cout<<"After finding path\n";
                // }
                // else
                //cout<<"Path to "<<x.first->pos<<" is : ";
                find_path(temp, temp->sub_nodes[0], x.first->pos, count, q, h, 0);
                vector<pair<super_node*, sub_node> > temp_;
                while(!q.empty())
                {
                    //paths_[x.first->pos].push_back(q.top());
                    temp_.push_back(q.top());
                    q.pop();
                }   
                // (pair<int, int>(1, 40))
                vector<pair<super_node*, sub_node> > temp_2(temp_.rbegin(), temp_.rend());
                raw_time += temp_2.size() * 2;                                                              /////////////// raw time
                paths_.insert(pair< pair<int,int> , vector< pair < super_node*, sub_node > > >(make_pair(x.first->pos, x.second.val), temp_2));
                // for(auto v = temp_.rbegin(); v!= temp_.rend(); v++)
                //     cout<<" ("<<v->first->pos<<", "<<v->second.val<<" ) --> ";
                // cout<<"\n";
                // //cout<<"----------\n";
                // //cout<<x.first->pos;
                no++;
                break;
            }
            count++;
        }
        // if(find(x.first->sub_nodes.begin(), x.first->sub_nodes.end(),x.second) != x.first->sub_nodes.begin())
        // {
        //     cout<<"Adding a number\n";
        //     no++;
        // }
        for(auto y = x.second.children.rbegin();y!=x.second.children.rend();y++)
        {
            s.push(make_pair(y->second, *(y->first)));
        }
    }
    cout<<"\n";
} 


void tree::dfs_traversal_for_negative_tree(super_node* temp ,int &no)
{
    order.clear();
    paths_.clear();
    // for(auto x = temp->children.begin();x!=temp->children.end();x++)
    //     dfs_type_traversal(*x);
    stack<pair<super_node*, sub_node> >s;
    s.push(make_pair(temp, temp->sub_nodes[0]));
    while(!s.empty())
    {
        auto x = s.top();
        s.pop();
        // cout<<"Super node is "<< x.first->pos<<" sub node is "<<x.second.val<<"\n";
        // //auto z = x.second;
        int count = 0;
        //sort(x.first->sub_nodes.begin(), x.first->sub_nodes.end(), sort_pred());
        for(auto z = x.first->sub_nodes.begin(); z != x.first->sub_nodes.end(); z++)
        {
            if(z->val == x.second.val && count!=0)
            {
                cout<<"Remove a block at "<<x.first->pos<<" ,"<<x.second.val<<"\n";
                order.push_back(make_pair(x.first->pos, x.second.val));
                stack<pair<super_node*, sub_node> >q, h;
                // if(x.first->pos == 6)
                // {
                //     cout<<"Before finding path\n";
                //     cout<<"temp pos = "<<temp->pos<<"\n";
                //     find_path(temp, x.first->pos, q, 1);
                //     cout<<"After finding path\n";
                // }
                // else
                //cout<<"Path to "<<x.first->pos<<" is : ";
                find_path(temp, temp->sub_nodes[0], x.first->pos, x.second.val, q, h, 0);
                vector<pair<super_node*, sub_node> > temp_;
                while(!q.empty())
                {
                    //paths_[x.first->pos].push_back(q.top());
                    temp_.push_back(q.top());
                    q.pop();
                }   
                // (pair<int, int>(1, 40))
                vector<pair<super_node*, sub_node> > temp_2(temp_.rbegin(), temp_.rend());
                raw_time += temp_2.size() * 2;                                                              /////////////// raw time
                paths_.insert(pair< pair<int,int> , vector< pair < super_node*, sub_node > > >(make_pair(x.first->pos, x.second.val), temp_2));
                // for(auto v = temp_.rbegin(); v!= temp_.rend(); v++)
                //     cout<<" ("<<v->first->pos<<", "<<v->second.val<<" ) --> ";
                // cout<<"\n";
                // //cout<<"----------\n";
                // //cout<<x.first->pos;

                no++;
                break;
            }
            count++;
        }
        // if(find(x.first->sub_nodes.begin(), x.first->sub_nodes.end(),x.second) != x.first->sub_nodes.begin())
        // {
        //     cout<<"Adding a number\n";
        //     no++;
        // }
        for(auto y = x.second.children.rbegin();y!=x.second.children.rend();y++)
        {
            s.push(make_pair(y->second, *(y->first)));
        }
    }
    //cout<<"\n";
} 


super_node* tree::dfs_type_build_positive(tree_node* temp, super_node* parent, int k)
{
    //if(temp->list_vals.size() <= k+1)
    if(temp->list_vals.size() >= k+1)
    {
        //cout<<"Building a super node for "<<temp->pos<<"\n";
        super_node* new_super_node = new super_node(temp->pos);
        int start_val = temp->list_vals[k-1];
        int end_val = temp->list_vals[k];
        // // cout<<"List values : ";
        // // for(auto x = temp->list_vals.begin(); x!=temp->list_vals.end();x++)
        // //     cout<<(*x)<<" ";
        // // cout<<"\n";
        // // cout<<"k = "<<k<<"\n";
        // // cout<<"Values : "<<temp->list_vals[k-1] << " "<<temp->list_vals[k]<<"\n";
        // cout<<"Start = "<<start_val<<" end = "<<end_val<<"\n";
        for(int num = start_val; num<=end_val; num++)
        {
            auto y = new sub_node(num);
            new_super_node->sub_nodes.push_back(*y);
        }
        new_super_node->parent = parent;
        for(auto x=new_super_node->sub_nodes.begin();x!=new_super_node->sub_nodes.end();x++)
        {
            if(new_super_node->parent!=NULL)
            {
                for(auto y=new_super_node->parent->sub_nodes.begin();y!=new_super_node->parent->sub_nodes.end();y++)
                {
                    if(y->val >= x->val - 1)
                    {
                        y->children.push_back(make_pair(&(*x), new_super_node));
                        break;
                    }
                }
            } 
        }

        for(auto x = temp->children.begin();x!=temp->children.end();x++)
        new_super_node->children.push_back(dfs_type_build_positive(*x, new_super_node, k));
        if(parent == NULL)
            root_of_tree = new_super_node;
        return new_super_node;

    }
    return NULL;
}

super_node* tree::dfs_type_build_negative(tree_node* temp, super_node* parent, int k)
{
    //if(temp->list_vals.size() <= k+1)
    //cout<<"Super node "<<temp->pos<<" k = "<<k<<"\n";
    if(temp->list_vals.size() >= k+1)
    {
        //cout<<"Building a super node for "<<temp->pos<<"\n";
        super_node* new_super_node = new super_node(temp->pos);
        int start_val = temp->list_vals[k-1];
        int end_val = temp->list_vals[k];
        // // cout<<"List values : ";
        // // for(auto x = temp->list_vals.begin(); x!=temp->list_vals.end();x++)
        // //     cout<<(*x)<<" ";
        // // cout<<"\n";
        // // cout<<"k = "<<k<<"\n";
        // // cout<<"Values : "<<temp->list_vals[k-1] << " "<<temp->list_vals[k]<<"\n";
        // cout<<"Start = "<<start_val<<" end = "<<end_val<<"\n";
        for(int num = start_val; num>=end_val; num--)
        {
            auto y = new sub_node(num);
            new_super_node->sub_nodes.push_back(*y);
        }
        new_super_node->parent = parent;
        for(auto x=new_super_node->sub_nodes.begin();x!=new_super_node->sub_nodes.end();x++)
        {
            if(new_super_node->parent!=NULL)
            {
                for(auto y=new_super_node->parent->sub_nodes.begin();y!=new_super_node->parent->sub_nodes.end();y++)
                {
                    if(y->val <= x->val )
                    {
                        y->children.push_back(make_pair(&(*x), new_super_node));
                        break;
                    }
                }
            } 
        }

        for(auto x = temp->children.begin();x!=temp->children.end();x++)
        new_super_node->children.push_back(dfs_type_build_negative(*x, new_super_node, k));
        if(parent == NULL)
            root_of_tree = new_super_node;
        return new_super_node;

    }
    return NULL;
}

super_node* tree::construct_positive_tree(int k)
{
    super_node* positive_root = dfs_type_build_positive(root, NULL, k);
    return positive_root;

}
super_node* tree::construct_negative_tree(int k)
{
    super_node* negative_root = dfs_type_build_negative(root, NULL, k);
    return negative_root;

}

void tree::generate_plan()
{
    //cout<<"M = "<<M<<" \n";
    int no_of_agents;
    // cout<<"Enter the number of agents : ";
    // cin>>no_of_agents;

    super_node* temp;
    int no_of_picks = 0;
    int no_of_drops = 0;
    for(int k=1;k<M;k++)
    {
        if(k%2!=0)
        {
            temp = construct_positive_tree(k);
            //dfs_type_traversal(temp, no_of_picks);
            if(temp == NULL)
                break;
            dfs_traversal_for_positive_tree(temp, no_of_picks);
            cout<<"Positive tree plan : \n";
            agent_plan();
            //cout<<"Came here\n";
        }
        else
        {
            temp = construct_negative_tree(k);
            if(temp == NULL)
                break;
            dfs_traversal_for_negative_tree(temp, no_of_drops);
            //cout<<"Removal done !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            cout<<"Negative tree plan : \n";
            agent_plan();
            //cout<<"Removal done !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }
    }
    cout<<"Total number of blocks added : "<<no_of_picks<<"\n";
    cout<<"Total number of blocks removed : "<<no_of_drops<<"\n";
    cout<<"Number of common edges used is : "<<edges_common<<"\n";
    cout<<"Raw time : "<<raw_time<<"\n";
    cout<<"Time taken : "<<tot_time<<"\n";
}



class Graph 
{ 
    int V;                  // No. of vertices 
    int workspace_length;
    int workspace_breadth;    
  
    // In a weighted graph, we need to store vertex 
    // and weight pair for every edge 
    list< pair<int, float> > *adj;
    list< pair<int, float> > *reweighted_adj; 
    vector<vector<float> > workspace;
  
public: 
    Graph(vector<vector<float> > &workspace, int workspace_length, int workspace_breadth);  // Constructor 
  
    // function to add an edge to graph 
    void addEdge(int u, int v, float w); 
  
    // Print MST using Prim's algorithm 
    void primMST(); 
    void reweight_edges();
}; 
  
// Allocates memory for adjacency list 
Graph::Graph(vector<vector<float> > &workspace_v, int workspace_length, int workspace_breadth) 
{ 
    this->workspace = workspace_v;
    this->workspace_length = workspace_length;
    this->workspace_breadth = workspace_breadth;
    this->V = this->workspace_length * this->workspace_breadth + 1; 
    adj = new list<pair<int, float> > [V]; 
    reweighted_adj = new list<pair<int, float> > [V];
} 
  
void Graph::addEdge(int u, int v, float w) 
{ 
    adj[u].push_back(make_pair(v, w)); 
    adj[v].push_back(make_pair(u, w)); 
} 
  
// Prints shortest paths from src to all other vertices 
void Graph::primMST() 
{ 
    // Create a priority queue to store vertices that 
    // are being preinMST. This is weird syntax in C++. 
    // Refer below link for details of this syntax 
    // http://geeksquiz.com/implement-min-heap-using-stl/ 

    // priority_queue< iPair, vector <iPair> , greater<iPair> > pq; 
    priority_queue< pair<int, float> , vector <pair<int, float> > , greater<pair<int, float> > > pq; 
    int src = 0; // Taking vertex 0 as source 
  
    // Create a vector for keys and initialize all 
    // keys as infinite (INF) 
    vector<int> key(V, INT_MAX); 
  
    // To store parent array which in turn store MST 
    vector<int> parent(V, -1); 
  
    // To keep track of vertices included in MST 
    vector<bool> inMST(V, false); 
  
    // Insert source itself in priority queue and initialize 
    // its key as 0. 
    pq.push(make_pair(0, src)); 
    key[src] = 0; 
  
    /* Looping till priority queue becomes empty */
    while (!pq.empty()) 
    { 
        // The first vertex in pair is the minimum key 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted key (key must be first item 
        // in pair) 
        int u = pq.top().second; 
        pq.pop(); 
  
        inMST[u] = true;  // Include vertex in MST 
  
        // 'i' is used to get all adjacent vertices of a vertex 
        list< pair<int, float> >::iterator i; 
        for (i = reweighted_adj[u].begin(); i != reweighted_adj[u].end(); ++i) 
        { 
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = (*i).first; 
            float weight = (*i).second; 
  
            //  If v is not in MST and weight of (u,v) is smaller 
            // than current key of v 
            if (inMST[v] == false && key[v] > weight) 
            { 
                // Updating key of v 
                key[v] = weight; 
                pq.push(make_pair(key[v], v)); 
                parent[v] = u; 
            } 
        } 
    } 
  
    // Print edges of MST using parent array 
    for (int i = 1; i < V; ++i) 
    {
        final_pairs.push_back(make_pair(parent[i], i));
        printf("%d - %d\n", parent[i], i); 
    }
} 

void Graph::reweight_edges()
{
    vector< vector<float> > u(workspace_breadth, vector<float>(workspace_length, 0.0));
    for(int x=0;x<workspace_breadth;x++)
        for(int y=0;y<workspace_length;y++)
        {
            for(int l=0;l<workspace_breadth;l++)
                for(int m=0;m<workspace_length;m++)
                {
                    if(x==l && m==y)
                        continue;
                    else
                    {
                        if(workspace[l][m]!=0 && workspace[l][m] > abs(x-l) + abs(y-m))
                        u[x][y] += (workspace[l][m] - abs(x-l) - abs(y-m) * 1.0)/workspace[l][m];
                    }
                }
        }
    set<pair<int,int> > s;
    for(int y=0;y<V;y++)
    {
        for(auto x = adj[y].begin(); x!= adj[y].end() ;x++)
        {
            if(y==0 || x->first==0)
            {
                reweighted_adj[y].push_back(make_pair(x->first, 0));
                reweighted_adj[x->first].push_back(make_pair(y, 0));
            }
            if(s.find(make_pair(y, x->first)) == s.end() && s.find(make_pair(x->first, y)) == s.end() && y!=0 && x->first!=0)
            {
                float nmr = 1.0 + abs(workspace[(y-1)/workspace_length][(y-1)%workspace_length] - workspace[(x->first-1)/workspace_length][(x->first-1)%workspace_length]);
                float dnr = 1.0 + u[(y-1)/workspace_length][(y-1)%workspace_length] + u[(x->first-1)/workspace_length][(x->first-1)%workspace_length];
                // cout<<"nmr = "<<nmr<<"dnr = "<<dnr<<"\n";
                reweighted_adj[y].push_back(make_pair(x->first, nmr*1.0/dnr));
                reweighted_adj[x->first].push_back(make_pair(y, nmr*1.0/dnr));
                s.insert(make_pair(x->first, y));
                s.insert(make_pair(y, x->first)); 
            }
            
        }
    }
 
    // cout<<"Actual adj matrix\n";
    // for(int y=0; y<V;y++)
    // {
    //     for(auto x = adj[y].begin(); x!= adj[y].end() ;x++)
    //     {
    //         cout<<y<<" - "<<x->first<<" cost = "<<x->second<<"\n";
            
    //     }
    // }

    // cout<<"\nreweight_edges adj matrix\n";
    // for(int y=0; y<V;y++)
    // {
    //     for(auto x = reweighted_adj[y].begin(); x!= reweighted_adj[y].end() ;x++)
    //     {
    //         cout<<y<<" - "<<x->first<<" cost = "<<x->second<<"\n";
            
    //     }
    // }
}



class termes
{
	int length, breadth;
	int leftmost = 0, rightmost = 0, topmost = 0, bottommost = 0;
	int workspace_length, workspace_breadth;
	vector<pair<int, int> > boundaries;
	vector< vector<int> > vals;
	vector<vector<float> > workspace;
    friend class Graph;
    friend class tree;
    public:
    	termes()
    	{
 			cout<<"Length  : ";
 			cin>>length;
 			cout<<"Breadth  : ";
 			cin>>breadth;
 			vals = vector<vector< int> >(breadth, vector<int>(length, 0));
 			boundaries.push_back(make_pair(0,0));
 			boundaries.push_back(make_pair(0,length));
 			boundaries.push_back(make_pair(length,breadth));
 			boundaries.push_back(make_pair(0,breadth));
    	}
    	void compute_workspace();
    	void show();
    	void input();
 		void convert_workspace_to_graph();
    	
};
void termes::compute_workspace()
{
	for(int x=0;x<breadth;x++)
		for(int y=0;y<length;y++)
		{
			leftmost = min(leftmost, y-vals[x][y]+1);
			rightmost = max(rightmost, y+vals[x][y]-1);
			topmost = min(topmost, x-vals[x][y]+1);
			bottommost = max(bottommost, x+vals[x][y]-1);
		}
	cout<<leftmost<<" "<<rightmost<<" "<<topmost<<" "<<bottommost<<"\n";
	workspace_length = rightmost - leftmost + 1;
	workspace_breadth = bottommost - topmost + 1;
	workspace = vector<vector<float> >(workspace_breadth, vector<float>(workspace_length, 0));
	for(int x=0;x<breadth;x++)
		for(int y=0;y<length;y++)
			workspace[x+(-topmost)][y+(-leftmost)] = 1.0 * vals[x][y];
	cout<<"Workspace is \n";
	for(int x=0;x<workspace_breadth;x++)
	{
		for(int y=0;y<workspace_length;y++)
			cout<<workspace[x][y]<<" ";
		cout<<"\n";
	}
}
void termes::show()
{
	for(auto x = boundaries.begin(); x!=boundaries.end(); x++)
		cout<<x->first <<" "<<x->second<<"\n";
	for(int x=0;x<breadth;x++)
	{
		for(int y=0;y<length;y++)
			cout<<vals[x][y]<<" ";
		cout<<"\n";
	}
}
void termes::input()
{
	cout<<"Enter the input\n";
	for(int x=0;x<breadth;x++)
		for(int y=0;y<length;y++)
			cin>>vals[x][y];
}


void termes::convert_workspace_to_graph()
{
	int V = workspace_length * workspace_breadth;
    Graph g(workspace, workspace_length, workspace_breadth); 
    for(int x=0;x<workspace_breadth;x++)
	{
		for(int y=0;y<workspace_length;y++)
		{
			if(y+1 < workspace_length)
				g.addEdge(x * workspace_length + y + 1, x * workspace_length + (y+1) + 1 , 1);
			if(x+1 < workspace_breadth)
				g.addEdge(x * workspace_length + y + 1, (x+1) * workspace_length + y + 1, 1);
		}
	}
    int source = 0;
    int no, a, b;
    cout<<"Number of nodes adj to source : ";
    cin>>no;
    for(int x = 0; x<no; x++)
    {
        cout<<"Enter the node's y and x co-ords (row first and column next): ";
        cin>>a>>b;
        g.addEdge(a*workspace_length + b + 1, source, 0);
    }
    cout<<"Enter the number of agents\n";
    cin>>no_of_agents;
    // g.addEdge(4*workspace_length + 0 + 1, source, 0);
    // g.addEdge(4*workspace_length + 3 + 1, source, 0);
    g.reweight_edges();
    // cout << "Edges of MST are \n"; 
    g.primMST();

    tree t(0);
    //cout<<"Checking "<<t.root->pos<<"######\n";
    t.bfs_way_add_nodes(final_pairs);
    //cout<<"Hello\n";
    //t.bfs_traversal();
    t.construct_list(workspace, workspace_length, workspace_breadth);
    t.generate_plan();

} 







    














int main()
{
	termes t;
	t.input();
	//t.show();
	t.compute_workspace();
	t.convert_workspace_to_graph();
	return 0;
}