#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

struct Node {
    int id;
    Node* next;
    Node(int val) : id(val), next(nullptr) {}
};

class JosephusSimulator {
private:
    Node* head;
    int count;

public:
    JosephusSimulator(int n) : head(nullptr), count(n) {
        if (n <= 0) return;
        head = new Node(1);
        Node* prev = head;
        for (int i = 2; i <= n; i++) {
            prev->next = new Node(i);
            prev = prev->next;
        }
        prev->next = head; // Make the list circular
    }

    void printRing() {
        if (!head) return;
        cout << "    Active Ring: ";
        Node* temp = head;
        do {
            cout << temp->id << " -> ";
            temp = temp->next;
        } while (temp != head);
        cout << "(back to " << head->id << ")\n";
    }

    void simulate(int k, int survivors = 1, int delayMs = 300) {
        if (!head || k <= 0) return;

        // Find the tail (node just before head) to maintain a trailing pointer
        Node* prev = head;
        while (prev->next != head) {
            prev = prev->next;
        }
        Node* curr = head;
        int round = 1;
        vector<int> eliminatedOrder;

        cout << "\n--- Starting Simulation (N = " << count << ", k = " << k << ") ---\n";
        printRing();

        while (count > survivors) {
            cout << "\n[Round " << round++ << "] Counting " << k << " steps: ";

            // Traverse k - 1 steps
            for (int step = 1; step < k; step++) {
                cout << curr->id << " -> ";
                prev = curr;
                curr = curr->next;
            }
            cout << "[" << curr->id << " ELIMINATED]\n";

            // Pointer operation display
            cout << "    Pointer Update: Node(" << prev->id << ")->next = Node(" << curr->next->id << ")\n";

            // Bypass and delete current node
            prev->next = curr->next;
            if (curr == head) {
                head = curr->next; // Update head if head was eliminated
            }

            Node* toDelete = curr;
            eliminatedOrder.push_back(toDelete->id);
            curr = curr->next;
            delete toDelete;
            count--;

            printRing();
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }

        cout << "\n========================================\n";
        cout << "Elimination Order: ";
        for (size_t i = 0; i < eliminatedOrder.size(); i++) {
            cout << eliminatedOrder[i] << (i + 1 < eliminatedOrder.size() ? " -> " : "\n");
        }

        cout << "Survivor(s): ";
        Node* temp = head;
        for (int i = 0; i < count; i++) {
            cout << "#" << temp->id << " ";
            temp = temp->next;
        }
        cout << "\n========================================\n";
    }
};

int main() {
    int n = 7, k = 3;
    cout << "Enter number of people (N): ";
    cin >> n;
    cout << "Enter step count (k): ";
    cin >> k;

    JosephusSimulator sim(n);
    sim.simulate(k);
    return 0;
}