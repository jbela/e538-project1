#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.


/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// global adjacency matrix initialized later
std::vector<std::vector<int>> adj;

// edge list: each row contains {source, target}
std::vector<std::vector<int>> edge_list;

void build_adj_matrix()
{
    adj = std::vector<std::vector<int>>(total_nodes, std::vector<int>(total_nodes, 0));

    for(int i = 0; i < edge_list.size(); i++) { // iterate through edge list
        int source = edge_list[i][0]; //get 2 nodes
        int target = edge_list[i][1];
        adj[source][target] = 1; // directed edge from src to tgt
    }
}

double calculate_fraction_of_ones()
{
    int count = 0;
    for(int i = 0; i < opinions.size(); i++) { // iterate through opinions vector
        if(opinions[i] == 1) {
            count++; // add to count if opinion is 1
        }
    }
    return (double)count / total_nodes; // return fraction
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    int count_one = 0;
    int count_zero = 0;
    for(int i = 0; i < total_nodes; i++) { // iterate through the node's neightbors
        if(adj[i][node] == 1) { // if neighbor
            if(opinions[i] == 1) {  // increase counts
                count_one++;
            } else {
                count_zero++;
            }
        }
    }

    if(count_one > count_zero) { // return majority
        return 1;
    } else {
        return 0;
    }
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // It was chosen that all opinions will be updated simultaneously based on the previous iteration's opinions. 
    bool changed = false;
    vector<int> updated_opinions = opinions; //  to hold updated opinions while we calc them

    for(int i = 0; i < total_nodes; i++) {
        int majority = get_majority_friend_opinions(i);
        if(majority != opinions[i]) {
            updated_opinions[i] = majority; // update new opinion
            // opinions[i] = majority; // immediately update new opinion
            changed = true;
        }
    }

    opinions = updated_opinions; // update opinions vector
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    /// (6)  //////////////////////////////////////////////

    if (calculate_fraction_of_ones() == 1.0 || calculate_fraction_of_ones() == 0.0) {
        opinions_changed = false; // skip loop
    }

    while(iteration < max_iterations) {
        opinions_changed = update_opinions();
        if (!opinions_changed) {
            break; // stop if no opinions changed
        }
        iteration++;
        cout << opinions_changed << endl;
        cout << "Iteration " << iteration << ": fraction of 1's = " 
             << calculate_fraction_of_ones() << endl;
    }
    
    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */