////// THe implementation of the termes software.
//////

#include<bits/stdc++.h>
using namespace std;



multimap<int,int> mapp;

vector<pair<int, int> > final_pairs;
typedef  pair<int, int> iPair; 


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
void tree::dfs_traversal_for_positive_tree(super_node* temp ,int &no)
{
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
                cout<<"Add a block at "<<x.first->pos<<"\n";
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


void tree::dfs_traversal_for_negative_tree(super_node* temp ,int &no)
{
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
                cout<<"Remove a block at "<<x.first->pos<<"\n";
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
        }
        else
        {
            temp = construct_negative_tree(k);
            if(temp == NULL)
                break;
            dfs_traversal_for_negative_tree(temp ,no_of_drops);
        }
    }
    cout<<"Total number of blocks added : "<<no_of_picks<<"\n";
    cout<<"Total number of blocks removed : "<<no_of_drops<<"\n";
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
    g.addEdge(4*workspace_length + 0 + 1, source, 0);
    g.addEdge(4*workspace_length + 3 + 1, source, 0);
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