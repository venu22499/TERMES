////// THe implementation of the termes software.
//////

#include<bits/stdc++.h>
using namespace std;






typedef  pair<int, int> iPair; 
  
// Structure to represent a graph 
struct Graph 
{ 
    int V, E; 
    int workspace_length, workspace_breadth;
    vector< pair<int, pair<iPair, iPair> > >edges; 
    Graph(int workspace_length, int workspace_breadth) 
    { 
        this->V = workspace_length * workspace_breadth + 1;
        this->workspace_length = workspace_length;
        this->workspace_breadth = workspace_breadth;
        this->E = 0; 
    } 
  
    // Utility function to add an edge 
    void addEdge(iPair u, iPair v, int w) 
    { 
    	E++;
        edges.push_back(make_pair(w, make_pair(u, v))); 
    } 
    int kruskalMST(); 
}; 
  
// To represent Disjoint Sets 
struct DisjointSets 
{ 
    map<iPair, iPair> parent;
    map<iPair, int> rnk; 
    int n; 
  
    // Constructor. 
    DisjointSets(int workspace_length, int workspace_breadth) 
    { 
        // // Allocate memory 
        // this->n = n; 
        // parent = new int[n+1]; 
        // rnk = new int[n+1]; 
  
        // Initially, all vertices are in 
        // different sets and have rank 0. 
        for(int x=0;x<workspace_breadth;x++)
		{
			for(int y=0;y<workspace_length;y++)
			{
				parent[make_pair(x,y)] = make_pair(x,y);
				rnk[make_pair(x,y)] = 0;
			}
		}
    } 
  
    // Find the parent of a node 'u' 
    // Path Compression 
    iPair find(iPair u) 
    { 
        /* Make the parent of the nodes in the path 
           from u--> parent[u] point to parent[u] */
        if (u != parent[u]) 
            parent[u] = find(parent[u]); 
        return parent[u]; 
    } 
  
    // Union by rank 
    void merge(iPair x, iPair y) 
    { 
        x = find(x), y = find(y); 
  
        /* Make tree with smaller height 
           a subtree of the other tree  */
        if (rnk[x] > rnk[y]) 
            parent[y] = x; 
        else // If rnk[x] <= rnk[y] 
            parent[x] = y; 
  
        if (rnk[x] == rnk[y]) 
            rnk[y]++; 
    } 
}; 
  
 /* Functions returns weight of the MST*/ 
 
struct sort_pred {
    bool operator()(pair<int, pair<iPair, iPair> > &left, pair<int, pair<iPair, iPair> > &right) {
        return left.first < right.first;
    }
};


int Graph::kruskalMST() 
{ 
    int mst_wt = 0; // Initialize result 
  
    // Sort edges in increasing order on basis of cost 
    sort(edges.begin(), edges.end(), sort_pred());
    // Create disjoint sets 
    DisjointSets ds(this->workspace_length, this->workspace_breadth); 
  
    // Iterate through all sorted edges 

    for (auto it = edges.begin(); it != edges.end(); it++) 
    { 
        iPair u = it->second.first; 
        iPair v = it->second.second; 
  
  		if(u == make_pair(100,100) || v == make_pair(100,100))
  		{
  			iPair set_u = ds.find(u); 
	        iPair set_v = ds.find(v); 
	  
	        // Check if the selected edge is creating 
	        // a cycle or not (Cycle is created if u 
	        // and v belong to same set) 
	        if (set_u != set_v) 
	        { 
	            // Current edge will be in the MST 
	            // so print it 
	            cout << u.first <<","<< u.second << " - " << v.first <<","<<v.second << endl; 
	  
	            // Update MST weight 
	            mst_wt += it->first; 
	  
	            // Merge two sets 
	            ds.merge(set_u, set_v); 
	        }
  		} 
    } 

    for (auto it = edges.begin(); it != edges.end(); it++) 
    { 
        iPair u = it->second.first; 
        iPair v = it->second.second; 
  
        iPair set_u = ds.find(u); 
        iPair set_v = ds.find(v); 
  
        // Check if the selected edge is creating 
        // a cycle or not (Cycle is created if u 
        // and v belong to same set) 
        if (set_u != set_v) 
        { 
            // Current edge will be in the MST 
            // so print it 
            cout << u.first <<","<< u.second << " - " << v.first <<","<<v.second << endl; 
  
            // Update MST weight 
            mst_wt += it->first; 
  
            // Merge two sets 
            ds.merge(set_u, set_v); 
        } 
    } 
  
    return mst_wt; 
} 




class termes
{
	int length, breadth;
	int leftmost = 0, rightmost = 0, topmost = 0, bottommost = 0;
	int workspace_length, workspace_breadth;
	vector<pair<int, int> > boundaries;
	vector< vector<int> > vals;
	vector<vector<int> > workspace;
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
	workspace = vector<vector<int> >(workspace_breadth, vector<int>(workspace_length, 0));
	for(int x=0;x<breadth;x++)
		for(int y=0;y<length;y++)
			workspace[x+(-topmost)][y+(-leftmost)] = vals[x][y];
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
    Graph g(workspace_length, workspace_breadth); 
    for(int x=0;x<workspace_breadth;x++)
	{
		for(int y=0;y<workspace_length;y++)
		{
			if(y+1 < workspace_length)
				g.addEdge(make_pair(x,y), make_pair(x,y+1), 1);
			if(x+1 < workspace_breadth)
				g.addEdge(make_pair(x,y), make_pair(x+1,y), 1);
		}
	}
    auto source = make_pair(100,100);
    g.addEdge(make_pair(4,0), source, 0);
    g.addEdge(make_pair(4,3), source, 0);
    cout << "Edges of MST are \n"; 
    int mst_wt = g.kruskalMST(); 
  
    cout << "\nWeight of MST is " << mst_wt; 

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