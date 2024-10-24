#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

// Class to represent the debt graph using an adjacency list
class minFlowGraph {
public:
    vector<vector<pair<int, int>>> adjList;  // Adjacency list to store debt relationships

    // Constructor to initialize the graph for n people
    minFlowGraph(int n) {
        adjList.resize(n);
    }

    // Function to add a debt relationship
    void addDebt(int debtor, int creditor, int amount) {
        adjList[debtor].push_back({creditor, amount});
    }

    // Function to compute the net balance for each person
    vector<int> computeBalances(int n) {
        vector<int> balances(n, 0);  // Initialize all balances to 0
        for (int debtor = 0; debtor < n; debtor++) {
            for (auto& creditor : adjList[debtor]) {
                int creditorId = creditor.first;
                int amount = creditor.second;
                balances[debtor] -= amount;      // Debtor owes money (subtract from their balance)
                balances[creditorId] += amount;  // Creditor is owed money (add to their balance)
            }
        }
        return balances;
    }
};

// Recursive function to minimize the number of transactions and store transactions
int minimizeTransactionsHelper(int idx, vector<int>& balances, vector<vector<int>>& transactions) {
    while (idx < balances.size() && balances[idx] == 0) {
        idx++;
    }

    if (idx == balances.size()) return 0;

    int minTransactions = INT_MAX;
    
    for (int i = idx + 1; i < balances.size(); i++) {
        if (balances[i] * balances[idx] < 0) {  // Opposite balances
            int transfer = min(abs(balances[idx]), abs(balances[i]));  // Calculate how much can be transferred
            balances[i] += balances[idx];  // Settle debt

            // Record the transaction
            transactions.push_back({idx, i, transfer});

            // Recurse to minimize transactions with updated balances
            int currentTransactions = 1 + minimizeTransactionsHelper(idx + 1, balances, transactions);

            // Update the minimum transactions
            minTransactions = min(minTransactions, currentTransactions);

            balances[i] -= balances[idx];  // Backtrack
            transactions.pop_back(); // Remove the last transaction for further exploration
        }
    }

    return minTransactions == INT_MAX ? 0 : minTransactions; // Return 0 if no transactions possible
}

// Wrapper function to minimize transactions and record who transfers to whom
int minimizeTransactions(vector<int>& balances, vector<vector<int>>& transactions) {
    return minimizeTransactionsHelper(0, balances, transactions);
}

int minTransactionsGreedy(){
    // TODO
}

int main() {
    int n, m;
    
    cout << "Enter the number of people: ";
    cin >> n;
    
    // Create a minFlowGraph object for n people
    minFlowGraph graph(n);

    cout << "Enter the number of debts (transactions): ";
    cin >> m;

    cout << "Enter each debt as: debtor creditor amount\n";
    for (int i = 0; i < m; i++) {
        int debtor, creditor, amount;
        cin >> debtor >> creditor >> amount;
        graph.addDebt(debtor, creditor, amount);  // Add the debt to the graph
    }

    // Compute the net balances for each person
    vector<int> balances = graph.computeBalances(n);

    cout << "\nBalances after transactions:\n";
    for (int i = 0; i < balances.size(); i++) {
        cout << "Person " << i << ": " << balances[i] << endl;
    }

    // Vector to store the actual transactions
    vector<vector<int>> transactions;

    // Call the function to minimize the number of transactions
    int result = minimizeTransactions(balances, transactions);
    cout << "\nMinimum number of transactions required: " << result << endl;

    // Print the transactions that settle the debts
    cout << "\nTransactions to settle the debts:\n";
    for (auto& trans : transactions) {
        cout << "Person " << trans[0] << " pays Person " << trans[1] << " amount: " << trans[2] << endl;
    }

    return 0;
}
